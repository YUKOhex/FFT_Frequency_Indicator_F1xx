#include "libFFT.h"
#include "stm32_dsp.h"
#include "math.h"

FFT_InitStruct FFT_data;
FrequencyStruct DataFrequencyDetermanition;

SearchSinusStruct foundSinusMax;


static const uint16_t startFrequency = 18;
static const uint16_t finishFrequency = NPT / 2;
//static const uint16_t MaxNormMag = 1000;
//uint16_t Freq;
//uint32_t freqflag;
//uint32_t freqprint;



uint32_t mag500, mag1000;
uint16_t SinFreq;




#ifdef DEBUG_FFT
	int32_t SignalSimulation [NPT];
	static inline void For_FFT_SignalSimulation(int32_t *arr, float Fd);
	static void SinusSignalParametr (FFT_InitStruct *data);
#endif

static inline void CalcPowerMag(FFT_InitStruct *data);
static FrequencyStruct FrequencyDetermination (uint32_t *data);

static SearchSinusStruct SearchSinusMaxMagnitude (uint32_t *data);

void libFFT_Init (void) {
	FFT_data.UpdateMagFlag = false;
	DataFrequencyDetermanition.Hz500 = false;
	DataFrequencyDetermanition.Hz1000 = false;
	DataFrequencyDetermanition.Hz1500 = false;
}

void libFFT_main (FFT_InitStruct *data) {

#if NPT == 1024
	cr4_fft_1024_stm32(data->OutArray, data->InArray, NPT);
#elif NPT == 256
	cr4_fft_256_stm32(data->OutArray, data->InArray, NPT);
#elif NPT == 64
	cr4_fft_64_stm32(data->OutArray, data->InArray, NPT);
#endif
	CalcPowerMag (data);
	DataFrequencyDetermanition = FrequencyDetermination (data->MagArray);
	foundSinusMax = SearchSinusMaxMagnitude (data->MagArray);
	data->UpdateMagFlag = false;
	mag500 = data->MagArray [21];
	mag1000 = data->MagArray [43];
	HAL_Delay(100);

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

static FrequencyStruct FrequencyDetermination (uint32_t *data) { //8999
	FrequencyStruct temp;
	const uint8_t threshold = 50;
	if (data [31] > 87 || data [32] > 87)
		temp.Hz500 = true;
	else
		temp.Hz500 = false;

	if (data [64] > 55 || data [65] > 55)
		temp.Hz1000 = true;
	else
		temp.Hz1000 = false;

	if (data [97] > threshold || data [98] > threshold)
		temp.Hz1500 = true;
	else
		temp.Hz1500 = false;
	return temp;
}

/*
static FrequencyStruct FrequencyDetermination (uint32_t *data) { //5999
	FrequencyStruct temp;
	if (data [21] > 130 || data [22] > 130)
		temp.Hz500 = true;
	else
		temp.Hz500 = false;

	if (data [43] > 75 || data [43] > 75)
		temp.Hz1000 = true;
	else
		temp.Hz1000 = false;

	if (data [65] > 200 || data [66] > 200)
		temp.Hz1500 = true;
	else
		temp.Hz1500 = false;
	return temp;
}
*/
/*

*/
static SearchSinusStruct SearchSinusMaxMagnitude (uint32_t *data) {
	SearchSinusStruct temp = {0};
	uint32_t max = 0;
	uint16_t freq = 0;
	for (uint16_t i  = startFrequency; i < finishFrequency; i++) {
			if (max < data[i]){
				max = data[i];
				freq = i;
		}
	}
	temp.frequency = freq;
	if (freq != 0)
		temp.Magnitude = data [freq];
	return temp;
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


static void SinusSignalParametr (FFT_InitStruct *data) {
	uint32_t maxbuff=0, jbuff = 0;
	for (uint16_t j = 23; j < 90; j++){
		if (maxbuff < data->MagArray[j]){
			maxbuff = data->MagArray[j];
			jbuff = j;
		}
	}
	SinFreq = jbuff;
	SinMag = data->MagArray[SinFreq];

}


static void Norm (uint32_t *data) {
	uint32_t max = 0;
	uint16_t i;
	for (i  = startFrequency; i < finishFrequency; i++) {
			if (max < data[i]){
				max = data[i];
		}
	}
	for (i  = startFrequency; i < finishFrequency; i++) {
		data[i] =  (data[i] * MaxNormMag) / max ;
	}
}


static void SearchFreq (uint32_t *data) {
	freq500 =  data[21];
	/*
	if (data[31] > data[24])
		freq500 =  data[23];
		else
			freq500 =  data[24];

	if (data[42] > data[43])
		freq1000 =  data[42];
		else
			freq1000 =  data[43];

	if (data[64] > data[65])
		freq1500 =  data[64];
		else
			freq1500 =  data[65];
			*/

}

#endif
