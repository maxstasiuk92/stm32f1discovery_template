/*
 * timer.c
 *
 *  Created on: 23 лип. 2020 р.
 *      Author: MaxCm
 */

#include "stm32f10x.h"
#include "config.h"
#include "usedPeriphMonitor.h"

ConfigResult configPwm1ToCtrlBridge(uint16_t period){
	return Config_Fail;
}

//CH1 - pos. half-bridge 1; CH2 - neg. half-bridge 1
//CH3 - pos. half-bridge 2; CH4 - neg. half-bridge 2
ConfigResult configPwm1ToCtrl2Drivers(uint16_t period){

	if (getUsedPinsInGPIOA() & (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11)) {
		return Config_Fail;
	}
	setUsedPinsInGPIOA(GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);

	//enable clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//config timer
	TIM1->PSC=0;
	TIM1->CNT=0;
	TIM1->ARR=period-1;

	TIM1->CCR1=0;
	TIM1->CCR2=0;
	TIM1->CCR3=0;
	TIM1->CCR4=0;

	TIM1->CR1=TIM_CR1_ARPE | TIM_CR1_URS;
	TIM1->CR2=0;
	TIM1->SMCR=0;
	TIM1->DIER=TIM_DIER_UIE;
	//PWM1, CCRx preload
	TIM1->CCMR1=TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE
			| TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
	TIM1->CCMR2=TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE
				| TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
	//off-state as OCx, lock all
	TIM1->BDTR=TIM_BDTR_OSSI | TIM_BDTR_OSSR | TIM_BDTR_LOCK_1 | TIM_BDTR_LOCK_0;
	//enable outputs, active high
	TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;

	//config pins
	GPIO_InitTypeDef gpioInit;
	gpioInit.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	gpioInit.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_Init(GPIOA, &gpioInit);

	//update interrupt
	NVIC_InitTypeDef nvicInit;
	nvicInit.NVIC_IRQChannel=TIM1_UP_TIM16_IRQn;
	nvicInit.NVIC_IRQChannelPreemptionPriority=IrqPrio_Pwm1Update;
	nvicInit.NVIC_IRQChannelSubPriority=0;
	nvicInit.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvicInit);

	TIM1->CR1 |= TIM_CR1_CEN;
	TIM1->BDTR |= TIM_BDTR_MOE;

	return Config_OK;
}

void pwm1Ctrl2Drivers(int32_t drv1, int32_t drv2){
	if (drv1>=0){
		TIM1->CCR1=drv1;
		TIM1->CCR2=0;
	} else {
		TIM1->CCR1=0;
		TIM1->CCR2=-drv1;
	}

	if (drv2>=0){
		TIM1->CCR3=drv2;
		TIM1->CCR4=0;
	} else {
		TIM1->CCR3=0;
		TIM1->CCR4=-drv2;
	}
}

ConfigResult configPwm3ToCtrlBridge(uint16_t period){
	return Config_Fail;
}

ConfigResult configTimer2(uint16_t period){
	TIM_TimeBaseInitTypeDef timInitStruct;
	TIM_OCInitTypeDef timOcInitStruct;
	NVIC_InitTypeDef nvicInitStruct;

	uint16_t autoReload=period-1;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timInitStruct.TIM_Prescaler = 0;
	timInitStruct.TIM_Period = autoReload;
	timInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	timInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timInitStruct);

	//Timer for driver
	timOcInitStruct.TIM_OCMode = TIM_OCMode_Timing;
	//timOcInitStruct.the rest = no effect
	TIM_OC1Init(TIM2, &timOcInitStruct);
	TIM_OC2Init(TIM2, &timOcInitStruct);
	TIM_OC3Init(TIM2, &timOcInitStruct);
	TIM_OC4Init(TIM2, &timOcInitStruct);
	TIM_SetCompare1(TIM2, (uint16_t)((uint32_t)autoReload*1/5));
	TIM_SetCompare2(TIM2, (uint16_t)((uint32_t)autoReload*2/5));
	TIM_SetCompare3(TIM2, (uint16_t)((uint32_t)autoReload*3/5));
	TIM_SetCompare4(TIM2, (uint16_t)((uint32_t)autoReload*4/5));
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	nvicInitStruct.NVIC_IRQChannel = TIM2_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = IrqPrio_Timer2;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

	TIM_Cmd(TIM2, ENABLE);

	return Config_OK;
}

ConfigResult configTimer4(uint16_t period){
	TIM_TimeBaseInitTypeDef timInitStruct;
	TIM_OCInitTypeDef timOcInitStruct;
	NVIC_InitTypeDef nvicInitStruct;

	uint16_t autoReload=period-1;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	timInitStruct.TIM_Prescaler = 0;
	timInitStruct.TIM_Period = autoReload;
	timInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	timInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timInitStruct);

	//Timer for driver
	timOcInitStruct.TIM_OCMode = TIM_OCMode_Timing;
	//timOcInitStruct.the rest = no effect
	TIM_OC1Init(TIM4, &timOcInitStruct);
	TIM_OC2Init(TIM4, &timOcInitStruct);
	TIM_OC3Init(TIM4, &timOcInitStruct);
	TIM_OC4Init(TIM4, &timOcInitStruct);
	TIM_SetCompare1(TIM4, (uint16_t)((uint32_t)autoReload*1/5));
	TIM_SetCompare2(TIM4, (uint16_t)((uint32_t)autoReload*2/5));
	TIM_SetCompare3(TIM4, (uint16_t)((uint32_t)autoReload*3/5));
	TIM_SetCompare4(TIM4, (uint16_t)((uint32_t)autoReload*4/5));
	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	NVIC_ClearPendingIRQ(TIM4_IRQn);
	nvicInitStruct.NVIC_IRQChannel = TIM4_IRQn;
	nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
	nvicInitStruct.NVIC_IRQChannelPreemptionPriority = IrqPrio_Timer4;
	nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInitStruct);

	TIM_Cmd(TIM4, ENABLE);

	return Config_OK;
}
