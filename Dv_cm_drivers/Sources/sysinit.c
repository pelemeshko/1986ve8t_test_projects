#include "1986ve8_lib/cm4ikmcu.h"


#ifdef _EXT_MEM_RLS_
const uint64_t ExtBusCfgCode[] __attribute__ ((at(0x10000400))) = 
{0x71, 0x71, 0,0,0,0,0,0,0,0};
#endif


extern uint32_t __Vectors;

void System_Init() {
  *((uint32_t*)0xE000ED08) = (uint32_t)&__Vectors;

  BKP->KEY = _KEY_;
  BKP->REG_60_TMR0 |= 1<<28; // diasable POR
  BKP->REG_60_TMR1 |= 1<<28; //
  BKP->REG_60_TMR2 |= 1<<28; //
    
  FT_CNTR->KEY = _KEY_;
  FT_CNTR->RESET_EVENT0 = 0x0; // hide all errors
  FT_CNTR->RESET_EVENT1 = 0x0; //
  FT_CNTR->RESET_EVENT2 = 0x0; //
  FT_CNTR->RESET_EVENT3 = 0x0; //
  FT_CNTR->RESET_EVENT4 = 0x0; //

  /*-------------------- Ports Init --------------------------*/
  CLK_CNTR->KEY = _KEY_;
  CLK_CNTR->PER0_CLK |= (1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17);   //port A,B,C,D,E clock enable

  /* PA[29,28,26,25,24,23,21,20]-MIL0 */
  PORTA->KEY = _KEY_;
  PORTA->SANALOG =   0xFFFFFFFF;
  PORTA->SFUNC[3] =  0x00CC0CCC;
  PORTA->SFUNC[2] =  0xC0CC0000;
  PORTA->SPWR[1] =   0x0A2A8A00;   //100ns
  PORTA->SPULLDOWN = 0xFFFFFFFF;

  /* PB[31,29,28]-MIL1; PB[26,25]-UART1; PB[19,18]-out1 TM; PB[17]-out1 MKADR; PB[16,15]-UART0; PB[14:9]-inpu MKADR; PB[0]-inpu ID */
  PORTB->KEY = _KEY_;
  PORTB->SANALOG =   0xFFFFFFFF;
  PORTB->SFUNC[3] =  0xD0DD0550;
  PORTB->SFUNC[2] =  0x00000005;
  PORTB->SFUNC[1] =  0x50000000;
  PORTB->SRXTX =     0x000E0000;
  PORTB->SOE =       0x000E0000;
  PORTB->SPWR[1] =   0x8A140055;
  PORTB->SPWR[0] =   0x40000000;
  PORTB->SPULLUP =   0x00007F01;
  PORTB->SPULLDOWN = 0x49F000FE;

  /* PC[31:30]-A[1:0]; PC[20:8]-ADC; PC[0]-MIL1 */
  PORTC->KEY = _KEY_;
  PORTC->SANALOG =   0xFFE000FF;
  PORTC->SFUNC[3] =  0x22000000;
  PORTC->SFUNC[0] =  0x0000000D;
  PORTC->SPWR[1] =   0xF0000000;
  PORTC->SPWR[0] =   0x00000002;
  PORTC->SPULLDOWN = 0xFFE000FE;

  /* PD[31:30]-D[1:0]; PD[24:23]-~WE,~OE; PD[19]-~CS;  PD[18:15]-out1 ~CS[4:1]; PD[12:0]-A[14:2] */  
  PORTD->KEY = _KEY_;
  PORTD->SANALOG =   0xFFFFFFFF;
  PORTD->SFUNC[3] =  0x22000002;
  PORTD->SFUNC[2] =  0x20002000;
  PORTD->SFUNC[1] =  0x00022222;
  PORTD->SFUNC[0] =  0x22222222;
  PORTD->SRXTX =     0x00078000;
  PORTD->SOE =       0x00078000;
  PORTD->SPWR[1] =   0xF003C0EA;
  PORTD->SPWR[0] =   0xC3FFFFFF;
  PORTD->SPULLDOWN = 0x3E706000;

  /* PE[22,20,18:12]-outod1; PE[23,21,19]-out0; PE[10,8,7]-SPI; PE[5:0]-D[7:2]*/
  PORTE->KEY = _KEY_;
  PORTE->SANALOG =   0xFFFFFFFF;
  PORTE->SFUNC[1] =  0x00000606;
  PORTE->SFUNC[0] =  0x60222222;
  PORTE->SRXTX =     0x0057F000;
  PORTE->SPD =       0x0057F000;
  PORTE->SOE =       0x00FFF000;
  PORTE->SPWR[1] =   0x00005555;
  PORTE->SPWR[0] =   0x55028FFF;
  PORTE->SPULLDOWN = 0xFF000E40;

  /*------------ enable regions -----------*/
  EXT_BUS_CNTR->KEY = _KEY_;
  EXT_BUS_CNTR->RGN0_CNTRL = 0x4000361;
  EXT_BUS_CNTR->RGN2_CNTRL = 0x321;

  /*---------- System clock, PLL0 ----------*/
  CLK_CNTR->KEY = _KEY_;
  CLK_CNTR->HSE0_CLK = (1<<27)|(1<<28); // Enable HSE0 gen
  while((CLK_CNTR->HSE0_STAT & (1 << 20)) == 0); //wait HSE0
  CLK_CNTR->PLL0_CLK = (2<<29)|(1<<28)|(1<<27)|(8<<8)|(1<<0);  //PLL0 On, Fout = 10*8/2 = 40MHz
  while((CLK_CNTR->PLL0_CLK & (1<<28)) == 0); //wait PLL ready
  CLK_CNTR->MAX_CLK = 8;  //MAX_CLOCK = PLL0 = 40 MHz
  CLK_CNTR->CPU_CLK = 0;  //Core clock: MAX_CLOCK

  /*---------- Clock for ADC0 -------------*/
  CLK_CNTR->ADC0_CLK = ((uint32_t)1<<28)|(1<<16)| 7;  //HSE0 clock for ADC, freq=0.5 MHz

#ifndef _INT_RAM_
  ICACHE->KEY = _KEY_;
  ICACHE->CNTL = 0x0B;
#endif
}



