/*
 * marklin.c
 *
 *  Created on: 4 mai 2017
 *      Author: mekiid.supertramp
 */

#include "marklin.h"


/* ***********************************************************
 * @brief	This function is used to stop the station. It will
 * 			stop every objects on the Marklin model.
 *
 * @param	none
 * @return 	none
 * ***********************************************************/
void station_stop(void){
	can_data_send datas;
	datas.DLC = DLC5;
	datas.ID = HASH;
	datas.id_size = ID_29B;
	for(int j = 0 ; j<datas.DLC ; j++)
		datas.data[j] = STOP_ALL;
	write_can(&datas);
}

/* ***********************************************************
 * @brief	This function is used to start the station. It will
 * 			start every objects on the Marklin model.
 *
 * @param	none
 * @return 	none
 * ***********************************************************/
void station_start(void){
	can_data_send datas;
	datas.DLC = DLC5;
	datas.ID = HASH;
	datas.id_size = ID_29B;
	for(int j = 0 ; j<datas.DLC-1 ; j++)
		datas.data[j] = 0;
	datas.data[datas.DLC-1] = START_ALL;
	write_can(&datas);
}

/* ***********************************************************
 * @brief	This function will stop a train.
 *
 * @param	id	the id of the train to stop
 * @return 	none
 * ***********************************************************/
void train_stop(uint32_t id){
	train_speed(id,0);
}

/* ***********************************************************
 * @brief	This function will stop the train and change his
 * 			direction. For the direction, we are three chose:
 *
 * 			- STRAIGTH
 *			- REVERSE
 *			- INVERT
 *
 * @param	id	the id of the train
 * @param	direction	the direction of the train
 * @return 	none
 * ***********************************************************/
void train_direction(uint32_t id, uint8_t direction){
	train_stop(id);
	can_data_send datas;
	datas.DLC = DLC5;
	datas.ID = HASH5;
	datas.id_size = ID_29B;
	datas.data[0] = id>>24;
	datas.data[1] = id>>16;
	datas.data[2] = train_protocol(id);
	datas.data[3] = id;
	datas.data[4] = direction;
	write_can(&datas);
}

/* ***********************************************************
 * @brief	This function will set the train speed. The speed
 * 			can be: 0..1000
 *
 * @param	id	the id of the train
 * @param	speed	the speed we want to set
 * @return 	none
 * ***********************************************************/
void train_speed(uint32_t id,uint16_t speed){
	can_data_send datas;
	datas.DLC = DLC6;
	datas.ID = HASH4;
	datas.id_size = ID_29B;
	datas.data[0] = id>>24;
	datas.data[1] = id>>16;
	datas.data[2] = train_protocol(id);	// the protocol for the speed of the train (DCC or MM2)
	datas.data[3] = id;
	datas.data[4] = speed>>8;
	datas.data[5] = speed;
	write_can(&datas);
}

/* ***********************************************************
 * @brief	This function can set the train light.
 *
 * @param	id	the id of the train
 * @param	on_off	if the light is on or off (0 or 1)
 * @return 	none
 * ***********************************************************/
void train_light(uint32_t id, bool on_off){
	can_data_send datas;
	datas.DLC = DLC6;
	datas.ID = HASH6;
	datas.id_size = ID_29B;
	datas.data[0] = id>>24;
	datas.data[1] = id>>16;
	datas.data[2] = train_protocol(id);	// the protocol for the speed of the train (DCC or MM2)
	datas.data[3] = id;
	datas.data[4] = 0;
	datas.data[5] = on_off;
	write_can(&datas);
}

/* ***********************************************************
 * @brief	This function control the signals/lights on the
 * 			model station. For each lights, we can chose 4
 * 			modes:
 *
 * 			READY 	->	green + orange/yellow lights
 *			DOUBLEG ->	double green
 *			STOP 	->	the red light
 *			GREEN	->	the green light
 *
 * @param	id	the id of the train
 * @param	ligth	the light mode
 * @return 	none
 * ***********************************************************/
void signal_cmd(uint32_t id, uint8_t light){
	can_data_send datas;
	datas.DLC = DLC6;
	datas.ID = HASH11;
	datas.id_size = ID_29B;
	datas.data[0] = id>>24;
	datas.data[1] = id>>16;
	datas.data[2] = (DCC_ACCESSORIES+id)>>8;
	if(light>1){
		datas.data[3] = DCC_ACCESSORIES+id-1;
		datas.data[4] = light-2;
	}else{
		datas.data[3] = DCC_ACCESSORIES+id;
		datas.data[4] = light;
	}
	datas.data[5] = 1;
	write_can(&datas);
}

/* ***********************************************************
 * @brief	This function allow to control the rail switch on
 * 			the station model. We need to know the previous
 * 			position before change it.
 *
 * 			OPEN	-> open the switch
 *			CLOSE	-> close the switch
 *
 * @param	id	the id of the train
 * @param	open	open or close the rail switch
 * @return 	none
 * ***********************************************************/
void rail_switch_cmd(uint32_t id, bool open){
	can_data_send datas;
	datas.DLC = DLC6;
	datas.ID = HASH11;
	datas.id_size = ID_29B;
	datas.data[0] = id>>24;
	datas.data[1] = id>>16;
	datas.data[2] = (MM2_ACCESSORIES+id)>>8;
	datas.data[3] = MM2_ACCESSORIES+id;
	datas.data[4] = open;
	datas.data[5] = 1;
	write_can(&datas);
}
