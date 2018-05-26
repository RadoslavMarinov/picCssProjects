#include <16F1508.h>
#FUSES INTRC_IO, NOWDT, PUT, NOMCLR, NOPROTECT, NOBROWNOUT, NOCLKOUT, NOIESO, NOFCMEN, NOWRT, NOLPBOR, NODEBUG, NOLVP
#USE delay(internal = 16M)
#use spi(FORCE_HW, SLAVE, BITS=8, ENABLE=PIN_C6, MODE=0, ENABLE_ACTIVE=0)

#define LED PIN_A5

unsigned char data;

#INT_SSP
void SpiRcv(void)
{
   data = spi_xfer(0);
   if(data == 0x55)
      output_toggle(LED);
   clear_interrupt(INT_SSP);
}



void main()
{
   setup_oscillator(OSC_16MHZ);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   clear_interrupt(INT_SSP);
     while(TRUE)
   {

   }  
}

