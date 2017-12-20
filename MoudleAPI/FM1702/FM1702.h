/****************************************************************************
* File :  fm1702.h                                                            *
* COPYRIGHT BY HUOYAN LTD.COMPANY                                           *
* Version:      V1.3                                      			        *
*                                                                           *
* Created:      18.10.2005                                                  *
* Last Change:  21.10.2005                                                  *
*                                                                           *
* Author:       mark                                                         *
*                                                                           *
* Compiler:     KEIL C51 V7.10                                              *
*                                                                           *
* Description:  AT89S52-Firmware for FM1702 Demo Serial Reader              *
*                                                                           *
****************************************************************************/
#ifndef _FM1702_H_
#define _FM1702_H_

#include "AllHand.h"
#include "lcd.h"

#ifdef IN_FM1702_GLOBALS
#define  IN_FM1702_EXT
#else
#define  IN_FM1702_EXT  extern
#endif

#define 	SSP1CSRF		(1 << 16)	              			//p0.16                
#define 	SET_SSPCSRF()	LPC_GPIO0->DIR |= SSP1CSRF;LPC_GPIO0->SET |= SSP1CSRF	                            
#define 	CLR_SSPCSRF()	LPC_GPIO0->DIR |= SSP1CSRF;LPC_GPIO0->CLR |= SSP1CSRF	                            
                            
#define 	SSP0CSFR		(1 << 21)	              			//p1.21               
#define 	SET_SSPCSFR()	LPC_GPIO1->DIR |= SSP0CSFR;LPC_GPIO1->SET |= SSP0CSFR	                            
#define 	CLR_SSPCSFR()	LPC_GPIO1->DIR |= SSP0CSFR;LPC_GPIO1->CLR |= SSP0CSFR	

#define 	FMRST			(1<<24)								//p1.24
#define		SET_FMRST()		LPC_GPIO1->DIR |= FMRST;LPC_GPIO1->SET |= FMRST
#define		CLR_FMRST()		LPC_GPIO1->DIR |= FMRST;LPC_GPIO1->CLR |= FMRST

#define		FMIRQ			(1<<23)							//p1.23
#define 	FMIRQ_STS()		(!(LPC_GPIO1->PIN & FMIRQ))									 								 
#define 	FMIRQ_INI()		(LPC_GPIO1->DIR &= ~FMIRQ)  

#define     STA_OFFLINE		0
#define     STA_FAULT		1
#define     STA_STANDBY		2
#define     STA_WORK		3
#define     STA_WAIT		4

IN_FM1702_EXT	uint8_t	CHARING_STA  ;
//const uint8_t PSW_ZONE  =	0x80   ; 
/************************************************************************
**
**	m1卡数据结构体
**
************************************************************************/
#define UIDLEN  4
#define MNELEN	4
#define PSWLEN	6

typedef struct
{
	uint32_t		No ;
	uint8_t		Moeny[MNYLEN] ;
	uint8_t		Password[PWDLEN] ;
	uint8_t		UserID[UIDLEN] ;
	BOOLEAN 	Chgstate ; 
	uint8_t	 	Vaild ;
	uint8_t 		Work; 
}CHGCARD_PARA;

extern void SET_LXTZSJ(CHGCARD_PARA *pChgcard);
extern void SET_JQSJ(CHGCARD_PARA * pChgcard);
extern void SET_ZXTZSJ(void);


IN_FM1702_EXT  CHGCARD_PARA	Chgcard[2]; 

//IN_FM1702_EXT OS_EVENT	*FRAMSem;

IN_FM1702_EXT  uint8_t FMRevBuffer[255]  ; 
IN_FM1702_EXT  uint8_t tagtype[255]  ; 
IN_FM1702_EXT  uint8_t cardtype  ;
IN_FM1702_EXT  uint8_t UID[4] ; 
IN_FM1702_EXT  uint8_t UID_BAK[5] ; 
IN_FM1702_EXT  uint8_t Secnr ; 

IN_FM1702_EXT  uint8_t DispCardNo[10];			//卡号的ASCII 码


#define HEX_0x80	0x80 
#define HEX_0x00	0x00 

/* FM1702命令码 */
#define Transceive	0x1E			/* 发送接收命令 */
#define Transmit	0x1a			/* 发送命令 */
#define ReadE2		0x03			/* 读FM1702 EEPROM命令 */
#define WriteE2		0x01			/* 写FM1702 EEPROM命令 */
#define Authent1	0x0c			/* 验证命令认证过程第1步 */
#define Authent2	0x14			/* 验证命令认证过程第2步 */
#define LoadKeyE2	0x0b			/* 将密钥从EEPROM复制到KEY缓存 */
#define LoadKey		0x19			/* 将密钥从FIFO缓存复制到KEY缓存 */
#define RF_TimeOut	0x3f			/* 发送命令延时时间 */
#define Req			0x01
#define Sel			0x02

/* 卡片类型定义定义 */
#define TYPEA_MODE		0			/* TypeA模式 */
#define TYPEB_MODE		1			/* TypeA模式 */
#define SHANGHAI_MODE	2			/* 上海模式 */

#define TM0_HIGH	0xf0			/* 定时器0高位,4MS定时 */
#define TM0_LOW		0x60			/* 定时器0低位 */
#define TIMEOUT		100			/* 超时计数器4MS×100=0.4秒 */
#define BLOCKADDR	17
/* 射频卡通信命令码定义 */
#define RF_CMD_REQUEST_STD	0x26
#define RF_CMD_REQUEST_ALL	0x52
#define RF_CMD_ANTICOL		0x93
#define RF_CMD_SELECT		0x93
#define RF_CMD_AUTH_LA		0x60
#define RF_CMD_AUTH_LB		0x61
#define RF_CMD_READ		    0x30
#define RF_CMD_WRITE		0xa0
#define RF_CMD_INC		    0xc1
#define RF_CMD_DEC		    0xc0
#define RF_CMD_RESTORE		0xc2
#define RF_CMD_TRANSFER		0xb0
#define RF_CMD_HALT		    0x50



/* 函数错误代码定义 ERR CODE  */
#define FM1702_OK			0		/* 正确 */
#define FM1702_NOTAGERR		1		/* 无卡 */
#define FM1702_CRCERR		2		/* 卡片CRC校验错误 */
#define FM1702_EMPTY		3		/* 数值溢出错误 */
#define FM1702_AUTHERR		4		/* 验证不成功 */
#define FM1702_PARITYERR	5		/* 卡片奇偶校验错误 */
#define FM1702_CODEERR		6		/* 通讯错误(BCC校验错) */
#define FM1702_SERNRERR		8		/* 卡片序列号错误(anti-collision 错误) */
#define FM1702_SELECTERR	9		/* 卡片数据长度字节错误(SELECT错误) */
#define FM1702_NOTAUTHERR	10		/* 卡片没有通过验证 */
#define FM1702_BITCOUNTERR	11		/* 从卡片接收到的位数错误 */
#define FM1702_BYTECOUNTERR	12		/* 从卡片接收到的字节数错误仅读函数有效 */
#define FM1702_RESTERR		13		/* 调用restore函数出错 */
#define FM1702_TRANSERR		14		/* 调用transfer函数出错 */
#define FM1702_WRITEERR		15		/* 调用write函数出错 */
#define FM1702_INCRERR		16		/* 调用increment函数出错 */
#define FM1702_DECRERR		17		/* 调用decrement函数出错 */
#define FM1702_READERR		18		/* 调用read函数出错 */
#define FM1702_LOADKEYERR	19		/* 调用LOADKEY函数出错 */
#define FM1702_FRAMINGERR	20		/* FM1702帧错误 */
#define FM1702_REQERR		21		/* 调用req函数出错 */
#define FM1702_SELERR		22		/* 调用sel函数出错 */
#define FM1702_ANTICOLLERR	23		/* 调用anticoll函数出错 */
#define FM1702_INTIVALERR	24		/* 调用初始化函数出错 */
#define FM1702_READVALERR	25		/* 调用高级读块值函数出错 */
#define FM1702_DESELECTERR	26
#define FM1702_CMD_ERR		42		/* 命令错误 */

/* FM1702寄存器定义 */
#define FM_Page_Sel			0x00	/* 页写寄存器 */
#define FM_Command			0x01	/* 命令寄存器 */
#define FM_FIFO				0x02	/* 64字节FIFO缓冲的输入输出寄存器 */
#define FM_PrimaryStatus	0x03	/* 发射器接收器及FIFO的状态寄存器1 */
#define FM_FIFO_Length		0x04	/* 当前FIFO内字节数寄存器 */
#define FM_SecondaryStatus	0x05	/* 各种状态寄存器2 */
#define FM_InterruptEn		0x06	/* 中断使能/禁止寄存器 */
#define FM_Int_Req			0x07	/* 中断请求标识寄存器 */
#define FM_Control			0x09	/* 控制寄存器 */
#define FM_ErrorFlag		0x0A	/* 错误状态寄存器 */
#define FM_CollPos			0x0B	/* 冲突检测寄存器 */
#define FM_TimerValue		0x0c	/* 定时器当前值 */
#define FM_Bit_Frame		0x0F	/* 位帧调整寄存器 */
#define FM_TxControl		0x11	/* 发送控制寄存器 */
#define FM_CWConductance	0x12	/* 选择发射脚TX1和TX2发射天线的阻抗 */
#define FM_ModConductance	0x13	/* 定义输出驱动阻抗 */
#define FM_CoderControl		0x14	/* 定义编码模式和时钟频率 */
#define FM_DecoderControl	0x1a	/* 解码控制寄存器 */
#define FM_RxControl1		0x19	/*接收增益控制*/
#define FM_RxControl2		0x1e	/* 解码控制及选择接收源 */
#define FM_RxWait			0x21	/* 选择发射和接收之间的时间间隔 */
#define FM_ChannelRedundancy 0x22	/* RF通道检验模式设置寄存器 */
#define FM_CRCPresetLSB		0x23
#define FM_CRCPresetMSB		0x24
#define FM_MFOUTSelect		0x26	/* mf OUT 选择配置寄存器 */
#define FM_TimerClock		0x2a	/* 定时器周期设置寄存器 */
#define FM_TimerControl		0x2b	/* 定时器控制寄存器 */
#define FM_TimerReload		0x2c	/* 定时器初值寄存器 */
#define FM_TypeSH			0x31	/* 上海标准选择寄存器 */
#define FM_TestDigiSelect	0x3d	/* 测试管脚配置寄存器 */

/* Status Values */
#define ALL		0x01
#define KEYB	0x04
#define KEYA	0x00
#define _AB		0x40
#define CRC_A	1
#define CRC_B	2
#define CRC_OK	0
#define CRC_ERR 1
#define BCC_OK	0
#define BCC_ERR 1

/* 卡类型定义 */
#define MIFARE_8K		0			/* MIFARE系列8KB卡片 */
#define MIFARE_TOKEN	1			/* MIFARE系列1KB TOKEN卡片 */
#define SHANGHAI_8K		2			/* 上海标准系列8KB卡片 */
#define SHANGHAI_TOKEN	3			/* 上海标准系列1KB TOKEN卡片 */

#define mifare1			1
#define mifarepro		2
#define mifarelight		3
#define unknowncard     4
///*********************************************************************************************************
//** Function name:		SSP0_Init
//** Descriptions:		将SSP控制器设置为主机
//** input parameters:	无
//** output parameters:	无
//**********************************************************************************************************/
//extern void  SSP0_Init(void)	; 

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
 extern void ddelay(INT16U dely);

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
extern void  FMSendByte(uint8_t data) ;

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
extern INT16U  FMReadByte(void ) ;

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
extern void FMWrite(uint8_t dataptr,uint8_t addr);

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
extern void	FMRead(uint8_t *dataptr, uint8_t addr);

///////////////////////////////////////////////////////////////////////
// IC卡处理函数
///////////////////////////////////////////////////////////////////////
extern void cmd_process(void);

/****************************************************************/
/*名称: Clear_FIFO */
/*功能: 该函数实现清空FM1702中FIFO的数据*/
/*输入: N/A */
/*输出: TRUE, FIFO被清空*/
/* FALSE, FIFO未被清空*/
/****************************************************************/
extern BOOLEAN FMClear_FIFO(void);

/****************************************************************/
/*名称: Write_FIFO */
/*功能: 该函数实现向FM1702的FIFO中写入x bytes数据*/
/*输入: count, 待写入字节的长度*/
/* buff, 指向待写入数据的指针*/
/*输出: N/A */
/****************************************************************/
extern void FMWrite_FIFO(uint8_t count, uint8_t *buff);
/****************************************************************/
/*名称: Read_FIFO */
/*功能: 该函数实现从FM1702的FIFO中读出x bytes数据*/
/*输入: buff, 指向读出数据的指针*/
/*输出: N/A */
/****************************************************************/
extern uint8_t FMRead_FIFO(uint8_t *buff);

/****************************************************************/
/*名称: Judge_Req */
/*功能: 该函数实现对卡片复位应答信号的判断*/
/*输入: *buff, 指向应答数据的指针*/
/*输出: TRUE, 卡片应答信号正确*/
/* FALSE, 卡片应答信号错误*/
/****************************************************************/
extern BOOLEAN FMJudge_Req(uint8_t *buff);

/****************************************************************/
/*名称: Check_UID */
/*功能: 该函数实现对收到的卡片的序列号的判断*/
/*输入: N/A */
/*输出: TRUE: 序列号正确*/
/* FALSE: 序列号错误*/
/****************************************************************/
extern BOOLEAN FMCheck_UID(void);

/****************************************************************/
/*名称: Save_UID */
/*功能: 该函数实现保存卡片收到的序列号*/
/*输入: row: 产生冲突的行*/
/* col: 产生冲突的列*/
/* length: 接収到的UID数据长度*/
/*输出: N/A */
/****************************************************************/
extern void FMSave_UID(uint8_t row, uint8_t col, uint8_t length);

/****************************************************************/
/*名称: Set_BitFraming */
/*功能: 该函数设置待发送数据的字节数*/
/*输入: row: 产生冲突的行*/
/* col: 产生冲突的列*/
/*输出: N/A */
/****************************************************************/
extern void FMSet_BitFraming(uint8_t row, uint8_t col);

/****************************************************************/
/*名称: Init_FM1702 */
/*功能: 该函数实现对FM1702初始化操作*/
/*输入: mode:工作模式, 0:TYPEA模式*/
/* 1:TYPEB模式*/
/* 2:上海模式*/
/*输出: N/A */
/****************************************************************/
extern void FMInit_FM1702(void );

/****************************************************************/
/*名称: Command_Send */
/*功能: 该函数实现向FM1702发送命令集的功能*/
/*输入: count, 待发送命令集的长度*/
/* buff, 指向待发送数据的指针*/
/* Comm_Set, 命令码*/
/*输出: TRUE, 命令被正确执行*/
/* FALSE, 命令执行错误*/
/****************************************************************/
extern BOOLEAN FMCommand_Send(uint8_t count, uint8_t *buff, uint8_t Comm_Set);

/****************************************************************/
/*名称:  */
/*功能: 该函数实现对放入FM1702操作范围之内的卡片的操作*/
/*输入: mode: ALL(监测所以FM1702操作范围之内的卡片) */
/* STD(监测在FM1702操作范围之内处于HALT状态的卡片) */
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_OK: 应答正确*/
/* FM1702_REQERR: 应答错误*/
/****************************************************************/
extern uint8_t FMRequest(uint8_t mode);

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
extern BOOLEAN FMRead_E2(uint8_t lsb, uint8_t msb, uint8_t count, uint8_t *buff);


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
extern BOOLEAN FMWrite_E2(uint8_t lsb, uint8_t msb, uint8_t count, uint8_t *buff);

/****************************************************************/
/*名称: MIF_Halt */
/*功能: 该函数实现暂停MIFARE卡*/
/*输入: N/A */
/*输出: FM1702_OK: 应答正确*/
/* FM1702_PARITYERR: 奇偶校验错*/
/* FM1702_CRCERR: CRC校验错*/
/* FM1702_NOTAGERR: 无卡*/
/****************************************************************/
extern uint8_t FMMIF_Halt(void);


///////////////////////////////////////////////////////////////////////
// 转换密钥格式
///////////////////////////////////////////////////////////////////////
extern uint8_t FMM500HostCodeKey(  uint8_t *uncoded, uint8_t *coded)   ;

/****************************************************************/
/*名称: Load_keyE2 */
/*功能: 该函数实现把E2中密码存入FM1702的keyRevBuffer中*/
/*输入: Secnr: EE起始地址*/
/*输出: True: 密钥装载成功*/
/* False: 密钥装载失败*/
/****************************************************************/
extern BOOLEAN FMLoad_keyE2_CPY(uint8_t Secnr, uint8_t Mode);


/****************************************************************/
/*名称: AntiColl */
/*功能: 该函数实现对放入FM1702操作范围之内的卡片的防冲突检测*/
/*输入: N/A */
/*输出: FM1702_NOTAGERR: 无卡*/
/* FM1702_BYTECOUNTERR: 接收字节错误*/
/* FM1702_SERNRERR: 卡片序列号应答错误*/
/* FM1702_OK: 卡片应答正确*/
/****************************************************************/
extern uint8_t FMAntiColl(void);
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

extern uint8_t FMSelect_Card(void);


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
extern uint8_t FMAuthentication(uint8_t *UID, uint8_t SecNR, uint8_t mode);

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
extern uint8_t FMMIF_READ(uint8_t *buff, uint8_t Block_Adr);

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
extern uint8_t FMMIF_Write(uint8_t *buff, uint8_t Block_Adr);

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
extern uint8_t FMMIF_Increment(uint8_t *buff, uint8_t Block_Adr);

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
extern uint8_t FMMIF_Decrement(uint8_t *buff, uint8_t Block_Adr);

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
extern uint8_t FMMIF_Restore(uint8_t Block_Adr);

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
extern uint8_t FMMIF_Transfer(uint8_t Block_Adr);


/****************************************************************/
/*名称: HL_Active */
/*功能: 该函数实现高级MIFARE卡激活命令*/
/*输入: Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出: 操作状态码*/
/* 读出数据存于RevBuffer中*/
/****************************************************************/
extern uint8_t FMHL_Active(uint8_t Block_Adr, uint8_t Mode);

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
extern uint8_t FMMIF_Initival(uint8_t *buff, uint8_t Block_Adr);

/****************************************************************/
/*名称: HL_Read */
/*功能: 该函数实现高级读命令*/
/*输入: Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出: 操作状态码*/
/* 读出数据存于RevBuffer中*/
/****************************************************************/
extern uint8_t FMHL_Read(uint8_t *buff, uint8_t Block_Adr, uint8_t Mode);

/****************************************************************/
/*名称: HL_Write */
/*功能: 该函数实现高级写命令*/
/*输入: buff: 待写入数据的首地址*/
/* Secnr: 扇区号*/
/* Block_Adr: 块地址*/
/*输出:操作状态码*/
/****************************************************************/
extern uint8_t FMHL_Write(uint8_t *buff, uint8_t Block_Adr, uint8_t Mode);

/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16进制数转换为ASCII码
* Input			: 
* Output		: 
* Note(s)		: 注意，转化的ascii数组最多是十个字节
* Contributor	: 10/12/2010	wangyao
***********************************************************************************************/
//extern void  HextoAscii(uint32_t hexdata,uint8_t *data) ;
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
//extern uint8_t	CompareArray(uint8_t *pt1, uint8_t *pt2, uint8_t length) ; 

/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16进制数转换为ASCII码
* Input			: 
* Output		: 
* Note(s)		: 注意，转化的ascii数组最多是十个字节
* Contributor	: 10/12/2010	wangyao
***********************************************************************************************/
//extern void  HextoAscii(uint32_t hexdata,uint8_t *data) ; 

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
extern uint8_t	CompareArray(uint8_t *pt1, uint8_t *pt2, uint8_t length) ; 

/***********************************************************************************************
* Function		: BSP_SaveNewKey
* Description	: 保存新设置的密码
* Input			: addr---保存地址
                  KEY---密钥
* Output		: 1--成功  0--不成功
* Note(s)		: 
* Contributor	: 17/06/2010	wangyao
***********************************************************************************************/
extern uint8_t SavePassword(const uint8_t *KEY) ; 

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
extern void FM1702SLHostCodeKey(const uint8_t *uncoded, uint8_t *coded)  ;  


#endif

//////////////////////////////////////////////////////////////////////////////
// End of File
//////////////////////////////////////////////////////////////////////////////
