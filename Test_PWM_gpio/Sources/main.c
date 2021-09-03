#include "1986ve8_lib/cm4ikmcu.h"
#include <string.h>
#include <stdio.h>
#include "sysinit.h"
#include "eerom.h"
#include "mko.h"
#include "adc.h"
#include "wdt.h"
#include "uarts.h"

#define MKO_Id      22
#define Timer3_IRQn (IRQn_Type)108

uint8_t Buff[256];
extern uint16_t ADCData[];

void Delay(int waitTicks);

int main() {
//  uint8_t leng;
//  uint16_t adc_data[7];

	  /*---------- System clock, PLL0 ----------*/
  CLK_CNTR->KEY = _KEY_;
  CLK_CNTR->HSE0_CLK = (1<<27)|(1<<28); // Enable HSE0 gen
  while((CLK_CNTR->HSE0_STAT & (1 << 20)) == 0); //wait HSE0
  CLK_CNTR->PLL0_CLK = (2<<29)|(1<<28)|(1<<27)|(8<<8)|(1<<0);  //PLL0 On, Fout = 10*8/2 = 40MHz
  while((CLK_CNTR->PLL0_CLK & (1<<28)) == 0); //wait PLL ready
  CLK_CNTR->MAX_CLK = 8;  //MAX_CLOCK = PLL0 = 40 MHz
  CLK_CNTR->CPU_CLK = 0;  //Core clock: MAX_CLOCK
	
  /*-------------------- Ports Init --------------------------*/
  CLK_CNTR->KEY = _KEY_;
  CLK_CNTR->PER0_CLK |= (1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17)|(1<<26);  //port A,B,C,D,E,timer3 clock enable
	CLK_CNTR->TIM3_CLK = (1<<16)|(39); ///clocking is enablede and equal to CoreClk
	
	//GPIO setting
	PORTE->KEY = _KEY_;
	PORTE->SANALOG = 0xFFFFFFFF;
	PORTE->CFUNC[2] = 0x0000FFFF;
  PORTE->SFUNC[2] = 0x00008888; // function 8
	PORTE->SPWR[1] = 0xff; //enable drivers (fast front 10ns for all ports)

	//Setting the timer function
	MDR_TMR3->CNT = 0;
	MDR_TMR3->PSG = 0;
	MDR_TMR3->ARR = 100000;  //frequency = 100 Hz
	MDR_TMR3->CCR1 = 50000;	//PWM = 50 %
	MDR_TMR3->CCR2 = 8000;	//PWM = 8 %
	MDR_TMR3->CCR3 = 50000;	//PWM = 50 %
	


	MDR_TMR3->CH1_CNTRL = (7<<9);
	MDR_TMR3->CH1_CNTRL1 = (1<<8)|(2<<10); //PE19
	
	MDR_TMR3->CH2_CNTRL = (7<<9);
	MDR_TMR3->CH2_CNTRL1 = (1<<8)|(2<<10)|(1<<0)|(2<<2); //PE17, PE18
	
	MDR_TMR3->CH3_CNTRL = (7<<9);
	MDR_TMR3->CH3_CNTRL1 = (1<<0)|(2<<2); //PE16
	

	

	MDR_TMR3->CNTRL = (1<<0); //Timer is enable

	//MDR_TMR3->STATUS = 0;
	//NVIC_EnableIRQ(Timer3_IRQn);

	while(1) {

	}

}
