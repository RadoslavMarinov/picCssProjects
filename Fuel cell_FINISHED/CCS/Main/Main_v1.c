
#include <16F1508.H>
#DEVICE*=16
#DEVICE ADC=8
#fuses INTRC_IO, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP 
#use delay(clock = 16MHZ)

#DEFINE PULSE_GEN_PIN   PIN_B6
#DEFINE DRIVER_ON     OUTPUT_LOW(PULSE_GEN_PIN)
#DEFINE DRIVER_OFF      OUTPUT_HIGH(PULSE_GEN_PIN)

#BYTE PR2 = getenv("SFR:PR2")

#define LCD_ENABLE_PIN  PIN_C1                                    
#define LCD_RS_PIN      PIN_A2                                   
#define LCD_RW_PIN      PIN_C0                                   
#define LCD_DATA4       PIN_C5                                  
#define LCD_DATA5       PIN_C4                                    
#define LCD_DATA6       PIN_C3                                    
#define LCD_DATA7       PIN_C6
#include <lcd.c> 


#INT_TIMER2
void timer2_interut_handler()
{
   DRIVER_OFF;
   disable_interrupts(INT_TIMER2);
}

#INT_RB
void interrupt_handler()
{
   DRIVER_ON;
   enable_interrupts(INT_TIMER2);
   set_timer2(0);
   clear_interrupt(INT_TIMER2);
   clear_interrupt(INT_RB7_L2H);
}

void chip_init();

void main()
{
unsigned int power;
unsigned int old = 0, new = 0;
chip_init();
   PR2 = 1;
   while(true)
   {
      PR2 = (unsigned int)(read_adc()* 0.608);
      new  = PR2;
      power = (unsigned int)(((float)PR2/155)*100);
      delay_ms(100);
      if(old != new)
      {
         printf(lcd_putc,"\fP=%u%%",power);
      }
      old = new;
   }        
}

void chip_init()
{
driver_off;
set_tris_b(1<<7);
setup_timer_2(T2_DIV_BY_64,255,4);

SETUP_ADC(ADC_CLOCK_INTERNAL);
SETUP_ADC_PORTS(sAN3);
SET_ADC_CHANNEL(3);
lcd_init();
delay_ms(200);
printf(lcd_putc,"\fHello!");
delay_ms(3500);
clear_interrupt(INT_RB7_L2H);
clear_interrupt(INT_TIMER2);
enable_interrupts(INT_RB7_L2H);
enable_interrupts(GLOBAl);
set_timer2(0);
}
