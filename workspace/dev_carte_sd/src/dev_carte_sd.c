/*
===============================================================================
 Name        : dev_carte_sd.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "gpio.h"
#include "utils.h"
#include "lcd.h"
#include "sd_card.h"
#include "fat.h"
#include "sys_call.h"

int main(void) {
	int i=1;
	char buffer[100];
	uint8_t block[BLOCK_LEN];
	uint16_t status;

    while(i) {
    	if(IS_CARD()){
    		Led8SetState(1);
    		i=0;
    	} else{
    		Led8SetState(0);
    		i=1;
    	}
    }
    Delay(1);

    init_lcd();
    clear_screen(LCD_BLACK);
    init_fat();

    status = sd_send_status();
    sd_read_block(block, 0);
    status = sd_send_status();
    sd_read_block(block, 0);
    status = sd_send_status();
    // read route.txt
    int fd = open("route.txt");
    status = sd_send_status();
    if (read(fd, buffer, 99) == 0) {
    	status = sd_send_status();
    	clear_screen(LCD_BLACK);
    	status = sd_send_status();
    	//lcd_print(0,0,SMALLFONT,LCD_WHITE,LCD_BLACK,buffer);
    	status = sd_send_status();
    }

    while((status = sd_send_status()) != 0);

//    // check the voltage values
//    send.cmd = START | CMD58;
//    send.arg = ARG0;
//    send.crc = NOCRC;
//    CARD_CS(0);
//    sd_send_command(&send);
//    sd_receive_command(&receive, REP_TYPE(CMD58));
//    CARD_CS(1);
//

    return 0;
}
