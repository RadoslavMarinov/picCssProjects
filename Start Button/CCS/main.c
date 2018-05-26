#include <16f628A.h>
#include <stdint.h>
#fuses INTRC_IO, WDT, MCLR, NOPROTECT, BROWNOUT
#device *=16
#use delay(clock=4M)
#USE RS232 (UART1, BAUD=9600, STREAM=DEBUG_STREAM, ERRORS)
#byte OPTION = 0x181
#bit TRMT = getenv("BIT:TRMT")

#define DEBUG 

#include "start_button.h"
#include "start_button_conf.h"
#include "os.h"
#include "os.c"



void ChipInit(void);

NEW_PROC(SleepTask);
THREAD(SleepTask)
{
   GoSleep();
}


THREAD(ButtonTask)
{
   BUTTON_FUNC();
}
NEW_PROC(ButtonTask);

THREAD(WakeUpTask)
{
   if(restart_cause() == WDT_FROM_SLEEP)
   {
      output_high(STARTER_LED);
      delay_ms(50);
      output_low(STARTER_LED);
   }
}
NEW_PROC(WakeUpTask);

THREAD(ToggleStarterLed)
{
   output_toggle(STARTER_LED);
}
NEW_PROC(ToggleStarterLed);

#include "start_button.c"

#INT_EXT
void ButtISR(void)
{
   PROC_POST(ToggleStarterLed);
   PROC_POST(WakeUpTask);
}

#INT_TIMER2
void Tmr2Isr(void)
{
   SetSysTimer(++GetSysTimer());
}

void main(void)
{

OsInit();
ChipInit();

//PROC_POST(ButtonTask);
   while(TRUE)
   {  
      while(proc_list.p_proc[proc_list.tail] != 0)
      {
            #IFDEF  DEBUG
               if(proc_list.proc_stack <= 0)
                  fprintf(DEBUG_STREAM, "Kernel problem !!!");
               fprintf(DEBUG_STREAM, "%s\n", proc_list.p_proc[proc_list.tail] -> name);
               fprintf(DEBUG_STREAM, "head = %u\n", (uint8_t)proc_list.head);
               fprintf(DEBUG_STREAM, "tail = %u\n", (uint8_t)proc_list.tail);
               while(!TRMT)
                  ;
            #ENDIF
            proc_list.p_proc[proc_list.tail]->func();
            proc_list.proc_stack--;
            proc_list.p_proc[proc_list.tail] = 0;
            proc_list.tail = (++proc_list.tail % 5);
      }
    restart_wdt();
    //PROC_POST(WakeUpTask);
   }
}

void ChipInit(void)
{
   setup_oscillator(OSC_4MHZ);
   OPTION &= (~(1 << 7));
   StartButtonInit();
   SETUP_CCP1(CCP_OFF);
   ext_int_edge(0, H_TO_L);
   setup_timer_2(T2_DIV_BY_1, 255, 4);
   ENABLE_INTERRUPTS(INT_EXT_H2L);
   ENABLE_INTERRUPTS(INT_TIMER2);
   ENABLE_INTERRUPTS(GLOBAL);
   setup_wdt(WDT_2304MS);
}  


