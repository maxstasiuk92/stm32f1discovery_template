/*
 * configSpi.h
 *
 *  Created on: 23 лип. 2020 р.
 *      Author: MaxCm
 */

#ifndef SPI_H_
#define SPI_H_
#include "config.h"

typedef struct {
	uint16_t SPI_CPHA;
	uint16_t SPI_CPOL;
	uint16_t SPI_BaudRatePrescaler;
	uint16_t SPI_FirstBit;
	uint8_t * buffer;
} SPIConfig;

ConfigResult configSPI1AsMaster(SPIConfig * spiConfig);
#define startSpi1Exchange(byteNum) do{\
								DMA_SetCurrDataCounter(DMA1_Channel2, byteNum);\
								DMA_SetCurrDataCounter(DMA1_Channel3, byteNum);\
								DMA_Cmd(DMA1_Channel2, ENABLE);\
								DMA_Cmd(DMA1_Channel3, ENABLE);}while(0)

ConfigResult configSPI2AsMaster(SPIConfig * spiConfig);
#define startSpi2Exchange(byteNum) do{\
								DMA_SetCurrDataCounter(DMA1_Channel4, byteNum);\
								DMA_SetCurrDataCounter(DMA1_Channel5, byteNum);\
								DMA_Cmd(DMA1_Channel4, ENABLE);\
								DMA_Cmd(DMA1_Channel5, ENABLE);}while(0)

#endif /* SPI_H_ */
