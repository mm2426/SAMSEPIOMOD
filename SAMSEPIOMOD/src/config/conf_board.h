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
#define CONF_UART0_PINS		1
#define RS485_USART			USART0
#define RS485_USART_ID		ID_USART0
#define RS485_BAUDRATE		9600
#define RS485_USART_Handler	USART0_Handler
#define RS485_USART_IRQn	USART0_IRQn


#endif // CONF_BOARD_H
