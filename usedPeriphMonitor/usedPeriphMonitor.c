/*
 * used_periph.c
 *
 *  Created on: 24 ���. 2020 �.
 *      Author: MaxCm
 */

#include "usedPeriphMonitor.h"

void initUsedPeriphMonitor(){
	initUsedPinsStruct();
	initUsedDmaStruct();
	initUsedExtiLines();
}
