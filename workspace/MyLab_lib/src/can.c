/*
===============================================================================
 Name        : can.c
 Author      : O. Antoniadis & M. Blazevic
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "can.h"

void CAN_IRQHandler(void) {

}

/* ***********************************************************
 * @brief	CAN initialization function. The CAN is initialized
 * 			to communicate with the Marklin model.
 *
 * 			There is no initialization yet for the interrupt (read)
 *
 * @param 	none
 * @return 	none
 * ***********************************************************/
void init_can(void) {
	LPC_PINCON->PINSEL1 |= 0b1111 << 10; // select the outputs pins
	LPC_SC->PCONP |= 1 << 13; 		     // power on

	LPC_CAN1->MOD = CAN_RM; 					// reset mode

	LPC_CAN1->GSR = 0;							// reset can status
	LPC_CAN1->CMR |= CAN_AT | CAN_RRB | CAN_CDO; // reset rx/tx buffers

	LPC_CAN1->BTR = 4 << CAN_BRP;
	LPC_CAN1->BTR |= (3 << CAN_SJW) | (12 << CAN_TSEG1) | (5 << CAN_TSEG2);

	LPC_CANAF->AFMR &= ~1; 		// disable off mode of the filter
	LPC_CANAF->AFMR |= 1 << 1;	// AFMR in bypass mode

	LPC_CAN1->IER = 0; 							// disable interrupts

	// uncomment when use the receive
	//temp = LPC_CAN1->ICR;						// clear interrupts
	// LPC_CAN1->IER = 0b11; 	 // enable transfer interruption
	// NVIC_EnableIRQ(CAN_IRQn); // enable the interruptions

	LPC_CAN1->MOD &= CAN_OM; 	// operation mode
}

/* ***********************************************************
 * @brief	This function set the CAN datas to the buffers.
 * 			We write the number of bytes needed which depend
 * 			on the DLC. We can choose the buffer to write in.
 *
 * @param	data	the pointer to the datas to send
 * @param 	bytes	number of bytes to write/send
 * @param 	buffer 	the number of the buffer to write (1,2 or 3)
 * @return 	none
 * ***********************************************************/
void setDatas(uint8_t *data, uint8_t bytes, uint8_t buffer){
	LPC_CAN1->TDA1 = LPC_CAN1->TDB1 = 0;
	LPC_CAN1->TDA2 = LPC_CAN1->TDB2 = 0;
	LPC_CAN1->TDA3 = LPC_CAN1->TDB3 = 0;
	for(int i=0 ; i < bytes ; i++){
		switch(buffer){
			case BUFFER1:
				if(i<4){
					LPC_CAN1->TDA1 |= data[i] << i*8;
				}else{
					LPC_CAN1->TDB1 |= data[i] << ((i-4)*8);
				}
				break;
			case BUFFER2:
				if(i<4){
					LPC_CAN1->TDA2 |= data[i] << i*8;
				}else{
					LPC_CAN1->TDB2 |= data[i] << ((i-4)*8);
				}
					break;
			case BUFFER3:
				if(i<4){
					LPC_CAN1->TDA3 |= data[i] << i*8;
				}else{
					LPC_CAN1->TDB3 |= data[i] << ((i-4)*8);
				}
					break;
		}
	}
}

/* ***********************************************************
 * @brief	This function will write the datas to the CAN
 * 			registers. The datas are writen to the first free
 * 			CAN buffer. For write datas, we need to fill the
 * 			can_data_send structure which contains the DLC,
 * 			the CAN id, the id size (11 or 29 bits) and a table
 * 			of datas (1 to 8 bytes).
 *
 * @param	data	the pointer to the datas to send
 * @param 	bytes	number of bytes to write/send
 * @param 	buffer 	the number of the buffer to write (1,2 or 3)
 * @return 	none
 * ***********************************************************/
void write_can(can_data_send* data) {
	if(((LPC_CAN1->SR >> BUF1_STAT) & 1)){
		LPC_CAN1->TFI1 &= ~(255 << 16);				// reset the DLC
		LPC_CAN1->TFI1 |= (data->DLC << 16);		// set the DLC
		CAN_FF1(data->id_size);						// 29 bit identifier
		LPC_CAN1->TID1 = data->ID;					// frame identifier
		setDatas(data->data,data->DLC,BUFFER1);
		LPC_CAN1->CMR = CAN_TR | CAN_STB1;			// transmission on tx buffer 1
	}else if(((LPC_CAN1->SR >> BUF2_STAT) & 1)){
		LPC_CAN1->TFI2 &= ~(255 << 16);				// reset the DLC
		LPC_CAN1->TFI2 |= (data->DLC << 16);		// set the DLC
		CAN_FF2(data->id_size);						// 29 bit identifier
		LPC_CAN1->TID2 = data->ID;					// frame identifier
		setDatas(data->data,data->DLC,BUFFER2);
		LPC_CAN1->CMR = CAN_TR | CAN_STB2;			// transmission on tx buffer 2
	}else if(((LPC_CAN1->SR >> BUF3_STAT) & 1)){
		LPC_CAN1->TFI3 &= ~(255 << 16);				// reset the DLC
		LPC_CAN1->TFI3 |= (data->DLC << 16);		// set the DLC
		CAN_FF3(data->id_size);						// 29 bit identifier
		LPC_CAN1->TID3 = data->ID;					// frame identifier
		setDatas(data->data,data->DLC,BUFFER3);
		LPC_CAN1->CMR = CAN_TR | CAN_STB3;			// transmission on tx buffer 3
	}
}

void read_can(void) {
	if (LPC_CAN1->SR & 1) {
		//data = LPC_CAN1->RDA;
		LPC_CAN1->CMR = 0x04;
	}
}
