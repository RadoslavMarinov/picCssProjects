#include <16f628A.h>
#fuses INTRC_IO, WDT, MCLR, NOPROTECT, BROWNOUT
#device *=16
#use delay(clock=4M)
#USE RS232 (UART1, BAUD=9600, STREAM=DEBUG_STREAM, ERRORS)
#byte OPTION = 0x181
#bit TRMT = getenv("BIT:TRMT")

#define DEBUG 

#include "start_button.h"
#include "start_button_conf.h"

void ChipInit(void);
void ButtonProcess(struct button_stat* p_butt);
void GoSleep();

THREAD(WakeUpTask)
{
   if(restart_cause() == WDT_FROM_SLEEP)
   {
      output_high(STARTER_LED);
      delay_ms(50);
      output_low(STARTER_LED);
   }
}

THREAD(ToggleStarterLed)
{
      output_toggle(STARTER_LED);
}


NEW_PROC(ToggleStarterLed);
NEW_PROC(WakeUpTask);

#INT_EXT
void ButtISR()
{
   PROC_POST(ToggleStarterLed);
}

void main(void)
{
int i;

ChipInit();
PROC_INIT(ToggleStarterLed);
PROC_INIT(WakeUpTask);

   while(TRUE)
   {

      
      for(i = 0; proc_list.p_proc[i] != 0; i++)
      {
         if(proc_list.p_proc[i] -> called == WAITING)
         {
            #IFDEF  DEBUG
               fprintf(DEBUG_STREAM, "%s\n", proc_list.p_proc[i] -> name);
               while(!TRMT)
                  ;
            #ENDIF
            proc_list.p_proc[i]->func();
            proc_list.p_proc[i]->called = IDLE;
         }
      }
    restart_wdt();
    sleep();
    
    PROC_POST(WakeUpTask);
   }
}
void ButtonProcess(struct button_stat* p_butt)
{
   unsigned int16 i;
   static enum start_button_state{STATE_1, STATE_2, STATE_3, STATE_4}state = STATE_1;
   
         switch(state)
         {
            case STATE_1:
               if(ButtonSignal(p_butt->p_perm))
                  {
                     state = STATE_2;
                  }
               break;
            case STATE_2:
                StarterState_1();
               if(ButtonSignal(p_butt->p_perm))
                  {
                     state = STATE_3;
                  }
               break;
            case STATE_3:
               StarterState_2();
               if(ButtonSignal(p_butt->p_perm))
               {
                  if(BREAKS_PRESSED && LAMP_ON)
                  {
                        delay_ms(10);
                        state = STATE_4;
                        break;
                  }
                  else
                  {
                     StartButtonInit();
                     state= STATE_1;
                     break;
                  }
               }
               break;
            case STATE_4:
               StarterState_5();
               i = 0;
               while(BUTTON_PRESSED)
               {
                  i++;
                  if(i >= 400)
                     i = 399;
                  delay_ms(10);
               }
               for(; i < 400; i++)
               {
                  i++;
                  delay_ms(10);
                  if(!LAMP_ON)
                  {
                     StarterState_2(); //this line added at revision 1.0 The trasission must be instantaneous
                     state = STATE_3;
                     i = 0;
                     break;
                  }
               }
               StarterState_2(); //this line added at revision 1.0s //The trasission must be instantaneous
               state = STATE_3;
               break;   
         }
         if(!BUTTON_PRESSED)
         {
            p_butt->perm = 1;
         }
}
void ChipInit(void)
{
   setup_oscillator(OSC_4MHZ);
   OPTION &= (~(1 << 7));
   StartButtonInit();
   SETUP_CCP1(CCP_OFF);
   ext_int_edge(0, H_TO_L);
   ENABLE_INTERRUPTS(INT_EXT_H2L);
   ENABLE_INTERRUPTS(GLOBAL);
   setup_wdt(WDT_2304MS);
  
}  

void GoSleep()
{
   set_tris_a(0xFF); 
   set_tris_b(0xFF);
   sleep();
}
