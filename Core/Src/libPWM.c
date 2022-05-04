#include "libPWM.h"
#include "libFFT.h"

extern FrequencyStruct DataFrequencyDetermanition;
static void LedSignal (PeriodSettingsEnum Period);

void PWMStart (void) {
	HAL_TIM_Base_Start_IT(&PWM_HTIM);
	 HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_4);
}
/*
void PWMStop (void) {

}
*/
uint16_t j = 0;
bool flag = true;
bool StopFlag = true;
uint8_t finish = 1;
bool onFlag = false, offFlag = true;
void libPWMmain (void) {
	PeriodSettingsEnum Period = tZero;
	if (DataFrequencyDetermanition.Hz500 > 5)
		Period = t200ms;
	if (DataFrequencyDetermanition.Hz1000 > 5)
		Period = t1000ms;

	if ((DataFrequencyDetermanition.Hz1000 > 5) && (DataFrequencyDetermanition.Hz500 > 5)){
		Period = t550ms;
	}
	if (offFlag == true && Period != tZero){
		offFlag = false;
		onFlag = false;
	LedSignal (Period);
	DataFrequencyDetermanition.Hz1000 = 0;
	DataFrequencyDetermanition.Hz500 = 0;
	}


}


static void LedSignal (PeriodSettingsEnum Period){ // milliseconds

	switch (Period) {
		case t200ms:
			__HAL_TIM_SET_AUTORELOAD (&PWM_HTIM, t200ms);
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, tPulse100ms);
			break;

		case t1000ms:
			__HAL_TIM_SET_AUTORELOAD (&PWM_HTIM, t1000ms);
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, tPulse500ms);
			break;
		case t550ms:
			__HAL_TIM_SET_AUTORELOAD (&PWM_HTIM, t550ms);
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, tPulse550ms);
			break;
		default:
			break;
	}
	PWMStart();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == PWM_HTIM.Instance){
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		if (onFlag == true) {
			HAL_TIM_Base_Stop_IT(htim);
			HAL_TIM_PWM_Stop_IT(htim, PWM_CHANEL);
			offFlag = true;
		}
	}

}

void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim){
	if (htim->Instance == PWM_HTIM.Instance){
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		onFlag = true;
	}
}

