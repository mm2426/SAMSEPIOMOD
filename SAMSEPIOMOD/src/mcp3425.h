/*
 * mcp3425.h
 *
 * Created: 07-Dec-17 6:44:23 PM
 *  Author: Mahesh
 */ 


#ifndef __MCP3425_H__
#define __MCP3425_H__

#include <asf.h>
#include "user_board.h"

/* MCP3425 7 bit I2C Address */
#define ADDR_MCP3425		0x68

#define MCP_BIT_CONF_G0		0
#define MCP_BIT_CONF_G1		1
#define MCP_BIT_CONF_S0		2
#define MCP_BIT_CONF_S1		3
#define MCP_BIT_CONF_OC		4
#define MCP_BIT_CONF_RDY	7

void InitMCP3425(Twi * Port, uint8_t conf);
void WriteMCP3425(Twi * Port, uint8_t conf);
void ReadMCP3425(Twi * Port, uint8_t *dPkt);

#endif /* MCP3425_H_ */