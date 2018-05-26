#include <18F67J60.h>
#use delay(clock=41666667)
#fuses NOWDT, H4_SW, NOIESO, NOFCMEN, PRIMARY, ETHLED
#use rs232(baud=9600, uart1, errors)
#USE FAST_IO (C)


#define FLASH_SELECT PIN_F7
#define FLASH_CLOCK  PIN_C3
#define FLASH_DI     PIN_C5
#define FLASH_DO     PIN_C4
#define LED PIN_B4
#define BUTT_PRESSED !INPUT(PIN_B3)
#define LED_ON OUTPUT_LOW(LED)
#define LED_OFF OUTPUT_HIGH(LED)
#define LED_TOGG OUTPUT_TOGGLE(LED)
#define DEF_BUFF 0
#define TRIS_C_SET 0x90 //4,7 as inputs and 0,1,2,3,5,6 as outputs
#define erase_buff {}

#include "at45db021.c"

void user_init(void);
void wait_butt(void);
void print_array(int8 *arr, int16 arr_lenght);


void main(void)
{
int8 data[264] = {0,1,2,3,4,5,6,7,8,9};
int8 rec_arr[264];
user_init();
wait_butt();
led_on;
while(True){ 


}


void wait_butt(void){
while(!butt_pressed);
while(butt_pressed);
}

void user_init(void){
   setup_oscillator(OSC_PLL_5_DIV_3|OSC_NORMAL);
   delay_ms(20);    //Power up time
   SETUP_SPI(SPI_DISABLED);
   led_off;
   output_low(pin_c0);
   output_low(pin_c1);
   set_tris_c(TRIS_C_SET);
   init_ext_flash();
}

void print_array(int8 *arr, int16 arr_lenght){
int16 i;
for(i=0;i<arr_lenght; i++)
   printf("%u ",arr[i]);
}
