/*
 * RH_ASK.h
 *
 *  Created on: Feb 27, 2023
 *      Author: Yoganathan.V
 */

/* Define to prevent recursive inclusion -----------------------------*/

#ifndef INC_RH_ASK_H_
#define INC_RH_ASK_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include <stdint.h>

/* Define ------------------------------------------------------------*/
#define RH_ASK_RX_RAMP_LEN 				160
#define RH_ASK_RAMP_TRANSITION 			(RH_ASK_RX_RAMP_LEN/2)

#define RH_ASK_RX_SAMPLES_PER_BIT 		8
#define RH_ASK_RAMP_INC 				(RH_ASK_RX_RAMP_LEN/RH_ASK_RX_SAMPLES_PER_BIT)

#define RH_ASK_RAMP_ADJUST 				9
#define RH_ASK_RAMP_INC_RETARD 			(RH_ASK_RAMP_INC - RH_ASK_RAMP_ADJUST)
#define RH_ASK_RAMP_INC_ADVANCE 		(RH_ASK_RAMP_INC + RH_ASK_RAMP_ADJUST)

#define RH_BROADCAST_ADDRESS 			0xff
#define RH_ASK_HEADER_LEN 				4
#define RH_ASK_PREAMBLE_LEN 			8

#define RH_ASK_MAX_PAYLOAD_LEN 			67

#define RH_ASK_MAX_MESSAGE_LEN 			(RH_ASK_MAX_PAYLOAD_LEN - RH_ASK_HEADER_LEN - 3)

/*
 *  ------------------ Payload Format - RF 433MHz --------------------------
  	 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
	+----------------------------------------------------------------------+
	|						Training Preamble (36Bits)			   	       |
	+----------------------+---------------+-------------------------------+
	| Start Symbol(12Bits) | Length(8Bits) |  Frame Check Length(16Bits)   |
	+----------------------+---------------+-------------------------------+

*/

/* Macro -------------------------------------------------------------*/

/* Typedef -----------------------------------------------------------*/
/*
 * Enum for Bool handle
 */
typedef enum
{
	False = 0,
	True = !False

}Bool_E;

/*
 * @brief Handles the RH mode states
 */
typedef enum
{
	RHModeInitialising = 0, 			/* RF is initializing				*/
	RHModeSleep,            			/* RF is in low power sleep mode 	*/
	RHModeIdle,             			/* RF is idle						*/
	RHModeTx,               			/* RF is transmit mode				*/
	RHModeRx,               			/* RF is Receive mode				*/
	RHModeCad               			/* RF is Detecting channel activity	*/

} Handle_RHMode_E;

/* Variables ---------------------------------------------------------*/

typedef struct {
	volatile uint8_t 	rxIntegrator;
	volatile uint8_t 	rxBufLen;
	volatile uint8_t 	rxPllRamp;
	volatile uint8_t 	rxBitCount;
	volatile uint8_t 	rxActive;
	volatile uint8_t 	rxCount;
	volatile uint8_t    rxHeaderTo;
	volatile uint8_t    rxHeaderFrom;
	volatile uint8_t    rxHeaderId;
	volatile uint8_t    rxHeaderFlags;

	volatile uint16_t   rxBad;
	volatile uint16_t 	rxBits;

	volatile uint8_t    thisAddress;
	volatile uint8_t 	txIndex;
	volatile uint8_t 	txBit;
	volatile uint8_t 	txSample;
	volatile uint8_t 	txBufLen;
	volatile uint8_t    txHeaderTo;
	volatile uint8_t    txHeaderFrom;
	volatile uint8_t    txHeaderId;
	volatile uint8_t    txHeaderFlags;

	volatile uint16_t   txGood;
	volatile uint16_t   rxGood;


	volatile Bool_E   	rxBufValid;
	volatile Bool_E     promiscuous;
	volatile Bool_E		rxLastSample;
	volatile Bool_E 	rxBufFull;

}Handle_RH_S;

/* Function prototypes -----------------------------------------------*/
void RH_HandleTimerInterrupt_16KHz(void);
void RH_ASK_Initialization(void);
Bool_E RH_recv(uint8_t* buf, uint8_t* len);
Bool_E RH_send(const uint8_t* data, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif /* INC_RH_ASK_H_ */
