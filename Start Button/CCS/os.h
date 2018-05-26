#IFNDEF _OS_H_
#DEFINE _OS_H_



typedef struct timer {
   unsigned int16 count;
}timer;

typedef int(*func_type)(void);

typedef struct process_data
{
   unsigned char* name;
   func_type func;
}process_data_t;

typedef struct processes
{
   process_data_t* p_proc[5];
   uint8_t head;
   uint8_t tail;
   uint8_t proc_stack;
}proc_queue;



#define THREAD(x)    void THREAD_##x(void)

#define NEW_PROC(x)  process_data_t proc_##x = {#x, &THREAD_##x}
                      
#define CALL_PROC(x) proc_##x.func()

#define PROC_INIT(x) proc_list.p_proc[proc_number++] = &proc_##x  

#define PROC_POST(x) do{\
                        proc_list.p_proc[proc_list.head] = &proc_##x;\
                        proc_list.head = ((++proc_list.head) % 5);\
                        proc_list.proc_stack++;\
                        }while(FALSE);


/*Function definitions*/
void OsInit(void);
void SetSysTimer(unsigned int16 count);
unsigned int16 GetSysTimer(void);

extern proc_queue proc_list;

THREAD(SleepTask);

#ENDIF
