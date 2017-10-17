/*
===============================================================================
 Name        : sd_card.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "sd_card.h"

/* ********************************************************
 * enable the GPIO which we need to communicate with the
 * micro SD card
 * output:	pin		-	  name			-	connector
 * 			P1.31	-	ship select		-	P1.20
 * @param none
 * @return none
 * *******************************************************/
void init_SDcard_GPIO(void) {
	LPC_GPIO1->FIODIR |= 1 << PIN_SD_CARD_CS;
}

uint8_t sd_send_command(sd_send_struct* sd_card){
	write_spi(sd_card->cmd);
	write_spi(sd_card->arg3);
	write_spi(sd_card->arg2);
	write_spi(sd_card->arg1);
	write_spi(sd_card->arg0);
	write_spi(sd_card->crc);
	return 0;
}

uint8_t sd_receive_command(sd_receive_struct *sd_card, response format){
	// write xFF until the card respond
	while((sd_card->reg1 = WAIT_FOR_DATAS()) == NORESP);

	switch(format){
	case R1:
		sd_card->ocr = NOR32;
		return R1;
		break;
	case R2:
		sd_card->reg2 = WAIT_FOR_DATAS();
		sd_card->ocr0 = sd_card->ocr1 = sd_card->ocr2 = NORESP;
		return R2;
		break;
	case R3:
		sd_card->reg2 = WAIT_FOR_DATAS();
		sd_card->ocr2 = WAIT_FOR_DATAS();
		sd_card->ocr1 = WAIT_FOR_DATAS();
		sd_card->ocr0 = WAIT_FOR_DATAS();
		return R3;
		break;
	}

	return 0;
}

void sd_receive_data(sd_datapacket_t *datapacket) {
	uint32_t i;
	for (i = 0; i < BLOCK_LEN; i++) {
		datapacket->data_block[i] = WAIT_FOR_DATAS();
	}
	datapacket->crc1 = WAIT_FOR_DATAS();
	datapacket->crc0 = WAIT_FOR_DATAS();
}

/*
 * If the command 0 and 8 are ok, we need to write to the card the CMD55 and
 * ACMD41 and check if the card is ok (response : R1 = 0x00). If the card is
 * not ready, R1 = 0x01 (idle state), then restart this procedure, until
 * the card is ready.
 */
uint8_t check_SDcard(sd_send_struct *sd_card, sd_receive_struct *sd_r_card){
	sd_card->cmd = START | CMD55;
	sd_card->arg = ARG0;
	sd_card->crc = NOCRC;
	sd_send_command(sd_card);
	sd_receive_command(sd_r_card,REP_TYPE(CMD55));

	sd_card->cmd = START | ACMD41;
	sd_card->arg = ARG41;
	sd_send_command(sd_card);
	sd_receive_command(sd_r_card,REP_TYPE(ACMD41));

	if(sd_r_card->reg1 == OKAY){
		return 0;
	}
	return 1;
}

/* ********************************************************
 * function which enable the communication with the
 * micro SD card on the myLab2 board.
 * 1) generate the clocks (>68)
 * 2) send the CMD0 (resp: 0x01 idle state)
 * 3) send the CMD8	(resp: 0x01 + the argument of the CMD8)
 * 4) send CMD55 + ACMD41 until the card is ready
 * @param none
 * @return none
 * *******************************************************/
void init_SDcard(void){
	//init_spi();
	init_SDcard_GPIO();
	sd_send_struct first;
	sd_receive_struct first_receive;

	first_receive.reg1 = NORESP;
	first.cmd = START | CMD0;
	first.arg = ARG0;
	first.crc = CRC0;

	// generate the clocks and send the first CMD0 command
	while (first_receive.reg1 != IDLE) {
		CARD_CS(1);
		for (int i=0; i<11 ; i++) write_spi(0xFF);
		CARD_CS(0);
		sd_send_command(&first); // send the first cmd
		sd_receive_command(&first_receive,REP_TYPE(CMD0)); // read the first response
	}

	// send the command CMD8
	first.cmd = START | CMD8;
	first.arg = ARG8;
	first.crc = CRC8;
	sd_send_command(&first);
	sd_receive_command(&first_receive,REP_TYPE(CMD8));

	// if the CMD8 is ok, then check until the card exit the idle state
	if(first_receive.ocr == ARG8){
		while(check_SDcard(&first,&first_receive));
	}
	CARD_CS(1);
}

uint16_t sd_send_status(void) {
	sd_receive_struct receive;
	sd_send_struct send;
	send.cmd = START | CMD13;
	CARD_CS(0);
	sd_send_command(&send);
	sd_receive_command(&receive,REP_TYPE(CMD13));
	CARD_CS(1);
	return (receive.reg1 << 8) | receive.reg2;
}

void sd_read_block(uint8_t *block, uint32_t addr) {
	sd_receive_struct receive;
	sd_send_struct send;
	sd_datapacket_t datapacket;
	datapacket.data_block = block;
	CARD_CS(0);
	send.cmd = START | CMD16;
	send.arg = BLOCK_LEN;
	send.crc = NOCRC;
	sd_send_command(&send);
	sd_receive_command(&receive, REP_TYPE(CMD16));

	send.cmd = START | CMD17;
	send.arg = addr;
	sd_send_command(&send);
	sd_receive_command(&receive, REP_TYPE(CMD17));
	while((datapacket.token = WAIT_FOR_DATAS()) == NORESP);

	sd_receive_data(&datapacket);
	CARD_CS(1);
}

void sd_split_block(void *ptr, uint32_t start, uint32_t size, uint8_t *block) {
	uint16_t i, buffer;
	if (size > BLOCK_LEN) buffer = BLOCK_LEN;
	else buffer = size;
	buffer = size;
	for (i = start; i < start + buffer; i++) {
		((uint8_t *)ptr)[i-start] = block[i];
	}
}
