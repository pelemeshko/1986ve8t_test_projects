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


uint8_t Buff[256];
extern uint16_t ADCData[];


int main() {
	uint8_t leng;
  uint16_t adc_data[7];

	
  System_Init();
	UART0_Init();
	
		//Setting the timer function
	MDR_TMR3->CNT = 0;
	MDR_TMR3->PSG = 0;
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
	
	sprintf((char*)Buff, "START\n\r");
  UART0_SendPacket(Buff, strlen((char*)Buff), 0);

	//MDR_TMR3->STATUS = 0;
	//NVIC_EnableIRQ(Timer3_IRQn);


while(1) {
    WDRST;
    if(UART0_GetPacket(Buff, &leng)) {
      if(Buff[0] == 'A') {
        NVIC_DisableIRQ(IRQn_ADC0);
        memcpy(adc_data, ADCData, sizeof(adc_data));
        NVIC_EnableIRQ(IRQn_ADC0);
        sprintf((char*)Buff, "%4d %4d %4d %4d %4d %4d %4d\n\r", adc_data[0], adc_data[1], adc_data[2], adc_data[3], adc_data[4], adc_data[5], adc_data[6]);
        UART0_SendPacket(Buff, strlen((char*)Buff), 0);
        }
      else if(Buff[0] == 'B') {
        sprintf((char*)Buff, "%X\n\r", EXT_BUS_CNTR->RGN0_CNTRL);
        UART0_SendPacket(Buff, strlen((char*)Buff), 0);
        }
      else if(Buff[0] == 'R') {
        PORTE->SRXTX = 0x10000;
        }
			else if(Buff[0] == 'T') {
				if(Buff[1] == '0'){
					MDR_TMR3->CNTRL = (0<<0); //Timer is disable
				}
				else if(Buff[1] == '1')
					MDR_TMR3->CNTRL = (1<<0); //Timer is enable
        }
			}
    }
}


