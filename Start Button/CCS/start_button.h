#IFNDEF _START_BUTTON_H_
#DEFINE _START_BUTTON_H_

#define DEBOUNCE_TIME         30             //milli secconds

/*Macros*/
#define on(x)                 output_high(x)
#define off(x)                output_low(x)
#define BUTTON_PRESSED        (!input(BUTTON))
#define BREAKS_PRESSED        (input(BREAKS))
#define LAMP_ON               (input(CHARGE))
#define MS_500                ((unsigned int16)1953)

typedef struct button_stat
   {
      unsigned char perm;
      unsigned char* p_perm;
   }button_stats_t;
   

void StartButtonInit(void);
void StarterState_1(void);
void StarterState_2(void);
void StarterState_5(void);
unsigned char ButtonSignal(void);
void ProcInit(void);
void ButtonProcess(void);
void GoSleep();
#ENDIF
