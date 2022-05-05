
#ifndef __LIBADC_H
#define __LIBADC_H

#include "main.h"
#include "stdbool.h"

#define HTIM htim3 //timer to generate the sampling rate
#define HADC hadc1

void libADCInit (void);
void libADCMain (void);

#endif /* __MAIN_H */
