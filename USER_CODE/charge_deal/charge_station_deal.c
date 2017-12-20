/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           charge_station_deal.c
** Last modified Date:  2017-11-16
** Last Version:        V0.01
** Descriptions:        it_isr  
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-11-16
** Version:             V0.01
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:				TP-EVWA-220-1C
** Descriptions:       
**
** Rechecked by:  		
*********************************************************************************************************/
#include "charge_station_deal.h"
#include "lcd.h"

#define     TIMEOUT_Gun_Insert           30

//提示拔枪 检测充电枪插入 检测车辆允许充电 开始充电 启动AD转化测量cp电压值
//如果充电枪与车辆未连接提示插入充电枪（等待超时则停止此次充电并发送信息帧通知服务器）
//如果已连接则开始检测cp点电压并开始充电 cp输出PWM


//等待扫码充电 显示二维码
void Station_Idle_deal(void)
{
    //鉴权通过 且充电枪连接车辆
    if((Charge_Flag & (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT)) == (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT) ) 
    {
        Station_State = STATION_Charge;
        return;
    }
    else if(Charge_Flag & CHANGE_AUTHTIC_BIT)
    {
        Station_State = STATION_Authtic;
        return;
    }
    else if(Charge_Flag & CHANGE_GUN_CONN_BIT)
    {
        Station_State = STATION_Connect;
        return;
    }
}    


//充电枪连接等待签权
void Station_Connect_deal(void)
{
    if((Charge_Flag & (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT)) == (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT) )
    {
        Station_State = STATION_Charge;
        return;
    }
}

//充电枪签权等待连接
void Station_Authtic_deal(void)
{
   
    if(Time_Seconds - Wait_Gun_Insert_Seconds > TIMEOUT_Gun_Insert) //插枪超时
    {
        Station_State = STATION_SETUP;
        
        //显示插充电枪超时
    }
    
    if((Charge_Flag & (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT)) == (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT) )
    {
        Station_State = STATION_Charge;
        return;
    }    
}

//充电中 显示充电情况信息
void Station_Charge_deal(void)
{    
    //车辆可以充电 切换CP控制输出PWM
    if(Charge_Flag == (CHANGE_AUTHTIC_BIT|CHANGE_CAR_READY_BIT|CHANGE_GUN_CONN_BIT)) 
    {
        Charge_Flag |= CHANGE_ING_BIT;
        P1P16ON();//打开充电继电器开关  //220继电器输出控制 
        ChargeBeginTime = Time_Seconds;
        SET_CHG_KSSJ();
        return;
    } 
    else if(Charge_Flag == (CHANGE_AUTHTIC_BIT|CHANGE_CAR_READY_BIT|CHANGE_GUN_CONN_BIT|CHANGE_ING_BIT)) //充电中
    {
        if(PWRPINSTS )//读状态 上拉电阻 高电平继电器断开
        {
            if(!(DisplayFlag&DisplayErrButtonOff))
            {
                DisplayFlag |= DisplayErrButtonOff;
                DisplayOpenButton();
            }
        }
        else
            DisplayFlag |= DisplayChargeInfoFlag;
    }
}


//充电结束 显示扣款及充电订单结算信息
void Station_ChargeEND_deal(void)
{
//    Station_State = STATION_SETUP;
}

void Station_ChargeError_deal(void)
{
    
}

/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/


