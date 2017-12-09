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

/* MODBUS Slave Address */
#define MBUS_SLA				0x01
/* MODBUS Register Indexes */
/* Digital Input Regs */
#define MBUS_REG_DI0	0
#define MBUS_REG_DI1	1
/* Analog Input Regs 0 - 10V */
#define MBUS_REG_AI0L	2
#define MBUS_REG_AI0H	3
#define MBUS_REG_AI1L	4
#define MBUS_REG_AI1H	5
/* Analog Input Regs 4 - 20mA */
#define MBUS_REG_AI2L	6
#define MBUS_REG_AI2H	7
#define MBUS_REG_AI3L	8
#define MBUS_REG_AI3H	9
/* Digital Output Regs */
#define MBUS_REG_DO1	10
/* Slave Address Selection Reg */
#define MBUS_REG_SLA	11

#define SBI(a,b)\
	a |= (1<<b)

#define CBI(a,b)\
	a &= ~(1<<b)

#define BIT_IS_SET(a,b)\
	(a&(1<<b))

#include <asf.h>
#include "user_board.h"
#include "conf_clock.h"
#include "rs485pdc.h"
#include "mbusStack.h"
#include "mcp3425.h"

//MODBUS Register Values
extern uint16_t mBusRegs[];
extern Pdc *rs485PdcBase;
pdc_packet_t pdcPkt;

enum analogStates
{
	anaRead0, anaRead1, anaRead2, anaRead3, anaBusy
}anaState, anaNextState;

//volatile rxstates uart0State = idle;

/* RS485 Rx buffer */
uint8_t rs485RxBuffer[RS485_BUFFER_SIZE];

void InitPeripherals(void);
void InitTWI(void);
void WriteMbusRegs(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len);
void SendRespPkt(uint8_t *pkt, uint8_t len);
void ReadAnalogIn(void);
void ReadDigitalIn(void);

int main (void)
{
	/* RS485 Tx buffer */
	uint8_t respBuff[RS485_BUFFER_SIZE], respLen = 0;
	uint8_t commState = 0, recvdBytes = 0, prevRecvdBytes;
	/* Init Mbus writeFunc() ptr */
	writeFunc = WriteMbusRegs;

	anaState = anaRead0;
	
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	/* Initialize all peripheral pins */
	board_init();	
	delay_init(sysclk_get_cpu_hz());
	
	InitPeripherals();
	
	//LED Toggle
	//gpio_toggle_pin(PIO_PC23_IDX);
	
	/* Start Receiving Data */
	usart_disable_tx(RS485_USART);
	usart_enable_rx(RS485_USART);
	Rs485PdcStartRx((uint32_t)rs485RxBuffer,RS485_BUFFER_SIZE);
	
	while(1)	
	{
		recvdBytes = Rs485PdcGetRxBytes();
		/* Check if data received on 485 */
		switch(commState)
        {
            case 0:
                if(recvdBytes!=0)
                {
                    commState = 1;
                    prevRecvdBytes = recvdBytes;
                }
                break;
            case 1:
                if(prevRecvdBytes != recvdBytes)
                {
                    prevRecvdBytes = recvdBytes;
                }
                else
                {
                    /* 1 Modbus Pkt received. Process it. */
					usart_disable_rx(RS485_USART);
                    pdc_disable_transfer(rs485PdcBase, PERIPH_PTCR_RXTEN);

					/* Parse Received MBUS Pkt, update actual variables */
					ParseMbusPkt(MBUS_SLA, rs485RxBuffer, recvdBytes, respBuff, &respLen);    
                        
                    /* Generate MBus Resp */
                    if(respLen)
                    {
						SendRespPkt(respBuff, respLen);
                    }
                        
					usart_enable_rx(RS485_USART);
					pdc_enable_transfer(rs485PdcBase, PERIPH_PTCR_RXTEN);
                    Rs485PdcStartRx((uint32_t)rs485RxBuffer,RS485_BUFFER_SIZE);

                    commState = 0;
                }
                break;
        }

		/* Update digital input regs */
		ReadDigitalIn();
		/* Update analog input regs */
		ReadAnalogIn();
	}
}

void InitPeripherals(void)
{
	/* Init RS485 port with PDC support */
	InitRs485Pdc();
	
	/* Init TWI */
	InitTWI();

	/* Init I/O Expander */

	/* Read Config from EEPROM */

	/* Set AMUX sel pin to low */
	gpio_set_pin_low(PIN_AMUXSEL2_IDX);
}

void InitTWI(void)
{
	twi_options_t twiSettings = {
		sysclk_get_peripheral_hz(),
		400000,
		0,
		0
	};
	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(BOARD_TWI_ID);

	/* Enable TWI master mode */
	twi_enable_master_mode(BOARD_TWI);

	/* Initialize TWI peripheral */
	twi_master_init(BOARD_TWI, &twiSettings);
}

void WriteMbusRegs(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len)
{
	uint16_t *ptr;
	
	while(len)
	{
		switch(regAddr)
		{
			case MBUS_REG_DO1:
				//Write digital output on pins
				break;
			default:
				break;
		}
		regAddr++;
		len--;
	}
}

void SendRespPkt(uint8_t *pkt, uint8_t len)
{
	usart_enable_tx(RS485_USART);
	/* 3.5 Char Wait Time */
	
	/* Start PDC Transfer */
	Rs485PdcStartTx((uint32_t)pkt, len);

	/* Wait till tx completes */
	while(!Rs485PdcGetTxStatus());

	/* 3.5 Char Wait Time */

	usart_disable_tx(RS485_USART);
}

void ReadAnalogIn(void)
{
	/* Read 0-10V, 4-20mA */
	float anaV = 0;
	uint16_t anaVal = 0;

	static uint8_t recvdData[3];
	
	switch(anaState)
	{
		case anaRead0:
			/* Store recvd data */
			anaVal = (((uint16_t)recvdData[0])<<8)|(recvdData[1]);
			/* Set Mux Select lines to channel 0 */
			gpio_set_pin_low(PIN_AMUXSEL0_IDX);
			gpio_set_pin_low(PIN_AMUXSEL1_IDX);
			delay_ms(1);
			anaNextState = anaRead1;
			anaState = anaBusy;
			/* Initiate ADC reading */
			WriteMCP3425(BOARD_TWI,(1<<MCP_BIT_CONF_RDY)|(1<<MCP_BIT_CONF_S1));
			break;
		case anaRead1:
			/* Store recvd data */
			anaVal = (((uint16_t)recvdData[0])<<8)|(recvdData[1]);
			/* Set Mux Select lines to channel 1 */
			gpio_set_pin_high(PIN_AMUXSEL0_IDX);
			gpio_set_pin_low(PIN_AMUXSEL1_IDX);
			delay_ms(1);
			anaNextState = anaRead2;
			anaState = anaBusy;
			/* Initiate ADC reading */
			WriteMCP3425(BOARD_TWI,(1<<MCP_BIT_CONF_RDY)|(1<<MCP_BIT_CONF_S1));
			break;
		case anaRead2:
			/* Store recvd data */
			anaVal = (((uint16_t)recvdData[0])<<8)|(recvdData[1]);
			/* Set Mux Select lines to channel 2 */
			gpio_set_pin_low(PIN_AMUXSEL0_IDX);
			gpio_set_pin_high(PIN_AMUXSEL1_IDX);
			delay_ms(1);
			anaNextState = anaRead3;
			anaState = anaBusy;
			/* Initiate ADC reading */
			WriteMCP3425(BOARD_TWI,(1<<MCP_BIT_CONF_RDY)|(1<<MCP_BIT_CONF_S1));
			break;
		case anaRead3:
			/* Store recvd data */
			anaVal = (((uint16_t)recvdData[0])<<8)|(recvdData[1]);
			/* Set Mux Select lines to channel 3 */
			gpio_set_pin_high(PIN_AMUXSEL0_IDX);
			gpio_set_pin_high(PIN_AMUXSEL1_IDX);
			delay_ms(1);
			anaNextState = anaRead0;
			anaState = anaBusy;
			/* Initiate ADC reading */
			WriteMCP3425(BOARD_TWI,(1<<MCP_BIT_CONF_RDY)|(1<<MCP_BIT_CONF_S1));
			break;
		case anaBusy:
			ReadMCP3425(BOARD_TWI, recvdData);
			if (!BIT_IS_SET(recvdData[2],MCP_BIT_CONF_RDY))
			{
				anaState = anaNextState;
			}
			break;
	}
}

void ReadDigitalIn(void)
{
	static uint8_t i = 0;
	switch(i)
	{
		case 0:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],0);
			else
				CBI(mBusRegs[MBUS_REG_DI0],0);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],8);
			else
				CBI(mBusRegs[MBUS_REG_DI0],8);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],0);
			else
				CBI(mBusRegs[MBUS_REG_DI1],0);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],8);
			else
				CBI(mBusRegs[MBUS_REG_DI1],8);

			gpio_set_pin_high(PIN_MUXSEL0_IDX);
			gpio_set_pin_low(PIN_MUXSEL1_IDX);
			gpio_set_pin_low(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 1:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],1);
			else
				CBI(mBusRegs[MBUS_REG_DI0],1);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],9);
			else
				CBI(mBusRegs[MBUS_REG_DI0],9);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],1);
			else
				CBI(mBusRegs[MBUS_REG_DI1],1);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],9);
			else
				CBI(mBusRegs[MBUS_REG_DI1],9);

			gpio_set_pin_low(PIN_MUXSEL0_IDX);
			gpio_set_pin_high(PIN_MUXSEL1_IDX);
			gpio_set_pin_low(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 2:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],2);
			else
				CBI(mBusRegs[MBUS_REG_DI0],2);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],10);
			else
				CBI(mBusRegs[MBUS_REG_DI0],10);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],2);
			else
				CBI(mBusRegs[MBUS_REG_DI1],2);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],10);
			else
				CBI(mBusRegs[MBUS_REG_DI1],10);

			gpio_set_pin_high(PIN_MUXSEL0_IDX);
			gpio_set_pin_high(PIN_MUXSEL1_IDX);
			gpio_set_pin_low(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 3:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],3);
			else
				CBI(mBusRegs[MBUS_REG_DI0],3);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],11);
			else
				CBI(mBusRegs[MBUS_REG_DI0],11);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],3);
			else
				CBI(mBusRegs[MBUS_REG_DI1],3);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],11);
			else
				CBI(mBusRegs[MBUS_REG_DI1],11);

			gpio_set_pin_low(PIN_MUXSEL0_IDX);
			gpio_set_pin_low(PIN_MUXSEL1_IDX);
			gpio_set_pin_high(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 4:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],4);
			else
				CBI(mBusRegs[MBUS_REG_DI0],4);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],12);
			else
				CBI(mBusRegs[MBUS_REG_DI0],12);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],4);
			else
				CBI(mBusRegs[MBUS_REG_DI1],4);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],12);
			else
				CBI(mBusRegs[MBUS_REG_DI1],12);
			
			gpio_set_pin_high(PIN_MUXSEL0_IDX);
			gpio_set_pin_low(PIN_MUXSEL1_IDX);
			gpio_set_pin_high(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 5:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],5);
			else
				CBI(mBusRegs[MBUS_REG_DI0],5);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],13);
			else
				CBI(mBusRegs[MBUS_REG_DI0],13);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],5);
			else
				CBI(mBusRegs[MBUS_REG_DI1],5);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],13);
			else
				CBI(mBusRegs[MBUS_REG_DI1],13);
			
			gpio_set_pin_low(PIN_MUXSEL0_IDX);
			gpio_set_pin_high(PIN_MUXSEL1_IDX);
			gpio_set_pin_high(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 6:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],6);
			else
				CBI(mBusRegs[MBUS_REG_DI0],6);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],14);
			else
				CBI(mBusRegs[MBUS_REG_DI0],14);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],6);
			else
				CBI(mBusRegs[MBUS_REG_DI1],6);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],14);
			else
				CBI(mBusRegs[MBUS_REG_DI1],14);

			gpio_set_pin_high(PIN_MUXSEL0_IDX);
			gpio_set_pin_high(PIN_MUXSEL1_IDX);
			gpio_set_pin_high(PIN_MUXSEL2_IDX);
			i++;
			break;
		case 7:
			if(gpio_pin_is_high(PIN_MUXOUT0_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],7);
			else
				CBI(mBusRegs[MBUS_REG_DI0],7);

			if(gpio_pin_is_high(PIN_MUXOUT1_IDX))
				SBI(mBusRegs[MBUS_REG_DI0],15);
			else
				CBI(mBusRegs[MBUS_REG_DI0],15);
			
			if(gpio_pin_is_high(PIN_MUXOUT2_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],7);
			else
				CBI(mBusRegs[MBUS_REG_DI1],7);

			if(gpio_pin_is_high(PIN_MUXOUT3_IDX))
				SBI(mBusRegs[MBUS_REG_DI1],15);
			else
				CBI(mBusRegs[MBUS_REG_DI1],15);
			
			i = 0;
			gpio_set_pin_low(PIN_MUXSEL0_IDX);
			gpio_set_pin_low(PIN_MUXSEL1_IDX);
			gpio_set_pin_low(PIN_MUXSEL2_IDX);
			break;
		default:
			i = 0;
			gpio_set_pin_low(PIN_MUXSEL0_IDX);
			gpio_set_pin_low(PIN_MUXSEL1_IDX);
			gpio_set_pin_low(PIN_MUXSEL2_IDX);
			break;
	}
}