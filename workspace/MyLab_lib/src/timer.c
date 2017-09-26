/*
===============================================================================
 Name        : timer.c
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "timer.h"

void initTimer(LPC_TIM_TypeDef* timer, uint32_t pclk) {
	if (timer == TIMER2) {
		LPC_SC->PCONP |= (1 << 22);
	} else if (timer == TIMER3) {
		LPC_SC->PCONP |= (1 << 22);
	}
	timer->PR = pclk;
	timer->TCR = 0b10;
}

void startTimer(LPC_TIM_TypeDef* timer) {
	timer->TCR = 0b01;
}

void stopTimer(LPC_TIM_TypeDef* timer) {
	timer->TCR = 0b10;
}

uint32_t getTime(LPC_TIM_TypeDef* timer) {
	return timer->TC;
}

void enableTimerInterrupt(LPC_TIM_TypeDef* timer, uint32_t time, uint8_t reset, uint8_t stop) {
	timer->MCR |= 1;
	timer->MCR |= reset<<1;
	timer->MCR |= stop<<2;
	timer->MR0 = time;
	if (timer == TIMER0) NVIC_EnableIRQ(TIMER0_IRQn);
	if (timer == TIMER1) NVIC_EnableIRQ(TIMER1_IRQn);
	if (timer == TIMER2) NVIC_EnableIRQ(TIMER2_IRQn);
	if (timer == TIMER3) NVIC_EnableIRQ(TIMER3_IRQn);
}
