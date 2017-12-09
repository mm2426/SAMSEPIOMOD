/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

/* USART0 Definitions*/
#define PINS_USART0_PIO		PIOA
/** USART0 pins (RXD0, TXD0 & RTS0(TXEN)) definitions, PA5,6,7. */
#define PINS_USART0			(PIO_PA5A_RXD0 | PIO_PA6A_TXD0 | PIO_PA7A_RTS0)
#define PINS_USART0_FLAGS	(PIO_PERIPH_A | PIO_DEFAULT)

/* USART1 Definitions*/
#define PINS_USART1_PIO		PIOA
/** USART1 pins (RXD1, TXD1 & RTS1(TXEN)) definitions, PA21,22,24. */
#define PINS_USART1			(PIO_PA21A_RXD1 | PIO_PA22A_TXD1 | PIO_PA24A_RTS1)
#define PINS_USART1_FLAGS	(PIO_PERIPH_A | PIO_DEFAULT)

/* TWI0 Definitions*/
#define PINS_TWI0_PIO		PIOA
/* TWI0 pins (TWCK0 & TWD0) definitions, PA4, 3. */
#define PINS_TWI0			(PIO_PA4A_TWCK0 | PIO_PA3A_TWD0)
#define PINS_TWI0_FLAGS		(PIO_PERIPH_A | PIO_DEFAULT)

/* Digital Mux Select Lines */
#define PIN_MUXSEL0_IDX		PIO_PA5_IDX
#define PIN_MUXSEL1_IDX		PIO_PA6_IDX
#define PIN_MUXSEL2_IDX		PIO_PB4_IDX

/* Digital Mux Output Lines */
#define PIN_MUXOUT0_IDX		PIO_PA25_IDX
#define PIN_MUXOUT1_IDX		PIO_PA26_IDX
#define PIN_MUXOUT2_IDX		PIO_PA12_IDX
#define PIN_MUXOUT3_IDX		PIO_PA11_IDX

/* Analog Mux Select Lines */
#define PIN_AMUXSEL0_IDX	PIO_PB0_IDX
#define PIN_AMUXSEL1_IDX	PIO_PB1_IDX
#define PIN_AMUXSEL2_IDX	PIO_PB2_IDX

/* Debug Switch Pin */
#define PIN_DEBUGSW_IDX		PIO_PB3_IDX

/* Relay Control Pins */
#define PIN_RLY1_IDX		PIO_PA23_IDX
#define PIN_RLY2_IDX		PIO_PA20_IDX

/* Debug LED Pin */
#define PIN_DEBUGLED_IDX	PIO_PB5_IDX

#endif // USER_BOARD_H
