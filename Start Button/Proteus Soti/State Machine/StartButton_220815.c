#include <16f628A.h>
#fuses INTRC_IO, NOWDT, NOMCLR, NOPROTECT
#device *=16
#use delay(clock=4M)
#use rs232(baud=9600, rcv=PIN_B0)

#define CONTACT_LED           PIN_A7
#define STARTER_LED           PIN_A6

#define IMMO                  PIN_B0
#define BUTTON                PIN_B5

#define IGN1                  PIN_A1
#define IGN2                  PIN_A0
#define ACC                   PIN_B6
#define STARTER               PIN_B7

#define BREAKS                PIN_B3
#define DINAMO                PIN_B2
#define STARTER_ON_SECS(x)    (x*100)     //resolution 10ms     

#byte OPTION = 0x181

//#define OPTION       (*((unsigned char *) 0x181))

#define BUTTON_PRESSED        (!(input(BUTTON)))
#define IMMO_IN               (input(IMMO))
#define BREAKS_ON             (input(BREAKS))
#define DINAMO_LAMP_IS_OFF    (!input(DINAMO))

#define AFTER_PRESS        45
#define DEBOUNCE_TIME      20
#define BEFORE_START_DELAY 20

void chip_init(void);
void starter_state_1(void);
void starter_state_2(void);
void starter_state_3(void);
void starter_state_2(void);
void starter_state_5(void);
unsigned char button_signal(void);

unsigned char button_permition_flag = 1;

void main(void)
{
enum {LOGIC_STATE_0, LOGIC_STATE_1, LOGIC_STATE_2, LOGIC_STATE_3, LOGIC_STATE_4, LOGIC_STATE_5}state_maschine = LOGIC_STATE_0;
   chip_init();
   while(TRUE)
   {
      switch (state_maschine)
      {

         case LOGIC_STATE_0:
            starter_state_3();
            if(button_signal())
            {
               state_maschine = LOGIC_STATE_1;
            }
            else
            {
               if(!BUTTON_PRESSED)
                  button_permition_flag = 1;
            }
            break;
///////////////////////////////////////////////////////////////////////////////            
         case LOGIC_STATE_1:
            starter_state_1();
            if(button_signal())
            {
               state_maschine = LOGIC_STATE_2;
            }
            else
            {
               if(!BUTTON_PRESSED)
                  button_permition_flag = 1;
            }
            break;
      
///////////////////////////////////////////////////////////////////////////////           
         case LOGIC_STATE_2:
            starter_state_2();
            if(!BREAKS_ON && button_signal())
            {
               state_maschine = LOGIC_STATE_0;
            }
            else if(BREAKS_ON && button_signal() && (!DINAMO_LAMP_IS_OFF))
            {
               unsigned int16 time = 0;
               delay_ms(BEFORE_START_DELAY);
               starter_state_5();   //1 1 0 1
               while(time < STARTER_ON_SECS(4))
               {
                  time++;
                  delay_ms(10);
                  if(DINAMO_LAMP_IS_OFF)
                  {
                     break;
                  }                 
               }
               while(BUTTON_PRESSED)
                  ;
               if(DINAMO_LAMP_IS_OFF)
                  state_maschine = LOGIC_STATE_3;
            }
            else
            {
               if(!BUTTON_PRESSED)
                  button_permition_flag = 1;
            }
            break;
///////////////////////////////////////////////////////////////////////////////         
         case LOGIC_STATE_3:
            starter_state_2();      //1 1 1 0
            if(!DINAMO_LAMP_IS_OFF)
            {
               state_maschine = LOGIC_STATE_2;
               break;
            }
            if(BREAKS_ON && button_signal())
            {
               state_maschine = LOGIC_STATE_0;
            }
            else
            {
               if(!BUTTON_PRESSED)
                  button_permition_flag = 1;
            }
            break;
///////////////////////////////////////////////////////////////////////////////
      }//Exit case switches
      
   }
}



void chip_init(void)
{
   output_low(STARTER);
   output_low(IGN1);
   output_low(IGN2);
   output_low(ACC);
   output_low(CONTACT_LED);
   output_low(STARTER_LED);
   OPTION &= (~(1 << 7));
}

void starter_state_1(void)
{
   output_high(ACC);
   output_low(IGN1);
   output_low(IGN2);
   output_low(STARTER);
}

void starter_state_2(void)
{
   output_high(ACC);
   output_high(IGN1);
   output_high(IGN2);
   output_low(STARTER);
}

void starter_state_3(void)
{
   output_low(ACC);
   output_low(IGN1);
   output_low(IGN2);
   output_low(STARTER);
}

void starter_state_5(void)
{
   output_high(ACC);
   output_high(IGN1);
   output_low(IGN2);
   output_high(STARTER);
}

unsigned char button_signal(void)
{
   if(BUTTON_PRESSED && button_permition_flag)
   {
      delay_ms(DEBOUNCE_TIME);
      if(BUTTON_PRESSED)
      {
         button_permition_flag = 0;
         return 1;
      }
      else 
         return 0;
   }
   else
      return 0;
}
