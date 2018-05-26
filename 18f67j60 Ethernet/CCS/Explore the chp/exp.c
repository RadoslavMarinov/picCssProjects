#include <18F67J60.h>
#use delay(clock=41666667)
#fuses NOWDT, H4_SW, NOIESO, NOFCMEN, PRIMARY, ETHLED
#use rs232(baud=9600, uart1, errors)

#define MM_Page_Program 0x82  //Main memory page program opcode
#define RSt_Reg         0xD7  //Read Status Register opcode
#define Buffer_Read     0xD4  //Read Buffer opcode
#define Buffer_Write    0x84  //Write Buffer opcpde

#define dont_care 0
#define page_lenght 264

#define clk pin_c3
#define CS pin_F7
#define led pin_b4
#define led_on output_low(led)
#define led_off output_high(led)
#define butt_pressed !input(pin_b3)
#define CS_low output_low(cs)
#define CS_high output_high(cs)



#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H)

typedef unsigned int16 address;

//initialize the user settings
void user_init(void){
   delay_ms(20);      //Power up time
   setup_oscillator(OSC_PLL_5_DIV_3|OSC_NORMAL);
   led_off;
   cs_high;
   setup_spi(SPI_MASTER|SPI_MODE_0|SPI_CLK_DIV_64);
}

//This function returns 8bit register value. 
int8 read_status_register(void){
static int st_reg;
cs_low;
spi_write(RSt_Reg);
st_reg = spi_read(0) & 0xFC;     //Last two bits are unknown and not relevant
cs_high;
return st_reg;
}

//Reads buffer mamory from &ofset to &(ofset+arr_lengt) 
void read_buffer(address offset, int8 *arr, int16 arr_lenght){
int16 i;
if((offset+arr_lenght)>page_lenght)
   printf("The array is too long, will overlap the buffer!\n");   //Of you dont want to get this message, comment out if nad print lines
cs_low;
spi_write(Buffer_Read);
spi_write(dont_care);
spi_write((int8)(offset>>8));
spi_write((int8)offset);
spi_write(dont_care);
for(i=0;i<arr_lenght;i++)
   arr[i] = spi_read(0);
cs_high; 
}

void write_buffer(address offset, int8 *arr, int16 arr_lenght){
int16 i;
if((offset+arr_lenght) > page_lenght)
   printf("The array is too long, will overlap the buffer!\n");   //Of you dont want to get this message, comment out if nad print lines
cs_low;
spi_write(Buffer_Write);
spi_write(dont_care);
spi_write((int8)offset>>8);
spi_write((int8)offset);
for(i=0;i<arr_lenght;i++)
   spi_write(arr[i]);
cs_high;
}

//This function writes directly into the main memmory
void write_main_memory(address page_address, address byte_address, int8 *data, int16 data_lenght){
unsigned int16 i;
//cs_low;  //CS Setup Time - min 5ns

spi_write(MM_Page_Program);
//printf("Opcode: %u\n", opcode);
spi_write(((int8)(page_address>>7) & 0x03));
//printf("Byte 16-23: %u\n",((int8)(page_address>>7) & 0x03));
spi_write(((int8)(page_address<<1))|((int8)(byte_address>>8)));
//printf("Byte 8-15: %u\n",((int8)(page_address<<1))|((int8)(byte_address>>8)));
spi_write((int8)byte_address);
//printf("Byte 0-7: %u\n",(int8)byte_address);
for(i=0;i<data_lenght;i++){
   spi_write(data[i]);
   //printf("%d ",data[i]);
   }

//cs_high;
}


void main(void){
int16 i;

int8 data_arr[2];
int8 rec_data[10];
user_init();

for(i=0;i<265;i++)
data_arr[i] = i+5;

while(!butt_pressed);
while(butt_pressed);
while(1){
   write_buffer(0, data_arr,sizeof(data_arr));
   delay_ms(10);
   read_buffer(10, rec_data,sizeof(rec_data));
   for(i=0;i<sizeof(rec_data);i++){
   printf("%u ",rec_data[i]);
   }
   //write_main_memory(opcode, page_address, byte_address,data_arr, sizeof(data_arr));
   while(!butt_pressed);while(butt_pressed);
   }
}
