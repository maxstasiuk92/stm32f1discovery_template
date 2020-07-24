/*
 * timer.h
 *
 *  Created on: 23 лип. 2020 р.
 *      Author: MaxCm
 */

#ifndef TIMER_H_
#define TIMER_H_

ConfigResult configPwm1ToCtrlBridge(uint16_t period);

ConfigResult configPwm1ToCtrl2Drivers(uint16_t period);
void pwm1Ctrl2Drivers(int32_t drv1, int32_t drv2);

ConfigResult configPwm3ToCtrlBridge(uint16_t period);

ConfigResult configTimer2(uint16_t period);
ConfigResult configTimer4(uint16_t period);

#endif /* TIMER_H_ */
