#include <16F1508.h>
#fuses NOMCLR
#use delay(clock = 16M)
//#use rs232(baud=9600, xmit=PIN_,rcv=PIN_)
#define led_15   pin_C7
#define led_30   pin_C6
#define led_45   pin_C5
#define led_60   pin_C4
#define led_75   pin_C3
#define led_90   pin_C2
#define led_105  pin_C1
#define led_120  pin_C0
#define led_135  pin_B4
#define led_150  pin_B5

#define butt_inc input(pin_A0)
#define butt_dec input(pin_A1)


void main()
{
unsigned int set_time;
unsigned int ms_10;
unsigned int sec;
unsigned int min;
output_c(0x00);
output_b(0x00);
while (true)
   {
   for(min=0;min<set_time;min++)
         {
         for(sec=0;sec<60;sec++)
            {
            for(ms_10=0;ms_10<100;ms_10++)
               {
               
               }
            }
         }
   }
}
