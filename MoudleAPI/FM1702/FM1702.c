/****************************************************************************
* File :  main.c                                                            *
* COPYRIGHT BY HUOYAN LTD.COMPANY                                           *
* Version:      V1.3                                      			        *
*                                                                           *
* Created:      18.10.2005                                                  *
* Last Change:  21.10.2005                                                  *
*                                                                           *
* Author:       NIL                                                         *
*                                                                           *
* Compiler:     KEIL C51 V7.10                                              *
*                                                                           *
* Description:  AT89S52-Firmware for FM1702 Demo Serial Reader              *
*                                                                           *
****************************************************************************/
#define IN_FM1702_GLOBALS 
#include "FM1702.h"


/**************************************************************************************************
**	函数名称:	ddelay()
**	功	能	:	FRAM读写延时程序
**	入口参数:	dely
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	无
**	作	者	:	邓小颖
**	日	期	:	2009年03月06日
**************************************************************************************************/
void ddelay(INT16U dely)
{
	INT8U i, j;
	for(i  =0; i <= dely/10; i++)
	{
		for(j = 0; j <= dely; j++)
		{
			;
		}
	}
}

///*********************************************************************************************************
//** Function name:		SSP0_Init
//** Descriptions:		将SSP控制器设置为主机
//** input parameters:	无
//** output parameters:	无
//**********************************************************************************************************/
//void  SSP0_Init(void)
//{
//    uint8_t uiRegVal = 0;
//	
//    //PCONP 	|= (1 << 21);		    									/* 打开SSP0电源                 */
//    LPC_SC->PCONP |= (0x1 << 21);                                           /* 开启SSP0外设                 */

//	/* 
//	 * 设定外设分频值
//	 */
//	uiRegVal = LPC_SC->PCLKSEL;
//	if (uiRegVal < 4) {
//			LPC_SC->PCLKSEL = 4;
//	}
//	
//	LPC_IOCON->P0_15 &= ~0x07;
//	LPC_IOCON->P0_15 |=  0x02;                                              /* SSP CLK                      */
//	//LPC_IOCON->P0_16 &= ~0x07;    
//	//LPC_IOCON->P0_16 |=  0x02;                                            /* SSP SSEL                     */
//	LPC_IOCON->P0_17 &= ~0x07;
//	LPC_IOCON->P0_17 |=  0x02;                                              /* SSP MISO                     */
//	LPC_IOCON->P0_18 &= ~0x07;    
//	LPC_IOCON->P0_18 |=  0x02;                                              /* SSP MOSI                     */
//		
//	//PINSEL0 |= (2ul << 30);												/* 设置管脚功能 p0.15                */
//	//PINSEL1 |= (0 << 0 ) | (2 << 2 ) | (2 << 4 );						    /* 设置管脚功能 p0.16  17 18              */
//	//FIO0DIR2 |= 1 << 0 ;												    /* cs 管脚 p0.16 */
//	
//	//SET_SSPCSRF() ; 
//	LPC_IOCON->P0_16 &= ~0x07;                                              /* P0.16为SSP0的CS管脚          */
//	LPC_GPIO0->DIR |= (0x1<<16);
//	LPC_GPIO0->SET |= (0x1<<16);

////	PINSEL0 |= (0 << 6);	
////	FIO0DIR0 |= 1 << 3 ;												    /* cs 管脚 p0.3 */
////	SET_SSPCSFR() ; 

//    LPC_SSP0->CR0  = 0x0807;												/* 设置数据长度为8位，帧格式SPI，SCK高有效，*/
//    LPC_SSP0->CPSR = 0x02;			    		                            /* 设置SSP从PCLK获得的分频  */
//                                                                            /* 值,主模式下有效,         */
//                                                                            /* 最小值为0x02	            */
//    LPC_SSP0->CR1  = 0x02;		   			                                /* 设置SSP为主机模式,       */
//	  //0x01                 		   			                            /* 设置SSP为主机模式,       */	
//	                                                                        /* 主模式下有效, 最小值为0x02*/
//}


/**************************************************************************************************
**	函数名称:	FMSendByte()
**	功	能	:	向FRAM发送一字节数据
**	入口参数:	dat				:	所要发送的数据
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	ddelay();
**	作	者	:	邓小颖
**	日	期	:	2012年04月09日
**************************************************************************************************/
void  FMSendByte(INT8U data)
{  
    //while ((SSP0SR & 0x02) == 0);	    				                    /* 等待发送FIFO留出空间   	*/
    while (!(LPC_SSP0->SR & 0x01));	    				                    /* 等待发送FIFO留出空间   	*/
		LPC_SSP0->DR = data;
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 				                /* 等待数据帧发送完毕		*/
}

/**************************************************************************************************
**	函数名称:	FMReadByte()
**	功	能	:	从FRAM读出一字节数据
**	入口参数:	无
**	出口参数:	无
**  返回值  ：  data			:	读出的数据
**	全局变量:	无
**	调用模块:	ddelay();
**	作	者	:	邓小颖
**	日	期	:	2012年04月09日
**************************************************************************************************/
INT16U  FMReadByte(void )
{  
    INT8U tmp ;
    while ((LPC_SSP0->SR & 0x04) == 0x04){
    	tmp = LPC_SSP0->DR ;
    }	    				                    							/* 清空接收FIFO留出空间   	*/
    
    while ((LPC_SSP0->SR & 0x01) == 0);	    				                /* 清空发送FIFO留出空间   	*/
	LPC_SSP0->DR = 0xff ;
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 				                /* 等待数据帧发送完毕		*/

    while ((LPC_SSP0->SR & 0x04) == 0x00);	 				                /* 等待数据帧接收完毕		*/

	tmp = LPC_SSP0->DR ;

    return tmp;
}

/**************************************************************************************************
**	函数名称:	FMWrite()
**	功	能	:	向FRAM写数据
**	入口参数:	*dataptr	:	指向所要发送的数据的指针
**				addr		:	FRAM的起始存储地址
**				length		:	数据长度
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	FramSendByte、FramState
**	作	者	:	邓小颖
**	日	期	:	2012年04月09日
**************************************************************************************************/
void FMWrite(INT8U dataptr,INT8U addr)
{	
	INT8U	err;
	
//	OSSemPend(FRAMSem,0,&err);
	addr = addr << 1  ; 
	addr &= 0x7e ; 
	
	CLR_SSPCSRF() ;
	ddelay(5) ;
	FMSendByte(addr);

	FMSendByte(dataptr);

	SET_SSPCSRF();
	ddelay(5) ;
	
//	OSSemPost(FRAMSem);
}
/**************************************************************************************************
**	函数名称:	FRAMRead()
**	功	能	:	读FRAM
**	入口参数:	*dataptr	:	指向所要存放的数据的指针
**				addr		:	FRAM的起始存储地址
**				length		:	数据长度
**	出口参数:	无
**  返回值  ：  无
**	全局变量:	无
**	调用模块:	FramSendByte、FramReadByte、FramState
**	作	者	:	邓小颖
**	日	期	:	2012年04月09日
**************************************************************************************************/
void	FMRead(INT8U *dataptr, INT8U addr)
{	
	INT8U	err;
	
//	OSSemPend(FRAMSem,0,&err);
	addr = addr << 1  ; 
	addr |= 0x80 ; 

	CLR_SSPCSRF() ;
	ddelay(5) ;
	
	FMSendByte(addr);
	
	*dataptr = FMReadByte();
	
	SET_SSPCSRF();
	ddelay(5) ;

//	OSSemPost(FRAMSem);
}


///////////////////////////////////////////////////////////////////////
// IC卡处理函数
///////////////////////////////////////////////////////////////////////
void cmd_process(void)
{
    INT8U cmd;
    INT8U status;
	
	cmd = FMRevBuffer[0];
	switch(cmd)
	{
		case 1:     // Halt the card     //终止卡的操作
			status=FMMIF_Halt();			
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;
			break;			
		case 2:     
        	//MIF_Halt();     //RF_CMD_REQUEST_ALL=0x52, all
        	status = FMRequest(RF_CMD_REQUEST_ALL);	//RF_CMD_REQUEST_STD=0x26,  Idle
        	if(status != FM1702_OK)
        	{
        	    status = FMRequest(RF_CMD_REQUEST_ALL);		
        	    if(status != FM1702_OK)
        	    {
        	        FMRevBuffer[0] = 1;
        	        FMRevBuffer[1] = FM1702_REQERR;
        	        break;
        	    }
        	}
			if(tagtype[0]==2)
				cardtype=mifarepro;     // Mifare Pro 卡
			else if(tagtype[0]==4)
				cardtype=mifare1;       // Mifare One 卡
			else if(tagtype[0]==16)
				cardtype=mifarelight;   // Mifare Light 卡
			else
				cardtype=unknowncard;

			FMRevBuffer[0]=3;	
			FMRevBuffer[1]=status;
			FMRevBuffer[2]=tagtype[0];
			FMRevBuffer[3]=tagtype[1];
			break;
	    case 3:                         // 防冲突 读卡的系列号 MLastSelectedSnr
			status = FMAntiColl();;
			if(status!=FM1702_OK)
			{
				FMRevBuffer[0]=1;	
				FMRevBuffer[1]=FM1702_ANTICOLLERR;
				break;
			}
			//memcpy(MLastSelectedSnr,&RevBuffer[2],4);
			FMRevBuffer[0]=5;
			FMRevBuffer[1]=status;
			FMRevBuffer[2]=UID[0];
			FMRevBuffer[3]=UID[1];
			FMRevBuffer[4]=UID[2];
			FMRevBuffer[5]=UID[3];
			break;	
		case 4:		                    // 选择卡 Select Card
			status = FMSelect_Card();
			if(status!=FM1702_OK)
			{
				FMRevBuffer[0]=1;	
				FMRevBuffer[1]=FM1702_SELERR;
				break;
			}
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;			
			break;
        case 6:                        //下载密钥
			status = FMLoad_keyE2_CPY(FMRevBuffer[2],FMRevBuffer[1]);   //%40
			status = FMAuthentication(UID, FMRevBuffer[2], FMRevBuffer[1]);	
        	if(status != FM1702_OK)
        	{
        		FMRevBuffer[0]=1;
			    FMRevBuffer[1]=status;
			    break;
        	}
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;			
			break;	
		case 8:                       //读卡
			status=FMMIF_READ(&FMRevBuffer[2],FMRevBuffer[1]);
			if(status != FM1702_OK)
			{
			    FMRevBuffer[0]=1;
			    FMRevBuffer[1]=status;
			    break;
			}
			else
			{
				if(cardtype==mifare1||cardtype==mifarepro)
					FMRevBuffer[0]=17;
				else if(cardtype==1)
					FMRevBuffer[0]=9;
				else
					FMRevBuffer[0]=16;
			}
			FMRevBuffer[1]=status;			
			break;	
		case 9:                  //写卡
			status=FMMIF_Write(&FMRevBuffer[2],FMRevBuffer[1]);
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;			
			break;	
		case 10:                //加值减值
			if(FMRevBuffer[1] == RF_CMD_INC)
			{
			    status = FMMIF_Increment(&FMRevBuffer[3],FMRevBuffer[2]);
			    FMMIF_Transfer(FMRevBuffer[2]);
			}
			else if(FMRevBuffer[1] == RF_CMD_DEC)
			{
			    status = FMMIF_Decrement(&FMRevBuffer[3],FMRevBuffer[2]);   
			    FMMIF_Transfer(FMRevBuffer[2]);
			}
			else
			{
			    status = 1;
			}
			FMRevBuffer[0]=1;	
			FMRevBuffer[1]=status;
			break;
		case 12:    
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=0;
			break;
		default:
		    FMRevBuffer[0] = 1;
		    FMRevBuffer[1] = 1;
		    break;	
	}
}


/****************************************************************/
/*名称: Clear_FIFO */
/*功能: 该函数实现清空FM1702中FIFO的数据*/
/*输入: N/A */
/*输出: TRUE, FIFO被清空*/
/* FALSE, FIFO未被清空*/
/****************************************************************/
BOOLEAN FMClear_FIFO(void)
{
	INT8U	temp;
	INT32U	i;
	
	FMRead(&temp , FM_Control);		        /* 清空FIFO */
	temp = (temp | 0x01);
	FMWrite(temp, FM_Control);

	for(i = 0; i < RF_TimeOut; i++) 		/* 检查FIFO是否被清空 */
	{
		FMRead(&temp , FM_FIFO_Length );	/* 清空FIFO */
		if(temp == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/****************************************************************/
/*名称: Write_FIFO */
/*功能: 该函数实现向FM1702的FIFO中写入x bytes数据*/
/*输入: count, 待写入字节的长度*/
/* buff, 指向待写入数据的指针*/
/*输出: N/A */
/****************************************************************/
void FMWrite_FIFO(INT8U count, INT8U *buff)
{
	for(; count > 0 ; count --)
	{
		FMWrite(*buff++ , FM_FIFO );
	}
}

/****************************************************************/
/*名称: Read_FIFO */
/*功能: 该函数实现从FM1702的FIFO中读出x bytes数据*/
/*输入: buff, 指向读出数据的指针*/
/*输出: N/A */
/****************************************************************/
INT8U FMRead_FIFO(INT8U *buff)
{
	INT8U temp  ;

	FMRead(&temp , FM_FIFO_Length );	

	if(temp == 0)
	{
		return 0;
	}
	else if (temp >= 24)		
	{
		temp = 24;	
	}

	for(; temp > 0 ; temp --)
	{
		FMRead(buff++ , FM_FIFO );	
	}

	return temp;
}

/****************************************************************/
/*名称: Judge_Req */
/*功能: 该函数实现对卡片复位应答信号的判断*/
/*输入: *buff, 指向应答数据的指针*/
/*输出: TRUE, 卡片应答信号正确*/
/* FALSE, 卡片应答信号错误*/
/****************************************************************/
BOOLEAN FMJudge_Req(INT8U *buff)
{
	INT8U temp1, temp2;
	
	temp1 = *buff;
	temp2 = *(buff + 1);

	if((temp1 == 0x02) || (temp1 == 0x04) || (temp1 == 0x05) || (temp1 == 0x53) || (temp1 == 0x03))
	{
		if (temp2 == 0x00)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************/
/*名称: Check_UID */
/*功能: 该函数实现对收到的卡片的序列号的判断*/
/*输入: N/A */
/*输出: TRUE: 序列号正确*/
/* FALSE: 序列号错误*/
/****************************************************************/
BOOLEAN FMCheck_UID(void)
{
	
	INT8U	temp;
	INT8U	i;
	
	temp = 0x00;
	for(i = 0; i < 5; i++)
	{
		temp = temp ^ UID[i];
	}

	if(temp == 0)
	{
		return TRUE;
	}

	return FALSE;
}

/****************************************************************/
/*名称: Save_UID */
/*功能: 该函数实现保存卡片收到的序列号*/
/*输入: row: 产生冲突的行*/
/* col: 产生冲突的列*/
/* length: 接収到的UID数据长度*/
/*输出: N/A */
/****************************************************************/
void FMSave_UID(INT8U row, INT8U col, INT8U length)
{
	INT8U	i;
	INT8U	temp;
	INT8U	temp1;
	
	if((row == 0x00) && (col == 0x00))
	{
		for(i = 0; i < length; i++)
		{
			UID[i] = FMRevBuffer[i];
		}
	}
	else
	{
		temp  = FMRevBuffer[0];
		temp1 = UID[row - 1];
		switch(col)
		{
		case 0:		temp1 = 0x00; row = row + 1;              break;
		case 1:		temp = temp & 0xFE; temp1 = temp1 & 0x01; break;
		case 2:		temp = temp & 0xFC; temp1 = temp1 & 0x03; break;
		case 3:		temp = temp & 0xF8; temp1 = temp1 & 0x07; break;
		case 4:		temp = temp & 0xF0; temp1 = temp1 & 0x0F; break;
		case 5:		temp = temp & 0xE0; temp1 = temp1 & 0x1F; break;
		case 6:		temp = temp & 0xC0; temp1 = temp1 & 0x3F; break;
		case 7:		temp = temp & 0x80; temp1 = temp1 & 0x7F; break;
		default:	break;
		}

		FMRevBuffer[0] = temp;
		UID[row - 1] = temp1 | temp;
		for(i = 1; i < length; i++)
		{
			UID[row - 1 + i] = FMRevBuffer[i];
		}
	}
}

/****************************************************************/
/*名称: Set_BitFraming */
/*功能: 该函数设置待发送数据的字节数*/
/*输入: row: 产生冲突的行*/
/* col: 产生冲突的列*/
/*输出: N/A */
/****************************************************************/
void FMSet_BitFraming(INT8U row, INT8U col)
{
	INT8U temp ; 
	
	switch(row)
	{
		case 0:		FMRevBuffer[1] = 0x20; break;
		case 1:		FMRevBuffer[1] = 0x30; break;
		case 2:		FMRevBuffer[1] = 0x40; break;
		case 3:		FMRevBuffer[1] = 0x50; break;
		case 4:		FMRevBuffer[1] = 0x60; break;
		default:	break;
	}

	switch(col)
	{
		case 0:		temp = 0x00 ; break;
		case 1:		temp = 0x11 ; break;
		case 2:		temp = 0x22 ; break;
		case 3:		temp = 0x33 ; break;
		case 4:		temp = 0x44 ; break;
		case 5:		temp = 0x55 ; break;
		case 6:		temp = 0x66 ; break;
		case 7:		temp = 0x77 ; break;
		default:	break;
	}

	FMWrite(temp, FM_Bit_Frame); 
	FMRevBuffer[1] = (FMRevBuffer[1] | col);
}

/****************************************************************/
/*名称: Init_FM1702 */
/*功能: 该函数实现对FM1702初始化操作*/
/*输入: mode:工作模式, 0:TYPEA模式*/
/* 1:TYPEB模式*/
/* 2:上海模式*/
/*输出: N/A */
/****************************************************************/
void FMInit_FM1702(void )
{
	
	INT8U  temp ;

	//FIO1DIR3 |= FMRST ; 
	//LPC_GPIO1->DIR |= FMRST;
	
	SET_FMRST() ;			            	/* FM1702复位 */
	OSTimeDly(OS_TICKS_PER_SEC/5);										// 扫描间隔
//	OSTimeDly(100); 						/*等待约140ms, 11.0592*/
	CLR_FMRST() ;
	OSTimeDly(OS_TICKS_PER_SEC/5);										// 扫描间隔
//	OSTimeDly(100);							/*等待约140ms, 11.0592*/
	for( temp = 1 ; temp != 0 ;)			/* 等待Command = 0,FM1702复位成功 */
	{
		FMRead(&temp , FM_Command );
	} 					

	FMWrite( HEX_0x80 , FM_Page_Sel);

	for(temp = HEX_0x80 ; temp & HEX_0x80 ; ) /* 延时 */
	{
		FMRead(&temp , FM_Command );
	}

	FMWrite(HEX_0x00 , FM_Page_Sel);		//初始化 SPI 完毕
	
	FMWrite(0x0b , FM_TimerClock  );		//address 2AH    /* 定时器周期设置寄存器 */
	FMWrite(0x02 , FM_TimerControl  );		//address 2BH	 /* 定时器控制寄存器 */
	FMWrite(0x42 , FM_TimerReload );		//address 2CH    /* 定时器初值寄存器 */
	FMWrite(0x7f , FM_InterruptEn );   		//address 06H    /* 中断使能/禁止寄存器 */ 
	FMWrite(0x7f , FM_Int_Req  );			//address 07H    /* 中断请求标识寄存器 */
	FMWrite(0x02 , FM_MFOUTSelect  );		//address 26H    /* mf OUT 选择配置寄存器 */  
	FMWrite(0x5b , FM_TxControl  );			//设置调制器的输入源为内部编码器, 并且设置TX1和TX2
	FMWrite(0x01 , FM_RxControl2 );
	FMWrite(0x05 , FM_RxWait  );

	FMWrite(0x00 , FM_TypeSH  );

}

/****************************************************************/
/*名称: Command_Send */
/*功能: 该函数实现向FM1702发送命令集的功能*/
/*输入: count, 待发送命令集的长度*/
/* buff, 指向待发送数据的指针*/
/* Comm_Set, 命令码*/
/*输出: TRUE, 命令被正确执行*/
/* FALSE, 命令执行错误*/
/****************************************************************/
BOOLEAN FMCommand_Send(INT8U count, INT8U *buff, INT8U Comm_Set)
{

	INT32U		j;
	INT8U	temp, temp1;
	
	FMWrite(0x00 , FM_Command );
	temp = FMClear_FIFO();
	FMWrite_FIFO(count,buff);

	FMWrite(Comm_Set , FM_Command  );			/* 命令执行 */

	for(j = 0; j < RF_TimeOut; j++) 			/* 检查命令执行否 */
	{
		FMRead(&temp , FM_Command );
		FMRead(&temp1 , FM_Int_Req );
		temp1 = temp1 & 0x80;
		if((temp == 0x00) || (temp1 == 0x80))
		{
			return TRUE;
		}
	}

	return FALSE;
}
/****************************************************************/
/*名称:  */
/*功能: 该函数实现对放入FM1702操作范围之内的卡片的操作*/
/*输入: mode: ALL(监测所以FM1702操作范围之内的卡片) */
/* STD(监测在FM1702操作范围之内处于HALT状态的卡片) */
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_OK: 应答正确*/
/* FM1702_REQERR: 应答错误*/
/****************************************************************/
INT8U FMRequest(INT8U mode)
{
	
	INT8U temp;
	
	FMWrite(0x63, FM_CRCPresetLSB );		//CRCPresetLSB address is 0x23
	FMWrite(0x3f, FM_CWConductance);

	FMRevBuffer[0] = mode;		            /* 模式选择 */

	FMWrite(0x07 , FM_Bit_Frame );		
	FMWrite(0x03 , FM_ChannelRedundancy);	
	FMRead(&temp , FM_Control );
	temp = temp & (0xf7);
	FMWrite(temp, FM_Control );			    //Control reset value is 00

	temp = FMCommand_Send(1, FMRevBuffer, Transceive);   //Transceive=0x1E	/* 发送接收命令 */
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	FMRead_FIFO(FMRevBuffer);		    /* 从FIFO中读取应答信息到RevBuffer[]中 */
	temp = FMJudge_Req(FMRevBuffer);	/* 判断应答信号是否正确 */
	if(temp == TRUE)
	{
		tagtype[0] = FMRevBuffer[0];
		tagtype[1] = FMRevBuffer[1];
		return FM1702_OK;
	}

	return FM1702_REQERR;
}

/****************************************************************/
/*名称: Read_E2 */
/*功能: 该函数实现从FM1702的EE中读出数据*/
/*输入: lsb, EE地址(低字节) */
/* msb, EE地址(高字节) */
/* count, 待读出数据EE的字节个数*/
/* buff, 指向待读出数据的指针*/
/*输出: TRUE, EE数据正确读出*/
/* FALSE, EE数据读出有误*/
/****************************************************************/
BOOLEAN FMRead_E2(INT8U lsb, INT8U msb, INT8U count, INT8U *buff)
{
	INT8U	temp;
	
	*buff = lsb;
	*(buff + 1) = msb;
	*(buff + 2) = count;
	temp = FMCommand_Send(3, buff, ReadE2);
	FMRead_FIFO(buff);
	if(temp == FALSE) return(TRUE);
	return(FALSE);
}

/****************************************************************/
/*名称: Write_E2 */
/*功能: 该函数实现向FM1702的EE中写入数据*/
/*输入: lsb, EE地址(低字节) */
/* msb, EE地址(高字节) */
/* count, 待写入数据EE的字节个数*/
/* buff, 指向待写入数据的指针*/
/*输出: TRUE, EE数据正确写入*/
/* FALSE, EE数据写入有误*/
/****************************************************************/
BOOLEAN FMWrite_E2(INT8U lsb, INT8U msb, INT8U count, INT8U *buff)
{
	INT8U temp, i;

	for(i = 0; i < count; i++){
		*(buff + count - i + 2) = *(buff - i + count);
	}

	*buff = lsb;
	*(buff + 1) = msb;
	temp = FMCommand_Send(count + 2, buff, WriteE2);
	FMRead(&temp , FM_SecondaryStatus);
	temp = temp & 0x40;
	if(temp == 0x40){
		FMWrite(0x00, FM_Command );			//added 2006/03/15
		return TRUE;
	}

	return FALSE;
}

/****************************************************************/
/*名称: MIF_Halt */
/*功能: 该函数实现暂停MIFARE卡*/
/*输入: N/A */
/*输出: FM1702_OK: 应答正确*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_NOTAGERR: 无卡*/
/****************************************************************/
INT8U FMMIF_Halt(void)
{
	INT8U	temp;
	INT32U	i;
	
	FMWrite(0x63, FM_CRCPresetLSB );
	FMWrite(0x3f, FM_CWConductance );
	FMWrite(0x03, FM_ChannelRedundancy);
	*FMRevBuffer = RF_CMD_HALT;
	*(FMRevBuffer + 1) = 0x00;
	temp = FMCommand_Send(2, FMRevBuffer, Transmit);
	if(temp == TRUE){
		for(i = 0; i < 0x50; i++){
		}
		return FM1702_OK;
	}
	else{
		FMRead(&temp , FM_ErrorFlag);
		if((temp & 0x02) == 0x02){
			return(FM1702_PARITYERR);
		}

		if((temp & 0x04) == 0x04){
			return(FM1702_FRAMINGERR);
		}

		return(FM1702_NOTAGERR);
	}
}

///////////////////////////////////////////////////////////////////////
// 转换密钥格式
///////////////////////////////////////////////////////////////////////
INT8U FMM500HostCodeKey(  INT8U *uncoded, INT8U *coded)   
{
//    INT8U status = FM1702_OK;
    INT8U cnt = 0;
    INT8U ln  = 0;     
    INT8U hn  = 0;      
    for (cnt = 0; cnt < 6; cnt++){
        ln = uncoded[cnt] & 0x0F;
        hn = uncoded[cnt] >> 4;
        coded[cnt * 2 + 1] = (~ln << 4) | ln;
        coded[cnt * 2 ] = (~hn << 4) | hn;
    }
    return FM1702_OK;
}

/****************************************************************/
/*名称: Load_keyE2 */
/*功能: 该函数实现把E2中密码存入FM1702的keyRevBuffer中*/
/*输入: Secnr: EE起始地址*/
/*输出: True: 密钥装载成功*/
/* False: 密钥装载失败*/
/****************************************************************/
BOOLEAN FMLoad_keyE2_CPY(INT8U Secnr, INT8U Mode)
{

//	INT8U status=0;
	INT8U	temp=0;	
    INT8U	msb = 0;
    INT8U	lsb = 0;
	INT8U coded_keys[12];

	INT8U	temp1=0;
	if(Secnr >= 0x20){
	  temp1 = Secnr - 0x20;
	  Secnr = 0x20 + temp1 * 4;
	}

	temp = Secnr * 12;

	if(Mode == RF_CMD_AUTH_LA){
		if(temp >= 0x80){
			lsb = temp - 0x80;
			msb = 0x01;
		}
		else{
			lsb = temp + 0x80;
			msb = 0x00;
		}
	}
	else{
		msb = 0x01;
		lsb = temp + 0x40;
	}
	FMRevBuffer[3] = 0xff ; 
	FMRevBuffer[4] = 0xff ; 
	FMRevBuffer[5] = 0xff ; 
	FMRevBuffer[6] = 0xff ; 
	FMRevBuffer[7] = 0xff ; 
	FMRevBuffer[8] = 0xff ; 

	FMM500HostCodeKey(&FMRevBuffer[3],coded_keys);////////////////
//	status = Write_E2(lsb, msb, 12, coded_keys);	     ////////////////
//	RevBuffer[0] = lsb;
//	RevBuffer[1] = msb;
//	
//	FMRevBuffer[0] = 0X80;
//	FMRevBuffer[1] = 0 ;
//	temp = FMCommand_Send(2, FMRevBuffer, LoadKeyE2); //注释掉，是因为该处是从EEPROM加载密钥

//	FMCommand_Send(12,coded_keys,LoadKey); //从FIFO加载密钥 ，修改耿学涛 09。6。2。

	msb = lsb;
	lsb = msb;

	FMCommand_Send(12,coded_keys,LoadKey); //从FIFO加载密钥 ，修改耿学涛 09。6。2。
	FMRead(&temp , FM_ErrorFlag) ;
	if((temp & 0x40) == 0x40){
		return FALSE;
	}

	return TRUE;
}


/****************************************************************/
/*名称: AntiColl */
/*功能: 该函数实现对放入FM1702操作范围之内的卡片的防冲突检测*/
/*输入: N/A */
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_SERNRERR: 卡片序列号应答错误*/
/* FM1702_OK: 卡片应答正确*/
/****************************************************************/
INT8U FMAntiColl(void)
{
	INT8U	temp;
	INT8U	i;
	INT8U	row, col;
	INT8U	pre_row;
	
	row = 0;
	col = 0;
	pre_row = 0;
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance );

	FMRevBuffer[0] = RF_CMD_ANTICOL;
	FMRevBuffer[1] = 0x20;
	FMWrite(0x03 , FM_ChannelRedundancy  );
	temp = FMCommand_Send(2, FMRevBuffer, Transceive);
	while(1)
	{
		if(temp == FALSE){
			return(FM1702_NOTAGERR);
		}

		//temp = ErrorFlag;

		FMRead(&temp , FM_FIFO_Length  );
		if(temp == 0){
			return FM1702_BYTECOUNTERR;
		}

		FMRead_FIFO(FMRevBuffer);
		FMSave_UID(row, col, temp);			/* 将收到的UID放入UID数组中 */
		
		
		FMRead(&temp , FM_ErrorFlag );				    /* 判断接収数据是否出错 */
		temp = temp & 0x01;
		if(temp == 0x00){
			temp = FMCheck_UID();			/* 校验收到的UID */
			if(temp == FALSE){
				return(FM1702_SERNRERR);
			}

			return(FM1702_OK);
		}
		else{
			FMRead(&temp , FM_CollPos);				/* 读取冲突检测寄存器 */
			row = temp / 8;
			col = temp % 8;
			FMRevBuffer[0] = RF_CMD_ANTICOL;
			FMSet_BitFraming(row + pre_row, col);	/* 设置待发送数据的字节数 */
			pre_row = pre_row + row;
			for(i = 0; i < pre_row + 1; i++){
				FMRevBuffer[i + 2] = UID[i];
			}

			if(col != 0x00){
				row = pre_row + 1;
			}
			else{
				row = pre_row;
			}

			temp = FMCommand_Send(row + 2, FMRevBuffer, Transceive);
		}
	}
}

/****************************************************************/
/*名称: Select_Card */
/*功能: 该函数实现对放入FM1702操作范围之内的某张卡片进行选择*/
/*输入: N/A */
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_OK: 应答正确*/
/* FM1702_SELERR: 选卡出错*/
/****************************************************************/
INT8U FMSelect_Card(void)
{
	INT8U	temp, i;
	
	FMWrite(0x63 , FM_CRCPresetLSB );
	FMWrite(0x3f , FM_CWConductance );
	FMRevBuffer[0] = RF_CMD_SELECT;
	FMRevBuffer[1] = 0x70;
	for(i = 0; i < 5; i++){
		FMRevBuffer[i + 2] = UID[i];
	}

	FMWrite(0x0f , FM_ChannelRedundancy );	
	temp = FMCommand_Send(7, FMRevBuffer, Transceive);
	if(temp == FALSE){
		return(FM1702_NOTAGERR);
	}
	else{
		FMRead(&temp , FM_ErrorFlag );
		if((temp & 0x02) == 0x02) return(FM1702_PARITYERR);
		if((temp & 0x04) == 0x04) return(FM1702_FRAMINGERR);
		if((temp & 0x08) == 0x08) return(FM1702_CRCERR);
		FMRead(&temp , FM_FIFO_Length);
		if(temp != 1) return(FM1702_BYTECOUNTERR);
		FMRead_FIFO(FMRevBuffer);	/* 从FIFO中读取应答信息 */
		temp = *FMRevBuffer;

		if((temp == 0x18) || (temp == 0x08) || (temp == 0x88) || (temp == 0x53))	/* 判断应答信号是否正确 */
			return(FM1702_OK);
		else
			return(FM1702_SELERR);
	}
}

/****************************************************************/
/*名称: Authentication */
/*功能: 该函数实现密码认证的过程*/
/*输入: UID: 卡片序列号地址*/
/* SecNR: 扇区号*/
/* mode: 模式*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_OK: 应答正确*/
/* FM1702_AUTHERR: 权威认证有错*/
/****************************************************************/
INT8U FMAuthentication(INT8U *UID, INT8U SecNR, INT8U mode)
{
	INT8U	i;
	INT8U	temp, temp1;

	INT8U temp0;
	
	if(SecNR >= 0x20){
		temp0 = SecNR -0x20;
		SecNR = 0x20 + temp0 * 4;
	}
	
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	if(mode == RF_CMD_AUTH_LB)			
		FMRevBuffer[0] = RF_CMD_AUTH_LB;
	else
		FMRevBuffer[0] = RF_CMD_AUTH_LA;
	FMRevBuffer[1] = SecNR * 4 + 3;
	for(i = 0; i < 4; i++){
		FMRevBuffer[2 + i] = UID[i];
	}

	FMWrite(0x0f , FM_ChannelRedundancy);	
	temp = FMCommand_Send(6, FMRevBuffer, Authent1);
	if(temp == FALSE){
		return 0x99;
	}

	FMRead(&temp , FM_ErrorFlag );             //ErrorFlag address is 0x0A.
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = FMCommand_Send(0, FMRevBuffer, Authent2);	
	if(temp == FALSE){
		return 0x88;
	}

	FMRead(&temp , FM_ErrorFlag );
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	FMRead(&temp1 , FM_Control );
	temp1 = temp1 & 0x08;	
	if(temp1 == 0x08){
		return FM1702_OK;
	}

	return FM1702_AUTHERR;
}

/****************************************************************/
/*名称: MIF_Read */
/*功能: 该函数实现读MIFARE卡块的数值*/
/*输入: buff: 缓冲区首地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_READ(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	
	FMWrite(0x63 , FM_CRCPresetLSB );
	FMWrite(0x3f , FM_CWConductance );
	FMWrite(0x3f , FM_ModConductance );
	FMWrite(0x0f , FM_ChannelRedundancy );

	buff[0] = RF_CMD_READ;
	buff[1] = Block_Adr;
	temp = FMCommand_Send(2, buff, Transceive);
	if(temp == 0){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_ErrorFlag );
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	FMRead(&temp , FM_FIFO_Length);
	if(temp == 0x10){
		FMRead_FIFO(buff);
		return FM1702_OK;
	}
	else if(temp == 0x04){
		FMRead_FIFO(buff);
		return FM1702_OK;
	}
	else{
		return FM1702_BYTECOUNTERR;
	}
}

/****************************************************************/
/*名称: MIF_Write */
/*功能: 该函数实现写MIFARE卡块的数值*/
/*输入: buff: 缓冲区首地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_WRITEERR: 写卡块数据出错*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_Write(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	INT8U	*F_buff;

	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance );
	F_buff = buff + 0x10;
	FMWrite(0x07 , FM_ChannelRedundancy );    /* Note: this line is for 1702, different from RC500*/
	*F_buff = RF_CMD_WRITE;
	*(F_buff + 1) = Block_Adr;
	temp = FMCommand_Send(2, F_buff, Transceive);
	if(temp == FALSE){
		return(FM1702_NOTAGERR);
	}

	FMRead(&temp , FM_FIFO_Length );
	if(temp == 0){
		return(FM1702_BYTECOUNTERR);
	}

	FMRead_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
    	case 0x00:	return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	break;
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_WRITEERR);
	}

	temp = FMCommand_Send(16, buff, Transceive);
	if(temp == TRUE){
		return(FM1702_OK);
	}
	else{
		FMRead(&temp , FM_ErrorFlag  );
		if((temp & 0x02) == 0x02)
			return(FM1702_PARITYERR);
		else if((temp & 0x04) == 0x04)
			return(FM1702_FRAMINGERR);
		else if((temp & 0x08) == 0x08)
			return(FM1702_CRCERR);
		else
			return(FM1702_WRITEERR);
	}
}

/****************************************************************/
/*名称: MIF_Increment */
/*功能: 该函数实现MIFARE卡自动增值操作*/
/*输入: buff: 四个字节数值起始地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_INCRERR: 卡片增款操作失败*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_Increment(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	INT8U	*F_buff;
	
	FMWrite(0x63 , FM_CRCPresetLSB );
	FMWrite(0x3f , FM_CWConductance );
	F_buff = buff + 4;
	*F_buff = RF_CMD_INC;
	*(F_buff + 1) = Block_Adr;
	FMWrite(0x07 , FM_ChannelRedundancy);
	temp = FMCommand_Send(2, F_buff, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length);
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
        case 0x00:	/* break; */return(FM1702_NOTAUTHERR);
        case 0x04:	return(FM1702_EMPTY);
        case 0x0a:	break;
        case 0x01:	return(FM1702_CRCERR);
        case 0x05:	return(FM1702_PARITYERR);
        default:	return(FM1702_INCRERR);
	}
        
	temp = FMCommand_Send(4, buff, Transmit);
	if(temp == FALSE){
		return FM1702_INCRERR;
	}

	return FM1702_OK;
}

/****************************************************************/
/*名称: MIF_Decrement */
/*功能: 该函数实现MIFARE卡自动减值操作*/
/*输入: buff: 四个字节数值起始地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_DECRERR: 卡片扣款操作失败*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_Decrement(INT8U *buff, INT8U Block_Adr)
{
	INT8U		temp;
	INT8U	*F_buff;
	
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	F_buff = buff + 4;
	*F_buff = RF_CMD_DEC;
	*(F_buff + 1) = Block_Adr;
	FMWrite(0x07 , FM_ChannelRedundancy);
	temp = FMCommand_Send(2, F_buff, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length );
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
    	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	break;
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_DECRERR);
	}

	temp = FMCommand_Send(4, buff, Transmit);
	if(temp == FALSE){
		return(FM1702_DECRERR);
	}

	return FM1702_OK;
}

/****************************************************************/
/*名称: MIF_Restore */
/*功能: 该函数实现MIFARE卡自动恢复,备份操作*/
/*输入: Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_RESTERR: 卡片恢复,备份操作失败*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_Restore(INT8U Block_Adr)
{
	INT8U	temp, i;
	
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	FMWrite(0x07 , FM_ChannelRedundancy);
	*FMRevBuffer = RF_CMD_RESTORE;
	*(FMRevBuffer + 1) = Block_Adr;
	temp = FMCommand_Send(2, FMRevBuffer, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length  );
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(FMRevBuffer);
	temp = *FMRevBuffer;
	switch(temp)
	{
    	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	break;
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_RESTERR);
	}

	for(i = 0; i < 4; i++) FMRevBuffer[i] = 0x00;
	temp = FMCommand_Send(4, FMRevBuffer, Transmit);
	if(temp == FALSE){
		return FM1702_RESTERR;
	}

	return FM1702_OK;
}

/****************************************************************/
/*名称: MIF_Transfer */
/*功能: 该函数实现MIFARE卡电子钱包保存操作*/
/*输入: Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_TRANSERR: 卡片恢复,备份操作失败*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_Transfer(INT8U Block_Adr)
{
	
	INT8U	temp;
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	FMWrite(0x07 , FM_ChannelRedundancy);
	FMRevBuffer[0] = RF_CMD_TRANSFER;
	FMRevBuffer[1] = Block_Adr;
	temp = FMCommand_Send(2, FMRevBuffer, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length );
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(FMRevBuffer);
	temp = *FMRevBuffer;
	switch(temp)
	{
    	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	return(FM1702_OK);
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_TRANSERR);
	}
}

/****************************************************************/
/*名称: HL_Active */
/*功能: 该函数实现高级MIFARE卡激活命令*/
/*输入: Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出: 操作状态码*/
/* 读出数据存于RevBuffer中*/
/****************************************************************/
INT8U FMHL_Active(INT8U Block_Adr, INT8U Mode)
{
	INT8U	temp ;
	
	Secnr = Block_Adr / 4;
	FMMIF_Halt();	/* Halt */
	temp = FMRequest(RF_CMD_REQUEST_STD);		
	if(temp != FM1702_OK){
		return(FM1702_REQERR);
	}

	temp = FMAntiColl();				
	if(temp != FM1702_OK){
		return(FM1702_ANTICOLLERR);
	}

	temp = FMSelect_Card();				
	if(temp != FM1702_OK){
		return(FM1702_SELERR);
	}

	FMLoad_keyE2_CPY(Secnr, Mode);	//%40	
	temp = FMAuthentication(UID, Secnr, Mode);	
	if(temp != FM1702_OK){
		return(FM1702_AUTHERR);
	}

	return FM1702_OK;
}

/****************************************************************/
/*名称: MIF_Initival */
/*功能: 该函数实现MIFARE卡初始化值操作*/
/*输入: buff: 四个字节初始化数值起始地址*/
/* Block_Adr: 块地址*/
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_NOTAUTHERR: 未经权威认证*/
/* FM1702_EMPTY: 数据溢出错误*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_WRITEERR: 写卡块数据出错*/
/* FM1702_OK: 应答正确*/
/****************************************************************/
INT8U FMMIF_Initival(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	INT8U		i;
	
	for(i = 0; i < 4; i++){
		*(buff + 4 + i) = ~(*(buff + i));
	}

	for(i = 0; i < 4; i++){
		*(buff + 8 + i) = *(buff + i);
	}

	*(buff + 12) = Block_Adr;
	*(buff + 13) = ~Block_Adr;
	*(buff + 14) = Block_Adr;
	*(buff + 15) = ~Block_Adr;
	temp = FMMIF_Write(buff, Block_Adr);
	return temp;
}

/****************************************************************/
/*名称: HL_Read */
/*功能: 该函数实现高级读命令*/
/*输入: Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出: 操作状态码*/
/* 读出数据存于RevBuffer中*/
/****************************************************************/
INT8U FMHL_Read(INT8U *buff, INT8U Block_Adr, INT8U Mode)
{	
	INT8U	temp;
	
	temp = FMHL_Active(Block_Adr, Mode);
	if(temp != FM1702_OK)
		return temp;
	
	temp = FMMIF_READ(&(*buff), Block_Adr);
	if(temp != FM1702_OK)
		return temp;

	return FM1702_OK;
}

/****************************************************************/
/*名称: HL_Write */
/*功能: 该函数实现高级写命令*/
/*输入: buff: 待写入数据的首地址*/
/* Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出:操作状态码*/
/****************************************************************/
INT8U FMHL_Write(INT8U *buff, INT8U Block_Adr, INT8U Mode)
{
	
	INT8U	temp;
//	INT8U xbyte[16] = {0,0x10,0,0,1,2,3,4,5,6,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa} ; 
	

	temp = FMHL_Active(Block_Adr, Mode);
	if(temp != FM1702_OK){
		return temp;
	}

//	temp = FMMIF_Write(xbyte, Block_Adr);
	
	temp = FMMIF_Write(buff, Block_Adr);
	if(temp != FM1702_OK){
		return FM1702_WRITEERR;
	}

//	temp = FMMIF_READ(&(*buff), Block_Adr);
//	if(temp != FM1702_OK)
//	{
//		return temp;
//	}

	return FM1702_OK;
}


void main_rfic( void )
{
	do {
   		OSTimeDly(OS_TICKS_PER_SEC/10);
	}while(1);
}

/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16进制数转换为ASCII码
* Input			: 
* Output		: 
* Note(s)		: 注意，转化的ascii数组最多是十个字节
* Contributor	: 10/12/2010	wangyao
***********************************************************************************************/
//void  HextoAscii(uint32 hexdata,uint8 *data)
//{
//	uint32 temp[10]={0};
//	uint32 hex;
//	uint8 i,j,k;
//	uint32 chushu=10;
////	INT32U yushu=0;
//	hex=hexdata	;
//	if(hex<=0xffffffff)
//	{
//		for(i=0;i<10;i++)
//		{
//			chushu=10;
//			if(i==9)
//			{
//				for(j=0;j<i-1;j++)
//					chushu*=10;
//			    temp[9-i] = hex/chushu;
//				temp[9-i]+='0';	
//			}
//			else
//			{
//				
//				for(j=0;j<i;j++)
//					chushu*=10;
//				temp[9-i] = hex%((INT32U)chushu);
//				hex-=temp[9-i];
//				for(k=0;k<i;k++)
//					temp[9-i]=temp[9-i]/10;
//				temp[9-i]+='0';
//			}
//		}
//	}
//	for(i=0;i<10;i++)
//		data[i]=(INT8U)temp[i];
//}
/**************************************************************************************************
**	函数名称:	CompareArray()
**	功	能	:	比较两个数组是否一样
**	入口参数:	*pt1			:	指向比较的数组的指针
**				*pt2			:	指向比较的数组的指针
**				length			:	比较的数组的长度
**	出口参数:	无
**  返回值  ：  1:数组相等		0 数组不相等
**	全局变量:	无
**	调用模块:	无
**	作	者	:	邓小颖
**	日	期	:	2009年04月14日
**************************************************************************************************/
//uint8	CompareArray(uint8 *pt1, uint8 *pt2, uint8 length)
//{
//	for(;length != 0; length--)
//	{
//		if(*pt1++ != *pt2++)
//		{
//			return 0;
//		}
//	}
//	return 1;
//}
/***********************************************************************************************
* Function		: BSP_SaveNewKey
* Description	: 保存新设置的密码
* Input			: addr---保存地址
                  KEY---密钥
* Output		: 1--成功  0--不成功
* Note(s)		: 
* Contributor	: 17/06/2010	wangyao
***********************************************************************************************/
INT8U SavePassword(const INT8U *KEY)
{ 
	INT32U addr=0x80;//从入参移到里面，避免外部使用人员不清楚什么意思
	INT8U  psw[12] ; 

	FM1702SLHostCodeKey(KEY,psw);

	if(FMWrite_E2(addr , 0x00 , 12 , psw ))
		return 1 ; 
	else 
	    return 0 ;  
}

/***********************************************************************************************
* Function		: FM1702SLHostCodeKey
* Description	: 密码格式的转换
* Input			: uncoded--没有转换的密码16进制数
*                 coded--格式转换后的密码
* Output		: 
* Note(s)		: 密钥必须以规定的格式存放在EEPROM内，每一BYTE的密钥分为低四位和高四位，
每四位按位反以及位存放在一个byte内，LoadKeyE2和LoadKey指令会按此来检查密钥是否满足这种格式，
使用这种格式，6byte的可以需存放在12byteEEPROM内，例如key为A0A1A2A3A4A5,则在EEPROM中存放的
格式为5AF05AE15AD25AC35AB45AA5，这里的A0用5AF0表示，也就是A用5A表示，其中5是A的取反，A是本身。
* Contributor	: 17/06/2010	wangyao
***********************************************************************************************/
void FM1702SLHostCodeKey(const INT8U *uncoded, INT8U *coded)   
{
	INT8U  cnt = 0;
	INT8U  ln  = 0;     // low nibble
	INT8U  hn  = 0;     // high nibble
	for (cnt = 0; cnt < 6; cnt++)
	{
		ln = uncoded[cnt] & 0x0F;  //取低四位
		hn = uncoded[cnt] >> 4;    //取高四位 
		coded[cnt * 2 + 1] = ((~ln) << 4) | ln;
		coded[cnt * 2 ] = ((~hn) << 4) | hn;
	}
}

//////////////////////////////////////////////////////////////////////////////
// End of File
//////////////////////////////////////////////////////////////////////////////

