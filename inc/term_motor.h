#ifndef __TERM_MOTOR__
#define __TERM_MOTOR__

#include <cpu.h>

#define TERM_MOTOR_MIN          600
#define TERM_MOTOR_MAX          2400
#define TERM_MOTOR_RANGE        (TERM_MOTOR_MAX - TERM_MOTOR_MIN)
#define TERM_MOTOR_MIDDLE       ((TERM_MOTOR_MAX+TERM_MOTOR_MIN)/2)

CPU_INT16U TERM_sensorXtoMotorX(CPU_INT16U sensor_value);
CPU_INT16U TERM_sensorYtoMotorY(CPU_INT16U sensor_value);

#endif