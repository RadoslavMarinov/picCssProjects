#include "NRF24L01.h"

void NRF_Init(void)
{
   output_low(CE);
   output_high(CS);
   delay_ms(250);    //whait for radio to settle
}



uint8_t WriteRegister(uint8_t addr, uint8_t data)
{
   uint8_t status_reg;
   addr &= ADDRESS_MASK;
   output_low(CS);
   status_reg = spi_xfer(addr | W_REGISTER_COMMAND);
   spi_xfer(data);
   output_high(CS);
   return status_reg;
}

uint8_t ReadRegister(uint8_t addr)
{
   uint8_t reg;
   addr &= ADDRESS_MASK;  
   output_low(CS);
   spi_xfer(addr);
   reg = spi_xfer(DUMMY_BYTE);
   output_high(CS);
   return reg;
}

uint8_t WriteMultyByteRegister(uint8_t addr, void *p_data, uint8_t length)
{
   uint8_t status_reg;
   uint8_t *p = (uint8_t *)p_data;
   uint8_t  i;
   addr &= ADDRESS_MASK;
   output_low(CS);
   status_reg = spi_xfer(addr | W_REGISTER_COMMAND);
   for(i = 0; i < length; i++)
   {
      spi_xfer(p[i]);
   }
   output_high(CS);
   return status_reg;
}

uint8_t ReadMultyByteRegister(uint8_t addr, void *p_data, uint8_t length)
{
   uint8_t status_reg;
   uint8_t *p = (uint8_t *)p_data;
   uint8_t  i;
   addr &= ADDRESS_MASK;
   output_low(CS);
   status_reg = spi_xfer(addr);
   for(i = 0; i < length; i++)
   {
      p[i] = spi_xfer(DUMMY_BYTE);
   }
   output_high(CS);
   return status_reg;
}

uint8_t ReadRxFifo(void *p_data, uint8_t length)
{
   uint8_t *p = (uint8_t *)p_data;
   uint8_t  i;
   output_low(CS);
   spi_xfer(R_RX_PAYLOAD_COMMAND);
   for(i = 0; i < length; i++)
   {
      p[i] = spi_xfer(DUMMY_BYTE);
   }
   output_high(CS);
   if(ReadRegister(FIFO_STATUS) & RX_EMPTY)
      return TRUE;
   else
      return FALSE;   
}

void WriteTxFifo(void *p_data, uint8_t length)
{
   uint8_t *p = (uint8_t *)p_data;
   uint8_t  i;
   output_low(CS);
   spi_xfer(W_TX_PAYLOAD_COMMAND);
   for(i = 0; i < length; i++)
   {
      spi_xfer(p[i]);
   }  
   output_high(CS);
}

uint8_t FlushTX(void)
{
   output_low(CS);
   spi_xfer(FLUSH_TX_COMMAND);
   output_high(CS);
   if(ReadRegister(FIFO_STATUS) & TX_EMPTY)
      return TRUE;
   else 
      return FALSE;
}

uint8_t FlushRX(void)
{
   output_low(CS);
   spi_xfer(FLUSH_RX_COMMAND);
   output_high(CS);
   if(ReadRegister(FIFO_STATUS) & RX_EMPTY)
      return TRUE;
   else 
      return FALSE;
}

void ReuseTxPayload(void)
{
   output_low(CS);
   spi_xfer(FLUSH_RX_COMMAND);
   output_high(CS);
}

uint8_t ReadRxPayloadWidth(void)
{
   uint8_t size;
   output_low(CS);
   spi_xfer(R_RX_PL_WID_COMMAND);
   size = spi_xfer(DUMMY_BYTE);
   output_high(CS);
   return size;
}

void WriteTxPayloadNoAck(void *p_data, uint8_t length)
{
   uint8_t *p = (uint8_t *)p_data;
   uint8_t  i;
   output_low(CS);
   spi_xfer(W_TX_PAYLOAD_NOACK_COMMAND);
   for(i = 0; i < length; i++)
   {
      spi_xfer(p[i]);
   }  
   output_high(CS);
}

uint8_t ReadStatusRegister(void)
{
   uint8_t status_reg;
   output_low(CS);
   status_reg = spi_xfer(NOP);
   output_high(CS); 
   return status_reg;
}
