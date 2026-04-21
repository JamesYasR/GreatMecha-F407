#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#define ucBuffSize 64
#define RECEIVED 1
#define UNRECEIVED 0


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern UART_HandleTypeDef huart6;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;

extern uint8_t ucTrans1[ucBuffSize];
extern uint8_t ucRecei1[ucBuffSize];

extern uint8_t ucTrans6[ucBuffSize];
extern uint8_t ucRecei6[ucBuffSize];

extern uint8_t ucBuffer1[ucBuffSize];
extern uint8_t ucBuffer6[ucBuffSize];
extern uint8_t IS_UART_RECEIVED[2];
extern uint16_t ucBuffer_len[2];

extern uint16_t uart_recNum[2];
extern uint16_t error_recNum[2];


void USART1_UART_Init(void);
void USART6_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */