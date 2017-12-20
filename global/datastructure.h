 /****************************************Copyright (c)*********************************************
**                                     惠州龙鼎盛电力科技有限公司
**									       开   发   部
**--------------文件信息---------------------------------------------------------------------------
**文   件   名: datastructure.h
**创   建   人: 邓小颖
**创 建 日 期 ：2009年03月06日
**描        述: 存放程序用到的个结构体变量和参数定义
**--------------当前版本修订-----------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: __TIME__
**
**************************************************************************************************/
#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

#include "stdint.h"

/*********************************************************************************************
** 
**   4K每天能存储多少个数据块 ＝ 4096 / 结构体尺寸
**	 存储一个月的数据 31 天，需要多少个数据块
**   负荷曲线最小记录 为 0 
**   负荷曲线最大记录为：共用多少块 * 块容量 
**********************************************************************************************/


/******************************************************************************************
**
**		数据类型
**
*******************************************************************************************/

/************************************************************************
**
**	计费策略下发数据数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	    MD5	    计费策略的MD5码	 BCD码	    16Byte	   16字节计费策略的MD5码
**	2	    JLSL	记录数量	     HEX码	    1Byte	   1个字节的记录数量
**	3	    JFCL	策略x   	     HEX码	    4*24Byte   24组4个字节的策略
************************************************************************/
typedef struct
{
	uint8_t   order_num[16];
	uint8_t		MD5[16] ;
	uint8_t		recode_num ;
	uint8_t		strategy_kwh[24*6] ;	
}CHASTRATEGY_UP_PARA;

/************************************************************************
**
**	下发标准费率数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	SXSJ	生效时间	BIN码	7Byte	CP56Time2a格式
**	2	SXSJ	失效时间	BIN码	7Byte	CP56Time2a格式
**	3	ZXZT	执行状态	BCD码	2Byte	0001-有效 0002-无效
**	4	JLLX	计量类型	BCD码	2Byte	0001-里程 0002-充电量 0003-放电量
**	5	JDJ		尖电价	BIN码	4Byte	精确到小数点后五位
**	6	FDJ		峰电价	BIN码	4Byte	精确到小数点后五位
**	7	PDJ		平电价	BIN码	4Byte	精确到小数点后五位
**	8	GDJ		谷电价	BIN码	4Byte	精确到小数点后五位
**
************************************************************************/
typedef struct
{
	uint8_t		QYSJ[7] ;
	uint8_t		SXSJ[7] ;
	uint16_t	ZXZT ;
	uint16_t	JLLX ;
	uint32_t	JDJ ;
	uint32_t	FDJ ;
	uint32_t	PDJ ;
	uint32_t	GDJ ;

}RATE_PARA;

/************************************************************************
**
**	黑名单下发时下行数据数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	LJKH	逻辑卡号	BCD码	8Byte	16位编码
**	2	ZT		状态	BCD码	2Byte	0001-挂失 0002-解挂
**	3	ZHGXSJ	最后更新时间	BIN码	7Byte	CP56Time2a格式
**
************************************************************************/
typedef struct
{
	uint8_t		LJKH[8] ;
	uint16_t	ZT ;
	uint8_t		ZHGXSJ[7] ;
		
}BLACKLIST_PARA;

/************************************************************************
**
**	鉴权下行数据数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	ZDJQBM	终端机器编码	BCD码	8Byte	充电桩资产编号，系统参数的编号
**	2	LJKH	物理卡号	BCD码	8Byte	16位编码
**  3 LOGIC_NUM 逻辑卡号
**	3	DDQCWYBS	电动汽车唯一标识	字符	32Byte	32位编码前五位是组织机构编码
**	4	JFMXBM	计费模型编码	BCD码	8Byte	8位编码
**	5	KKHKYE	扣款后卡余额	BIN码	4Byte	精确到小数点后两位
**	6	JQCGBZ	鉴权成功标志	BIN码	1Byte	布尔型（1，鉴权成功；0，鉴权失败）
**	7	JQSBYY	鉴权失败原因	BCD码	2Byte	0000-成功 0001-账户余额不足 0002-套餐余额不足 0003-非法用户 0004-挂失卡 0005-车卡不匹配
**	8	SYLC	剩余里程	BIN码	4Byte	精确到小数点后两位
**	9	SYDL	剩余电量	BIN码	4Byte	精确到小数点后两位
**	10 SYCS	剩余次数    BIN码	4Byte	精确到小数点后两位
**	11 OrderNo 服务器下发的16位订单编号 刷卡充电时下发的鉴权返回不带此编号，只有APP扫码充电下发的鉴权才带此编号   2016年9月23日，北京新后台增加此功能 
************************************************************************/
typedef struct
{
	uint8_t		ZDJQBM[8] ;
	uint8_t		LJKH[8] ;
	uint8_t		LOGIC_NUM[16];
	uint8_t		DDQCWYBS[32] ;
	uint8_t		JFMXBM[8] ;
	uint32_t	KKHKYE ;
	uint8_t		JQCGBZ ;
	uint16_t	JQSBYY ;
	uint32_t	SYLC ;
	uint32_t	SYDL ;
	uint32_t	SYCS ;
	uint8_t		OrderNo[16];			//APP扫码充电时，服务器会下发16位的订单编号 	
}CHECKDOWN_PARA;

/************************************************************************
**
**	鉴权数据包数据结构体
**  序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	ZDJQBM		终端机器编码		BCD码	8Byte	充电桩资产编号，系统参数的编号
**	2	LJKH		物理卡号			BCD码	8Byte	16位编码
**	3	KMM			卡密码				字符	16Byte	16位字符串，同输入密码相同
**  4	SRMM		输入密码			字符	16Byte	16位字符串
**	5	KYE			卡余额				BIN码	4Byte	精确到小数点后两位，不需要填写
**  6	KZT			卡状态				BIN码	2Byte	0001-正常0002-挂失0003-欠费 0004-锁定 0005-注销，不需要填写
**	7	DDQCWYBS	电动汽车唯一标识	字符	32Byte	32位编码前五位是组织机构编码，不需要填写
**	8	JFMXBM		计费模型编码		BCD码	8Byte	8位编码，不需要填写
************************************************************************/
typedef struct
{
	uint8_t		ZDJQBM[8] ;
	uint8_t		LJKH[8] ;
	uint8_t		KMM[16] ;
	uint8_t		SRMM[16] ;
	uint32_t	KYE ;
	uint16_t	KZT ;
	uint8_t		DDQCWYBS[32] ;
	uint8_t		JFMXBM[8] ;
		
}CHECKUP_PARA;

/************************************************************************
**
**	离线交易上线后上传交易记录数据数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	ZDJQBM	终端机器编码	BCD码	8Byte	16位设备编码
**	2	JYLSH	交易流水号	BCD码	10Byte	19位交易代码
**	3	YHBH	用户编号	BCD码	8Byte	16位设备编码
**	3	LJKH	物理卡号	BCD码	8Byte	16位编码
**		LOGIC_NUM		逻辑卡号
**	4	LXJYLX	离线交易类型	BCD码	1Byte	0：1：
**	5	KSSJ	开始时间	BIN码	7Byte	CP56Time2a格式
**	6	JSSJ	结束时间	BIN码	7Byte	CP56Time2a格式
**	7	JQSZ	尖起示值	BIN码	4Byte	整型
**	8	JZSJ	尖止示值	BIN码	4Byte	整型
**	9	FQSZ	峰起示值	BIN码	4Byte	整型
**	10	FZSZ	峰止示值	BIN码	4Byte	整型
**	11	PQSZ	平起示值	BIN码	4Byte	整型
**	12	PZSZ	平止示值	BIN码	4Byte	整型
**	13	GQSZ	谷起示值	BIN码	4Byte	整型
**	14	GZSZ	谷止示值	BIN码	4Byte	整型
**	15	JLLX	计量类型	BCD码	2Byte	0001-里程 0002-充电量 0003-放电量
**	16	BCJLSS	本次计量示数	BIN码	4Byte	整型
**	17	SCJLSS	上次计量示数	BIN码	4Byte	整型
**	18	GDJ		尖单价	BIN码	4Byte	精确到小数点后五位
**	29	GDL		尖电量	BIN码	4Byte	整型
**	20	GGE		尖金额	BIN码	4Byte	精确到小数点后两位
**	21	FDJ		峰单价	BIN码	4Byte	精确到小数点后五位
**	22	FDL		峰电量	BIN码	4Byte	整型
**	23	FJE		峰金额	BIN码	4Byte	精确到小数点后两位
**	24	PDJ		平单价	BIN码	4Byte	精确到小数点后五位
**	25	PDL		平电量	BIN码	4Byte	整型
**	26	PJE		平金额	BIN码	4Byte	精确到小数点后两位
**	27	GDJ		谷单价	BIN码	4Byte	精确到小数点后五位
**	28	GDL		谷电量	BIN码	4Byte	整型
**	29	GJE		谷金额	BIN码	4Byte	精确到小数点后两位
**	30	ZDL		总电量	BIN码	4Byte	整型
**	31	YWLX	业务类型	BCD码	2Byte	0001-充电 0002-换电
**	32	XFSZ	消费数值	BIN码	4Byte	整型
**	33	XFDJ	消费单价	BIN码	4Byte	精确到小数点后五位
**	34	XFJE	消费金额	BIN码	4Byte	精确到小数点后两位
**	35	DDQCWYBS	电动汽车唯一标识	字符	32Byte	32位编码前五位是组织机构编码
**
************************************************************************/
typedef struct
{
	uint8_t		ZDJQBM[8] ;
	uint8_t		JYLSH[16] ;
	uint8_t		YHBH[8] ;
	uint8_t		LJKH[8] ;
	uint8_t		LOGIC_NUM[16];
	uint8_t		LXJYLX ;
	uint8_t		KSSJ[7] ;
	uint8_t		JSSJ[7] ;
	uint32_t	JQSZ ;
	uint32_t	JZSZ ;
	uint32_t	FQSZ ;
	uint32_t	FZSZ ;
	uint32_t	PQSZ ;
	uint32_t	PZSZ ;
	uint32_t	GQSZ ;
	uint32_t	GZSZ ;
	uint16_t	JLLX ;
	uint32_t	BCJLSS ;
	uint32_t	SCJLSS ;
	uint32_t	JDJ ;
	uint32_t	JDL ;
	uint32_t	JJE ;
	uint32_t	FDJ ;
	uint32_t	FDL ;
	uint32_t	FJE ;
	uint32_t	PDJ ;
	uint32_t	PDL ;
	uint32_t	PJE ;
	uint32_t	GDJ ;
	uint32_t	GDL ;
	uint32_t	GJE ;
	uint32_t	ZDL ;
	uint16_t	YWLX ;
	uint32_t	XFSZ ;
	uint32_t	XFDJ ;
	uint32_t	XFJE ;
	uint8_t		DDQCWYBS[32] ;
	uint8_t		cs;	
}CHGOFFLINE_PARA;

/************************************************************************
**
**	在线情况下停止充电时上传记录数据数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	JYLSH	交易流水号		BCD 码	16Byte	32位交易代码
**	2	YWLX	业务类型		BCD码	2Byte	0001-充电 0002-换电
**	3	ZDJQBM	终端机器编码	BCD码	8Byte	充电桩资产编号，系统参数的编号
**	4	YHBH	用户编号		BCD码	8Byte	16位设备编码,不需要填写
**	5	LJKH	物理卡号		BCD码	8Byte	16位编码
**		LOGIC_NUM		逻辑卡号
**	6	KSSJ	开始时间		BIN码	7Byte	CP56Time2a格式
**	7	JSSJ	结束时间		BIN码	7Byte	CP56Time2a格式
**	8	JQSZ	尖起示值		BIN码	4Byte	精确到小数点后二位
**	9	JZSZ	尖止示值		BIN码	4Byte	精确到小数点后二位
**	10	FQSZ	峰起示值		BIN码	4Byte	精确到小数点后二位
**	11	FZSZ	峰止示值		BIN码	4Byte	精确到小数点后二位
**	12	PQSZ	平起示值		BIN码	4Byte	精确到小数点后二位
**	13	PZSZ	平止示值		BIN码	4Byte	精确到小数点后二位
**	14	GQSZ	谷起示值		BIN码	4Byte	精确到小数点后二位
**	15	GZSZ	谷止示值		BIN码	4Byte	精确到小数点后二位
**	16	JDL		尖电量			BIN码	4Byte	精确到小数点后二位
**	17	FDL		峰电量			BIN码	4Byte	精确到小数点后二位
**	18	PDL		平电量			BIN码	4Byte	精确到小数点后二位
**	19	GDL		谷电量			BIN码	4Byte	精确到小数点后二位
**	20	ZDL		总电量			BIN码	4Byte	精确到小数点后二位
**	21	JLSSLX	计量示数类型	BCD码	2Byte	0001-里程 0002-充电量 0003-放电量， 
**	22	BCJLSS	本次计量示数	BIN码	4Byte	整型，不需要填写
**	23	SCJLSS	上次计量示数	BIN码	4Byte	整型，不需要填写
**	24	SCCHDZBH	上次充换电站编号	BCD码	5Byte	9位部门编码  离散充电桩附加集中器模式上传桩编号前5位，不需要填写
**	25	SCYWLX	上次业务类型	BCD码	2Byte	0001-充电 0002-换电，不需要填写
**	26	DDQCWYBS	电动汽车唯一标识	字符	32Byte	32位编码前五位是组织机构编码，不需要填写
**
************************************************************************/
typedef struct
{
	uint8_t		JYLSH[16] ;
	uint16_t	YWLX ;
	uint8_t		ZDJQBM[8] ;
	uint8_t		YHBH[8] ;
	uint8_t		LJKH[8] ;
	uint8_t		LOGIC_NUM[16];
	uint8_t		KSSJ[7] ;
	uint8_t		JSSJ[7] ;
	uint32_t	JQSZ ;
	uint32_t	JZSZ ;
	uint32_t	FQSZ ;
	uint32_t	FZSZ ;
	uint32_t	PQSZ ;
	uint32_t	PZSZ ;
	uint32_t	GQSZ ;
	uint32_t	GZSZ ;
	uint32_t	JDL ;
	uint32_t	FDL ;
	uint32_t	PDL ;
	uint32_t	GDL ;
	uint32_t	ZDL ;
	uint16_t	JLSSLX ;
	uint32_t	BCJLSS ;
	uint32_t	SCJLSS ;
	uint8_t		SCCHDZBH[5] ;
	uint16_t	SCYWLX ;
	uint8_t		DDQCWYBS[32] ;
		
}CHGONLINE_PARA;

/************************************************************************
**
**	扣款下行数据数据结构体
**	序号	参数代码	参数名称	字段类型	字段长度	备注
**	1	ZDJQBM	终端机器编码	BCD码	8Byte	16位设备编码
**	2	LJKH	物理卡号	BCD码	8Byte	16位编码
**  3 LOGIC_NUM 逻辑卡号 16字节
**	3	KKJE	扣款金额	BIN码	4Byte	精确到小数点后两位
**	4	ZHYE	帐户余额	BIN码	4Byte	精确到小数点后两位
**	5	KKCGBZ	扣款成功标志	BIN码	1Byte	布尔型,(1，扣款成功，0扣款失败)
**	6	KKSBYY	扣款失败原因	BCD码	2Byte	0000-成功0001-账户余额不足 0002-套餐余额不足 0003-交易相同 0004-挂失卡 0005-车卡不匹配
**	7	KCLC	扣除里程	BIN码	4Byte	精确到小数点后两位
**	8	SYLC	剩余里程	BIN码	4Byte	精确到小数点后两位
**	9	KCDL	扣除电量	BIN码	4Byte	精确到小数点后两位
**	10	SYDL	剩余电量	BIN码	4Byte	精确到小数点后两位
**	11	KCCS	扣除次数	BIN码	4Byte	精确到小数点后两位
**	12	SYCS	剩余次数	BIN码	4Byte	精确到小数点后两位
**
************************************************************************/
typedef struct
{
	uint8_t		ZDJQBM[8] ;
	uint8_t		LJKH;
	uint8_t   LOGIC_NUM[16];
	uint32_t	KKJE ;
	uint32_t	ZHYE ;
	uint8_t		KKCGBZ ;
	uint16_t	KKSBYY ;
	uint32_t	KCLC ;
	uint32_t	SYLC ;
	uint32_t	KCDL ;
	uint32_t	SYDL ;
	uint32_t	KCCS ;
	uint32_t	SYCS ;
		
}BALANCEDOWN_PARA;

/******************************************************************************************
**
**		时间结构类型
**
*******************************************************************************************/

typedef	struct
{
	uint8_t	second;							//0-59
	uint8_t	minute;							//0-59
	uint8_t	hour;							//0-23
	uint8_t	day;							//1-31
	uint8_t	month;							//1-12
	uint16_t year ;							//2000 - 
}DATE7;

typedef	struct
{
	uint8_t	minute;							//0-59
	uint8_t	hour;							//0-23
	uint8_t	day;							//1-31
	uint8_t	month;							//1-12
	uint8_t	year;							//00-99
}DATE5;

typedef	struct
{
	uint8_t	minute;							//0-59
	uint8_t	hour;							//0-23
	uint8_t	day;							//1-31
}DATE3;

typedef	struct
{
	uint8_t	second ;      					//0-59
	uint8_t	minute;							//0-59
	uint8_t	hour;							//0-23
}TIME3;


/*****************************************************************************
**
**终端参数
**
******************************************************************************/

/*终端参数0X801F结构体*/
typedef struct					// 0xF8 Byte
{
	uint16_t  	ParaFlag ;
	uint8_t		ChargeNo[8];					/*充电桩编号*/
	uint8_t		TerminalNo[8];					/*终端机器编号*/
	uint8_t		CHGMode ;						/*充电桩通信模式*/
	uint8_t		CHGProtocol; 					/*充电桩通信协议*/
	uint8_t		APN[16];						/*APN数据*/
	uint8_t		APNUSER[32];					/*APN登陆用户名*/
	uint8_t		APNPassword[32];				/*APN登陆密码*/
    uint8_t     ServerIP[4];   					/*服务器IP*/ 
    uint16_t   	ServerPort ;					/*服务器端口*/
	uint8_t     GateWay[4] ;                	/*网关*/
    uint8_t		SelfIP[4] ;						/*自身IP*/
    uint16_t   	SelfPort ;						/*自身端口*/
    uint8_t 	SelfMASK[4];           			/*子网掩码*/
    uint8_t 	SelfMAC[6];            			/*物理地址*/
    uint16_t   	RatedVoltage ;					/*额定电压*/
    uint16_t   	RatedCurrent ;					/*额定电流*/
    uint16_t   	Overload ;						/*过流比例*/
    uint16_t   	OverloadRecovery ;				/*过流恢复比例*/
    uint16_t   	Overvoltage ;					/*过压比例*/
    uint16_t   	OvervoltagRecovery ;			/*过压恢复比例*/
    uint16_t   	Undervoltage ;					/*欠压比例*/
    uint16_t   	UndervoltagRecovery ;			/*欠压恢复比例*/
    uint16_t   	MinCurrent ;					/*最小电流*/
    uint16_t   	MaxCurrent ;					/*最大电流*/
    uint32_t	AdminPassword;					/*管理员密码*/
    uint8_t   	JudgeTime ;						/*过载、过压、欠压判断时间*/
	uint8_t		HeartBeat;						/*心跳周期 单位 min*/
	uint8_t		ComTimeOut ;					/*通讯超时时间*/
	uint8_t		RepeatTime ;					/*重发次数 */
    uint16_t   	LedStart ;						/*LED灯开启时间*/
    uint16_t   	LedStop ;						/*LED灯关闭时间*/
    uint16_t    SoftVer ;						/*终端软件版本号*/
    uint16_t	HardwareVer ;					/*终端硬件版本号*/
	uint8_t		WifiParas[64] ;					/*WIFI配置参数*/
	uint32_t	Jingdu;							/*经度*/
	uint32_t	Weidu;							/*纬度*/
	uint8_t		ChargeStation[8] ;				/*充电站编号*/
	uint8_t		ChargePoint ;					/*充电桩类型*/
	uint8_t		DutyCycle;						/*占空比*/
    uint8_t		cs ;
}TERM_PARA;

#endif

/*************************************************************************************************
**                            End Of File
**************************************************************************************************/
