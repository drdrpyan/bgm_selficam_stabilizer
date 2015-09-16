#include "term_usart.h"

#include "os.h"

static OS_FLAG_GRP USART_input_flag;

void TERM_USART_initUSART(void)
{
  OS_ERR err;
  OSFlagCreate(&USART_input_flag, "usart input flag", (OS_FLAGS)0, &err);
}

void TERM_USART_isrHandlerHook(void)
{
  OS_ERR err;
  OSFlagPost(&USART_input_flag, (OS_FLAGS)0x01, OS_OPT_POST_FLAG_SET, &err);
}

void TERM_USART_waitInput(void)
{
  OS_ERR err;
  CPU_TS ts;
  OSFlagPend(&USART_input_flag, (OS_FLAGS)0x01, 0,
             OS_OPT_PEND_BLOCKING + OS_OPT_PEND_FLAG_SET_ANY +
             OS_OPT_PEND_FLAG_CONSUME,
             &ts,
             &err);
}