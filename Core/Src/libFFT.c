#include "libFFT.h"
#include "stm32_dsp.h"
#include "math.h"


#ifdef DEBUG_FFT
	int32_t SignalSimulation [NPT];
	static inline void For_FFT_SignalSimulation(int32_t *arr, float Fd);
#endif




























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
