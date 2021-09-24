#include "1986ve8_lib/cm4ikmcu.h"
#include "timer.h"

int time_ARR = 1000000; //ititial tic time
int PWM_time = 0; //adder of events

void Timers_Init(void)
{
	 /*-------------------- Ports Init --------------------------*/
	CLK_CNTR->KEY = _KEY_;
	CLK_CNTR->PER0_CLK |= (1<<26);  //enable Timer0, Timer3
	CLK_CNTR->TIM3_CLK = (1<<16)|(39); ///clocking is enable, timer clock freq = 1 MHz
	//timer0 - reserved
	//timer3 - are used for PWM (PE16, PE17)

	
	//*--------------------	Inizialization --------------------------*/
	MDR_TMR3->CNTRL = 0x00000000;

	//*--------------------	Timer3 (PWM)--------------------------*/
	MDR_TMR3->CNT = 0;
	MDR_TMR3->PSG = 0; //Frequency selector
	MDR_TMR3->IE = 0x00000002;  //generate IRQ during CNT=ARR
	MDR_TMR3->ARR = time_ARR;  //frequency = 0,2 Hz
	
	MDR_TMR3->CH2_CNTRL = (7<<9);
	MDR_TMR3->CH3_CNTRL = (7<<9);
	/*
	MDR_TMR3->CCR2 = 0.2*time_ARR;	//PWM = 50 %
	MDR_TMR3->CCR3 = 0.2*time_ARR;	//PWM = 50 %
	MDR_TMR3->CH2_CNTRL1 = (1<<8)|(2<<10); //PE17(relay switcher1)
	MDR_TMR3->CH3_CNTRL1 = (1<<0)|(2<<2); //PE16(relay switcher2)
	MDR_TMR3->CNTRL = (1<<0); //Timer is enable
	*/	
	NVIC_EnableIRQ(IRQn_TMR3);
	
}

void INT_TMR3_Handler(void) 
{
    MDR_TMR3->STATUS = 0x0000;
    PWM_time += MDR_TMR3->ARR;
    if (PWM_time == 2*1000000) 
	{
		Timers_Stop(3);
		PWM_time=0;
	}
}

void Timers_Start(uint8_t num, uint32_t time)
{  
    switch (num)
    {
        case 3:
            NVIC_DisableIRQ(IRQn_TMR3);
            MDR_TMR3->ARR = time;  // time, min value 1.000.000 = 1 Hz, max 2.000 = 500 Hz
            MDR_TMR3->CCR2 = 0.2*time;	//PWM = 30 %
            MDR_TMR3->CCR3 = 0.2*time;	//PWM = 30 %
            MDR_TMR3->CH2_CNTRL1 = (1<<8)|(2<<10); //PE17(relay switcher1)
            MDR_TMR3->CH3_CNTRL1 = (1<<0)|(2<<2); //PE16(relay switcher2)
            MDR_TMR3->CNT = 0x00000000;  //Initial counter value
            MDR_TMR3->CNTRL = 0x00000001;  //Counting up the TIM_CLK.  Timer Enable
            NVIC_EnableIRQ(IRQn_TMR3);
            break;
    }
}


void Timers_Stop(uint8_t num)
{  
    switch (num)
    {
        case 3:
            NVIC_DisableIRQ(IRQn_TMR3);
            MDR_TMR3->CNTRL = 0x00000000;  //Timer Disable
           //MDR_TMR3->ARR = 0x00;  //time in ms
            MDR_TMR3->CNT = 0x00000000;  //Initial counter value
            MDR_TMR3->CH2_CNTRL1 = (1<<8)|(0<<10); //PE17(relay switcher1)
            MDR_TMR3->CH3_CNTRL1 = (1<<0)|(0<<2); //PE16(relay switcher2)
            NVIC_EnableIRQ(IRQn_TMR3);
            break;
    }
}


/**
  * @brief  Set PWM value for Timer3
  * @param  pwm_val is PWM value; available values are 0-99 (step 1%)
  */
void Timer_PWM_Set(uint8_t num, uint16_t pwm_val)
{   
		switch (num)
    {
			case 1:
					if (pwm_val >= 100){
						MDR_TMR3->CCR1 = 99;
					}
					else{
						MDR_TMR3->CCR1 = pwm_val;
					}
			case 2:
					if (pwm_val >= 100){
						MDR_TMR3->CCR2 = 99;
					}
					else{
						MDR_TMR3->CCR2 = pwm_val;
					}
        case 3:
					if (pwm_val >= 100){
						MDR_TMR3->CCR3 = 99;
					}
					else{
						MDR_TMR3->CCR3 = pwm_val;
					}
		}
}


