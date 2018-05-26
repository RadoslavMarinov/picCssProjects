#include <16f1508.h>
#fuses INTRC_IO,NOMCLR,NOWDT
#use delay(clock=16M)

#define inc_butt pin_a5
#define dec_butt pin_a4
#define confirm_butt pin_c5
#define led_r pin_c3
#define led_w pin_c4
#define relay_SB pin_c6
#define T1 pin_c7
#define T2 pin_b7
#include <flex_lcd.c>
unsigned int16 delay=300;
unsigned int16 rpm=100;
int1 print=1;

#INT_RA
void isr()                            
{
   if(!input(inc_butt)&&rpm<10000)
   {
      if(rpm<500)
         rpm+=100;
      else
         rpm+=500;
   }
   else if(!input(dec_butt)&&rpm>100)
   {
      if(rpm>500)
         rpm-=500;
      else
         rpm-=100;
   }
   print=1;
   write_program_eeprom(0xfff,rpm);
   clear_interrupt(INT_RA5_H2L|INT_RA4_H2L);
   }

void print_f();
void initialization();
void session();

void main()
{
output_high(led_w);
DISABLE_INTERRUPTS(INT_RA5_H2L|INT_RA4_H2L);
delay_ms(1500);  //wait for lcd booting
lcd_init();
output_low(T1);
output_low(T2);
initialization();
output_low(led_w);
session();
output_high(relay_SB);
ENABLE_INTERRUPTS(INT_RA5_H2L|INT_RA4_H2L);
ENABLE_INTERRUPTS(GLOBAL);
clear_interrupt(INT_RA5_H2L|INT_RA4_H2L);
output_high(led_r);
   while (true)
   {
      while(rpm<=3000)
         {
            if(print==1)
               print_f();
            output_low(T2);
            output_high(T1);
            delay_ms(delay);
            output_low(T1); 
            output_high(T2);
            delay_ms(delay); 
         }
      while(rpm>3000)
         {
            if(print==1)
               print_f();
            output_low(T2);
            output_high(T1);
            delay_us(delay);
            output_low(T1); 
            output_high(T2);
            delay_us(delay);
         }
   }
}
void session()
   {
      int c;
      printf(lcd_putc,"\f->Start Sessin");
      printf(lcd_putc,"\n  Last Session");     
      for(c=0;c<255;c++)
         {
            if((!input(dec_butt)))
               {
                  delay_ms(10);
                  if(!input(dec_butt))
                     {
                        printf(lcd_putc,"\f  Start Sessin");
                        printf(lcd_putc,"\n->Last Session");
                   
                        rpm=read_program_eeprom(0xfff);
                        if(rpm>6500)
                           output_high(led_w);
                           break;
                     }
               }
            if((!input(inc_butt)))
               {
               delay_ms(10);
               if(!input(inc_butt))
                  {
                     printf(lcd_putc,"\f->Start Sessin");
                     printf(lcd_putc,"\n  Last Session"); 
                     rpm=100;
                     break;
                  }
               }
            delay_ms(20);
         }
   }

void print_f()
   {
      switch(rpm)
         {
            case 100: 
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=300;
                  print=0;
               }
               break;
            case 200:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=150;
                  print=0;
               }
               break;
            case 300:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=100;
                  print=0;
               }
               break;
            case 400:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=75;
                  print=0;
               }
               break;
            case 500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=60;
                  print=0;
               }
               break;
            case 1000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=30;
                  print=0;
               }
               break;
            case 1500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=20;
                  print=0;
               }
               break;
            case 2000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=15;
                  print=0;
               }
               break;
            case 2500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=12;
                  print=0;
               }
               break;
            case 3000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=10;
                  print=0;
               }
               break;
            case 3500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=8570;
                  print=0;
               }
               break;
            case 4000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=7500;
                  print=0;
               }
               break;
            case 4500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=6666;
                  print=0;
               }
               break;
            case 5000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=6000;
                  print=0;
               }
               break;
            case 5500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=5454;
                  print=0;
               }
               break;
            case 6000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=5000;
                  print=0;
               }
               break;
            case 6500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=4615;
                  output_low(led_w);
                  print=0;
               }
               break;
            case 7000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=4285;
                  output_high(led_w);
                  print=0;
               }
               break;
            case 7500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=4000;
                  print=0;
               }
               break;
            case 8000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=3750;
                  print=0;
               }
               break;
            case 8500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=3529;
                  print=0;
               }
               break;
           case 9000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=3333;
                  print=0;
               }
               break;
            case 9500:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=3157;
                  print=0;
               }
               break;
            case 10000:
               {
                  printf(lcd_putc,"\fRPM %lu",rpm);
                  delay=3000;
                  print=0;
               }
               break;
               
         }
   }
   
void initialization()
   {
      int i;
      char dev[23] = "\fDEVELOPED BY\nRIKOTECH";
      char mail[23] = "\fprorandy88@\ngmail.com";
      char logo[27] = "\fDOCTOR MOTORS\nCAR SERVICE";
      for(i=0;i<=sizeof(dev)-2;i++)
         {
            lcd_putc(dev[i]);
            delay_ms(100);
         }
      delay_ms(2000);
      for(i=0;i<=sizeof(mail)-2;i++)
         {
            lcd_putc(mail[i]);
            delay_ms(150);
         }
      delay_ms(2000);
      for(i=0;i<=sizeof(logo)-2;i++)
         {
            lcd_putc(logo[i]);
            delay_ms(100);
         }
      delay_ms(2000);
   }

