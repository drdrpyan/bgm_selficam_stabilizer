#ifndef __TERM_PWM__
#define __TERM_PWM__

#define TERM_PWM_TIMER       TIM3
#define TERM_PWM_CCR_X       CCR1
#define TERM_PWM_CCR_Y       CCR2

#define TERM_PWM_PULSE_X     TERM_PWM_TIMER->TERM_PWM_CCR_X
#define TERM_PWM_PULSE_Y     TERM_PWM_TIMER->TERM_PWM_CCR_Y



void TERM_init_pwm(void);

#endif