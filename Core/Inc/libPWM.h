#ifndef LIBPWM_H_
#define LIBPWM_H_

#include "main.h"
#include "tim.h"
#include <stdbool.h>

#define PWM_HTIM htim4
#define PWM_CHANEL TIM_CHANNEL_4

typedef enum {
	t200ms_F500 = 11999U,
	t1000ms_F1000 = 59999U,
	t550ms_F500_F1000 = 32999U,
	tZero = 0U
}PeriodSettings;

enum PulseSettingsEnum {
	 t200ms_Pulse100ms = 5999U,
	 t1000ms_Pulse500ms = 29999U,
	 t550ms_Pulse50ms = 2999U
};

void libPWMmainLEDBlink (void);

#endif
