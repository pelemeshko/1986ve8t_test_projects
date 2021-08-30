#include "mdr32f8_config.h"             // Keil::Device:Startup
#include "RTE_Components.h"             // Component selection
#include "mdr32f8_port.h"               // Keil::Drivers:PORT
#include "MDR1986VE8T.h"

int main (void)
{
	CLK_CNTR->KEY = 0x8555AAA1;
  CLK_CNTR->PER0_CLK |= (1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17);   //port A,B,C,D,E clock enable
	
	PORTE->KEY = 0x8555AAA1;
	
	PORTE->SANALOG = 0xffffffff;
	PORTE->CFUNC[2] = 0x00000f00;
	PORTE->SPWR[1] =  0x00000020;
	PORTE->SOE = (1<<18);
	
	PORTE->SRXTX = (1<<18);
	PORTE->CRXTX = (1<<18);
	
	
	PORT_InitTypeDef GPIO_user_ini;
	GPIO_user_ini.PORT_CFUNC = PORT_CFUNC_USER;
	GPIO_user_ini.PORT_CANALOG = PORT_CANALOG_DIGITAL;
	GPIO_user_ini.PORT_COE = PORT_COE_OUT;
	GPIO_user_ini.PORT_CPULLDOWN =PORT_CPULLDOWN_ON;
	GPIO_user_ini.PORT_CPULLUP = PORT_CPULLUP_OFF;
	GPIO_user_ini.PORT_Pin=PORT_Pin_18;
	
	PORT_Init(PORTE,&GPIO_user_ini);
	
	PORT_SetBits(PORTE,PORT_Pin_18);

	while (1)
	{
	
	}
		
}
