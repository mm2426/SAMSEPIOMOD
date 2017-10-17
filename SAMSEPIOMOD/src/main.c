/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/** Size of the receive buffer and transmit buffer. */
#define RS485_BUFFER_SIZE		2000

#include <asf.h>
#include "user_board.h"
#include "conf_clock.h"
#include "rs485pdc.h"

/** RS485 buffers */
uint32_t rs485RxBuffer[RS485_BUFFER_SIZE];
uint32_t rs485TxBuffer[RS485_BUFFER_SIZE];

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	/* Initialize all peripherals */
	board_init();	
	
	InitRs485Pdc();
	gpio_configure_pin(PIO_PC23_IDX, (PIO_OUTPUT_1 | PIO_DEFAULT));

	rs485TxBuffer[0] = 'M';

	Rs485PdcStartRx(rs485RxBuffer, 1);
	Rs485PdcStartTx(rs485TxBuffer, 1);

	while(1)	
	{
		gpio_toggle_pin(PIO_PC23_IDX);
		delay_ms(1000);
	}
}
