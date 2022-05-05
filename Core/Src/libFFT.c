#include "libFFT.h"
#include "stm32_dsp.h"
#include "math.h"

FFT_InitStruct FFT_data; //structure for calculating the fast Fourier transform
FrequencyStruct FrequencyDetectorData; // structure for storage of detected frequencies

#ifdef DEBUG_FFT
	SearchSinusStruct foundSinusMax;
	static const uint16_t startFrequency = 18;
	static const uint16_t finishFrequency = NPT / 2;
	int32_t SignalSimulation [NPT];
	static void For_FFT_SignalSimulation(int32_t *arr, float Fd);
	static void SinusSignalParametr (FFT_InitStruct *data);
	static SearchSinusStruct SearchSinusMaxMagnitude (uint32_t *data);
	static void Norm (uint32_t *data);
#endif

static inline void CalcPowerMag(FFT_InitStruct *data);
static void FrequencyDetector (uint32_t *data, FrequencyStruct *Frequency);

void libFFTInit (void) {
	FFT_data.UpdateFlag = false;
	MeasurementsClear(&FrequencyDetectorData);
}

void libFFTmain (FFT_InitStruct *data) {
	if (data->UpdateFlag == true){
		#if NPT == 1024
			cr4_fft_1024_stm32(data->OutArray, data->InArray, NPT);
		#elif NPT == 256
			cr4_fft_256_stm32(data->OutArray, data->InArray, NPT);
		#elif NPT == 64
			cr4_fft_64_stm32(data->OutArray, data->InArray, NPT);
		#endif
		CalcPowerMag (data); //magnitude calculation
		FrequencyDetector (data->MagArray, &FrequencyDetectorData); // Detecting frequencies in 500Hz and 1000 Hz
		data->UpdateFlag = false; //The data is processed
	}
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
}

static void FrequencyDetector (uint32_t *data, FrequencyStruct *Frequency) {
	if (data [position500Hz] > Detect500Hz)
		Frequency->Hz500++;
	if (data [position1000Hz] > Detect1000Hz)
		Frequency->Hz1000++;
#ifdef DEBUG_FFT
		Frequency->cycleCounter++;
#endif
}

void MeasurementsClear (FrequencyStruct *DataFrequency){
	DataFrequency->Hz500 = 0;
	DataFrequency->Hz1000 = 0;
#ifdef DEBUG_FFT
	DataFrequency->cycleCounter = 0;
#endif
}

#ifdef DEBUG_FFT
static void For_FFT_SignalSimulation(int32_t *arr, float Fd)
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

static void Norm (uint32_t *data) {
	uint32_t max = 0;
	uint16_t i;
	for (i  = startFrequency; i < finishFrequency; i++) {
			if (max < data[i]){
				max = data[i];
		}
	}
	for (i  = startFrequency; i < finishFrequency; i++) {
		data[i] =  (data[i] * 1000) / max ;
	}
}

#endif
