#ifndef __TERM_USART__
#define __TERM_USART__

void TERM_USART_initUSART(void);
void TERM_USART_waitInput(void);

void TERM_USART_isrHandlerHook(void);

#endif