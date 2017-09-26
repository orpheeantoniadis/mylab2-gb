/*
===============================================================================
 Name        : gpio.c
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#include "gpio.h"
#include "utils.h"
#include "timer.h"

void LedSetState(uint8_t led, uint8_t state) {
	LPC_GPIO2->FIODIR |= 1 << led;
	LPC_GPIO2->FIOMASK = ~(1 << led);
	if (state == 1) LPC_GPIO2->FIOPIN = (1 << led);
	else LPC_GPIO2->FIOPIN = ~(1 << led);
	LPC_GPIO2->FIOMASK = 0;
}

void Led8SetState(uint8_t value) {
	LPC_GPIO2->FIODIR0 = 0xff;
	LPC_GPIO2->FIOPIN0 = value;
}

uint8_t JoystickGetState(uint8_t pos) {
	LPC_GPIO1->FIODIR |= 1 << pos;
	LPC_GPIO1->FIODIR ^= 1 << pos;
	LPC_GPIO1->FIOMASK = ~(1 << pos);
	uint8_t state = !(LPC_GPIO1->FIOPIN >> pos);
	LPC_GPIO1->FIOMASK = 0;
	return state;
}

uint8_t SwitchGetState() {
	LPC_GPIO2->FIODIR0 = 0;
	Delay(1);
	return ~(LPC_GPIO2->FIOPIN0);
}

void enableGPIOInterrupt(LPC_GPIO_TypeDef* gpio, uint32_t mode, uint8_t pin) {
	gpio->FIODIR &= ~(1 << pin);
	NVIC_EnableIRQ(EINT3_IRQn);
	if (mode == FALLING) {
		if (gpio == GPIO0) {
			LPC_GPIOINT->IO0IntEnF |= 1 << pin;
		}
		if (gpio == GPIO2) {
			LPC_GPIOINT->IO2IntEnF |= 1 << pin;
		}
	}
	if (mode == RISING) {
		if (gpio == GPIO0) {
			LPC_GPIOINT->IO0IntEnR |= 1 << pin;
		}
		if (gpio == GPIO2) {
			LPC_GPIOINT->IO2IntEnR |= 1 << pin;
		}
	}
}

void clearGPIOInterrupt(LPC_GPIO_TypeDef* gpio, uint8_t pin) {
	if (gpio == GPIO0) {
		LPC_GPIOINT->IO0IntClr |= 1 << pin;
	}
	if (gpio == GPIO2) {
		LPC_GPIOINT->IO2IntClr |= 1 << pin;
	}
}

uint32_t getGPIOInterruptStatus(LPC_GPIO_TypeDef* gpio, uint32_t mode, uint8_t pin) {
	if (mode == FALLING) {
		if (gpio == GPIO0) {
			return LPC_GPIOINT->IO0IntStatF >> pin;
		}
		if (gpio == GPIO2) {
			return LPC_GPIOINT->IO2IntStatF >> pin;
		}
	}
	if (mode == RISING) {
		if (gpio == GPIO0) {
			return LPC_GPIOINT->IO0IntStatR >> pin;
		}
		if (gpio == GPIO2) {
			return LPC_GPIOINT->IO2IntStatR >> pin;
		}
	}
	return 0;
}
