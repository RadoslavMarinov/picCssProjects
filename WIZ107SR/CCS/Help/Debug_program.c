#include <18LF4550.h>
#fuses HSPLL, PLL5, CPUDIV1, USBDIV, MCLR, NOWDT, PUT, NOXINST, NOXINST, NOPBADEN, VREGEN
#use delay(clock = 48M)
#use RS232 (baud=9600, UART1)

#DEFINE G_LED PIN_D0
#DEFINE Y_LED PIN_D1
#DEFINE R_LED PIN_C2
#DEFINE PUTime 500

#PRIORITY INT_RDA, INT_TIMER0
#BIT FSEN = getenv("SFR:UCFG").2

void USB_CDC_ISR();                    //interruption when usb_cdc character received into the buffer
void usb_enumeration();
#INCLUDE <usb_cdc.h>

#INT_TIMER0
void tmr0()
{
usb_task();
}

#INT_RDA
void rda_isr()
{
char rs_ch;
   if(kbhit())
   {
      rs_ch=getc();
      usb_cdc_putc_fast(rs_ch);
   }
}

void main()
{
delay_ms(PUTime);
usb_init_cs();
usb_enumeration();
SETUP_TIMER_0(T0_DIV_1);
ENABLE_INTERRUPTS(INT_TIMER0);
ENABLE_INTERRUPTS(INT_RDA);
ENABLE_INTERRUPTS(GLOBAL);
while(TRUE)
{
   output_toggle(Y_LED);
   delay_ms(1000);
}
}

void usb_enumeration()
   {
      output_low(G_LED);
      output_low(Y_LED);
      output_high(R_LED);
      do
      {
         output_toggle(R_LED);
         delay_ms(70);
         usb_task();
      }
      while(!usb_enumerated());
      output_low(R_LED);
      output_high(G_LED);
   }

void USB_CDC_ISR()
   {
   static char usb_ch;
      if(usb_cdc_kbhit())
      {
         usb_ch=usb_cdc_getc();
         putc(usb_ch);
      }
   }
