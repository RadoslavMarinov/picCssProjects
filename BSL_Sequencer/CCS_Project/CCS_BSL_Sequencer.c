#include <10F200.h>
#include <stdint.h>
#fuses NOWDT, NOMCLR
#use delay(clock = 4000000)
#USE FAST_IO (B)

#define LED                   PIN_B1
#define LED_ON                output_high(LED)
#define LED_OFF               output_low(LED)
#define BUTTON_IS_PRESSED     (!input(pin_b3))
#define TEST                  PIN_B2
#define RST                   PIN_B0
#define TEST_H                output_high(TEST)
#define TEST_L                output_low(TEST)
#define RST_H                 output_high(RST)
#define RST_L                 output_low(RST)
#define DEBOUNCE_DELAY        delay_ms(50);

void gpio_init(void)
{
   TEST_L;
   RST_L;
   LED_OFF;
   *((uint8_t*)0x05) &= ~(0x01);
}

void entry_seq(void)
{
   RST_L;
   delay_us(50);
   TEST_H;
   delay_us(30);
   TEST_L;
   delay_us(8);
   TEST_H;
   delay_us(15);
   RST_H;
   delay_us(15);
   TEST_L;
}

void exit_seq(void)
{
   TEST_H;
   delay_us(30);
   TEST_L;
   delay_us(30);
   TEST_H;
   delay_us(30);
   TEST_L;
   delay_us(60);
   RST_L;
   delay_us(60);
   RST_H;
}

void main()
{
uint8_t in_bsl_mode = 0, flag = 1;
   set_tris_b(0x08);
   
   #ASM
   CLRW
   MOVLW 0x00
   OPTION
   CLRW
   #ENDASM
   
   gpio_init();
   while (TRUE)
   {
   
      if(BUTTON_IS_PRESSED && !in_bsl_mode && flag)
      {
         DEBOUNCE_DELAY;
         if(BUTTON_IS_PRESSED)
         {
            flag = 0;
            entry_seq();
            in_bsl_mode = 1;
            LED_ON;
         }
      }
      
      
      if(BUTTON_IS_PRESSED && in_bsl_mode && flag)
      {
         DEBOUNCE_DELAY;
         if(BUTTON_IS_PRESSED)
         {
            flag = 0;
            exit_seq();
            in_bsl_mode = 0;
            LED_OFF;
         }
      }
      
      
      if(!BUTTON_IS_PRESSED)
         flag = 1;
   }
}
