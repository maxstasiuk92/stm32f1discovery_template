/*
 * board.h
 *
 *  Created on: 23 лип. 2020 р.
 *      Author: MaxCm
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <stdint.h>
#include "config.h"

ConfigResult setSwdPinsUsed();
ConfigResult setHsePinsUsed();

ConfigResult configLeds();
void greenLedCtrl(uint8_t state);
void blueLedCtrl(uint8_t state);

ConfigResult configButtonForPolling();
uint8_t isButtonPushed();

#endif /* BOARD_H_ */
