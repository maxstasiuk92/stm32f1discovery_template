#include "stm32f10x.h"

//for PWM1(on TIM1)
void TIM1_UP_TIM16_IRQHandler(){
	if(TIM_GetITStatus(TIM1, TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		//TODO: add logic on PWM1 update
	}
}

//for Timer2 (on TIM2)
void TIM2_IRQHandler(){
	if(SET == TIM_GetITStatus(TIM2, TIM_IT_CC1)){
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM2, TIM_IT_CC2)){
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM2, TIM_IT_CC3)){
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM2, TIM_IT_CC4)){
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM2, TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		//TODO: add some logic
	}
}

//for timer4(on TIM4)
void TIM4_IRQHandler(){
	if(SET == TIM_GetITStatus(TIM4, TIM_IT_CC1)){
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM4, TIM_IT_CC2)){
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM4, TIM_IT_CC3)){
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM4, TIM_IT_CC4)){
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
		//TODO: add some logic
	}
	if(SET == TIM_GetITStatus(TIM4, TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		//TODO: add some logic
	}
}
