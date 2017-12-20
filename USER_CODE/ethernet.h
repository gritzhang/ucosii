#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#include "w5100.h"
#include "socket.h"
#include "AllHand.h"
#include "ptl104.h"


#define ETHERNET_INIT           0       //初始态
#define ETHERNET_CFG            1       //配置完成状态
#define ETHERNET_CONNECTING     2       //连接中状态
#define ETHERNET_CONNECT_OK     3       //连接成功
#define ETHERNET_DISCONNECT     4       //连接断开
#define ETHERNET_FAILURE        5       //多次连接尝试后 连接失败 启动自动诊断

#define IMR_VALUE     0xcf
#define RTR_VALUE     0x07d0
#define RCR_VALUE     0x08

#define MAX_SIZE        2048
#define RX_Size         1024
#define TX_Size         1024

//PTL104_State
#define PTL104_LOGIN            0x80
#define PTL104_ACTV             0x40
#define PTL104_SETTIME          0x20
#define PTL104_JF_STRGY         0x10
#define PTL104_LOGIN_SENT       0x08
#define PTL104_RECV             0x04

//
//uint8_t			time_flg; //bit7 secflg每秒标识 刷新LCD
#define     SECFLG                  0x80    //time_flg 上的相应的位
#define     PTL104_TIME_TO_HEART    0x20    //time_flg 上的相应的位
#define     PTL104_TIME_TO_EVENT    0x10    //time_flg 上的相应的位
#define     PTL104_HEART_WAIT_ACK   0x02    //time_flg 上的相应的位
#define     PTL104_EVENT_WAIT_ACK   0x01    //time_flg 上的相应的位
#define     PTL104_EVENT_CHANGE_UP   0x04    //time_flg 上的相应的位

#define     SECONDS_PER_HEART_BEAT      30      //心跳时间
#define     SECONDS_PER_EVENT_UPDATE    120     //事件上报帧时间

#define     TIMEOUT_HEART_W_A           20
#define     TIMEOUT_EVENT_W_A           20
#define     TIMEOUT_LOGIN_W_A           10

extern uint8_t         DisplayFlag;             //LCD屏幕显示标识位
extern uint8_t         Charge_Flag;             //充电准备标识 签权成功标志bit7 枪连接标识bit5
extern uint8_t         Ethernet_State;          //网络状态


extern uint8_t      time_flg;                       //时间相关标识  bit7 secflg每秒标识 刷新LCD
extern uint32_t     Time_Seconds;                   //每秒加1 系统运行时间
extern uint32_t    DisplayWaitTimeSeconds;
extern uint32_t     Wait_Ack_Seconds;
extern uint32_t     Wait_Gun_Insert_Seconds;        //等待充电枪连接 单位s
extern uint32_t     Wait_Heart_Ack_Seconds;
extern uint32_t     Wait_Event_Ack_Seconds;

extern uint8_t RX_Buffer[RX_Size];
extern uint8_t TX_Buffer[RX_Size];
extern uint8_t Ethernet_config(void);
extern void Ethernet_Init(void);
extern void Keep_Connect(void);
extern void Check_IntR(void);

extern void Ethernet_Work(uint8_t Value);
extern void Ethrenet_Connecting(uint8_t Value);
#endif
