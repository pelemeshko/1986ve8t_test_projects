//#include "1986ve8_lib/cm4ikmcu.h"
#include "timer.h"

void Timer_Init(void)
{
	 /*-------------------- Ports Init --------------------------*/
  CLK_CNTR->KEY = _KEY_;
  CLK_CNTR->PER0_CLK |= (1<<23)|(1<<26);  enable Timer0, Timer3
	CLK_CNTR->TIM3_CLK = (1<<16)|(39); ///clocking is enable, timer clock freq = 1 MHz
	//timer0 - reserved
	//timer3 - are used for PWM (PE16-PE19)
	//
	
	//*--------------------	Inizialization --------------------------*/
  MDR_TMR0->CNTRL = 0x00000000;
  MDR_TMR3->CNTRL = 0x00000000;

	//*--------------------	Timer3 (PWM)--------------------------*/
	MDR_TMR3->CNT = 0;
	MDR_TMR3->PSG = 0;
	MDR_TMR2->IE = 0x00000002;  //generate IRQ during CNT=ARR
	MDR_TMR3->ARR = 5000000;  //frequency = 0,2 Hz
	MDR_TMR3->CCR1 = 50000;	//PWM = 50 %
	MDR_TMR3->CCR2 = 80000;	//PWM = 80 %
	MDR_TMR3->CCR3 = 5000;	//PWM = 50 %
	

	MDR_TMR3->CH1_CNTRL = (7<<9);
	MDR_TMR3->CH1_CNTRL1 = (1<<8)|(2<<10); //PE19
	MDR_TMR3->CH2_CNTRL = (7<<9);
	MDR_TMR3->CH2_CNTRL1 = (1<<8)|(2<<10)|(1<<0)|(2<<2); //PE17, PE18
	MDR_TMR3->CH3_CNTRL = (7<<9);
	MDR_TMR3->CH3_CNTRL1 = (1<<0)|(2<<2); //PE16
	MDR_TMR3->CNTRL = (1<<0); //Timer is enable
	
	//NVIC_EnableIRQ(IRQn_TMR3);
	
}
