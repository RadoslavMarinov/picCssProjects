#DEFINE CS_LOW OUTPUT_LOW(CS)
#DEFINE CS_HIGH OUTPUT_HIGH(CS)



spi_w(int MSB, int LSB)
{
   cs_low;
   if(MSB!=0)
      spi_write (MSB);
   spi_write(LSB);
   cs_high;
}
