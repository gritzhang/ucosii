#ifndef	__LCD_H__
#define	__LCD_H__

#include "t6963.h"


#define	LNIC  	1					//显示定位
#define CNTM	1				   	//实时时间
#define CNXH	11		 			//信号强度
#define CNZX	12		   			//在线标识
#define CNCK	13			   		//下行标识
#define CNTK	14	  				//启动标识

#define LNSBH	15  


#define UPCDLEN		20				//银联闪付卡号长度
#define	UPJELEN		12				//银联闪付卡金额长度
#define	UPJEDOT		10				//银联闪付卡金额小数点位置

#define ASTPST		11 				//"*"位置偏移量

#define MNYLEN		6  				//金额（显示）十进制位数
#define MNYDPS		4				//金额（显示）小数点位置
#define CDNLEN		16	   			//卡号（显示）十进制位数
#define PWDLEN		16				//密码（显示）十进制位数
#define OUTLEN		1			   	//端口（显示）十进制位数
#define PWRLEN		8				//电量（显示）十进制位数
#define PWRDPS		6 				//电量（显示）小数点位置
#define SUMLEN		MNYLEN			//总额（显示）十进制位数
#define SUMDPS		MNYDPS			//总额（显示）小数点位置
#define AMTLEN		MNYLEN			//总计（显示）十进制位数
#define AMTDPS		MNYDPS			//总计（显示）小数点位置
#define BALLEN		MNYLEN			//余额（显示）十进制位数
#define BALDPS		MNYDPS			//余额（显示）小数点位置

#define NONDPS		0				//无小数点
#define CRRLEN		4				//电压（显示）十进制位数
#define CRRDPS		3				//电压（显示）小数点位置
#define VLLLEN		4				//电压（显示）十进制位数
#define VLLDPS		3				//电压（显示）小数点位置
#define TIMLEN		2				//时间（显示）十进制位数

#define LNJGNR	10
#define LNJG	6
#define CNJG	6
#define CN4B	5

#define CNTS	1
#define LNKH	4
#define CNDW	12

#define LNJEBL	8
#define LNYEBL	10

#define LNKH	4
#define LNDY	6
#define LNDL	8
#define LNDD	10
#define LNJE	12
#define LNYE	14

#define CNDY	9
#define CNDL	9
#define CNDD	7
#define CNJE	8
#define CNKH	4


#define LNXXBL	12

#define LNKHBL	4

#define QrCODE_X    15
#define QrCODE_Y    64

//显示待机界面
void dspstandbypage(void);

//显示二维码
void dspQR_Code(uint8_t ppixel[][65],int Version);

//时钟图标
void dspTimeAndIcon(uint8_t mode );

extern uint8_t DspJQFHerr(uint8_t errcode);
//显示请插入充电枪
extern void Display_GUN_Insert(void);

extern void DisplayChargeInfo(void);

extern void DisplayChargeEndInfo(void);

extern void DisplayChargeError(void);

extern void DisplayOpenButton(void);

//数据反转然后 2位bcd码占用一个字节 分开1位bcd码占用一个字节
void bcdtwo2bcdone(unsigned char *sbuf,unsigned char *dbuf,unsigned int dlen);
#endif
