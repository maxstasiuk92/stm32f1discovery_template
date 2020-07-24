/*
 * configSpi.c
 *
 *  Created on: 23 лип. 2020 р.
 *      Author: MaxCm
 */

#include <spi.h>
#include "stm32f10x.h"
#include "config.h"
#include "usedPeriphMonitor.h"


ConfigResult configSPI1AsMaster(SPIConfig * spiConfig) {
	GPIO_InitTypeDef pinInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	SPI_InitTypeDef spiInitStruct;
	DMA_InitTypeDef dmaInitStruct;

	if(getUsedPinsInGPIOA() & (GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)){
		return Config_Fail;
	}
	setUsedPinsInGPIOA(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	if (isChannelUsedInDMA(1, 2) || isChannelUsedInDMA(1, 3)) {
		return Config_Fail;
	}
	setUsedChannelInDMA(1, 2);
	setUsedChannelInDMA(1, 3);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//Config. pins
	//PA7 - MOSI, PA5 - SCK
	pinInitStruct.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_7;
	pinInitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	pinInitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &pinInitStruct);
	//PA6 - MISO
	pinInitStruct.GPIO_Pin=GPIO_Pin_6;
	pinInitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &pinInitStruct);

	//Config. SPI
	spiInitStruct.SPI_Mode=SPI_Mode_Master;
	spiInitStruct.SPI_CPHA=spiConfig->SPI_CPHA;
	spiInitStruct.SPI_CPOL=spiConfig->SPI_CPOL;
	spiInitStruct.SPI_NSS=SPI_NSS_Soft;
	spiInitStruct.SPI_DataSize=SPI_DataSize_8b;
	spiInitStruct.SPI_FirstBit=spiConfig->SPI_FirstBit;
	spiInitStruct.SPI_BaudRatePrescaler=spiConfig->SPI_BaudRatePrescaler;
	spiInitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	spiInitStruct.SPI_CRCPolynomial=0;
	SPI_Init(SPI1, &spiInitStruct);
	SPI_CalculateCRC(SPI1, DISABLE);
	//Config. TX with DMA
	dmaInitStruct.DMA_Mode=DMA_Mode_Normal;
	dmaInitStruct.DMA_DIR=DMA_DIR_PeripheralDST;
	dmaInitStruct.DMA_M2M=DMA_M2M_Disable;
	dmaInitStruct.DMA_Priority=DmaPrio_SPI1;

	dmaInitStruct.DMA_MemoryBaseAddr=(uint32_t)spiConfig->buffer;
	dmaInitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dmaInitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;

	dmaInitStruct.DMA_PeripheralBaseAddr=(uint32_t)&(SPI1->DR);
	dmaInitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dmaInitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;

	dmaInitStruct.DMA_BufferSize=0; //will be set before Tx/Rx
	DMA_Init(DMA1_Channel3, &dmaInitStruct);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

	//Config. RX with DMA
	dmaInitStruct.DMA_Mode=DMA_Mode_Normal;
	dmaInitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;
	dmaInitStruct.DMA_M2M=DMA_M2M_Disable;
	dmaInitStruct.DMA_Priority=DmaPrio_SPI1;

	dmaInitStruct.DMA_MemoryBaseAddr=(uint32_t)spiConfig->buffer;
	dmaInitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dmaInitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;

	dmaInitStruct.DMA_PeripheralBaseAddr=(uint32_t)&(SPI1->DR);
	dmaInitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dmaInitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;

	dmaInitStruct.DMA_BufferSize=0; //will be set before Tx/Rx
	DMA_Init(DMA1_Channel2, &dmaInitStruct);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE); //interrupt after Rx

	nvicInitStruct.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = IrqPrio_Spi1Finish;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

	//Enable SPI
	SPI_Cmd(SPI1, ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set); //TODO: any sense?

	return Config_OK;
}

//data size - 8bits, Tx/Rx via buffer using DMA
ConfigResult configSPI2AsMaster(SPIConfig * spiConfig){
	GPIO_InitTypeDef pinInitStruct;
	NVIC_InitTypeDef nvicInitStruct;
	SPI_InitTypeDef spiInitStruct;
	DMA_InitTypeDef dmaInitStruct;

	if(getUsedPinsInGPIOB() & (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)){
		return Config_Fail;
	}
	setUsedPinsInGPIOB(GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	if (isChannelUsedInDMA(1, 4) || isChannelUsedInDMA(1, 5)) {
		return Config_Fail;
	}
	setUsedChannelInDMA(1, 4);
	setUsedChannelInDMA(1, 5);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//Config. pins
	//PB15 - MOSI, PB13 - SCK
	pinInitStruct.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_13;
	pinInitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	pinInitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &pinInitStruct);

	//PB14 - MISO
	pinInitStruct.GPIO_Pin=GPIO_Pin_14;
	pinInitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &pinInitStruct);

	//Config. SPI
	spiInitStruct.SPI_Mode=SPI_Mode_Master;
	spiInitStruct.SPI_CPHA=spiConfig->SPI_CPHA;
	spiInitStruct.SPI_CPOL=spiConfig->SPI_CPOL;
	spiInitStruct.SPI_NSS=SPI_NSS_Soft;
	spiInitStruct.SPI_DataSize=SPI_DataSize_8b;
	spiInitStruct.SPI_FirstBit=spiConfig->SPI_FirstBit;
	spiInitStruct.SPI_BaudRatePrescaler=spiConfig->SPI_BaudRatePrescaler;
	spiInitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	spiInitStruct.SPI_CRCPolynomial=0;
	SPI_Init(SPI2, &spiInitStruct);
	SPI_CalculateCRC(SPI2, DISABLE);
	//Config. TX with DMA
	dmaInitStruct.DMA_Mode=DMA_Mode_Normal;
	dmaInitStruct.DMA_DIR=DMA_DIR_PeripheralDST;
	dmaInitStruct.DMA_M2M=DMA_M2M_Disable;
	dmaInitStruct.DMA_Priority=DmaPrio_SPI2;

	dmaInitStruct.DMA_MemoryBaseAddr=(uint32_t)spiConfig->buffer;
	dmaInitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dmaInitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;

	dmaInitStruct.DMA_PeripheralBaseAddr=(uint32_t)&(SPI2->DR);
	dmaInitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dmaInitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;

	dmaInitStruct.DMA_BufferSize=0; //will be set before Tx/Rx
	DMA_Init(DMA1_Channel5, &dmaInitStruct);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

	//Config. RX with DMA
	dmaInitStruct.DMA_Mode=DMA_Mode_Normal;
	dmaInitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;
	dmaInitStruct.DMA_M2M=DMA_M2M_Disable;
	dmaInitStruct.DMA_Priority=DmaPrio_SPI2;

	dmaInitStruct.DMA_MemoryBaseAddr=(uint32_t)spiConfig->buffer;
	dmaInitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dmaInitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;

	dmaInitStruct.DMA_PeripheralBaseAddr=(uint32_t)&(SPI2->DR);
	dmaInitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dmaInitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;

	dmaInitStruct.DMA_BufferSize=0; //will be set before Tx/Rx
	DMA_Init(DMA1_Channel4, &dmaInitStruct);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); //interrupt after Rx

	nvicInitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = IrqPrio_Spi2Finish;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

	//Enable SPI
	SPI_Cmd(SPI2, ENABLE);
	SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set); //TODO: any sense?

	return Config_OK;
}
