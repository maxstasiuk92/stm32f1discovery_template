#include "usedDmas.h"

#define NumberOfDmas			2

typedef struct {
	uint16_t channels;
} UsedDma;

UsedDma usedDma[NumberOfDmas];

void initUsedDmaStruct(){
	for(uint8_t i = 0; i < NumberOfDmas; i++) {
		usedDma[i].channels = 0;
	}
}

//dma=[1, NumberOfDmas], returns 0 if dma is out of this range
uint8_t getNumberOfChannelsInDma(uint8_t dma){
	uint8_t numberOfChannels;
	switch (dma) {
	case 1:
		numberOfChannels = 7;
		break;
	case 2:
		numberOfChannels = 7;
		break;
	default:
		numberOfChannels = 0;
		break;
	}
	return numberOfChannels;
}

//channel=[1, <number of channels>], returns "true" if channel is out of this range
//dma=[1, NumberOfDmas], returns "true" if dma is out of this range
uint16_t isChannelUsedInDMA(uint8_t dma, uint8_t channel){
	uint8_t numberOfChannels = getNumberOfChannelsInDma(dma);
	if(numberOfChannels < 1){
		return 1;
	}
	if (channel < 1 || channel > numberOfChannels) {
		return 1;
	}

	return (usedDma[dma].channels & (1 << channel)) ? 1 : 0;
}

void setUsedChannelInDMA(uint8_t dma, uint8_t channel){
	uint8_t numberOfChannels = getNumberOfChannelsInDma(dma);
	if(numberOfChannels > 0 && channel > 0 && channel <= numberOfChannels) {
		usedDma[dma].channels |= (1<<channel);
	}
}
