#include "libPWM.h"
#include "libFFT.h"

static bool FlagPeriodElapsed = false;
static bool PulseFinished = false;
static bool FlagOffTimer = true;

static PeriodSettings FrequencyParser (FrequencyStruct * frequency);
static void PWMTimerSetup (PeriodSettings settings);
static void PeriodElapsedHandler (void);

void PWMStart (void) {
	HAL_TIM_Base_Start_IT(&PWM_HTIM);
	HAL_TIM_PWM_Start_IT(&PWM_HTIM, PWM_CHANEL);
}

void libPWMmainLEDBlink (void) {
	PeriodSettings LedSettings = tZero;

	PeriodElapsedHandler ();
	if (FlagOffTimer == true){
		LedSettings = FrequencyParser (&FrequencyDetectorData);
		if (LedSettings != tZero){
			MeasurementsClear(&FrequencyDetectorData);
			PWMTimerSetup (LedSettings);
			PWMStart();
		}
	}
}

static PeriodSettings FrequencyParser (FrequencyStruct * frequency) {
	const uint16_t frequencyDetectionThreshold = 1;
	PeriodSettings parser = tZero;

	if ((frequency->Hz1000 >= frequencyDetectionThreshold) && (frequency->Hz500 >= frequencyDetectionThreshold))
		parser = t550ms_F500_F1000;
	else if (frequency->Hz1000 >= frequencyDetectionThreshold)
		parser = t1000ms_F1000;
	else if (frequency->Hz500 >= frequencyDetectionThreshold)
		parser = t200ms_F500;
	return parser;
}

static void PWMTimerSetup (PeriodSettings settings){
	switch (settings) {
		case t200ms_F500:
			__HAL_TIM_SET_AUTORELOAD (&PWM_HTIM, t200ms_F500);
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, t200ms_Pulse100ms);
			break;
		case t1000ms_F1000:
			__HAL_TIM_SET_AUTORELOAD (&PWM_HTIM, t1000ms_F1000);
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, t1000ms_Pulse500ms);
			break;
		case t550ms_F500_F1000:
			__HAL_TIM_SET_AUTORELOAD (&PWM_HTIM, t550ms_F500_F1000);
			__HAL_TIM_SET_COMPARE (&PWM_HTIM, PWM_CHANEL, t550ms_Pulse50ms);
			break;
		default:
			break;
	}
}

static void PeriodElapsedHandler (void) {
	if (FlagPeriodElapsed == true){
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		if (PulseFinished == true) {
			HAL_TIM_PWM_Stop_IT(&PWM_HTIM, PWM_CHANEL);
			HAL_TIM_Base_Stop_IT(&PWM_HTIM);
			FlagOffTimer = true;
			FlagPeriodElapsed = false;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == PWM_HTIM.Instance)
		FlagPeriodElapsed = true;
}

void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef *htim){
	if (htim->Instance == PWM_HTIM.Instance){
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		PulseFinished = true;
	}
}
