//////////////////////////////////////////////////////////////////////////////
//
// ex7a.c - Example 7A from the Embedded Internet/Embedded Ethernet tutorial
//
// Displays incoming Ethernet packets over RS232
//
//////////////////////////////////////////////////////////////////////////////
#include "ccstcpip.h"

void MACDisplayHeader(MAC_ADDR *mac, unsigned int8 type) 
{
   char str[20];
   
   MACToString(str, mac);

   printf("\r\nMAC: %s  PROT:0x08%X ", str, type);

   if (type==MAC_IP)
      printf("[IP]");
   else if (type==MAC_ARP)
      printf("[ARP]");
}

void main(void) {
   MAC_ADDR mac;
   unsigned int8 type;   
   
   Init();
   
   printf("\r\n\nCCS TCP/IP TUTORIAL 7A\r\n");

   // Init MAC engine.
   // Not needed in normal applications that call StackInit()
   MACInit();

   while(TRUE) 
   {
     #if STACK_USE_WIFI
      //attempt connect/reconnect to wifi router
      WIFIConnectTask();   //uses hardcoded ap/router info

      //handle wifi tasks.
      //not needed in normal applications that call StackTask().
      MACProcess();
      
      //don't attempt anything TCP/IP related if not connected.
      if (!MyWFisConnected())
         continue;
     #endif
      
      if (MACGetHeader(&mac, &type))
      {
         if (type!=MAC_UNKNOWN) 
         {
            MACDisplayHeader(&mac, type);
         }
      }
   }
}
