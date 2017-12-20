#ifndef _ALLHAND_H_
#define _ALLHAND_H_

#include <LPC177x_8x.H>
#include "stdint.h"
#include "ucos_ii.h"

//#include "PTL104.h"
//#include "lcd.h"
//#include "main.h"

#include "PCF8563.h"
#include "rtc.h"
#include "t6963.h"
#include "datastructure.h"
#include "global.h"
#include "lcd.h"
#include "ethernet.h"
#include "socket.h"
#include "w5100.h"
#include "lpc_adc.h"
#include "relay.h"
#include "charge_station_deal.h"
#include "rs232.h"
#include "dl645a.h"
#include "QR_Encode.h"


#define LOCAL_DEBUG 1


//充电桩工作状态定义
#define STATION_SETUP       (0)
#define STATION_Idle        (1)
#define STATION_Charge      (2)
#define STATION_ChargeEND   (3)
#define STATION_Connect     (4)
#define STATION_Authtic     (5)

//充电桩充电准备位
#define         CHANGE_READY_OK             0xA0

#define         CHANGE_AUTHTIC_BIT          0x80        //鉴权成功位 1 鉴权成功
#define         CHANGE_CAR_READY_BIT        0x40        //车辆可以充电  1准备OK
#define         CHANGE_GUN_CONN_BIT         0x20        //充电枪连接车辆成功位  1连接成功
#define         CHANGE_ING_BIT              0x10
#define         CHANGE_END_PAY              0x08
#define         CHANGE_END_Full_BIT         0x04        //电池充满结束
#define         CHANGE_END_Ctrl_BIT         0x02        //APP结束充电
#define         CHANGE_END_Pull_BIT         0x01        //拔充电枪结束充电

//显示标识位
#define         ChargeInfoTitle             0x80
//#define         ChargeEndInfoTitle          0x40 
//#define         ChargeInfoUpdata            0x20        //充电信息更新
#define         ChargeGunInsert             0x10
#define         DisplayChargeInfoFlag       0x08        //充电信息显示
#define         DisplayChargeEndInfoFlag    0x04        //结算信息显示
#define         DisplayStartFlag            0x02        //初始界面信息显示

#define         DisplayErrButtonOff         0x01        //旋开急停按钮

#endif

