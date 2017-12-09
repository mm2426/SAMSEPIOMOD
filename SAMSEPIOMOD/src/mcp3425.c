/*
 * mcp3425.c
 *
 * Created: 07-Dec-17 6:44:02 PM
 *  Author: Mahesh
 */ 

 #include "mcp3425.h"

 void InitMCP3425(Twi *Port, uint8_t conf)
 {
	WriteMCP3425(Port, conf);
 }
 
 void WriteMCP3425(Twi *Port, uint8_t conf)
 {
	twi_packet_t pkt;
	/* Set Device Address */
	pkt.chip = ADDR_MCP3425;
	/* No Address Bytes to be clocked */
	pkt.addr_length = 0;
	/* Address of data to be transferred */
	pkt.buffer = &conf;
	/* No of bytes to transfer */
	pkt.length = 1;
	twi_master_write(Port, &pkt);
 }

 void ReadMCP3425(Twi * Port, uint8_t *dPkt)
 {
	twi_packet_t pkt;
	/* Set Device Address */
	pkt.chip = ADDR_MCP3425;
	/* No Address Bytes to be clocked */
	pkt.addr_length = 0;
	/* Address of buffer where recvd data is to be stored */
	pkt.buffer = dPkt;
	/* No of bytes to read */
	pkt.length = 3;
	twi_master_read(Port, &pkt);
 }
