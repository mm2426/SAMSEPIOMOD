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
	 
	 /* Disable the watchdog */
	 WDT->WDT_MR = WDT_MR_WDDIS;

	 /* Init IO Port service to enable configuring pins */
	 ioport_init();

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

	 //Configure TWI0 Pins
	 gpio_configure_group(PINS_TWI0_PIO, PINS_TWI0, PINS_TWI0_FLAGS);

	 //Configure Mux Sel Pins
	 gpio_configure_pin(PIN_MUXSEL0_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
	 gpio_configure_pin(PIN_MUXSEL1_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
	 gpio_configure_pin(PIN_MUXSEL2_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));

	 //Configure Mux Sel Pins
	 gpio_configure_pin(PIN_AMUXSEL0_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
	 gpio_configure_pin(PIN_AMUXSEL1_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
	 gpio_configure_pin(PIN_AMUXSEL2_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));

	 //Configure MUX Output Pins
	 gpio_configure_pin(PIN_MUXOUT0_IDX, (PIO_INPUT | PIO_DEFAULT));
	 gpio_configure_pin(PIN_MUXOUT1_IDX, (PIO_INPUT | PIO_DEFAULT));
	 gpio_configure_pin(PIN_MUXOUT2_IDX, (PIO_INPUT | PIO_DEFAULT));
	 gpio_configure_pin(PIN_MUXOUT3_IDX, (PIO_INPUT | PIO_DEFAULT));

	 //Configure Debug Switch Pin
	 gpio_configure_pin(PIN_DEBUGSW_IDX, (PIO_INPUT | PIO_DEFAULT));
	 
	 //Configure Debug LED Pin
	 gpio_configure_pin(PIN_DEBUGLED_IDX, (PIO_OUTPUT_0 | PIO_DEFAULT));
}

