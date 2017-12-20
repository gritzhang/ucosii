/*
 * (c)COPYRIGHT
 * ALL RIGHT RESERVED
 *
 * FileName : socket.c
 * Revision History :
 * ---------- -------   ------------------------------------------------
 *  Date      version     Description
 * ---------- -------   ------------------------------------------------
 * 03/13/2012   1.6     Modified connect() and sendto() functions
 *                    	because of the ARP errata.
 * ----------	-------	------------------------------------------------
 * 10/28/2013	1.8			Modified connect() and sendto() functions 
 *							because of the ARP errata.
 *							but it was only solved the Erratum3.
 *							Supports the local subnet mask broadcasting				
 * ---------- -------   ------------------------------------------------
 */
#include "types.h"
#include "w5100.h"
#include "socket.h"

static uint16_t local_port;


/**
@brief  This Socket function initialize the channel in perticular mode, and set the port and wait for W5100 done it.
@return   1 for sucess else 0.
*/
uint8_t socket(
  SOCKET s,     /**< for socket number */
  uint8_t protocol,   /**< for socket protocol */
  uint16_t port,    /**< the source port for the socket */
  uint8_t flag    /**< the option for the socket */
  )
{
  uint8_t ret;
#ifdef __DEF_IINCHIP_DBG__
  printf("socket()\r\n");
#endif
  if ((protocol == Sn_MR_TCP) || (protocol == Sn_MR_UDP) || (protocol == Sn_MR_IPRAW) || (protocol == Sn_MR_MACRAW) || (protocol == Sn_MR_PPPOE))
  {
    close(s);
    IINCHIP_WRITE(Sn_MR(s),protocol | flag);
    if (port != 0) {
      IINCHIP_WRITE(Sn_PORT0(s),(uint8_t)((port & 0xff00) >> 8));
      IINCHIP_WRITE((Sn_PORT0(s) + 1),(uint8_t)(port & 0x00ff));
    } else {
      local_port++; // if don't set the source port, set local_port number.
      IINCHIP_WRITE(Sn_PORT0(s),(uint8_t)((local_port & 0xff00) >> 8));
      IINCHIP_WRITE((Sn_PORT0(s) + 1),(uint8_t)(local_port & 0x00ff));
    }
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_OPEN); // run sockinit Sn_CR

    /* +20071122[chungs]:wait to process the command... */
    while( IINCHIP_READ(Sn_CR(s)) )
      ;
    /* ------- */
    ret = 1;
  }
  else
  {
    ret = 0;
  }
#ifdef __DEF_IINCHIP_DBG__
  printf("Sn_SR = %.2x , Protocol = %.2x\r\n", IINCHIP_READ(Sn_SR(s)), IINCHIP_READ(Sn_MR(s)));
#endif
  return ret;
}


/**
@brief  This function close the socket and parameter is "s" which represent the socket number
*/
void close(SOCKET s)
{
#ifdef __DEF_IINCHIP_DBG__
  printf("close()\r\n");
#endif

  IINCHIP_WRITE(Sn_CR(s),Sn_CR_CLOSE);

  /* +20071122[chungs]:wait to process the command... */
  while( IINCHIP_READ(Sn_CR(s)) )
    ;
  /* ------- */

  /* +2008.01 [hwkim]: clear interrupt */
  #ifdef __DEF_IINCHIP_INT__
      /* m2008.01 [bj] : all clear */
         putISR(s, 0x00);
  #else
      /* m2008.01 [bj] : all clear */
    IINCHIP_WRITE(Sn_IR(s), 0xFF);
  #endif
}


/**
@brief  This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return 1 for success else 0.
*/
uint8_t listen(
  SOCKET s  /**< the socket number */
  )
{
  uint8_t ret;
#ifdef __DEF_IINCHIP_DBG__
  printf("listen()\r\n");
#endif
  if (IINCHIP_READ(Sn_SR(s)) == SOCK_INIT)
  {
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_LISTEN);
    /* +20071122[chungs]:wait to process the command... */
    while( IINCHIP_READ(Sn_CR(s)) )
      ;
    /* ------- */
    ret = 1;
  }
  else
  {
    ret = 0;
#ifdef __DEF_IINCHIP_DBG__
  printf("Fail[invalid ip,port]\r\n");
#endif
  }
  return ret;
}


/**
@brief  This function established  the connection for the channel in Active (client) mode.
    This function waits for the untill the connection is established.

@return 1 for success else 0.
*/
uint8_t connect(SOCKET s, uint8_t * addr, uint16_t port)
{
  uint8_t ret;
#ifdef __DEF_IINCHIP_DBG__
  printf("connect()\r\n");
#endif
  if
    (
      ((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      (port == 0x00)
    )
  {
    ret = 0;
#ifdef __DEF_IINCHIP_DBG__
  printf("Fail[invalid ip,port]\r\n");
#endif
  }
  else
  {
    ret = 1;
    // set destination IP
	IINCHIP_WRITE((Sn_DIPR0(s) + 0),addr[3]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[2]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[1]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[0]); 
    IINCHIP_WRITE(Sn_DPORT0(s),(uint8_t)((port & 0xff00) >> 8));
    IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8_t)(port & 0x00ff));
    // m2012.03.13 [ys]: set/clear subnet for ARP Errata
       clearSUBR();
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_CONNECT);
        /* m2008.01 [bj] :  wait for completion */
    while ( IINCHIP_READ(Sn_CR(s)) );
//    {
//        
//    }
/*
    // check the SYN packet sending...
   while ( IINCHIP_READ(Sn_SR(s)) != SOCK_SYNSENT )
    {
       if(IINCHIP_READ(Sn_SR(s)) == SOCK_ESTABLISHED)
       {
          break;
       }
       if (getSn_IR(s) & Sn_IR_TIMEOUT)
       {
          setSn_IR(s,(Sn_IR_TIMEOUT ));  // clear TIMEOUT Interrupt
          ret = 0;
          break;
       }
    }
	*/
      applySUBR();	

   }

  return ret;
}



/**
@brief  This function used for disconnect the socket and parameter is "s" which represent the socket number
@return 1 for success else 0.
*/
void disconnect(SOCKET s)
{
#ifdef __DEF_IINCHIP_DBG__
  printf("disconnect()\r\n");
#endif
  IINCHIP_WRITE(Sn_CR(s),Sn_CR_DISCON);

  /* +20071122[chungs]:wait to process the command... */
  while( IINCHIP_READ(Sn_CR(s)) )
    ;
  /* ------- */
}


/**
@brief  This function used to send the data in TCP mode
@return 1 for success else 0.
*/
uint16_t send(
  SOCKET s,     /**< the socket index */
  const uint8_t * buf,  /**< a pointer to data */
  uint16_t len    /**< the data size to be send */
  )
{
  uint8_t status=0;
  uint16_t ret=0;
  uint16_t freesize=0;
#ifdef __DEF_IINCHIP_DBG__
  printf("send()\r\n");
#endif

   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

   // if freebuf is available, start.
  do
  {
    freesize = getSn_TX_FSR(s);
//    status = IINCHIP_READ(Sn_SR(s));
    status = getSn_SR(0);
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
    {
      ret = 0;
      break;
    }
#ifdef __DEF_IINCHIP_DBG__
    printf("socket %d freesize(%d) empty or error\r\n", s, freesize);
#endif
  } while (freesize < ret);

      // copy data
  send_data_processing(s, (uint8_t *)buf, ret);
  IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);

  /* +20071122[chungs]:wait to process the command... */
  while( IINCHIP_READ(Sn_CR(s)) )
    ;
  /* ------- */

/* +2008.01 bj */
#ifdef __DEF_IINCHIP_INT__
  while ( (getISR(s) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
#else
  while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
#endif
    {
    /* m2008.01 [bj] : reduce code */
        if ( IINCHIP_READ(Sn_SR(s)) == SOCK_CLOSED )
        {
#ifdef __DEF_IINCHIP_DBG__
            printf("SOCK_CLOSED.\r\n");
#endif
            close(s);
            return 0;
        }
    }
/* +2008.01 bj */
#ifdef __DEF_IINCHIP_INT__
    putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
#else
  IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
#endif
    return ret;
}


/**
@brief  This function is an application I/F function which is used to receive the data in TCP mode.
    It continues to wait for data as much as the application wants to receive.

@return received data size for success else 0.
*/
uint16_t recv(
  SOCKET s,   /**< socket index */
  uint8_t * buf,  /**< a pointer to copy the data to be received */
  uint16_t len  /**< the data size to be read */
)
{
  uint16_t ret=0;
#ifdef __DEF_IINCHIP_DBG__
  printf("recv()\r\n");
#endif


  if ( len > 0 )
  {
    recv_data_processing(s, buf, len);
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV);

    /* +20071122[chungs]:wait to process the command... */
    while( IINCHIP_READ(Sn_CR(s)) )
      ;
    /* ------- */
    ret = len;
  }
  return ret;
}


/**
@brief  This function is an application I/F function which is used to send the data for other then TCP mode.
    Unlike TCP transmission, The peer's destination address and the port is needed.

@return This function return send data size for success else -1.
*/
uint16_t sendto(
  SOCKET s,     /**< socket index */
  const uint8_t * buf,  /**< a pointer to the data */
  uint16_t len,     /**< the data size to send */
  uint8_t * addr,     /**< the peer's Destination IP address */
  uint16_t port   /**< the peer's destination port number */
  )
{
//  uint8_t status=0;
//  uint8_t isr=0;
  uint16_t ret=0;

#ifdef __DEF_IINCHIP_DBG__
  printf("sendto(%s)\r\n", buf);
#endif
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;
  if
    (
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      ((port == 0x00)) ||(ret == 0)
    )
  {
     /* +2008.01 [bj] : added return value */
     ret = 0;
#ifdef __DEF_IINCHIP_DBG__
  printf("[ch%d] Fail[%.3d.%.3d.%.3d.%.3d, %.d, %d]\r\n",s, addr[0], addr[1], addr[2], addr[3] , port, len);
  printf("Fail[invalid ip,port]\r\n");
#endif
  }
  else
  {
    IINCHIP_WRITE(Sn_DIPR0(s),addr[0]);
    IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
    IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
    IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
    IINCHIP_WRITE(Sn_DPORT0(s),(uint8_t)((port & 0xff00) >> 8));
    IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8_t)(port & 0x00ff));
    // copy data
    send_data_processing(s, (uint8_t *)buf, ret);
    // m2012.03.13 [ys]: set/clear subnet for ARP Errata
    clearSUBR();
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);
    /* +20071122[chungs]:wait to process the command... */
    while( IINCHIP_READ(Sn_CR(s)) );

    /* ------- */

/* +2008.01 bj */
#ifdef __DEF_IINCHIP_INT__
    while ( (getISR(s) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
#else
     while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
#endif
    {
#ifdef __DEF_IINCHIP_INT__
        if (getISR(s) & Sn_IR_TIMEOUT)
#else
        if (IINCHIP_READ(Sn_IR(s)) & Sn_IR_TIMEOUT)
#endif
      {
#ifdef __DEF_IINCHIP_DBG__
        printf("send fail.\r\n");
#endif
/* +2008.01 [bj]: clear interrupt */
#ifdef __DEF_IINCHIP_INT__
          putISR(s, getISR(s) & ~(Sn_IR_SEND_OK | Sn_IR_TIMEOUT));  /* clear SEND_OK & TIMEOUT */
#else
          IINCHIP_WRITE(Sn_IR(s), (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
#endif
      return 0;
      }
    }

/* +2008.01 bj */
#ifdef __DEF_IINCHIP_INT__
     putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
#else
     IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
#endif
		 applySUBR();
	}
	return ret;
}



/**
@brief  This function is an application I/F function which is used to receive the data in other then
  TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well.

@return This function return received data size for success else -1.
*/
uint16_t recvfrom(
  SOCKET s,   /**< the socket number */
  uint8_t * buf,  /**< a pointer to copy the data to be received */
  uint16_t len,   /**< the data size to read */
  uint8_t * addr,   /**< a pointer to store the peer's IP address */
  uint16_t *port  /**< a pointer to store the peer's port number. */
  )
{
  uint8_t head[8];
  uint16_t data_len=0;
  uint16_t ptr=0;
#ifdef __DEF_IINCHIP_DBG__
  printf("recvfrom()\r\n");
#endif

  if ( len > 0 )
  {
    ptr = IINCHIP_READ(Sn_RX_RD0(s));
    ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(s) + 1);
#ifdef __DEF_IINCHIP_DBG__
    printf("ISR_RX: rd_ptr : %.4x\r\n", ptr);
#endif
    switch (IINCHIP_READ(Sn_MR(s)) & 0x07)
    {
    case Sn_MR_UDP :
        read_data(s, (uint8_t *)ptr, head, 0x08);
        ptr += 8;
        // read peer's IP address, port number.
        addr[0] = head[0];
        addr[1] = head[1];
        addr[2] = head[2];
        addr[3] = head[3];
        *port = head[4];
        *port = (*port << 8) + head[5];
        data_len = head[6];
        data_len = (data_len << 8) + head[7];

#ifdef __DEF_IINCHIP_DBG__
        printf("UDP msg arrived\r\n");
        printf("source Port : %d\r\n", *port);
        printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif

      read_data(s, (uint8_t *)ptr, buf, data_len); // data copy.
      ptr += data_len;

      IINCHIP_WRITE(Sn_RX_RD0(s),(uint8_t)((ptr & 0xff00) >> 8));
      IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8_t)(ptr & 0x00ff));
        break;

    case Sn_MR_IPRAW :
        read_data(s, (uint8_t *)ptr, head, 0x06);
        ptr += 6;

        addr[0] = head[0];
        addr[1] = head[1];
        addr[2] = head[2];
        addr[3] = head[3];
        data_len = head[4];
        data_len = (data_len << 8) + head[5];

#ifdef __DEF_IINCHIP_DBG__
        printf("IP RAW msg arrived\r\n");
        printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif
      read_data(s, (uint8_t *)ptr, buf, data_len); // data copy.
      ptr += data_len;

      IINCHIP_WRITE(Sn_RX_RD0(s),(uint8_t)((ptr & 0xff00) >> 8));
      IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8_t)(ptr & 0x00ff));
        break;
    case Sn_MR_MACRAW :
        read_data(s,(uint8_t*)ptr,head,2);
        ptr+=2;
        data_len = head[0];
        data_len = (data_len<<8) + head[1] - 2;

        read_data(s,(uint8_t*) ptr,buf,data_len);
        ptr += data_len;
        IINCHIP_WRITE(Sn_RX_RD0(s),(uint8_t)((ptr & 0xff00) >> 8));
        IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8_t)(ptr & 0x00ff));

#ifdef __DEF_IINCHIP_DGB__
      printf("MAC RAW msg arrived\r\n");
      printf("dest mac=%.2X.%.2X.%.2X.%.2X.%.2X.%.2X\r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
      printf("src  mac=%.2X.%.2X.%.2X.%.2X.%.2X.%.2X\r\n",buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
      printf("type    =%.2X%.2X\r\n",buf[12],buf[13]);
#endif
      break;

    default :
        break;
    }
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV);

    /* +20071122[chungs]:wait to process the command... */
    while( IINCHIP_READ(Sn_CR(s)) )
      ;
    /* ------- */
  }
#ifdef __DEF_IINCHIP_DBG__
  printf("recvfrom() end ..\r\n");
#endif
  return data_len;
}


uint16_t igmpsend(SOCKET s, const uint8_t * buf, uint16_t len)
{
  uint8_t status=0;
//  uint8_t isr=0;
  uint16_t ret=0;

#ifdef __DEF_IINCHIP_DBG__
  printf("igmpsend()\r\n");
#endif
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

  if  (ret == 0)
  {
     ;
#ifdef __DEF_IINCHIP_DBG__
  printf("%d Fail[%d]\r\n",len);
#endif
  }
  else
  {
    // copy data
    send_data_processing(s, (uint8_t *)buf, ret);
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);
/* +2008.01 bj */
    while( IINCHIP_READ(Sn_CR(s)) )
      ;
/* ------- */

/* +2008.01 bj */
#ifdef __DEF_IINCHIP_INT__
    while ( (getISR(s) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
#else
     while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
#endif
    {
      status = IINCHIP_READ(Sn_SR(s));
#ifdef __DEF_IINCHIP_INT__
        if (getISR(s) & Sn_IR_TIMEOUT)
#else
        if (IINCHIP_READ(Sn_IR(s)) & Sn_IR_TIMEOUT)
#endif
      {
#ifdef __DEF_IINCHIP_DBG__
        printf("igmpsend fail.\r\n");
#endif
         /* in case of igmp, if send fails, then socket closed */
         /* if you want change, remove this code. */
         close(s);
         /* ----- */

        return 0;
      }
    }

/* +2008.01 bj */
#ifdef __DEF_IINCHIP_INT__
      putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
#else
     IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
#endif
   }
  return ret;
}

