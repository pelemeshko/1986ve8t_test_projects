#ifndef _timer_H_
#define _timer_H_

#include <string.h>
#include <stdio.h>
#include <math.h>


#define IRQn_TMR3 (IRQn_Type)120

void Timers_Init(void);
void Timers_Start(uint8_t num, uint32_t time);
void Timers_Stop(uint8_t num);
void Timer_PWM_Set(uint8_t num, uint16_t pwm_val);
void INT_TMR3_Handler(void); 

#endif

