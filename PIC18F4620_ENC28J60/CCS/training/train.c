#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT, NOPBADEN, NOMCLR, STVREN, NOLVP, NOPROTECT, NOCPD, NOWRT, NOWRTC
#use delay (crystal = 10M)
#define STACK_USE_CCS_PICENS TRUE
#define STACK_USE_MCPENC TRUE

#include "stacktsk.c"


void main(void){
while(true){
   output_toggle(pin_a0);
   delay_ms(500);
   }

}
