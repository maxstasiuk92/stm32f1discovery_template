/*
 * board.c
 *
 *  Created on: 23 лип. 2020 р.
 *      Author: MaxCm
 */
#include "stm32f10x.h"
#include "usedPeriphMonitor.h"
#include "board.h"
#include "config.h"

ConfigResult setSwdPinsUsed(){
	if (getUsedPinsInGPIOA(GPIO_Pin_13 | GPIO_Pin_14)) {
		return Config_Fail;
	}
	setUsedPinsInGPIOA(GPIO_Pin_13 | GPIO_Pin_14);

	return Config_OK;
}

ConfigResult setHsePinsUsed(){
	if (getUsedPinsInGPIOC(GPIO_Pin_14 | GPIO_Pin_15)) {
		return Config_Fail;
	}
	setUsedPinsInGPIOC(GPIO_Pin_14 | GPIO_Pin_15);

	return Config_OK;
}

ConfigResult configLeds(){
	GPIO_InitTypeDef pinInitStruct;

	if(getUsedPinsInGPIOC() & (GPIO_Pin_8 | GPIO_Pin_9)){
		return Config_Fail;
	}
	setUsedPinsInGPIOC(GPIO_Pin_8 | GPIO_Pin_9);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	pinInitStruct.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	pinInitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	pinInitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_Init(GPIOC, &pinInitStruct);

	return Config_OK;
}

void greenLedCtrl(uint8_t state){
	if (state){
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
	} else {
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
}

void blueLedCtrl(uint8_t state){
	if (state){
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
	} else {
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	}
}

ConfigResult configButtonForPolling(){
	GPIO_InitTypeDef pinInitStruct;

	if(getUsedPinsInGPIOA() & GPIO_Pin_0){
		return Config_Fail;
	}
	setUsedPinsInGPIOA(GPIO_Pin_0);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	pinInitStruct.GPIO_Pin=GPIO_Pin_0;
	pinInitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	GPIO_Init(GPIOA, &pinInitStruct);

	return Config_OK;
}

uint8_t isButtonPushed(){
	return (GPIO_ReadInputData(GPIOA) & GPIO_Pin_0) ? 1 : 0;
}


