/*
 * used_periph.c
 *
 *  Created on: 24 лип. 2020 р.
 *      Author: MaxCm
 */

#include "usedPeriphMonitor.h"

void initUsedPeriphMonitor(){
	initUsedPinsStruct();
	initUsedDmaStruct();
	initUsedExtiLines();
}
