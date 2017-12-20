/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-08-08
** Last Version:        V0.01
** Descriptions:        The main() function 
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-09-30
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
#include "main.h"

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/

//各个任务优先级安排
#define     MainTask_Prio           10
#define     UartCfgTas_Prio         15
#define     NetTask_Prio	        18
#define     RFCardTask_Prio	        13
#define     Uart485EleMeTas_Prio     19
#define     DisplayTas_Prio         30

/*********************************************************************************************************
  定义全局变量常量
*********************************************************************************************************/
static OS_STK stkMainTask[1024];			//主任务
static OS_STK stkNetTask[1024];			    //网络任务
static OS_STK stkRFCardTask[1024];			//刷卡任务
static OS_STK stkUartCfgTask[1024];			//串口配置参数任务
static OS_STK stkUart485Task[1024];		    //485读表任务
static OS_STK stkDisplayTask[1024];         //显示任务

uint8_t			time_flg;                   //bit7 secflg每秒标识 刷新LCD
uint8_t         Charge_Flag = 0;                //充电准备标识 签权成功标志bit7 枪连接标识bit5
uint8_t         DisplayFlag = 0;            //LCD屏幕显示标识位
//#define         CHANGE_READY_OK         0xA0
//#define         CHANGE_AUTHTIC_BIT      0x80
//#define         CHANGE_GUN_CONN_BIT     0x20

uint8_t         Ethernet_State = 0;         //网络状态
uint8_t         Station_State  = 0;         //充电桩状态

uint32_t    Time_Seconds;                   //开始计时时间 单位s 系统上电运行时间
uint32_t    Wait_Ack_Seconds;               //等待应答计时 单位s
uint32_t    Wait_Heart_Ack_Seconds;         //心跳等待计时 单位s
uint32_t    Wait_Event_Ack_Seconds;         //事件等待计时 单位s

uint32_t    DisplayWaitTimeSeconds;         //显示等待时间
uint32_t    Wait_Gun_Insert_Seconds;        //等待充电枪连接 单位s
uint32_t    ChargeBeginTime;                //充电开始时间

const uint8_t softVer[4] = {9,01,17,20};

/*********************************************************************************************************
** Function name:       RFCardTask
** Descriptions:        RF刷卡任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void RFCardTask(void *pvData)
{
	pvData = pvData;
        
	while (1) 
	{
        OSTimeDly(OS_TICKS_PER_SEC/4);
	}
}

/*********************************************************************************************************
** Function name:       NetTask
** Descriptions:        网络连接任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void NetTask(void *pvData)
{
	pvData = pvData;
    
	while (1) 
	{ 
        uint8_t Sn_IR_Value;
        if(W5100_INT_STS()){
            ISR_Process();
        }
        Sn_IR_Value = getISR(0);
        switch(Ethernet_State){
            case ETHERNET_INIT:
                Ethernet_Init();
                Ethernet_State = ETHERNET_CFG;
                break;
            case ETHERNET_CFG:
                Ethernet_config();
                if(connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort)){ 
                    Ethernet_State = ETHERNET_CONNECTING;
                }
                break;
            case ETHERNET_CONNECTING://2
                Ethrenet_Connecting(Sn_IR_Value);
                break;
            case ETHERNET_CONNECT_OK:
                Ethernet_Work(Sn_IR_Value);
                break; 
            case ETHERNET_DISCONNECT:
                if(connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort)){ 
                    Ethernet_State = ETHERNET_CONNECTING;
                }
                break; 
            case ETHERNET_FAILURE:
                if(connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort)){ 
                    Ethernet_State = ETHERNET_CONNECTING;
                }
                break; 
            default:
                Ethernet_State = ETHERNET_CFG;
                break;             
        }
        if(Ethernet_State != ETHERNET_CONNECT_OK)
        {
            if((Charge_Flag&CHANGE_END_Ctrl_BIT)||(Charge_Flag&CHANGE_END_Full_BIT)||(Charge_Flag&CHANGE_END_Pull_BIT))    
            {
                Charge_Flag &= 0xf0;
                Charge_Flag |= CHANGE_END_PAY;
//                Save_StrateChargeInfo_Frame(&sPTL_104_I_SND);       //保存计费策略充电情况
//                Save_OfflineChargeInfo_Frame(&sPTL_104_I_SND);       //保存离线充电情况    
            }            
        }
        OSTimeDly(OS_TICKS_PER_SEC/10);
	}
}

/*********************************************************************************************************
** Function name:       Uart485_E_M_Task
** Descriptions:        485读表任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void Uart485_E_M_Task(void *pvData)
{
	pvData = pvData;
    
    main_rs232();

}

/*********************************************************************************************************
** Function name:       UartCfgTask
** Descriptions:        串口配置任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UartCfgTask(void *pvData)
{
	pvData = pvData;
    
	main_uart1();
}

/*********************************************************************************************************
** Function name:       DisplayTask
** Descriptions:        显示任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void DisplayTask(void *pvData)
{
    pvData = pvData;
    
    while(1)
    {
    	if(time_flg & SECFLG)
        {
    		time_flg &= ~SECFLG;
    		dspTimeAndIcon(Term_Para1.CHGMode);
       	} 
        switch(Station_State)
        {
            case STATION_SETUP: 
                sbhmaskflag=0;  
                dspstandbypage();               //LCD显示初始界面                     
                Charge_Flag = 0x0;
                DisplayFlag = 0;
                memset(&sPTL_104_I_DW_JQSJ,0,sizeof(sPTL_104_I_DW_JQSJ));
                Station_State = STATION_Idle;            
                break;
            case STATION_Idle:                  //等待扫码充电 显示二维码                 
                break;      
            case STATION_Connect:               //等待充电鉴权
                
                break;   
            case STATION_Authtic:               //等待充电枪连接
                Display_GUN_Insert();           //显示请将充电枪插入电动汽车充电插座
                break;             
            case STATION_Charge:                //充电中 显示充电情况信息
                if(DisplayFlag&DisplayChargeInfoFlag)
                    DisplayChargeInfo();
                break;        
            case STATION_ChargeEND:             //充电结束 显示扣款及充电订单结算信息
                DisplayChargeEndInfo();
                if(Time_Seconds - DisplayWaitTimeSeconds > TIMEOUT_DISPLAYWAIT)
                {                  
                    Station_State = STATION_SETUP;
                }

                
//                OSTimeDly(OS_TICKS_PER_SEC*8);  //充电结束信息显示延时  不能用这个延时会影响走秒
                break;            
            default : 
                DisplayChargeError();
                break;
        }   
        OSTimeDly(OS_TICKS_PER_SEC/5);        
    }
}

////直接保存充电参数
//void SaveCurrent(void)
//{
//	OS_ENTER_CRITICAL();
//	SysCtlWrod.cs	= CheckSum1((uint8_t *)&SysCtlWrod, (uint8_t *)&SysCtlWrod.cs);
//	memcpy((uint8_t *)&SysCtlWrodBak,(uint8_t *)&SysCtlWrod,sizeof(SysCtlWrodBak));
//	FRAMWrite((uint8_t *)&SysCtlWrodBak,CHGPARA_ADD,sizeof(SysCtlWrodBak));
//	FRAMWrite((uint8_t *)&SysCtlWrodBak,CHGPARA_ADD_BAK,sizeof(SysCtlWrodBak));
//	OS_EXIT_CRITICAL();
//}
////保存充电状态下充电参数
//void SavePara2Fram(void )
//{	
//	if(SysCtlWrod.card1pwdok==1){
//		if(memcmp((uint8_t *)&SysCtlWrodBak,(uint8_t *)&SysCtlWrod,sizeof(SysCtlWrodBak))!=0)
//			SaveCurrent();
//	}					
//}

/*********************************************************************************************************
** Function name:       mainTask
** Descriptions:        主任务 显示提示信息
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void mainTask(void *pvData)
{
    char QrCode[60] = { 0x68,0x74,0x74,0x70,0x3A,0x2F,0x2F,0x77,0x78,0x2E,
                        0x74,0x65,0x6C,0x6C,0x75,0x73,0x70,0x6F,0x77,0x65,
                        0x72,0x2E,0x63,0x6E,0x3F,0x63,0x6F,0x64,0x65,0x3D,
                        0x31,0x39,0x2C,0x31,0x31,0x30,0x38,0x2C,};
	pvData = pvData;
       
	GolbalVarInit();										//变量初始化
	SystemParaInit();										//参数初始化
          
    //生成二维码 数据在 m_byModuleData  版本信息 m_nVersion
    //二维码字符串数组//"http://wx.telluspower.cn?code=19,1108,1322111110001676"
   
//    FillQrCode(QrCode);
//    EncodeData(QrCode);
                        
    EncodeData("http://wx.telluspower.cn?code=19,1108,1322111110001676");
    
    T6963_Init();								   			//LCD显示初始化
    dspstandbypage();                                       //LCD显示初始界面   
    ADCInit(ADC_CLK);                                       //ADC初始化
    
    OSTaskCreate(UartCfgTask, (void *)0, &stkUartCfgTask[sizeof(stkUartCfgTask) / 4 - 1], UartCfgTas_Prio);
    OSTaskCreate(NetTask, (void *)0, &stkNetTask[sizeof(stkNetTask) / 4 - 1], NetTask_Prio);
    OSTaskCreate(RFCardTask, (void *)0, &stkRFCardTask[sizeof(stkRFCardTask) / 4 - 1], RFCardTask_Prio);
    OSTaskCreate(Uart485_E_M_Task, (void *)0, &stkUart485Task[sizeof(stkUart485Task) / 4 - 1], Uart485EleMeTas_Prio);
    OSTaskCreate(DisplayTask, (void *)0, &stkDisplayTask[sizeof(stkDisplayTask) / 4 - 1], DisplayTas_Prio);
    
    Station_State = STATION_SETUP;
    pwmDb1Init(HIVCNT);
    PWMCON();                       //cp 控制连接继电器     引脚 P1.5
    OSTimeDly(OS_TICKS_PER_SEC/4);  //稍微延时  后面电压检测
    
//    P3P0OFF();          //打开枪锁
//    P3P1ON();
//    OSTimeDly(OS_TICKS_PER_SEC);
//    P3P1OFF();    
//    if(GUNPINSTS)       //枪拔下来为 0
//    {
//        P3P1OFF(); 
//    }

//    P3P0ON();           //锁上枪锁
//    P3P1OFF();
//    OSTimeDly(OS_TICKS_PER_SEC);
//    P3P0OFF();
//    if(GUNPINSTS)       //枪拔下来为 0
//    {
//        P3P0OFF();
//    }    
    
	while (1) 
	{
//        if(!GUNPINSTS)       //枪拔下来为0 
//        {
//            P3P0ON();           //锁上枪锁
//            P3P1OFF();
//            OSTimeDly(OS_TICKS_PER_SEC);
//            P3P0OFF();  
//        }
        CP_Pin_Dector();
        switch(Station_State)
        {
            case STATION_SETUP: //充电相关数据清0 
                break;
            case STATION_Idle:                  //等待扫码充电 
                Station_Idle_deal();
                break;      
            case STATION_Connect:               //等待充电鉴权
                Station_Connect_deal();
                break;   
            case STATION_Authtic:               //等待充电枪连接
                Station_Authtic_deal();
                break;             
            case STATION_Charge:                //充电中 
                Station_Charge_deal();
                break;        
            case STATION_ChargeEND:             //充电结束 
                Station_ChargeEND_deal();
                break;            
            default : 
                Station_ChargeError_deal();
                break;
        }	
        if(Station_State==STATION_Charge)
            OSTimeDly(OS_TICKS_PER_SEC/25);
        else
            OSTimeDly(OS_TICKS_PER_SEC/5);
	}
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        用户程序入口函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main(void)
{
    
    SSP0_Init();													// rf_FM1702 & FM25Cl64 SPI接口
    SSP1_Init();													// W5100 SPI接口	

    i2c1Init(100000);                                               // 初始化I2C1 实时时钟PCF8563
    
    All_Relay_Init();
    
    main_rtc();
    
    
    OSInit();                                                                                                
    OSTaskCreate(mainTask, (void *)0, &stkMainTask[sizeof(stkMainTask) / 4 - 1], MainTask_Prio);
    OSStart();
}
/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

