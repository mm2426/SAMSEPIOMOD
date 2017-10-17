/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

//Use USART0 for RS485 communication
#define CONF_UART1_PINS		1
#define RS485_USART			USART1
#define RS485_USART_ID		ID_USART1
#define RS485_BAUDRATE		9600
#define RS485_USART_Handler	USART1_Handler
#define RS485_USART_IRQn	USART1_IRQn


#endif // CONF_BOARD_H
