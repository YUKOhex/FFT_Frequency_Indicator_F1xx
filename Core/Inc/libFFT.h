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
#endif

#ifdef FFT_64_NPT
	#define NPT 64
#endif

typedef struct {
	int32_t InArray [NPT];
	int32_t OutArray [NPT];
	uint32_t MagArray [NPT/2];
	bool UpdateMagFlag;
} FFT_InitStruct;

typedef struct {
	uint16_t Hz500, Hz1000, Hz1500;
}FrequencyStruct;

typedef struct {
	uint16_t frequency;
	uint32_t Magnitude;
}SearchSinusStruct;


#ifdef DEBUG_FFT
typedef struct {
	uint32_t magnitude;
	uint16_t frequency;
}MagParametrStruct;
#endif
extern void libFFT_Init (void);
extern void libFFT_main (FFT_InitStruct *data);
extern FFT_InitStruct FFT_data;

#endif

