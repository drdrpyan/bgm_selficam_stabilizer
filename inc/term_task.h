#ifndef __TERM_TASK__
#define __TERM_TASK__

//void TERM_init_semaphore(void);
void TERM_createTask(void);

void TERM_TASK_sensor(void *p_arg);
void TERM_TASK_pwm(void *p_arg);
void TERM_TASK_motor_x(void *p_arg);
void TERM_TASK_motor_y(void *p_arg);
void TERM_TASK_usart(void *p_arg);
void TERM_TASK_button(void *p_arg);
//void TERM_TASK_debug(void *p_arg);
  
#endif