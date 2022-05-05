#ifndef __LIBFFT_H__
#define __LIBFFT_H__

#include "main.h"
#include "stdbool.h"

//#define DEBUG_FFT


/*choose one of the options that have been implemented*/

//#define FFT_1024_NPT // unrealized
//#define FFT_256_NPT // implemented
#define FFT_64_NPT // implemented

/******************************************************/

#ifdef FFT_1024_NPT
	#define NPT 1024
#endif

#ifdef FFT_256_NPT
	#define NPT 256
	#define PERIOD_TIM3_INIT 8999U

	enum PositioinFrequency{
		position500Hz = 16U,
		position1000Hz = 32U
	};

	enum FrequencyDetectionLevel {
		Detect500Hz = 55U,
		Detect1000Hz = 45U
	};
#endif

#ifdef FFT_64_NPT
	#define NPT 64
	#define PERIOD_TIM3_INIT 11999U

	enum PositioinFrequency{
		position500Hz = 5U,
		position1000Hz = 11U
	};

	enum FrequencyDetectionLevel {
		Detect500Hz = 65,
		Detect1000Hz = 70U
	};
#endif

typedef struct {
	int32_t InArray [NPT];
	int32_t OutArray [NPT];
	uint32_t MagArray [NPT/2];
	bool UpdateFlag;
} FFT_InitStruct;

typedef struct {
	uint32_t Hz500, Hz1000;
#ifdef DEBUG_FFT
	uint32_t cycleCounter;
#endif
}FrequencyStruct;

#ifdef DEBUG_FFT
typedef struct {
	uint16_t frequency;
	uint32_t Magnitude;
}SearchSinusStruct;
#endif

extern FFT_InitStruct FFT_data;
extern FrequencyStruct FrequencyDetectorData;
extern void libFFTInit (void);
extern void libFFTmain (FFT_InitStruct *data);
void MeasurementsClear (FrequencyStruct *DataFrequency);

#endif

