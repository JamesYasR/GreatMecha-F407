#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#define ucBuffSize 64

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern uint8_t ucTrans[ucBuffSize];
extern uint8_t ucRecei[ucBuffSize];
void USART1_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */