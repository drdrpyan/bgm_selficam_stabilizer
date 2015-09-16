#include "term_task.h"

#include <os.h>
#include <cpu.h>
#include "bsp.h"

#include "term_adc.h"
#include "term_accsensor.h"
#include "term_pwm.h"
#include "term_motor.h"


#define TERM_STKSIZE_SENSOR 128
#define TERM_PRIO_SENSOR 5
static OS_TCB TERM_TCB_sensor;
static CPU_STK TERM_STK_sensor[TERM_STKSIZE_SENSOR];

#define TERM_STKSIZE_MOTOR_X 128
#define TERM_PRIO_MOTOR_X 4
static OS_TCB TERM_TCB_motor_x;
static CPU_STK TERM_STK_motor_x[TERM_STKSIZE_MOTOR_X];
static CPU_INT16U acc_sensor_x;

#define TERM_STKSIZE_MOTOR_Y 128
#define TERM_PRIO_MOTOR_Y 4
static OS_TCB TERM_TCB_motor_y;
static CPU_STK TERM_STK_motor_y[TERM_STKSIZE_MOTOR_Y];
static CPU_INT16U acc_sensor_y;

#define TERM_STKSIZE_USART 128
#define TERM_PRIO_USART 6
static OS_TCB TERM_TCB_usart;
static CPU_STK TERM_STK_usart[TERM_STKSIZE_USART];

#define TERM_STKSIZE_BUTTON 128
#define TERM_PRIO_BUTTON 3
static OS_TCB TERM_TCB_button;
static CPU_STK TERM_STK_button[TERM_STKSIZE_BUTTON];


/////////SEMAPHORE///////////
//OS_SEM Semaphore_x;
//static  CPU_INT16U      Bias_Y;
//OS_SEM SEM_bias_x;
static OS_SEM SEM_buttonISR;
static OS_SEM SEM_bias_y;
static CPU_INT16U bias_y = 0;
//////////SEMAPHORE//////////
static  CPU_INT16U Bias_X=0;


void TERM_createTask(void)
{
    OS_ERR  err;
    
    //OSSemCreate(&SEM_bias_x, "for bias_x sharing", 1, &err);
    OSSemCreate(&SEM_bias_y, "for bias_y sharing", 1, &err);
    
    OSTaskCreate((OS_TCB     *)&TERM_TCB_sensor, 
                 (CPU_CHAR   *)"task polling sensor",
                 (OS_TASK_PTR )TERM_TASK_sensor, 
                 (void       *)0,
                 (OS_PRIO     )TERM_PRIO_SENSOR,
                 (CPU_STK    *)&TERM_STK_sensor[0],
                 (CPU_STK_SIZE)TERM_STKSIZE_SENSOR / 10,
                 (CPU_STK_SIZE)TERM_STKSIZE_SENSOR,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    OSTaskCreate((OS_TCB     *)&TERM_TCB_motor_x, 
                 (CPU_CHAR   *)"motor x",
                 (OS_TASK_PTR )TERM_TASK_motor_x, 
                 (void       *)0,
                 (OS_PRIO     )TERM_PRIO_MOTOR_X,
                 (CPU_STK    *)&TERM_STK_motor_x[0],
                 (CPU_STK_SIZE)TERM_STKSIZE_MOTOR_X / 10,
                 (CPU_STK_SIZE)TERM_STKSIZE_MOTOR_X,
                 (OS_MSG_QTY  )2,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    OSTaskCreate((OS_TCB     *)&TERM_TCB_motor_y, 
                 (CPU_CHAR   *)"motor y",
                 (OS_TASK_PTR )TERM_TASK_motor_y, 
                 (void       *)0,
                 (OS_PRIO     )TERM_PRIO_MOTOR_Y,
                 (CPU_STK    *)&TERM_STK_motor_y[0],
                 (CPU_STK_SIZE)TERM_STKSIZE_MOTOR_Y / 10,
                 (CPU_STK_SIZE)TERM_STKSIZE_MOTOR_Y,
                 (OS_MSG_QTY  )2,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
     OSTaskCreate((OS_TCB     *)&TERM_TCB_button, 
                 (CPU_CHAR   *)"button task",
                 (OS_TASK_PTR )TERM_TASK_button, 
                 (void       *)0,
                 (OS_PRIO     )TERM_PRIO_BUTTON,
                 (CPU_STK    *)&TERM_STK_usart[0],
                 (CPU_STK_SIZE)TERM_STKSIZE_BUTTON / 10,
                 (CPU_STK_SIZE)TERM_STKSIZE_BUTTON,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    //OSTaskCreate((OS_TCB     *)&TERM_TCB_usart, 
    //            (CPU_CHAR   *)"usart task",
    //           (OS_TASK_PTR )TERM_TASK_usart, 
    //             (void       *)0,
    //             (OS_PRIO     )TERM_PRIO_USART,
    //             (CPU_STK    *)&TERM_STK_usart[0],
    //             (CPU_STK_SIZE)TERM_STKSIZE_USART / 10,
    //             (CPU_STK_SIZE)TERM_STKSIZE_USART,
    //             (OS_MSG_QTY  )0,
    //             (OS_TICK     )0,
    //             (void       *)0,
    //             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
    //             (OS_ERR     *)&err);              
    
}

void TERM_TASK_sensor(void *p_arg)
{
  OS_ERR err;

  while(DEF_ON)
  {
    TERM_get_sensor_value(&acc_sensor_x, &acc_sensor_y);
    //acc_sensor_x=TERM_get_accsensor_x();
    OSTaskQPost((OS_TCB *)&TERM_TCB_motor_x, 		
	  (void *)acc_sensor_x,
	  (OS_MSG_SIZE)sizeof(acc_sensor_x),
	  (OS_OPT )OS_OPT_POST_FIFO,
	  (OS_ERR *)&err);    
    
    //acc_sensor_y=TERM_get_accsensor_y();
    OSTaskQPost((OS_TCB *)&TERM_TCB_motor_y, 		
      (void *)acc_sensor_y,
      (OS_MSG_SIZE)sizeof(acc_sensor_y),
      (OS_OPT )OS_OPT_POST_FIFO,
      (OS_ERR *)&err);
       
    OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_STRICT, &err);    
  }
}

void TERM_TASK_motor_x(void *p_arg)
{
      
      OS_ERR err;
      OS_MSG_SIZE size;
      CPU_TS ts;
      
      CPU_INT16U x_input, prev_x_input = TERM_MOTOR_MIDDLE;
      CPU_INT16U pulse;
      
     while (DEF_ON) {
      
       //sensor의 값을 기다린다.
       x_input = (CPU_INT16U)OSTaskQPend(0,  
		           (OS_OPT )OS_OPT_PEND_BLOCKING,
		           (OS_MSG_SIZE *)&size,
		           (CPU_TS *)&ts,
                           (OS_ERR *)&err);
       pulse = TERM_sensorXtoMotorX(x_input);
       //sensor의 값을 안정화
       if(x_input - prev_x_input < 20
          && x_input - prev_x_input > -20)
         x_input = prev_x_input;
       prev_x_input = x_input;
       
       
       
       
       //모터의 각도 설정
       TERM_PWM_PULSE_X = TERM_MOTOR_MAX + TERM_MOTOR_MIN - pulse;
       
       BSP_Ser_Printf("\n receive x : %d, pulse : %d ccr1 : %d\n\n", x_input, pulse, TERM_PWM_PULSE_X);
       //BSP_Ser_Printf("\n BIAS : %d", Bias_X);
     }
}


void TERM_TASK_motor_y(void *p_arg)
{
      
      OS_ERR err;
      OS_MSG_SIZE size;
      CPU_TS ts; 
      
      CPU_INT16U y_input, prev_y_input = TERM_MOTOR_MIDDLE;
      CPU_INT16U pulse;
      CPU_INT16U bias = 0;
      
     while (DEF_ON) {

       y_input = (CPU_INT16U)OSTaskQPend(0,  
		           (OS_OPT )OS_OPT_PEND_BLOCKING,
		           (OS_MSG_SIZE *)&size,
		           (CPU_TS *)&ts,
                           (OS_ERR *)&err);
       if(y_input - prev_y_input < 20
          && y_input - prev_y_input > -20)
         y_input = prev_y_input;
       prev_y_input = y_input;
       
       pulse = TERM_sensorYtoMotorY(y_input);
       
       //bias의 값을 가져온다.
       OSSemPend(&SEM_bias_y, 10, OS_OPT_PEND_NON_BLOCKING, &ts, &err);
       switch(err){
       case OS_ERR_NONE:
          bias = bias_y;
          OSSemPost(&SEM_bias_y, OS_OPT_POST_1, &err);
          break;
        }
       
       TERM_PWM_PULSE_Y = bias + TERM_MOTOR_MAX + TERM_MOTOR_MIN - pulse;
       
       BSP_Ser_Printf("\n receive y : %d, pulse : %d ccr2 : %d\n\n", y_input, pulse, TERM_PWM_PULSE_Y);
       BSP_Ser_Printf("\n BIAS : %d", bias);
    }
}

void ButtonISRHandler(void) {
  OS_ERR err;
  OS_SEM_CTR ctr;
  ctr = OSSemPost(&SEM_buttonISR, OS_OPT_POST_1, &err);
  EXTI_ClearITPendingBit(EXTI_Line0);
}


void TERM_TASK_button(void *p_arg)
{
  
  OS_ERR err;
  CPU_TS ts;
  
  //init button
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  
  BSP_PeriphEn(BSP_PERIPH_ID_IOPA);             
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  BSP_IntVectSet(BSP_INT_ID_EXTI0, ButtonISRHandler);
  BSP_IntEn(BSP_INT_ID_EXTI0);
  
  OSSemCreate(&SEM_buttonISR, "button isr", 0, &err);
  
  while (DEF_TRUE) {
    OSSemPend(&SEM_buttonISR, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    
    
    OSSemPend(&SEM_bias_y, 0, OS_OPT_PEND_NON_BLOCKING, &ts, &err);
    switch(err){
    case OS_ERR_NONE:
      
      bias_y += 100;
      OSSemPost(&SEM_bias_y, OS_OPT_POST_1, &err);
      break;
    }
    
  }  
}

/*void TERM_TASK_usart(void *p_arg)
{
  OS_ERR err;
  CPU_TS ts;
  CPU_INT16U input;
  FlagStatus rxne_status;
  
  TERM_USART_initUSART();
  
  
  while(DEF_ON)
  { 
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
    TERM_USART_waitInput();
    input = BSP_Ser_RdByte();
    OSSemPend(&Semaphore_x, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    
    switch(err){
    case OS_ERR_NONE:
      
      Bias_X=input;
      OSSemPost(&Semaphore_x,
                OS_OPT_POST_1,
                &err);
      break;
    }

    
    if(state == 0)
    {
      switch(input)
      {
      case '0':
        state = 0;
        break;
      case '1':
        state = 1;
        break;
      }
      BSP_Ser_Printf("1:-60\n2:-30\n3:0\n4:30\n5:60\n");
    }
    else if(state == 1)
    {
      switch(input)
      {
      case '1':
        bias = -480;
        break;
      case '2':
        state = -240;
        break;
      case '3':
        state = 0;
        break;
      case '4':
        state = 240;
        break;
      case '5':
        state = 480;
        break;
      }
      state = 0;
      BSP_Ser_Printf("1:x rotate\n2: y rotate\n(1~2) : \n");
    }
    
  }
}*/