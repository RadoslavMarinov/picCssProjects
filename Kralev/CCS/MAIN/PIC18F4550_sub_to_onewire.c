#include <18F4550.h> 
#fuses NOMCLR,HSPLL,PUT,USBDIV,PLL5,CPUDIV1,VREGEN,NOWDT
#use delay(clock = 48M) 

#define LED_Y PIN_D1 
#define LED_W PIN_D0 
#define USB_CON_SENSE_PIN PIN_B2

#include <usb_cdc.h> 


void main() 
{ 
   output_float(pin_b2);
  char str[9]; 
  int16 i=0; 
  usb_init_cs(); 
  
  while(!usb_enumerated()) 
  { 
       output_high(LED_Y);
       usb_task();
       delay_ms(20);
  } 
  output_low(LED_Y); 
    
   while (true) 
   { 
       usb_task();
           output_high(LED_W); 
       i++; 
       sprintf(str,"\r%lu sec",i); 
       usb_cdc_puts(str);
       delay_ms(500);
   } 
}
