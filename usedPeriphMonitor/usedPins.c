#include "usedPins.h"

#include "stm32f10x.h"

typedef struct {
	uint16_t GPIOA_Pins;
	uint16_t GPIOB_Pins;
	uint16_t GPIOC_Pins;
	uint16_t GPIOD_Pins;
} UsedPins;

UsedPins usedPins;

void initUsedPinsStruct(){
	usedPins.GPIOA_Pins=0;
	usedPins.GPIOB_Pins=0;
	usedPins.GPIOC_Pins=0;
	usedPins.GPIOD_Pins=0;
}

uint16_t getUsedPinsInGPIOA() {
	return usedPins.GPIOA_Pins;
}

uint16_t getUsedPinsInGPIOB() {
	return usedPins.GPIOB_Pins;
}

uint16_t getUsedPinsInGPIOC() {
	return usedPins.GPIOC_Pins;
}

uint16_t getUsedPinsInGPIOD() {
	return usedPins.GPIOD_Pins;
}

void setUsedPinsInGPIOA(uint16_t pins) {
	usedPins.GPIOA_Pins |= pins;
}

void setUsedPinsInGPIOB(uint16_t pins) {
	usedPins.GPIOB_Pins |= pins;
}

void setUsedPinsInGPIOC(uint16_t pins) {
	usedPins.GPIOC_Pins |= pins;
}

void setUsedPinsInGPIOD(uint16_t pins) {
	usedPins.GPIOD_Pins |= pins;
}

void pullUpUnusedPins(){
	GPIO_InitTypeDef pinInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	pinInitStruct.GPIO_Mode=GPIO_Mode_IPU;

	pinInitStruct.GPIO_Pin=~usedPins.GPIOA_Pins;
	GPIO_Init(GPIOA, &pinInitStruct);

	pinInitStruct.GPIO_Pin=~usedPins.GPIOB_Pins;
	GPIO_Init(GPIOB, &pinInitStruct);

	pinInitStruct.GPIO_Pin=~usedPins.GPIOC_Pins;
	GPIO_Init(GPIOC, &pinInitStruct);

	pinInitStruct.GPIO_Pin=~usedPins.GPIOD_Pins;
	GPIO_Init(GPIOD, &pinInitStruct);
}
