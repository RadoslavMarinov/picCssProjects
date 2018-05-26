//////////////////////////////////////////////////////////////////////////////
//
// ex14.c - Example 14 from the Embedded Internet/Embedded Ethernet tutorial
//
// A very simple webserver.  Webserver is written in this example (see
// HTTPTask()), does not use a CCS or Microchip written HTTP library.
//
// NOTE: Change the code in IPAddrInit() to your desired IP address, which
//       is based on your network.
//
//////////////////////////////////////////////////////////////////////////////

// force enabling of LCD library.  if hardware doesn't have LCD hardware,
// then it will use RS232/Serial for lcd_putc().
#define STACK_USE_PICDEM_LCD     1

#define STACK_USE_ICMP_SERVER
#define STACK_USE_TCP

// configure TCP stack for the correct number of sockets and RAM needed.
#define TCP_CONFIGURATION  1   //tell TCPIPConfig.h to not include default TCPSocketInitializer[] array.
// See ex13b.c for comments.
#define TCP_ETH_RAM_SIZE   1280

#include "ccstcpip.h"

// one general purpose TCP socket
TCPSocketInitializer_t TCPSocketInitializer[TCP_CONFIGURATION] =
{
    {TCP_PURPOSE_DEFAULT, TCP_ETH_RAM, 500, 500}
};

#define MY_HTTP_SOCKET  80

unsigned int8 HTTPSocket=INVALID_SOCKET;

void LCDTask(void) 
{
   static enum {LCD_INIT=0, LCD_NO_MAC, LCD_NO_DHCP, LCD_NO_WEB_INIT, LCD_NO_WEB, LCD_IDLE} state=0;

   switch(state) 
   {
      case LCD_INIT:
         printf(lcd_putc, "\fWebserver\n\rNo MAC Link");
         state = LCD_NO_MAC;
         LED_OFF(USER_LED1);
         break;
         
      case LCD_NO_MAC:
         if (MACIsLinked())
         {
           #if defined(STACK_USE_DHCP)
            if (!DHCPIsEnabled(0))
           #else
            if (1)
           #endif
            {
               state = LCD_NO_WEB_INIT;
            }
            else
            {
               printf(lcd_putc, "\fWebserver\n\rDHCP Not Bound");
               state = LCD_NO_DHCP;
            }
         }
         break;

   #if defined(STACK_USE_DHCP)
     case LCD_NO_DHCP:
         if (!MACIsLinked())
         {
            state = LCD_INIT;
            break;
         }
         if (DHCPIsBound(0))
         {
            state = LCD_NO_WEB_INIT;
            LED_ON(USER_LED1);
         }
         break;
   #endif
   
      case LCD_NO_WEB_INIT:
            printf(lcd_putc, "\fWebserver Idle\n\r%u.%u.%u.%u", MY_IP_BYTE1, MY_IP_BYTE2,
               MY_IP_BYTE3, MY_IP_BYTE4);
            state = LCD_NO_WEB;
            break;
            
      case LCD_NO_WEB:
         if (
               !MACIsLinked() 
              #if defined(STACK_USE_DHCP)
               || (DHCPIsEnabled(0) && !DHCPIsBound(0))
              #endif
            )
         {
            state = LCD_INIT;
         }
         if ((HTTPSocket!=INVALID_SOCKET) && TCPIsConnected(HTTPSocket))
         {
            SyncTCBStub(HTTPSocket);
            SyncTCB();
            
            printf(lcd_putc,"\fConnected to\n\r%u.%u.%u.%u", MyTCB.remote.niRemoteMACIP.IPAddr.v[0],
                     MyTCB.remote.niRemoteMACIP.IPAddr.v[1],
                     MyTCB.remote.niRemoteMACIP.IPAddr.v[2],
                     MyTCB.remote.niRemoteMACIP.IPAddr.v[3]
                   );
            state = LCD_IDLE;
         }
         break;

      case LCD_IDLE:
         if (
               !MACIsLinked()
              #if defined(STACK_USE_DHCP)
               || (DHCPIsEnabled(0) && !DHCPIsBound(0))
              #endif
            )
         {
            state = LCD_INIT;
         }
         if (!TCPIsConnected(HTTPSocket))
         {
            state = LCD_NO_WEB_INIT;
         }
         break;
   }
}

void HTTPPut(char c) {
   TCPPut(HTTPSocket, c);
}

void HTTPTask(void) {
   static enum {HTTP_ST_CONNECT=0, HTTP_ST_WAIT_CONNECT=1, HTTP_ST_GET=2,
      HTTP_ST_PUT=3, HTTP_ST_DISCONNECT=4} state=0;
   static TICK timeout_counter;
   static char lc, lc2;
   char c;
   char content[250];
   unsigned int8 a0;
  #if defined(SECONDARY_ADC_CHANNEL)
   unsigned int8 a1;
  #endif

   if (HTTPSocket==INVALID_SOCKET)
      state=HTTP_ST_CONNECT;
   else if (!TCPIsConnected(HTTPSocket))
      state=HTTP_ST_WAIT_CONNECT;
   else if (TickGetDiff(TickGet(), timeout_counter) > TICKS_PER_SECOND*60)
      state=HTTP_ST_DISCONNECT;

   switch(state) {
      case HTTP_ST_CONNECT:
         HTTPSocket=TCPListen(MY_HTTP_SOCKET);
         if (HTTPSocket!=INVALID_SOCKET) {
            printf("\r\nHTTP: Listening");
            state=HTTP_ST_WAIT_CONNECT;
            timeout_counter=TickGet();
         }
         break;

      case HTTP_ST_WAIT_CONNECT:
         timeout_counter=TickGet();
         if (TCPIsConnected(HTTPSocket)) {
            state=HTTP_ST_GET;
            printf("\r\nHTTP: Connected");
         }
         break;

      case HTTP_ST_GET:
         if (TCPIsGetReady(HTTPSocket)) {
            while (TCPGet(HTTPSocket, &c)) {
               if ( (c=='\n') && (lc2=='\n') ) {
                  state=HTTP_ST_PUT;
                  printf("\r\nHTTP: Got Page Request");
               }
               lc2=lc;
               lc=c;
            }
         }
         break;

      case HTTP_ST_PUT:
         if (TCPIsPutReady(HTTPSocket)) {
            printf("\r\nHTTP: Putting Page Response");
            a0=read_adc();
           #if defined(SECONDARY_ADC_CHANNEL)
            set_adc_channel(SECONDARY_ADC_CHANNEL);
            delay_us(20);
            a1=read_adc();
            set_adc_channel(STANDARD_ADC_CHANNEL);
           #endif
            sprintf(content, "<HTML><HEAD><TITLE>PICNET</TITLE></HEAD><BODY><H1>HELLO</H1>");
            sprintf(&content[0]+strlen(content),"<P>%s = 0x%X", STANDARD_ADC_STRING, a0);
           #if defined(SECONDARY_ADC_CHANNEL)
            sprintf(&content[0]+strlen(content),"<BR>%s = 0x%X", SECONDARY_ADC_STRING, a1);
           #endif
            sprintf(&content[0]+strlen(content),"</BODY></HTML>");
            printf(HTTPPut, "HTTP/1.1 200 OK\r\n");
            printf(HTTPPut, "Content-Type: text/html\r\n");
            printf(HTTPPut, "Content-Length: %u\r\n",strlen(content));
            printf(HTTPPut, "\r\n");
            printf(HTTPPut, "%s", content);
            printf("\r\nHTTP: Flushing...");
            TCPFlush(HTTPSocket);
            printf("\r\nHTTP: Flushed!");
            state=HTTP_ST_GET;
         }
         break;

      case HTTP_ST_DISCONNECT:
         if (TCPIsPutReady(HTTPSocket)) {
            printf("\r\nHTTP: Force Disconnect");
            TCPDisconnect(HTTPSocket);
            state=HTTP_ST_WAIT_CONNECT;
         }
         break;
   }
}

void main(void) 
{
   Init();
   
   printf("\r\n\nCCS TCP/IP TUTORIAL, EXAMPLE 14 (HTTP SERVER)\r\n");

   lcd_init();

   printf(lcd_putc, "\fInit Stack...");

   StackInit();
   
  #if STACK_USE_WIFI
   WIFIConnectTask();
  #endif

   while(TRUE) 
   {
      restart_wdt();

      LCDTask();

      StackTask();

     #if STACK_USE_WIFI
      WIFIConnectTask();   //uses hardcoded ap/router info

      //don't attempt anything else TCP/IP related if not connected.
      if (!MyWFisConnected())
         continue;
     #endif
      
      HTTPTask();
   }
}
