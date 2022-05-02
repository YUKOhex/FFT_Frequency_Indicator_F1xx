#ifndef __LIBFFT_H__
#define __LIBFFT_H__

#include "main.h"
#define DEBUG_FFT

#define FFT_1024_NPT
//#define FFT_256_NPT
//#define FFT_64_NPT

#ifdef FFT_1024_NPT
	#define NPT 1024
#endif

#ifdef FFT_256_NPT
	#define NPT 256
#endif

#ifdef FFT_64_NPT
	#define NPT 1024
#endif



#endif

