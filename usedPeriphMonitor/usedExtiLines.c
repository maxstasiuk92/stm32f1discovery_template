/*
 * used_exti.c
 *
 *  Created on: 24 лип. 2020 р.
 *      Author: MaxCm
 */

#include "usedExtiLines.h"

uint32_t usedEXTI_Lines;

void initUsedExtiLines(){
	usedEXTI_Lines = 0;
}

uint32_t getUsedExtiLines(){
	return usedEXTI_Lines;
}

void setUsedExtiLines(uint32_t EXTI_Lines) {
	usedEXTI_Lines |= EXTI_Lines;
}
