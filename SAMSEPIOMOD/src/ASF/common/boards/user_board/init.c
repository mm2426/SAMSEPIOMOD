/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "compiler.h"
#include "board.h"
#include "conf_board.h"
#include "gpio.h"
#include "ioport.h"


void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	 //Configure GPIO LEDs
	 //Configure Digital Inputs
	 
	 //Configure RS485
	 #ifndef RS485_USART
		#define CONF_UART0_PINS		1
		#define RS485_USART			USART0
		#define RS485_USART_ID		ID_USART0
		#define RS485_BAUDRATE		9600
		#define RS485_USART_Handler	USART0_Handler
		#define RS485_USART_IRQn	USART0_IRQn
	 #endif
	 #if defined(CONF_UART0_PINS)
		gpio_configure_group(PINS_USART0_PIO, PINS_USART0, PINS_USART0_FLAGS);
	 #else
		gpio_configure_group(PINS_USART1_PIO, PINS_USART1, PINS_USART1_FLAGS);
	 #endif

	 //Configure other services. 
}

