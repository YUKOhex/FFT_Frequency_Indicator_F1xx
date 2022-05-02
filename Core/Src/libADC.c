#include "libADC.h"
#include "adc.h"
#include "tim.h"
#include "dma.h"
#include <string.h>
#include "stm32_dsp.h"
#include "math.h"

bool DMAflag = false;


#include "stm32_dsp.h"
#include "math.h"
#include <stdlib.h>
#include <string.h>

#define NPT 1024
int32_t lBufInArray [NPT];
int32_t lBufOutArray [NPT];
uint32_t lBufMagArray [NPT/2];

volatile uint32_t printspectr;
volatile uint32_t printflag;
uint32_t Freq;
//void InitBufInArray(void);
void GetPowerMag(void);


void GetPowerMag(void);


void InitBufInArray(void);

void libADCInit (void) {
//	InitBufInArray ();
	HAL_TIM_Base_Start(&htim3);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &lBufInArray, NPT*2);
}

void libADCMain (void){
	uint32_t maxBuff = 0;
	uint32_t jBuff = 0;
	uint16_t j = 0;
	if (DMAflag == true){
		#if NPT == 1024
			cr4_fft_1024_stm32(lBufOutArray, lBufInArray, NPT);
		#else
			cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
		#endif

		GetPowerMag();
/*
		   printflag = 1200;
		for (uint16_t j = 0; j < NPT/2; j++){
			printspectr = lBufMagArray[j];
			HAL_Delay(35);
		}
		printflag = 0;
*/
		for (j = 7; j < NPT/2; j++){
			if (maxBuff < lBufMagArray[j]){
				maxBuff = lBufMagArray[j];
				jBuff = j;
			}
		}
	//	Freq = (float)jBuff * 37.05;
		Freq = (float)jBuff;
		DMAflag = false;

		HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &lBufInArray, NPT*2);
		 HAL_Delay(1000);

	}

}



void GetPowerMag(void)
{
   int16_t lX,lY;
   float X,Y,Mag;
   uint16_t i;
   for(i=0; i<NPT/2; i++)
   {
       lX  = (lBufOutArray[i] << 16) >> 16;
       lY  = (lBufOutArray[i] >> 16);
      X = NPT * ((float)lX) / 32768.0;
       Y = NPT * ((float)lY) / 32768.0;
      Mag = sqrt(X * X + Y * Y) / NPT;
      if(i == 0)
          lBufMagArray[i] = (uint32_t)((Mag * 32768));
      else
          lBufMagArray[i] = (uint32_t)((Mag * 65536));
   }


}
#ifdef F

void libADCMain (void){

	for (uint16_t j = 0; j < NPT; j++){
		lBufInArray[j] = lBufInArray[j] << 16;
	}
	  cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	  GetPowerMag();
//	  char str [9];
	 // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
	//  HAL_Delay(1000);
	  //for (uint16_t i = 0; i < (NPT / 2); i++){
	 // itoa (lBufMagArray[i], str, 10);
/*
	//  HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 10);
	  for (uint8_t j = 0; j < 9; j++){
		  str[j] = 0;
	  }
	  str [0] = '\r';
	  str [1] = '\n';
	  HAL_UART_Transmit(&huart1, (uint8_t *) str, strlen(str), 10);
	  }
	  */
	  while (1);

}


#endif
