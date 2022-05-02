#include "libADC.h"
#include "adc.h"
#include "tim.h"
#include "dma.h"
#include "libFFT.h"

bool DMAflag = false; //

void libADCInit (void) {
#ifdef __LIBFFT_H__
	libFFT_Init();
#endif
	HAL_TIM_Base_Start(&HTIM);
	HAL_ADC_Start_DMA(&HADC,(uint32_t*) &FFT_data.InArray, NPT * 2);
}

void libADCMain (void){
	if (DMAflag == true){
		libFFT_main (&FFT_data);
		HAL_ADC_Start_DMA(&HADC,(uint32_t*) &FFT_data.InArray, NPT*2);
		DMAflag = false;
	}

}

void DMA_IRQHandler (void) {  //extern _it.c DMAx_Channelx_IRQHandler
	DMAflag = true;
	HAL_ADC_Stop_DMA(&HADC);
}
