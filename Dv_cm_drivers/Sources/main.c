#include "1986ve8_lib/cm4ikmcu.h"
#include <string.h>
#include <stdio.h>
#include "sysinit.h"
#include "eerom.h"
#include "mko.h"
#include "adc.h"
#include "wdt.h"
#include "uarts.h"
#include "timer.h"


#define MKO_Id      22


uint8_t Buff[256];
extern uint16_t ADCData[];


int main() {
	uint8_t leng;
  uint16_t adc_data[14];

  System_Init();
	UART0_Init();
	Timers_Init();
	ADC_Init();
	
	sprintf((char*)Buff, "START\n\r");
	UART0_SendPacket(Buff, strlen((char*)Buff), 0);

while(1) {
		sprintf((char*)Buff, "ready\n\r");
//		UART0_SendPacket(Buff, strlen((char*)Buff), 0);
    WDRST;
    if(UART0_GetPacket(Buff, &leng)) {
      if(Buff[0] == 'A') {
        NVIC_DisableIRQ(IRQn_ADC0);
        memcpy(adc_data, ADCData, sizeof(adc_data));
        NVIC_EnableIRQ(IRQn_ADC0);
        sprintf((char*)Buff, "%4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d\n\r", adc_data[0], adc_data[1], adc_data[2], adc_data[3], adc_data[4], adc_data[5], adc_data[6], adc_data[7], adc_data[8], adc_data[9], adc_data[10], adc_data[11], adc_data[12], adc_data[13]);
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
					Timers_Stop(3);
					sprintf((char*)Buff, "PWM is disable\n\r");
					UART0_SendPacket(Buff, strlen((char*)Buff), 0);
				}
				else if(Buff[1] == '1'){
					Timers_Start(3, 1000000/Buff[2]); //	1Hz(Buff[2]), 255Hz(Buff[255]) 
					sprintf((char*)Buff, "Time is set\n\r");
					UART0_SendPacket(Buff, strlen((char*)Buff), 0);
				}
			}
    }
	}
}
