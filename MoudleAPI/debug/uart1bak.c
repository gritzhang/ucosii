/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2009-05-12
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chengmingji
** Created date:        2009-07-24
** Version:             V1.00
** Descriptions:        添加用户应用程序
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         Li Baihua
** Modified date:       2009-07-27
** Version:				V1.01
** Descriptions:        编写UART1例程
**
** Rechecked by:        Zhang bin1
*********************************************************************************************************/
#include "UART1.h"

/*********************************************************************************************************
  变量与宏定义
*********************************************************************************************************/

/*********************************************************************************************************
  
*********************************************************************************************************/
uint8_t               RcvBufU1[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint8_t               SndBufU1[UARTBUFLEN];                                      /*  串口接收数据缓冲区          */
uint32_t              GulNumU1;                                             		 /*  串口接收数据的个数          */
uint32_t              RcvLenU1;                                              /*  串口接收数据的个数          */
uint32_t              SndLenU1;                                              /*  串口接收数据的个数          */
uint8_t         			RcvNewU1;                            		/* 串口接收新数据的标志         */


#define UART1_BPS  	9600 												/* 串口通信波特率               */
/*********************************************************************************************************
** Function name:	    UART1Init
** Descriptions:	    UART1初始化，设置为8位数据位，1位停止位，无校验，波特率为UART1_BPS
** input parameters:    ulDly:   值越大，延时时间越长
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1Init (void)
{
		       
    uint32_t Fdiv;
    
		LPC_SC->PCONP |= 0x00000010;
    #if 0
        LPC_IOCON->P0_15 &= ~0x07;
        LPC_IOCON->P0_15 |= 1;                                      /* P0.15?aU1_TXD                */
        LPC_IOCON->P0_16 &= ~0x07;
        LPC_IOCON->P0_16 |= 1;                                      /* P0.16?aU1_RXD                */
    #endif
    #if 1
        LPC_IOCON->P3_16 &= ~0x07;
        LPC_IOCON->P3_16 |= 3;                                      /* P3.16?aU1_TXD                */
        LPC_IOCON->P3_17 &= ~0x07;
        LPC_IOCON->P3_17 |= 3;                                      /* P3.17?aU1_RXD                */
    #endif
    #if 0
        LPC_IOCON->P2_0  &= ~0x07;
        LPC_IOCON->P2_0  |= 2;                                      /* P2.0?aU1_TXD                 */
        LPC_IOCON->P2_1  &= ~0x07;
        LPC_IOCON->P2_1  |= 2;                                      /* P2.1?aU1_TXD                 */
    #endif
    /*
     * 8?aêy?Y??￡??T?????ì?é??￡?1??í￡?1??
     */
    LPC_UART1->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) /  (UART1_BPS*2);                    /*?ó3yêy?μ                      */
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                          /* ???¨3yêy·??ê                 */
    LPC_UART1->FCR = 0x07;                                          /* ê1?ü2￠?′??FIFO               */
    
    NVIC_EnableIRQ(UART1_IRQn);
  
    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;    
 
}

/*********************************************************************************************************
** Function name:	    UART1RxbufClear
** Descriptions:    	清空接受缓存
** input parameters:    
**                      
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1RxbufClear(void )
{
		if(RcvNewU1!=0||RcvLenU1!=0){
			RcvNewU1 = 0;                                              		/* 清除标志*/
			RcvLenU1 = 0;
			memset(RcvBufU1,0,sizeof(RcvBufU1));
		}
}

/*********************************************************************************************************
** Function name:	    UART1SendByte
** Descriptions:	    向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1SendByte (uint8_t ucDat)
{
    LPC_UART1->THR = ucDat;                                                      /* 写入数据                     */
    while ((LPC_UART1->LSR & 0x20) == 0);                                		/* 等待数据发送完毕             */
}


/*********************************************************************************************************
** Function name:	    UART1SendStr
** Descriptions:    	向串口发送字符串
** input parameters:    pucStr:  要发送的字符串指针
**                      ulNum:   要发送的数据个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UART1SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
		UART1RxbufClear();
    for (i = 0; i < ulNum; i++) {                                       /* 发送指定个字节数据           */
        UART1SendByte (*pucStr++);
    }

}

/*********************************************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : UART1 中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void UART1_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;
// 		uint32_t UART1Status = 0;

  	uiIIRValue = LPC_UART1->IIR;

    uiIIRValue >>= 1;                                                     /* 中断标志寄存器值右移1位    */
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {                                        /* 接收线状态中断             */
        uiLSRValue = LPC_UART1->LSR;
        /* 
         * 溢出中断、奇偶中断、帧中断、UART RBR中包含错误数据和帧间隔中断处理
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
//             UART1Status = uiLSRValue;
            uiDummy = LPC_UART1->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {                                     /* 接收FIFO不为空中断         */
// 					  UART1Buffer[UART1Count] = LPC_UART1->RBR;
//             UART1Count++;
//             if ( UART1Count == UARTBUFLEN ) {
//                 UART1Count = 0;
//             }
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++){                 /*  连续接收8个字节             */
							if ( GulNumU1+RcvLenU1 >= UARTBUFLEN ) {
									UART1RxbufClear();
							}
							RcvBufU1[GulNumU1+RcvLenU1] = LPC_UART1->RBR;
            }
						RcvLenU1+=GulNumU1;
						RcvNewU1 = 1; 					
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {                                   /* 接收数据可用中断           */
//         UART1Buffer[UART1Count] = LPC_UART1->RBR;
//         UART1Count++;
//         if ( UART1Count == UARTBUFLEN ) {
//             UART1Count = 0;
//         }
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++){                 /*  连续接收8个字节             */
							if ( GulNumU1+RcvLenU1 >= UARTBUFLEN ) {
									UART1RxbufClear();
							}
							RcvBufU1[GulNumU1+RcvLenU1] = LPC_UART1->RBR;
            }
						RcvLenU1+=GulNumU1;
						RcvNewU1 = 1; 				
			
			
    }
    else if ( uiIIRValue == IIR_CTI ) {                                   /* 字符超时中断               */
          while ((LPC_UART1->LSR & 0x01) == 0x01){  
						if(RcvLenU1 >= UARTBUFLEN){
							UART1RxbufClear();
						}                       								/*  判断数据是否接收完毕        */ 
				    RcvBufU1[RcvLenU1] = LPC_UART1->RBR;
            RcvLenU1++;
					}
				  RcvNewU1 = 1; 											/* 	接收到完整数据帧 			*/
    }
    else if ( uiIIRValue == IIR_THRE ) {                                  /* 发送保持寄存器空中断       */
        uiLSRValue = LPC_UART1->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            //UART1TxEmpty = 1;
        } else {
            //UART1TxEmpty = 0;
        }
    }
}

/*********************************************************************************************************
** Function name:       main_UART1
** Descriptions:        UnionPay Card通讯
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void main_uart1 (void)
{
  OSTimeDly(OS_TICKS_PER_SEC); 										//延时等待系统其它部分正常工作
  
  UART1Init();                                                       /* 串口初始化                   */
 
	UART1RxbufClear();
	while(1){
		OSTimeDly(OS_TICKS_PER_SEC/10);
		if(RcvNewU1){
			OSTimeDly(OS_TICKS_PER_SEC/10);
			uart2SendStr(RcvBufU1,RcvLenU1);
			UART1RxbufClear();
		}
	}
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
