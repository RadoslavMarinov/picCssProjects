#include <16F84A.h>
#fuses XT, NOWDT, PUT, NOPROTECT
#use delay (clock = 4M)

void main()
{
   while (1)
   {
      output_high(PIN_B3);
      delay_ms(500);
      output_Low(PIN_B3);
      delay_ms(500);
   }
}
