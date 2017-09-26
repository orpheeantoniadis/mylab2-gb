/*
===============================================================================
 Name        : sd_card.h
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _SD_CARD_
#define _SD_CARD_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "spi.h"
#include "utils.h"

#define BLOCK_LEN	512

//struct used for send datas (6 bytes / 48 bits)
typedef struct{
	uint8_t  cmd;
	union{
		uint32_t arg;
		struct{
			uint16_t argl;
			uint16_t argh;
		};
		struct{
			uint8_t arg0;
			uint8_t arg1;
			uint8_t arg2;
			uint8_t arg3;
		};
	};
	uint8_t  crc;
} sd_send_struct;

// struct used for receive datas (5 bytes / 40 bits)

typedef struct {
	uint8_t reg1;
	union{
		uint32_t ocr;
		struct{
			uint8_t ocr0;
			uint8_t ocr1;
			uint8_t ocr2;
			uint8_t reg2;
		};
	};
} sd_receive_struct;

typedef struct {
	uint8_t token;
	uint8_t *data_block;
	union {
		uint16_t crc;
		struct {
			uint8_t crc0;
			uint8_t crc1;
		};
	};
} sd_datapacket_t;

typedef enum response response;
enum response {
    R1, R2, R3
};


#define PIN_SD_CARD_CS	31	// chip select
#define PIN_SD_CARD_CD	28	// card detect

// commands to send
#define START	0x40	// 01xx xxxx the start bits
//#define STOP	0x01	// xxxx xxx1 the stop bit
#define CMD0	0x00	// tell the card to reset and enter its idle state
#define CMD8	0x08	// initialise the sd card
#define CMD55	0x37	// the next data will be application specific (ACMDXX)
#define ACMD41	0x29	// initialize the sd card
#define CMD58	0x3A	// read OCR (operating conditions register)

#define CMD13	0x0D	// get the status register
#define CMD16 	0x10	// select the block length
#define CMD17	0x11	// read a single block
#define CMD24	0x18	// write a single block

// CRC
#define NOCRC	0x01	// the crc is not used (x00<<1 | STOP)
#define CRC0	0x95	// the crc value for the command CMD0 data transfer (x4A<<1 | STOP)
#define CRC8	0x87	// the crc value fot the command CMD8 data transfer (x43<<1 | STOP)

// arguments
#define ARG0	0x00		// the standard argument
#define ARG8	0x1AA 		// the argument for the command CMD8
#define ARG41	0x40000000	// the argument for the commande ACMD41


// format of the response to every command in SPI mode
#define OKAY	0x00
#define IDLE    0x01	// data sent when the card is ready, the card is in idle state
#define ERARES 	0x02	// erase reset
#define ILCMD	0x04	// illegal command
#define CRCER	0x08	// CRC error
#define ERASER	0x10	// erase sequence error
#define ADDERR	0x20	// address error
#define PARERR	0x40	// parameter error
#define NORESP	0xFF	// no response
#define NOR32	0xFFFFFFFF // 32 bits format no resp

static inline void CARD_CS(uint8_t val) {if (val) LPC_GPIO1->FIOSET=1<<PIN_SD_CARD_CS; else LPC_GPIO1->FIOCLR=1<<PIN_SD_CARD_CS;}
static inline uint8_t IS_CARD(void) {if(~(LPC_GPIO1->FIOPIN>>PIN_SD_CARD_CD)&1) return 1; else return 0;}
static inline uint8_t WAIT_FOR_DATAS(void){return read_spi();}
static inline response REP_TYPE(uint8_t c){if((c==CMD58)||(c==CMD8)){return R3;} if(c==CMD13){return R2;} return R1; }

void init_SDcard(void);
uint16_t sd_send_status(void);
void sd_read_block(uint8_t *block, uint32_t addr);
void sd_split_block(void *ptr, uint32_t start, uint32_t size, uint8_t *block);

#endif /* _SD_CARD_ */
