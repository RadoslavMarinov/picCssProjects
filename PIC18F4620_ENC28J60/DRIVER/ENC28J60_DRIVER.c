#DEFINE CS PIN_D3 //Define SPI Chip Select Pin according to your hardware
#DEFINE RESET PIN_D2
#DEFINE CS_LOW OUTPUT_LOW(CS)
#DEFINE CS_HIGH OUTPUT_HIGH(CS)


void ENC_INIT()
{
   output_high(reset);
}

void spi_w(int MSB, int LSB)
{
   cs_low;
   if(MSB!=0)
      spi_write (MSB);
   spi_write(LSB);
   cs_high;
}

void spi_r(int r_val)
{
   spi_read(r_val);
}

void W_CON_R(int OPC_ARG, int DATA)
{
   cs_low;
   spi_write(OPC_ARG|0x40);//|0x40 adds 010 before argument
   spi_write(DATA);
   cs_high;
}

int R_CON_R(int OPC_ARG)
{
   int data_in;
   cs_low;
   spi_read(OPC_ARG&0x1F);
   data_in=spi_read(0);
   cs_high;

   delay_us(10);
   
   return data_in;
}
