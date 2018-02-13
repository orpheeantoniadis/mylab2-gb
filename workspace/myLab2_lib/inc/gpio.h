/*
===============================================================================
 Name        : gpio.h
 Author      : O. Antoniadis
 Copyright   : HES-SO hepia
 Year        : 2016-2017
===============================================================================
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#define GPIO0	LPC_GPIO0
#define GPIO2	LPC_GPIO2
#define RISING	0
#define FALLING	1

#define JOYSTICK_CENTER	19
#define JOYSTICK_UP		20
#define JOYSTICK_LEFT	23
#define JOYSTICK_DOWN	22
#define JOYSTICK_RIGHT	21

#define BUTTON_A		10
#define BUTTON_B		19
#define TOUCH_SCREEN	11

void LedSetState(uint8_t led, uint8_t state);
void Led8SetState(uint8_t value);
uint8_t JoystickGetState(uint8_t pos);
uint8_t SwitchGetState();

void enableGPIOInterrupt(LPC_GPIO_TypeDef* gpio, uint32_t mode, uint8_t pin);
void clearGPIOInterrupt(LPC_GPIO_TypeDef* gpio, uint8_t pin);
uint8_t getGPIOInterruptStatus(LPC_GPIO_TypeDef* gpio, uint32_t edge, uint8_t pin);

#endif
