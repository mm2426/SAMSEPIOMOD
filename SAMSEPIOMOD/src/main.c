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

#include <asf.h>
#include "user_board.h"
#include "conf_clock.h"
#include "rs485pdc.h"
#include "mbusStack.h"

//MODBUS Register Values
extern uint16_t mBusRegs[];
extern Pdc *rs485PdcBase;
pdc_packet_t pdcPkt;

//volatile rxstates uart0State = idle;

/* RS485 Rx buffer */
uint8_t rs485RxBuffer[RS485_BUFFER_SIZE];

void writeMbusRegs(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len);
void sendRespPkt(uint8_t *pkt, uint8_t len);
void ReadAnalogIn(void);
void ReadDigitalIn(void);

int main (void)
{
	/* RS485 Tx buffer */
	uint8_t respBuff[RS485_BUFFER_SIZE], respLen = 0;
	uint8_t commState = 0, recvdBytes = 0, prevRecvdBytes;
	/* Init Mbus writeFunc() ptr */
	writeFunc = writeMbusRegs;
	
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	/* Initialize all peripherals */
	board_init();	
	delay_init(sysclk_get_cpu_hz());
	
	/* Init RS485 port with PDC support */
	InitRs485Pdc();
	
	//LED PIN
	gpio_configure_pin(PIO_PC23_IDX, (PIO_OUTPUT_1 | PIO_DEFAULT));
	
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
					parseMbusPkt(MBUS_SLA, rs485RxBuffer, recvdBytes, respBuff, &respLen);    
                        
                    /* Generate MBus Resp */
                    if(respLen)
                    {
						sendRespPkt(respBuff, respLen);
                    }
                        
					usart_enable_rx(RS485_USART);
					pdc_enable_transfer(rs485PdcBase, PERIPH_PTCR_RXTEN);
                    Rs485PdcStartRx((uint32_t)rs485RxBuffer,RS485_BUFFER_SIZE);

                    commState = 0;
                }
                break;
        }

		//Read 32 input regs
		//Read 0-10V, 4-20mA
		delay_ms(20);
	}
}


void writeMbusRegs(uint16_t *mbusBuff, uint8_t regAddr, uint8_t len)
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

void sendRespPkt(uint8_t *pkt, uint8_t len)
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
	
}

void ReadDigitalIn(void)
{

}