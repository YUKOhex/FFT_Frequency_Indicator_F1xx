#include "libFFT.h"
#include "stm32_dsp.h"
#include "math.h"

FFT_InitStruct FFT_data;

uint16_t Freq;

#ifdef DEBUG_FFT
	int32_t SignalSimulation [NPT];
	static inline void For_FFT_SignalSimulation(int32_t *arr, float Fd);
#endif

static inline void CalcPowerMag(FFT_InitStruct *data);


void libFFT_Init (void) {
	FFT_data.UpdateMagFlag = false;
}

void libFFT_main (FFT_InitStruct *data) {
	uint32_t maxbuff=0, jbuff = 0;

#if NPT == 1024
	cr4_fft_1024_stm32(data->OutArray, data->InArray, NPT);
#elif NPT == 256
	cr4_fft_256_stm32(data->OutArray, data->InArray, NPT);
#elif NPT == 64
	cr4_fft_256_stm32(data->OutArray, data->InArray, NPT);
#endif
	CalcPowerMag (data);
	for (uint16_t j = 7; j < NPT/2; j++){
		if (maxbuff < data->MagArray[j]){
			maxbuff = data->MagArray[j];
			jbuff = j;
		}
	}
	data->UpdateMagFlag = false;
	Freq = jbuff;
}

static inline void CalcPowerMag(FFT_InitStruct *data) {
   int16_t lX,lY;
   float X,Y,Mag;

   for(uint16_t i = 0; i < NPT / 2; i++){
      lX  = data->OutArray[i] & 0xFFFFU;
      lY  = data->OutArray[i] >> 16;
      X = NPT * ((float)lX) / 32768.0;
      Y = NPT * ((float)lY) / 32768.0;
      Mag = sqrt(X * X + Y * Y) / NPT;
      if(i == 0)
    	  data->MagArray[i] = (uint32_t)(Mag * 32768);
      else
    	  data->MagArray[i] = (uint32_t)(Mag * 65536);
   }
   data->UpdateMagFlag = true;
}

#ifdef DEBUG_FFT
static inline void For_FFT_SignalSimulation(int32_t *arr, float Fd)
{
    float fx;
    for( uint16_t i = 0; i < NPT; i++)
    {
        fx = 100 * sin(6.28 * i * 5000.0 / Fd) +
             500 * sin(6.28 * i * 8400.0 / Fd) +
             1500 * sin(6.28 * i * 18725.0 / Fd);
        arr[i] = ((int16_t)fx) << 16;
    }
}
#endif
