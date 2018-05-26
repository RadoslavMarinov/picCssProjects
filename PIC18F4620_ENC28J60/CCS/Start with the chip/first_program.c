#include <18F4620.h>
#use delay(clock=40000000)
#fuses H4, NOWDT, NOLVP, NODEBUG
#use rs232(baud=9600, uart1, stream=STREAM_UI, errors)

#define BUTT PIN_A4
#define BUTT_PRESSED (!INPUT(BUTT))
#define G_LED PIN_A5
#define R_LED PIN_B5
#define Y_LED PIN_B4

#DEFINE LED_OFF(X) OUTPUT_HIGH(X)
#DEFINE LED_ON(X) OUTPUT_LOW(X)

#define LCD_ENABLE_PIN  PIN_E2
#define LCD_RS_PIN      PIN_E0
#define LCD_RW_PIN      PIN_E1
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7

#include <lcd.c>


void main ()
{
high(led);
delay_ms(200);
lcd_init();

low(led);
lcd_putc("R");
while(1){
   
   while(butt_pressed)
      high(led);
   low(led);
   printf(lcd_putc,"\fRiko");
   delay_ms(500);
   }

}
