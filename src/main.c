/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "board.h"
#include "config.h"
#include "spi.h"
#include "usedPeriphMonitor.h"

#define loopIfConfigFails(config) if(Config_Fail == config) while(1);

int main(void)
{
	initUsedPeriphMonitor();
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	loopIfConfigFails(setHsePinsUsed());
	loopIfConfigFails(setSwdPinsUsed());
	loopIfConfigFails(configLeds());
	loopIfConfigFails(configButtonForPolling());
	loopIfConfigFails(configSPI2AsMaster(0));

	pullUpUnusedPins();

	for(;;);
}
