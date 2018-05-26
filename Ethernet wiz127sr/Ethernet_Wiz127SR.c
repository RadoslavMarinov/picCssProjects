#include <18LF4550.h>
#fuses HSPLL, PLL4, CPUDIV1, VREGEN,  PUT, NODEBUG, CPUDIV1, USBDIV, NOXINST, NOIESO, NOWDT, NOPBADEN, NOMCLR
#use delay(clock =48M)
#use RS232(baud=9600, UART1, PARITY=N, BITS = 8, STOP=1, ERRORS)

#include <usb_cdc.h>
#DEFINE G_LED PIN_D0
#DEFINE Y_LED PIN_D1
#DEFINE R_LED PIN_C2


#INT_TIMER0 
void TMR0(){ 
usb_task();
output_toggle(R_LED);
} 

void main()
{
   int i;
   char ch_usb;
   char ch_rs;
   delay_ms(500);
   usb_init_cs(); 
   SETUP_TIMER_0(T0_DIV_1);
   
      while(!usb_enumerated()) 
      { 
         usb_task();
         output_toggle(Y_LED);
         delay_ms(50);

      }
   output_low(Y_LED);
   output_high(G_LED);
   ENABLE_INTERRUPTS(INT_TIMER0); 
   ENABLE_INTERRUPTS(GLOBAL); 
 
while (true)
   {
      if(usb_cdc_kbhit())
      {
         ch_usb=usb_cdc_getc();
         if(ch_usb=='E')output_toggle(Y_LED);
         putc(ch_usb);
      }
      if(kbhit())
      {
         ch_rs=getc();
         if(ch_rs=='R')output_toggle(Y_LED);
         usb_cdc_putc(ch_rs);
      }
      delay_ms(5);
   }
}
