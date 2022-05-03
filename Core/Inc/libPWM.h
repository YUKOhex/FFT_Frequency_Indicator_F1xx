#ifndef LIBPWM_H_
#define LIBPWM_H_

#include "main.h"
#include "tim.h"
#include <stdbool.h>

#define PWM_HTIM htim4
#define PWM_CHANEL TIM_CHANNEL_4

typedef enum {
	t200ms = 11999U,
	t1000ms = 59999,
	t550ms = 32999U
}PeriodSettingsEnum;

typedef enum {
	tPulse100ms = 5999U,
	tPulse500ms = 29999U,
	tPulse = 2999U
}PulseSettingsEnum;

void libPWMInit (void);
void libPWMmain (void);

#endif
