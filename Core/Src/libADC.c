#include "libADC.h"
#include "adc.h"
#include "tim.h"
#include "dma.h"
#include "libFFT.h"
#include "string.h"

static bool DMAflag = false;
static uint16_t ADCDMABuff [NPT];

void libADCInit (void) {
	HAL_TIM_Base_Start(&HTIM);
	HAL_ADC_Start_DMA(&HADC,(uint32_t*) &ADCDMABuff, NPT*2);
}

void libADCMain (void){
	if (DMAflag == true){
		if (FFT_data.UpdateFlag == false){
			for (uint16_t i = 0; i < NPT; i++)
				FFT_data.InArray [i] = ADCDMABuff [i];
			HAL_ADC_Start_DMA(&HADC,(uint32_t*) &ADCDMABuff, NPT*2);
			DMAflag = false;
			FFT_data.UpdateFlag = true;
		}
	}
}

void DMA_IRQHandler (void) {  //extern _it.c DMAx_Channelx_IRQHandler
	DMAflag = true;
	HAL_ADC_Stop_DMA(&HADC);
}
