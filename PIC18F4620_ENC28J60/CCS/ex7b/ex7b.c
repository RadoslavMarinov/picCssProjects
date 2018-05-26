//////////////////////////////////////////////////////////////////////////////
//
// ex7b.c - Example 7B from the Embedded Internet/Embedded Ethernet tutorial
//
// Broadcasts an Ethernet frame (with a UDP/IP payload) every second.
// Use Ethereal to view network traffic.
//
// This example is largely academic, and shows raw MAC access.  Applications
// using the TCP/IP stack will usually not call or use these routines.
//
//////////////////////////////////////////////////////////////////////////////

#include "ccstcpip.h"



void main(void) {
MAC_ADDR mac_dest;
set_tris_b(0);
MACAddrInit();

MY_MAC_BYTE1=0xab;
MY_MAC_BYTE2=0xac;
MY_MAC_BYTE3=0xad;
MY_MAC_BYTE4=0xae;
MY_MAC_BYTE5=0xaf;
MY_MAC_BYTE6=0xbf;

MACInit();

mac_dest.v[0]=0xFF;
mac_dest.v[1]=0xFF;
mac_dest.v[2]=0xFF;
mac_dest.v[3]=0xFF;
mac_dest.v[4]=0xFF;
mac_dest.v[5]=0xFF;

while(TRUE) {
   if (MACIsTxReady()) {
         MACPutHeader(&mac_dest, ETHER_IP,sizeof(ExampleIPDatagram) + sizeof(ExampleUDPPacket));
         
         MACPutArray(ExampleIPDatagram, sizeof(ExampleIPDatagram));
         
         MACPutArray(ExampleUDPPacket, sizeof(ExampleUDPPacket));
         MACFlush();
         output_toggle(USER_LED2);
         printf("\n Riko e super   ");
         delay_ms(1000);
      }
}
}


