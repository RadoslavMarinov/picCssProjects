#IFNDEF _START_BUTTON_CONF_H_
#DEFINE _START_BUTTON_CONF_H_

/*input signals*/
#define IMMO                  
#define BUTTON                PIN_B0
#define BREAKS                PIN_A3
#define CHARGE                PIN_A2
#define SYS_CLK_OUTPUT        PIN_A6

/*output signals*/
#define CONTACT_LED           PIN_B4
#define STARTER_LED           PIN_B5
#define IGN1                  PIN_A1
#define IGN2                  PIN_A0
#define ACC                   PIN_B6
#define STARTER               PIN_B7

#define T1SYNC                (1 << 2)
#define T1OSCEN               (1 << 3)
#define TMR1CS                (1 << 1)
#define TMR1ON                (1)

#define STANDBY_TIME          60000       //~60sec

//Callbacks
#define BUTTON_FUNC()         ButtonProcess()

#ENDIF
