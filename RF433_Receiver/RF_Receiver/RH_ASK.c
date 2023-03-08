/*
 * RH_ASK.c
 *
 *  Created on: Feb 27, 2023
 *      Author: Yoganathan.V
 */

/* Includes ------------------------------------------------------------------*/
#include "RH_ASK.h"
#include "main.h"
#include <string.h>

/* Typedef -------------------------------------------------------------------*/
static Handle_RHMode_E 	RHmode;
static Handle_RH_S 		RH_S	= {0};

/* Define --------------------------------------------------------------------*/
#define lo8(x) 							((x) & 0xff)
#define hi8(x) 							((x) >> 8)

/* Macro ---------------------------------------------------------------------*/
#define RH_ASK_START_SYMBOL 			0xb38

/* Variables -----------------------------------------------------------------*/
uint8_t rxBuf[RH_ASK_MAX_PAYLOAD_LEN] = {0};
uint8_t txBuf[(RH_ASK_MAX_PAYLOAD_LEN * 2) + RH_ASK_PREAMBLE_LEN] = {0};

/* 4 bit to 6 bit symbol converter table */
static uint8_t symbols[] = {
    0xd,  0xe,  0x13, 0x15, 0x16, 0x19, 0x1a, 0x1c,
    0x23, 0x25, 0x26, 0x29, 0x2a, 0x2c, 0x32, 0x34
};

/* Function prototypes -------------------------------------------------------*/
 
void RH_ASK_Initialization(void)
{
	RHmode = RHModeInitialising;
    RH_S.thisAddress 	= RH_BROADCAST_ADDRESS;
	RH_S.txHeaderTo 	= RH_BROADCAST_ADDRESS;
	RH_S.txHeaderFrom 	= RH_BROADCAST_ADDRESS;
	RH_S.txHeaderId 	= False;
    RH_S.txHeaderFlags 	= False;
    RH_S.rxBad 			= False;
	RH_S.rxGood 		= False;
    RH_S.txGood 		= False;

    uint8_t preamble[RH_ASK_PREAMBLE_LEN] = {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x38, 0x2c};
    memcpy(txBuf, preamble, sizeof(preamble));
}

/*
 * @brief : RF set Receive IDLE
 * @param : none
 * @retval : none
 */
static void RH_setModeIdle(void)
{
    if (RHmode != RHModeIdle) {

    	/* Disable the transmitter hardware */
    	HAL_GPIO_WritePin(RH_TX_GPIO_Port, RH_TX_Pin, GPIO_PIN_RESET);
    	RHmode = RHModeIdle;
    }
}

/*
 * @brief : RF set Receive Mode
 * @param : none
 * @retval : none
 */
static void RH_setModeRx(void)
{
    if (RHmode != RHModeRx) {

    	/* Disable the transmitter hardware */
    	HAL_GPIO_WritePin(RH_TX_GPIO_Port, RH_TX_Pin, GPIO_PIN_RESET);
    	RHmode = RHModeRx;
    }
}

/*
 * @brief : RF set transmit Mode
 * @param : none
 * @retval : none
 */
static void RH_setModeTx(void)
{
    if (RHmode != RHModeTx) {

    	/* Prepare state varibles for a new transmission */
    	RH_S.txIndex = 0;
    	RH_S.txBit = 0;
    	RH_S.txSample = 0;

    	/* Enable the transmitter hardware */
    	HAL_GPIO_WritePin(RH_TX_GPIO_Port, RH_TX_Pin, GPIO_PIN_SET);
    	RHmode = RHModeTx;
    }
}

/*
 * @brief : CRC Verification Function
 * @param : crc 	- CRC data
 * 			data 	- Data
 * @retval : crc 	- CRC Value
 */
static uint16_t RH_CRC_update (uint16_t crc, uint8_t data)
{
    data ^= lo8 (crc);
    data ^= data << 4;

    return ((((uint16_t)data << 8) | hi8 (crc)) ^ (uint8_t)(data >> 4)
	    ^ ((uint16_t)data << 3));
}

/*
 * @brief : Check whether the latest received message is complete and uncorrupted
 	 	 	 	 We should always check the FCS at user level, not interrupt level since it is slow
 * @param : none
 * @retval : none
 */
static void RH_validateRxBuf(void)
{
    uint16_t crc = 0xffff;
    /* The CRC covers the byte count, headers and user data */
    for (uint8_t i = 0; i < RH_S.rxBufLen; i++) {
    	crc = RH_CRC_update(crc, rxBuf[i]);
    }
    if (crc != 0xf0b8) {
    	/* Reject and drop the message */
    	RH_S.rxBad++;
    	RH_S.rxBufValid = False;
    	return;
    }

    /* Extract the 4 headers that follow the message length */
    RH_S.rxHeaderTo    = rxBuf[1];
    RH_S.rxHeaderFrom  = rxBuf[2];
    RH_S.rxHeaderId    = rxBuf[3];
    RH_S.rxHeaderFlags = rxBuf[4];
    if ((RH_S.promiscuous) || (RH_S.rxHeaderTo == RH_S.thisAddress)
    		||	(RH_S.rxHeaderTo == RH_BROADCAST_ADDRESS)) {
    	RH_S.rxGood++;
    	RH_S.rxBufValid = True;
    }
}

/*
 * @brief : RH Function that checks the recieved data
 * @param : none
 * @retval : Bool - ture or Flase
 */
static Bool_E RH_available(void)
{
    if (RHmode == RHModeTx) {
    	return False;
    }
    RH_setModeRx();
    if (RH_S.rxBufFull) {
		RH_validateRxBuf();
		RH_S.rxBufFull = False;
    }
    return RH_S.rxBufValid;
}

/*
 * @brief : RH Receive payload Function
 * @param : buf 	- received Buffer
 * 			len 	- size of received len
 * @retval : Bool 	- ture or Flase
 */
Bool_E RH_recv(uint8_t* buf, uint8_t* len)
{
    if (!RH_available()) {
    	return False;
    }

    if(buf && len) {
		/* Skip the length and 4 headers that are at the beginning of the rxBuf
		 	 and drop the trailing 2 bytes of FCS */
		uint8_t message_len = RH_S.rxBufLen-RH_ASK_HEADER_LEN - 3;
		if (*len > message_len) {
			*len = message_len;
		}
		memcpy(buf, rxBuf+RH_ASK_HEADER_LEN+1, *len);
    }
    RH_S.rxBufValid = False;
    return True;
}

/*
 * @brief : Wait till mode will be TX mode.
 * @param : none
 * @retval : Bool_E - mode status
 */
static Bool_E RH_wait_TillPacketSent(void)
{
    while(RHmode == RHModeTx);
    return True;
}

/*
 * @brief : Send the Tx data outpin pin, taking into account platform type and inversion.
 * @param : data - to send
 * 			len	 - data length
 * @retval : Bool_E - TX status
 */
Bool_E RH_send(const uint8_t* data, uint8_t len)
{
    uint8_t i;
    uint16_t index = 0;
    uint16_t crc = 0xffff;
    uint8_t *p = txBuf + RH_ASK_PREAMBLE_LEN;
    uint8_t count = len + 3 + RH_ASK_HEADER_LEN;

    if (len > RH_ASK_MAX_MESSAGE_LEN) {
    	return False;
    }

    /* Wait for transmitter to become available */
    RH_wait_TillPacketSent();

    /* Encode the message length */
    crc = RH_CRC_update(crc, count);
    p[index++] = symbols[count >> 4];
    p[index++] = symbols[count & 0xf];

    /* Encode the headers */
    crc = RH_CRC_update(crc, RH_S.txHeaderTo);
    p[index++] = symbols[RH_S.txHeaderTo >> 4];
    p[index++] = symbols[RH_S.txHeaderTo & 0xf];

    crc = RH_CRC_update(crc, RH_S.txHeaderFrom);
    p[index++] = symbols[RH_S.txHeaderFrom >> 4];
    p[index++] = symbols[RH_S.txHeaderFrom & 0xf];

    crc = RH_CRC_update(crc, RH_S.txHeaderId);
    p[index++] = symbols[RH_S.txHeaderId >> 4];
    p[index++] = symbols[RH_S.txHeaderId & 0xf];

    crc = RH_CRC_update(crc, RH_S.txHeaderFlags);
    p[index++] = symbols[RH_S.txHeaderFlags >> 4];
    p[index++] = symbols[RH_S.txHeaderFlags & 0xf];

    /* Encode the message into 6 bit symbols. Each byte is converted into
     	 2 6-bit symbols, high nybble first, low nybble second */
    for (i = 0; i < len; i++) {
		crc = RH_CRC_update(crc, data[i]);
		p[index++] = symbols[data[i] >> 4];
		p[index++] = symbols[data[i] & 0xf];
    }

    /* Append the fcs, 16 bits before encoding (4 6-bit symbols after encoding)
		 Caution: VW expects the _ones_complement_ of the CCITT CRC-16 as the FCS
		 VW sends FCS as low byte then hi byte */
    crc = ~crc;
    p[index++] = symbols[(crc >> 4)  & 0xf];
    p[index++] = symbols[crc & 0xf];
    p[index++] = symbols[(crc >> 12) & 0xf];
    p[index++] = symbols[(crc >> 8)  & 0xf];

    /* Total number of 6-bit symbols to send */
    RH_S.txBufLen = index + RH_ASK_PREAMBLE_LEN;

    /* Start the low level interrupt handler sending symbols */
    RH_setModeTx();

    return True;
}

/*
 * @brief : Read the RX data input pin, taking into account platform type and inversion.
 * @param : none
 * @retval : Bool_E - RX Value
 */
static Bool_E RH_readRx(void)
{
    Bool_E value;
    value = HAL_GPIO_ReadPin(RH_RX_GPIO_Port, RH_RX_Pin);
    return value;
}

/*
 * @brief : Write the TX output pin, taking into account platform type.
 * @param : Bool_E TX Value
 * @retval none
 */
static void RH_writeTx(Bool_E value)
{
	HAL_GPIO_WritePin(RH_TX_GPIO_Port, RH_TX_Pin, value);
}

/*
 * @brief : Convert a 6 bit encoded symbol into its 4 bit decoded equivalent
 * @param : symbol to convert 6 to 4
 * @retval : symbol result
 */
static uint8_t symbol_6to4(uint8_t symbol)
{
    uint8_t i;
    uint8_t count;

     /* Linear search :-( Could have a 64 byte reverse lookup table?
     	 There is a little speedup here courtesy Ralph Doncaster:
     	 The shortcut works because bit 5 of the symbol is 1 for the last 8
		 symbols, and it is 0 for the first 8.
		 So we only have to search half the table */
    for (i = (symbol>>2) & 8, count=8; count-- ; i++) {
		if (symbol == symbols[i]) {
			return i;
		}
    }

    return -1;
}

/*
 * @brief : RH Receive function to receive data
 * @param : none
 * @retval : none
 */
static void RH_receiveTimer(void)
{
    Bool_E rxSample = RH_readRx();

    /* Bool_E grate each sample */
    if (rxSample) {
    	RH_S.rxIntegrator++;
    }

    if (rxSample != RH_S.rxLastSample) {

    	/* Transition, advance if ramp > 80, retard if < 80 */
    	RH_S.rxPllRamp += ((RH_S.rxPllRamp < RH_ASK_RAMP_TRANSITION)
    				   ? RH_ASK_RAMP_INC_RETARD
    				   : RH_ASK_RAMP_INC_ADVANCE);
    	RH_S.rxLastSample = rxSample;

    }else {

    	/* No transition - Advance ramp by standard 20 (== 160/8 samples) */
    	RH_S.rxPllRamp += RH_ASK_RAMP_INC;
    }

    if (RH_S.rxPllRamp >= RH_ASK_RX_RAMP_LEN) {

    	/* Add this to the 12th bit of _rxBits, LSB first
    		The last 12 bits are kept */
    	RH_S.rxBits >>= 1;

    	/* Check the integrator to see how many samples in this cycle were high.
    	 	 If < 5 out of 8, then its declared a 0 bit, else a 1; */
    	if (RH_S.rxIntegrator >= 5) {
    		RH_S.rxBits |= 0x800;
    	}

    	RH_S.rxPllRamp -= RH_ASK_RX_RAMP_LEN;
    	RH_S.rxIntegrator = 0;

    	if (RH_S.rxActive) {

    		/* We have the start symbol and now we are collecting message bits,
    		     6 per symbol, each which has to be decoded to 4 bits */
    		if (++RH_S.rxBitCount >= 12) {

    			/* Have 12 bits of encoded message == 1 byte encoded
					Decode as 2 lots of 6 bits into 2 lots of 4 bits
					The 6 lsbits are the high nybble */
				uint8_t this_byte =
						(symbol_6to4(RH_S.rxBits & 0x3f) << 4)
						| symbol_6to4(RH_S.rxBits >> 6);

				/* The first decoded byte is the byte count of the following message
				 	 the count includes the byte count and the 2 trailing FCS bytes
				 	 REVISIT: may also include the ACK flag at 0x40 */

				if (RH_S.rxBufLen == 0) {
					 /* The first byte is the byte count
					 	 Check it for sensibility. It cant be less than 7, since it
					 	 includes the byte count itself, the 4 byte header and the 2 byte FCS */
					RH_S.rxCount = this_byte;
					if (RH_S.rxCount < 7 || RH_S.rxCount > RH_ASK_MAX_PAYLOAD_LEN) {
						RH_S.rxActive = False;
						RH_S.rxBad++;
                        return;
					}
				}

				rxBuf[RH_S.rxBufLen++] = this_byte;
				if (RH_S.rxBufLen >= RH_S.rxCount) {
					/* Got all the bytes now */
					RH_S.rxActive = False;
					RH_S.rxBufFull = True;
					RH_setModeIdle();
				}
				RH_S.rxBitCount = 0;
    		}

    	}else if (RH_S.rxBits == RH_ASK_START_SYMBOL) {
		    /* Have start symbol, start collecting message */
    		RH_S.rxActive = True;
    		RH_S.rxBitCount = 0;
    		RH_S.rxBufLen = 0;
		}
    }
}

/*
 * @brief : RH Transmit function to send data
 * @param : none
 * @retval : none
 */
static void RH_transmitTimer(void)
{
    if (RH_S.txSample++ == 0) {

		 /* Send next bit Symbols are sent LSB first
		 	 Finished sending the whole message? (after waiting one bit period
		 	 since the last bit) */
    	if (RH_S.txIndex >= RH_S.txBufLen) {
    		RH_setModeIdle();
    		RH_S.txGood++;

    	}else {
		    RH_writeTx(txBuf[RH_S.txIndex] & (1 << RH_S.txBit++));
		    if (RH_S.txBit >= 6) {
		    	RH_S.txBit = 0;
		    	RH_S.txIndex++;
		    }
    	}
    }

    if (RH_S.txSample > 7) {
    	RH_S.txSample = 0;
    }
}

/*
 * @brief : RH Timer Callback function to send and receive data
 * @param : none
 * @retval : none
 */
void RH_HandleTimerInterrupt_16KHz(void)
{
    if (RHmode == RHModeRx) {
    	RH_receiveTimer();

    }else if (RHmode == RHModeTx) {
    	RH_transmitTimer();
    }
}

/*********************************END OF FILE**********************************/
