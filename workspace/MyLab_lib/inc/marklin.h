/*
 * marklin.h
 *
 *  Created on: 4 mai 2017
 *      Author: mekiid.supertramp
 */

#ifndef MARKLIN_H_
#define MARKLIN_H_

#include "can.h"

// hashes + commandes
#define HASH		0x4711
#define HASH1		0x24711  //HASH + commande = 1
#define HASH2		0x44711  //HASH + commande = 2
#define HASH3		0x64711  //HASH + commande = 3
#define HASH4		0x84711  //HASH + commande = 4
#define HASH5		0xA4711  //HASH + commande = 5
#define HASH6		0xC4711  //HASH + commande = 6
#define HASH11	    0x164711 //HASH + commande = 11

// for the most important function of the station
#define STOP_ALL	0x00
#define START_ALL	0x01


// DLC
#define DLC5	5	// used for many commands
#define DLC6	6

// id length
#define ID_29B	1	// id 29 bits
#define ID_11B	0	// id 11 bits

// protocoles
#define MM2		0x00	// the protocole of the locomotive
#define DCC		0xC0	// the protocole of the locomotive
#define MM2_ACCESSORIES   0x3000 // the protocole of the accessory
#define DCC_ACCESSORIES   0x3800 // the protocole of the accessory

// used for the direction of the locomotive
enum{
	STRAIGTH = 1,
	REVERSE = 2,
	INVERT = 3
};

// used for the rail switch
enum{
	OPEN = 0,
	CLOSE = 1
};

// used for the signals
enum{
	READY = 0,
	DOUBLEG = 1,
	STOP = 2,
	GREEN = 3
};


static inline uint8_t train_protocol(uint8_t id) {if((id==14)){return MM2;} if((id==12)){return DCC;} return 0;}

void station_stop(void);
void station_start(void);
void train_stop(uint32_t id);
void train_direction(uint32_t id, uint8_t direction);
void train_speed(uint32_t id,uint16_t speed);
void train_light(uint32_t id, bool on_off);
void signal_cmd(uint32_t id,uint8_t light);
void rail_switch_cmd(uint32_t id, bool open);

#endif /* MARKLIN_H_ */
