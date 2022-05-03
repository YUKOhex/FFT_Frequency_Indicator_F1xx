#include "libPWM.h"

bool StopFlag = false;

static void LedSignal (PeriodSettingsEnum Period);

void PWMStart (void) {
	HAL_TIM_Base_Start_IT(&PWM_HTIM);
	HAL_TIM_PWM_Start_IT(&PWM_HTIM, PWM_CHANEL);

}

void PWMStop (void) {
	HAL_TIM_Base_Stop_IT(&PWM_HTIM);
	HAL_TIM_PWM_Stop_IT(&PWM_HTIM, PWM_CHANEL);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}


void libPWMmain (void) {

	if (StopFlag == true){
		PWMStop();
		StopFlag = false;
	}
	if (StopFlag == false)
		LedSignal (t200ms);
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
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, tPulse);
			break;
		default:
			break;
	}
	PWMStart();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == PWM_HTIM.Instance){
		StopFlag = true;
	}

}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == PWM_HTIM.Instance){
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	}
}

