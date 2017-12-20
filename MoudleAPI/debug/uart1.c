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


#define 							UART1_BPS  	9600 												/* 串口通信波特率               */
/*********************************************************************************************************
  变量
*********************************************************************************************************/
uint8_t               RcvBufU1[UARTBUFLEN];                 /*  串口接收数据缓冲区          */
uint8_t               SndBufU1[UARTBUFLEN];                 /*  串口接收数据缓冲区          */
uint32_t              GulNumU1;                             /*  串口接收数据的个数          */
uint32_t              RcvLenU1;                             /*  串口接收数据的个数          */
uint32_t              SndLenU1;                             /*  串口接收数据的个数          */
uint8_t         			RcvNewU1;                       /* 串口接收新数据的标志         */


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
			memset(&RcvBufU1[0],0,sizeof(RcvBufU1));
		}
}

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
     * 8,n,1
     */
    LPC_UART1->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) /  (UART1_BPS*2);                    /*?ó3yêy?μ                      */
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                          /* ???¨3yêy·??ê                 */
    LPC_UART1->FCR = 0x87;                                          /* ê1?ü2￠?′??FIFO               */
    
    NVIC_EnableIRQ(UART1_IRQn);
  
    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;    
 
	UART1RxbufClear();
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

void UART1SendStr1 (uint8_t const *pucStr, uint32_t ulNum)
{
    UART1RxbufClear();

    while (1) {
        if (*pucStr == '\0') break;                                     /* 遇到结束符，退出             */
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


//Packet_Send_232Frame
void Packet_Send_232Frame(uint8_t dateLen, uint8_t * data,uint8_t len)
{
    uint8_t  cs=0,j,counter;
    SndBufU1[8] |= 0x80;
    SndBufU1[9] = dateLen;
    counter = len;
    memcpy(&SndBufU1[21],data,counter);
    //累加计算校验值
    counter += 21;
    for(j=0;j<counter;j++)
        cs += SndBufU1[j]; 
    SndBufU1[counter++] = cs;
    SndBufU1[counter++] = 0x16;
    SndLenU1 = counter;    
}

//232配置读取桩号帧
void Packet_Send_Pile_No(void)
{
    Packet_Send_232Frame(0x12,Term_Para1.ChargeNo,sizeof(Term_Para1.ChargeNo));
}

//232配置读取机器码信息帧
void Packet_Send_Maching_No(void)
{
    Packet_Send_232Frame(0x12,Term_Para1.TerminalNo,sizeof(Term_Para1.TerminalNo));
}

//232配置读取服务器ip信息帧
void Packet_Send_ServerIP(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.ServerIP,sizeof(Term_Para1.ServerIP));
}

//232配置读取网关ip信息帧
void Packet_Send_GateWay(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.GateWay,sizeof(Term_Para1.GateWay));
}

//232配置读取本机ip信息帧
void Packet_Send_SelfIP(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.SelfIP,sizeof(Term_Para1.SelfIP));  
}

//232配置读取子网掩码信息帧
void Packet_Send_SelfMASK(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.SelfMASK,sizeof(Term_Para1.SelfMASK));  
}

//232配置读取子网掩码信息帧
void Packet_Send_SelfMAC(void)
{
    Packet_Send_232Frame(0x10,Term_Para1.SelfMAC,sizeof(Term_Para1.SelfMAC));       
}


//发送232设置成功
void SendResetSucessFrame(uint8_t dateLen)
{
    uint8_t  cs=0,j,counter;
    SndBufU1[8] |= 0x80;
    SndBufU1[9] = dateLen;
    SndBufU1[10] = 0x00;
    SndBufU1[11] = 0x00;
    SndBufU1[12] = SndBufU1[16];
    SndBufU1[13] = SndBufU1[17];
    for(j=0;j<dateLen-3;j++)
        SndBufU1[14+j]= 0x00;  
    counter = 14+j;
    for(j=0;j<counter;j++)
        cs += SndBufU1[j];
    SndBufU1[counter++] = cs;
    SndBufU1[counter++] = 0x16;
    SndLenU1 = counter;       
}

//接收到232设置桩号帧
void Reset_Pile_No(void)
{
    memcpy(Term_Para1.ChargeNo,&SndBufU1[18],8);
    FRAMWrite(Term_Para1.ChargeNo,TERMINAL_PARA_ADD+2,8);
    SendResetSucessFrame(0x05);
}

//接收到232设置机器号帧
void Reset_Maching_No(void)
{
    memcpy(Term_Para1.TerminalNo,&SndBufU1[18],8);    
    FRAMWrite(Term_Para1.TerminalNo,TERMINAL_PARA_ADD+10,8);
    SendResetSucessFrame(0x05);    
}


//接收到232设置服务器ip帧
void Reset_ServerIP(void)
{
    memcpy(Term_Para1.ServerIP,&SndBufU1[18],4);
    FRAMWrite(Term_Para1.ServerIP,TERMINAL_PARA_ADD+100,4);
    FRAMWrite(Term_Para1.ServerIP,TERMINAL_PARA_ADD_BAK+100,4);    
    SendResetSucessFrame(0x04);    
}


//接收到232设置网关帧
void Reset_GateWay(void)
{
    memcpy(Term_Para1.GateWay,&SndBufU1[18],4); 
    FRAMWrite(Term_Para1.GateWay,TERMINAL_PARA_ADD+106,4); 
    SendResetSucessFrame(0x04);     
}


//接收到232设置本机ip帧
void Reset_SelfIP(void)
{
    memcpy(Term_Para1.SelfIP,&SndBufU1[18],4); 
    FRAMWrite(Term_Para1.SelfIP,TERMINAL_PARA_ADD+110,4);
    SendResetSucessFrame(0x04);     
}


//接收到232设置子网掩码
void Reset_SelfMASK(void)
{
    memcpy(Term_Para1.SelfMASK,&SndBufU1[18],4);
    FRAMWrite(Term_Para1.SelfMASK,TERMINAL_PARA_ADD+116,4);
    SendResetSucessFrame(0x04);     
}


//接收到232设置物理地址
void Reset_SelfMAC(void)
{
    memcpy(Term_Para1.SelfMAC,&SndBufU1[18],6);
    FRAMWrite(Term_Para1.SelfMAC,TERMINAL_PARA_ADD+120,6);
    SendResetSucessFrame(0x04);     
}



void CommandProcess(uint8_t *pdat,uint8_t no)
{
	uint8_t  temp,cmd;
    cmd = *(pdat+8);
    if(cmd == 0x01) //读取信息
    {
        memcpy(SndBufU1,RcvBufU1,RcvLenU1);        
        temp = *(pdat+19);
        switch(temp){
            case 0x01:
                Packet_Send_Pile_No();
                break;
            case 0x02:
                Packet_Send_Maching_No();
                break;
            case 0x08:
                Packet_Send_ServerIP();
                break; 
            case 0x0A:
                Packet_Send_GateWay();
                break;
            case 0x0B:
                Packet_Send_SelfIP();
                break;
            case 0x0D:
                Packet_Send_SelfMASK();
                break;
            case 0x0E:
                Packet_Send_SelfMAC();
                break;            
            default:
                break;            
        }
        UART1SendStr(SndBufU1,SndLenU1);        
    }else if(cmd == 0x08) //写信息
    {
        memcpy(SndBufU1,RcvBufU1,RcvLenU1);        
        temp = *(pdat+16);
        switch(temp){
            case 0x01:
                Reset_Pile_No();
                break;
            case 0x02:
                Reset_Maching_No();
                break;
            case 0x08:
                Reset_ServerIP();
                break; 
            case 0x0A:
                Reset_GateWay();
                break;
            case 0x0B:
                Reset_SelfIP();
                break;
            case 0x0D:
                Reset_SelfMASK();
                break;
            case 0x0E:
                Reset_SelfMAC();
                break;            
            default:
                break;            
        }        
        UART1SendStr(SndBufU1,SndLenU1);
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
    //uint8_t timdiv;
	
	OSTimeDly(OS_TICKS_PER_SEC); 										//延时等待系统其它部分正常工作		
	UART1Init();                                                       /* 串口初始化                   */	

	while (1)
	{
        OSTimeDly(OS_TICKS_PER_SEC/4);	
        if(RcvNewU1)
        {                                                       
            if(CheckSum0904((uint8_t *)&RcvBufU1,RcvLenU1)){
        		CommandProcess((uint8_t *)&RcvBufU1,1);
            }
            else
            {
        		OSTimeDly(OS_TICKS_PER_SEC/4);
        		if(CheckSum0904((uint8_t *)&RcvBufU1,RcvLenU1))	{
    				CommandProcess((uint8_t *)&RcvBufU1,1);
                }
            }
            UART1RxbufClear();								
        }
        
        
	}
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
