#IFNDEF _START_BUTTON_H_
#DEFINE _START_BUTTON_H_
//#include "start_button_conf.h"
#define DEBOUNCE_TIME         30             //milli secconds

/*Macros*/
#define on(x)                 output_high(x)
#define off(x)                output_low(x)
#define BUTTON_PRESSED        (!input(BUTTON))
#define BREAKS_PRESSED        (input(BREAKS))
#define LAMP_ON               (input(CHARGE))
#define MS_500                ((unsigned int16)1953)

#define WAITING               1
#define IDLE                  0

typedef int(*func_type)(void);

typedef struct process_data
{
   unsigned char* name;
   func_type func;
   int8 called;
   struct process_data* p_process;
}process_data_t;


struct button_stat
   {
      unsigned char perm;
      unsigned char* p_perm;
   };
   
typedef struct timer {
   unsigned int16 count;
}timer;


 


typedef struct processes
{
   process_data_t* p_proc[5];
}proc_queue;

extern int proc_number;
extern proc_queue proc_list;


#define THREAD(x)    void THREAD_##x(void)

#define NEW_PROC(x)  process_data_t proc_##x = {#x, &THREAD_##x, 0, 0}
                      
#define CALL_PROC(x) proc_##x.func()

#define PROC_INIT(x) proc_list.p_proc[proc_number++] = &proc_##x  

#define PROC_POST(x) proc_##x.called = WAITING



/*Function definitions*/
void StartButtonInit(void);
void StarterState_1(void);
void StarterState_2(void);
void StarterState_5(void);
unsigned char ButtonSignal(unsigned char* perm);
void ProcInit(void);
#ENDIF
