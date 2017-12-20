/****************************************Copyright (c)****************************************************
**                            ShangHai FuDian Technology Co.,LTD.
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL698.h
** Latest modified Date:       2014-5-20
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 AndyLau
** Created date:               2014-5-20
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2014-5-20
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __PTL698_H
#define __PTL698_H

#define		gx_ofs_jzq_len		1
#define		gx_ofs_jzq_ctl		6
#define		gx_ofs_jzq_addr		7
#define		gx_ofs_jzq_afn		12
#define		gx_ofs_jzq_seq		13
#define		gx_ofs_jzq_dat		14

#define		gx_ofs_jzq_mes			18
#define		gx_ofs_jzq_wcode	   	21    //操作码
#define		gx_ofs_jzq_RETwcode	   	26    //对应操作码
#define		gx_ofs_jzq_ResultCode	27    //结果码

#define		gx_ofs_ctlb_dir			7	
#define		gx_ofs_ctlb_prm			6
#define		gx_ofs_ctlb_fcb			5
#define		gx_ofs_ctlb_fcv			4

#define		gx_ofs_seqb_TpV			7	
#define		gx_ofs_seqb_FIR			6
#define		gx_ofs_seqb_FIN			5
#define		gx_ofs_seqb_CON			4

#define		AFN_OK_NOK			0x00		//确认∕否认
#define		AFN_RESET			0x01		//复位
#define		AFN_ROAD_CHK		0x02		//链路接口检测
#define		AFN_ROAD_SET		0x04		//信道设置参数
#define		AFN_PW_CFG			0x06		//身份认证及密钥协商
#define		AFN_ROAD_GET		0x0A		//查询信道设置参数
#define		AFN_FILE_SEND		0x0F		//文件传输
#define		AFN_DATA_SEND		0x10		//数据转发
#define		AFN_PRAR_SET		0x84		//居民集抄设置参数
#define		AFN_CTRL_CMD		0x85		//居民集抄控制命令
#define		AFN_PRAR_GET		0x8A		//居民集抄查询参数
#define		AFN_REAL_DAT		0x8C		//居民集抄请求1 类数据（实时数据）
#define		AFN_ROLD_DAT		0x8D		//居民集抄请求2 类数据（历史数据）
#define		AFN_RERR_DAT		0x8E		//居民集抄请求3 类数据（事件数据）



typedef struct 	GX_FRAME_TIME
{
		uint8_t		PFC;			//BCD 2BYTE 行政区划码	
		uint8_t		TIM[4];			//BIN 2BYTE 集中器地址
		uint8_t		DLY;			//BIN 1BYTE主站地址和组地址标志	
}gx_frm_tim;

typedef struct 	GX_FRAME_DATA
{
		uint8_t		AFN;			//应用层功能码AFN
		uint8_t		SEQ;			//帧序列域SEQ
		uint16_t		DA;				//信息点DA 由信息点元DA1 和信息点组DA2 两个字节构成。	
		uint16_t		DT;				//信息类DT 由信息类元DT1 和信息类组DT2 两个字节构成。
		uint16_t		PW;
		uint16_t		EC;				//事件计数器
		gx_frm_tim	struct_frm_Tp;
}gx_frm_dat;

typedef struct 	GX_FRAME_ADDR
{
		uint16_t		A1;				//BCD 2BYTE 行政区划码	
		uint16_t		A2;				//BIN 2BYTE 集中器地址
		uint8_t		A3;				//BIN 1BYTE主站地址和组地址标志	
}gx_frm_adr;

typedef struct 	GX_FRAME
{
		uint16_t		gx_dat_len;
		
		uint8_t		gx_frm_ctl;
		
		gx_frm_adr	gx_jzq_adr;

		gx_frm_dat	gx_jzq_dat;
		
}GX_DAT_FRM;

/* 时间结构体定义 */	
typedef struct time_type
{
	uint8_t  hour;    // 小时
	uint8_t  min;     // 分钟
	uint8_t  sec;     // 秒钟
	uint8_t  dow;     // 星期
}TIME;
/* 日期结构体定义 */
typedef struct date_type
{
	uint8_t  mon;     // 月份
	uint8_t  day;     // 日期
	uint16_t year;    // 年份
}DATE;


//升级用结构体
typedef struct UpProgram2{
	
		uint8_t	ChuangShangNUM[2];  //厂商编号
		uint8_t	ChuangShangNUM_Ext;  //厂商扩展编号
		uint8_t	WorkCode;    //操作码
		uint8_t	ComNUM[4];         //会话号
		uint8_t	Mima[3];
		uint8_t	UpVersion; 
		uint8_t	LimitTime;
		uint8_t   bak[9];    //备用

		uint8_t   UpGX_Result_CODE;           //　　结果码
		uint8_t   UpGX_Result_OKorNO;        //1/表示正确应答　　　0表示出错
		char    FileRoad[64];		
		char    FileName[32];
		uint8_t   UpFileVersion;
 
		uint32_t  FileSize;  //文件大小                          //要初始化
		uint8_t   LastFileVersion;                        //要初始化
		uint8_t   LastUpFileSta[2];  //上次升级状态     //要初始化
		uint8_t   LastUpMask[128];     
 
		uint16_t  LastTotalBlock; //、、 /块总数
		uint16_t  k;
		uint8_t   CrC[3];
}Struct_UpProgram;




#endif
/*********************************************************************************************************
**											END OF FILE												   	**
**********************************************************************************************************/


