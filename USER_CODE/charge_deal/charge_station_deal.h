#ifndef __CHARGE_STATION_DEAL_H__
#define __CHARGE_STATION_DEAL_H__
#include "AllHand.h"

extern uint8_t         Station_State;         //充电桩状态
extern uint8_t         Charge_Flag;                //充电准备标识 签权成功标志bit7 枪连接标识bit5

extern uint32_t    ChargeBeginTime;                //充电开始时间

//等待扫码充电 显示二维码
void Station_Idle_deal(void);

//等待充电枪连接
void Station_Connect_deal(void);

//充电中 显示充电情况信息
void Station_Charge_deal(void);

//充电结束 显示扣款及充电订单结算信息
void Station_ChargeEND_deal(void);

void Station_ChargeError_deal(void);

//充电枪签权等待连接
void Station_Authtic_deal(void);



#endif