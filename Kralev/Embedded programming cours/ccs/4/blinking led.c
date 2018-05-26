#include <16F84A.h>
#fuses XT, NOWDT, PUT, NOPROTECT
#use delay(clock = 4M)

void main()
{
   while (1)
   {
      output_low(PIN_B3);
      delay_ms(500);
      output_low(PIN_B3);
      delay_ms(500);
   }

}
