

#ifndef __RS232C__H 
#define __RS232C__H

#include "AllHand.h"

/////////////////////////////////////////////////////////
#define MODEM_TEST    0

#define IER_RBR     0x01
#define IER_THRE    0x02
#define IER_RLS     0x04

#define IIR_PEND    0x01
#define IIR_RLS     0x03
#define IIR_RDA     0x02
#define IIR_CTI     0x06
#define IIR_THRE    0x01

#define LSR_RDR     0x01
#define LSR_OE      0x02
#define LSR_PE      0x04
#define LSR_FE      0x08
#define LSR_BI      0x10
#define LSR_THRE    0x20
#define LSR_TEMT    0x40
#define LSR_RXFE    0x80
//////////////////////////////////////////////////////////////


extern uint8_t         MD5[16];                 //计费策略的MD5码
extern uint8_t         Strategy[24*4];          //策略0-23
extern uint8_t         RecodeNum;               //计费策略的记录数量

extern uint8_t	MD5_BAK[16];
extern uint8_t RecodeNum_BAK;
extern uint8_t Strategy_BAK[24*4];
extern uint8_t StrategyRev;

extern uint32_t RxMeterDtLen(uint32_t id );
extern void MeterParaInit(void );
extern void delayNS (uint32_t ulDly);
extern void uart2Init (void);
extern void uart2SendByte (uint8_t ucDat);
extern void uart2SendStr (uint8_t const *pucStr, uint32_t ulNum);
extern void Uart2RxbufClear(void );
extern uint8_t CheckSum0904(uint8_t *pdat,uint16_t dlen);
extern void ClearDataBuffer(void);
extern void main_rs232 (void);
#endif


