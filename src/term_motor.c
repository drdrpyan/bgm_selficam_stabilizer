#include "term_motor.h"

#include "term_accsensor.h"

CPU_INT16U TERM_sensorXtoMotorX(CPU_INT16U sensor_value)
{
  CPU_INT16U result = sensor_value;
  
  if(result < TERM_SENSOR_X_MIN)
    result = TERM_SENSOR_X_MIN;
  else if(result > TERM_SENSOR_X_MAX)
    result = TERM_SENSOR_X_MAX;
  
  result -= TERM_SENSOR_X_MIN;
  //result = TERM_MOTOR_MAX -
  //  (TERM_MOTOR_MIN + (result * ((TERM_MOTOR_RANGE * 2) / TERM_SENSOR_X_RANGE)) / 2);
  result = TERM_MOTOR_MIN + (result * TERM_MOTOR_RANGE / TERM_SENSOR_X_RANGE);
  
  return result;
}

CPU_INT16U TERM_sensorYtoMotorY(CPU_INT16U sensor_value)
{
  CPU_INT16U result = sensor_value;
  
  if(result < TERM_SENSOR_Y_MIN)
    result = TERM_SENSOR_Y_MIN;
  else if(result > TERM_SENSOR_Y_MAX)
    result = TERM_SENSOR_Y_MAX;
  
  result -= TERM_SENSOR_Y_MIN;
  //result = TERM_MOTOR_MAX -
  //  (TERM_MOTOR_MIN + (result * ((TERM_MOTOR_RANGE * 2) / TERM_SENSOR_Y_RANGE)) / 2);
  result = TERM_MOTOR_MIN + (result * TERM_MOTOR_RANGE / TERM_SENSOR_Y_RANGE);
  
  return result;
}