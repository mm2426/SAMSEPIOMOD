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

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000

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

#endif // USER_BOARD_H
