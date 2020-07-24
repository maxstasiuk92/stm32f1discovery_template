/*
 * used_dma.h
 *
 *  Created on: 24 лип. 2020 р.
 *      Author: MaxCm
 */

#ifndef USEDDMAS_H_
#define USEDDMAS_H_

#include <stdint.h>

void initUsedDmaStruct();
uint16_t isChannelUsedInDMA(uint8_t dma, uint8_t channel);
void setUsedChannelInDMA(uint8_t dma, uint8_t channel);

#endif /* USEDDMAS_H_ */
