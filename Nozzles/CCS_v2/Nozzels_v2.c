#include <16F1508.H> 
#fuses INTRC_IO, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP 
#use delay(clock = 16MHZ)

//TRANZISTORS ASSIGNEMT
#define T1              PIN_C7
#define T2              PIN_B7
#define T_ON(x)         output_high(x)
#define T_OFF(x)        output_low(x)
//LED ASSIGNEMT
#define RED_LED         PIN_C4 
#define GREEN_LED       PIN_C3
#define LED_ON(x)       output_low(x)
#define LED_OFF(x)      output_high(x)
//BUTT ASSIGNEMT
#define UP_BUTT         input(PIN_A5)
#define DOWN_BUTT       input(PIN_A4)
#define BUTT_PRESSED(x) (!(x)) 
//relay activate output 
#define RELAY_ON()      output_high(PIN_C6)
#define RELAY_OFF()     output_low(PIN_C6)
//Limits assignment
#define MAX_RPM         8000
#define MIN_RPM         800
#define RPM_STEP        100
#define MAX_TIME_ON     10000
//LCD ASSIGNEMT
#define LCD_EXTENDED_NEWLINE
#define LCD_ENABLE_PIN  PIN_C2                               
#define LCD_RS_PIN      PIN_C0                                 
#define LCD_RW_PIN      PIN_C1                                    
#define LCD_DATA4       PIN_B4
#define LCD_DATA5       PIN_B5                                   
#define LCD_DATA6       PIN_B6                              
#define LCD_DATA7       PIN_A2 
#include  <lcd.c>

unsigned short int mode = 0;
unsigned long int rpm = 800;
unsigned long int time_on, time_off;
enum lcd_message {LOGO = 1, MAX_RPM_REACHED, MIN_RPM_REACHED, PRINT_RPM} message = LOGO;
const char logo_str[] = "Hello Riko...";

void chip_init(void);
void nozzle_off(void);
void nozzle_task(void);
void lcd_task(void);

#INT_TIMER1
void timer0_isr (void)
{
   static unsigned short int increase = 1;
   nozzle_off();
   if ((increase) && (rpm < 8000))
   {
      rpm += 100;  
   }
   else
   {
   increase = 0;
   }
   if ((!increase) && (rpm > 800))
   {
      rpm -= 100;
   }
   else
   {
      increase = 1;
   }

   if (rpm)
      if (rpm < 3000)
      {
         time_on = MAX_TIME_ON;
         time_off = (unsigned long int)((unsigned int32)(60000000 / rpm) - time_on);
      }
      else
      {
         time_off = (unsigned int16)((unsigned int32)(60000000 / rpm) / 2);
         time_on = time_off;
      }
}

#INT_RA
void int_handler(void)
{
   if (butt_pressed(UP_BUTT))
   {
      if (rpm < 7000)
      {
         rpm += 100;
         message = PRINT_RPM;
         if (rpm < 3000)
         {
            time_on = MAX_TIME_ON;
            time_off = (unsigned long int)((unsigned int32)(60000000 / rpm) - time_on);
         }
         else
         {
            time_off = (unsigned int16)((unsigned int32)(60000000 / rpm) / 2);
            time_on = time_off;
         }
      }
      else
      {
         message = MAX_RPM_REACHED;
      }
   }
   if (butt_pressed(DOWN_BUTT))
   {
      if (rpm > 800)
      {
         rpm -= 100;
         message = PRINT_RPM;
         if (rpm < 3000)
         {
            time_on = MAX_TIME_ON;
            time_off = (unsigned long int)((unsigned int32)(60000000 / rpm) - time_on);
         }
         else
         {
            time_off = (unsigned int16)((unsigned int32)(60000000 / rpm) / 2);
            time_on = time_off;
         }
      }
      else
      {
         message = MIN_RPM_REACHED;
      }
   }
clear_interrupt(INT_RA5_H2L);
clear_interrupt(INT_RA4_H2L);
}

//interrupt shall be cleared befor enableing 


void main(void)
{
chip_init();
   while (!mode)
   {
      if (message)
      {
         LED_ON(GREEN_LED);
         lcd_task();
         LED_OFF(GREEN_LED);
      }
      nozzle_task();
   }
   while (mode)
   {
      nozzle_task();
   }
}

void chip_init(void)
{
   relay_off();
   nozzle_off();
   led_off(GREEN_LED);
   led_off(RED_LED);
   SETUP_SPI(SPI_DISABLED);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(VREF_OFF);
   SETUP_DAC(DAC_OFF);
   setup_cwg(0,0,0,0);                 //CWG - Disabled
   setup_nco(0,0);                     //NCO - Disabled
   SETUP_ADC(ADC_OFF);
   setup_adc_ports(NO_ANALOGS);
   set_tris_a(0x03<<4);
   ENABLE_INTERRUPTS(INT_RA5_H2L);
   ENABLE_INTERRUPTS(INT_RA4_H2L);
   ENABLE_INTERRUPTS(GLOBAL);
   lcd_init();
   printf(lcd_putc, "\fInit...");
   delay_ms(1500);
   if (butt_pressed(DOWN_BUTT))
   {
      LED_ON(RED_LED);
      delay_ms(3000);
      if (butt_pressed(down_butt))
      {
         LED_OFF(RED_LED);
         DISABLE_INTERRUPTS (INT_RA5_H2L);
         DISABLE_INTERRUPTS (INT_RA5_H2L);
         SETUP_TIMER_1(T1_INTERNAL|T1_DIV_BY_4);
         set_timer1(0);
         ENABLE_INTERRUPTS (INT_TIMER1);
         clear_interrupt(INT_TIMER1);
         mode = 1;
      }
   }
   relay_on();
   
}

void nozzle_off(void)
{
   T_OFF(T1);   
   T_OFF(T2); 
}

void nozzle_task(void)
{
   T_ON(T1);
   delay_us(time_on);
   T_OFF(T1);
   T_ON(T2);
   delay_us(time_on);
   T_OFF(T2);
   if (time_on != time_off)
   {
      delay_us(time_off - time_on); 
   }
}

void lcd_task(void)
{
nozzle_off();
   switch (message)
   {
      case PRINT_RPM: 
         printf (lcd_putc,"\fRPM=%lu", rpm);
         break;
      case MAX_RPM_REACHED:
         printf (lcd_putc,"\fMax rpm reached");
         break;
      case MIN_RPM_REACHED:
         printf (lcd_putc,"\fMin rpm reached");
         break;
      case LOGO:
         printf (lcd_putc,"\f%s", logo_str);
         break;
       default: ;
   }
   message = 0;
}


