#ifndef __TERM_ADC__
#define __TERM_ADC__

#include <cpu.h>

void TERM_init_adc(void);
void TERM_get_sensor_value(CPU_INT16U* x_value, CPU_INT16U* y_value);

#endif