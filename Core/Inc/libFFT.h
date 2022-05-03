#ifndef __LIBFFT_H__
#define __LIBFFT_H__

#include "main.h"
#include "stdbool.h"

//#define DEBUG_FFT

//#define FFT_1024_NPT
#define FFT_256_NPT
//#define FFT_64_NPT

#ifdef FFT_1024_NPT
	#define NPT 1024
#endif

#ifdef FFT_256_NPT
	#define NPT 256
	typedef enum {
		position500Hz = 16U,
		position1000Hz = 32U
	}PositioinFrequencyEnum;

	typedef enum {
		Detect500Hz = 50,
		Detect1000Hz = 45
	}FrequencyDetectionLevelEnum;

#endif

#ifdef FFT_64_NPT
	#define NPT 64
#endif

typedef struct {
	int32_t InArray [NPT];
	int32_t OutArray [NPT];
	uint32_t MagArray [NPT/2];
	bool UpdateFlag;
} FFT_InitStruct;

typedef struct {
	uint32_t Hz500, Hz1000;
	uint32_t cycleCounter;
}FrequencyStruct;




#ifdef DEBUG_FFT
typedef struct {
	uint16_t frequency;
	uint32_t Magnitude;
}SearchSinusStruct;
#endif
extern void libFFTInit (void);
extern void libFFTmain (FFT_InitStruct *data);
extern FFT_InitStruct FFT_data;

#endif

