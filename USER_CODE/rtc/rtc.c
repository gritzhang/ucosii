/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2009-05-12
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chengmingji
** Created date:        2009-07-24
** Version:             V1.00
** Descriptions:        添加用户应用程序
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         Li Baihua
** Modified date:       2009-07-27
** Version:             V1.01
** Descriptions:        编写I2C例程
**
** Rechecked by:        CaiHua
*********************************************************************************************************/
#include "rtc.h"

/*********************************************************************************************************
** Function name:       rtcInit
** Descriptions:        初始化 LPC1778 RTC
** input parameters:    ucrtcTime:输入初始化时间
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint32_t rtcInit (PCF8563_DATE *pTimeAndDate )
{	
  	OS_ENTER_CRITICAL();

    LPC_SC->PCONP |= (1 << 9);                                              /* 访问 RTC 要用到外部时钟   */

    if ( LPC_RTC->RTC_AUX & (0x1<<4) )                                      /* RTC 振荡器失效探测标志    */
    {
    	LPC_RTC->RTC_AUX |= (0x1<<4);                                       /* 清除                      */
    }
	
	LPC_RTC->CCR   = 0x00;                                                  /* 禁止时间计数器            */
    LPC_RTC->ILR   = 0x03;                                                  /* 清除中断                  */
    LPC_RTC->CIIR  = 0x1;                                                   /* 使能秒增量中断            */
	//LPC_RTC->CIIR = 0;                                                    /* 屏蔽增量报警              */
	LPC_RTC->AMR   = 0xff;                                                  /* 禁止报警中断              */

	//LPC_RTC->CALIBRATION = 0x0;                                           /* 校准寄存器清0             */

		
    LPC_RTC->YEAR  = pTimeAndDate->year;                                    /* 设置时间计数寄存器        */
    LPC_RTC->MONTH = pTimeAndDate->month;
    LPC_RTC->DOM   = pTimeAndDate->day;
    LPC_RTC->DOW   = pTimeAndDate->week;
    LPC_RTC->HOUR  = pTimeAndDate->hour;
    LPC_RTC->MIN   = pTimeAndDate->minute;
    LPC_RTC->SEC   = pTimeAndDate->second;
    
    LPC_RTC->CIIR  = 0x01;
    //zyIsrSet(NVIC_RTC, (unsigned long)rtcIntIsr,PRIO_EIGHTEEN);           /* 设置中断并使能                  */
    NVIC_EnableIRQ(RTC_IRQn);                                               /* 设置中断并使能                  */
    NVIC_SetPriority(RTC_IRQn, 18);
   
    LPC_RTC->CIIR = 0x00000001;                                             /* Set counter increment interrupt */
    LPC_RTC->ILR = (0x01 | 0x02);                                           /* 清除增量、报警中断 标志         */

    LPC_RTC->CCR   = 0x01;                                                  /* 启动RTC                         */   
    OS_EXIT_CRITICAL();
    return 1; 
}

/*********************************************************************************************************
** Function name:       ExRTCSet
** Descriptions:        设置外部时钟
** input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ExRTCSet(PCF8563_DATE *pTimeAndDate )
{	 
    PCF8563_SS(Stop);                                                   /* 停止PCF8563                  */                             
    PCF8563_Set(pTimeAndDate);                                          /* 初始化日期                   */	                                
    PCF8563_Set_Timer(TimerOn|TimerClk64Hz,64);                         /* 设置定时器每秒钟溢出一次     */
    PCF8563_INT_State(SetINT|TIE|TITP);                                 /* 使能定时器中断               */
    PCF8563_SS(Start);                                                  /* 开启PCF8563                  */                                           
} 
/*********************************************************************************************************
** Function name:       ExRTCInit
** Descriptions:        外部时钟初始化
** input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ExRTCInit(void)
{
    PCF8563_SS(Stop);                                                   /* 停止PCF8563                  */
    TimeAndDate.year     = 2014;                                        /* 初始化年                     */
    TimeAndDate.month    = 6;                                           /* 初始化月                     */
    TimeAndDate.day      = 5;                                           /* 初始化日                     */
    TimeAndDate.week     = 4;                                           /* 初始化周                     */
    TimeAndDate.hour     = 12;                                          /* 初始化时                     */
    TimeAndDate.minute   = 12;                                          /* 初始化分                     */
    TimeAndDate.second   = 12;                                          /* 初始化秒                     */
    PCF8563_Set(&TimeAndDate);                                          /* 初始化日期                   */
    PCF8563_Set_Timer(TimerOn|TimerClk64Hz,64);                         /* 设置定时器每秒钟溢出一次     */
    PCF8563_INT_State(SetINT|TIE|TITP);                                 /* 使能定时器中断               */
    PCF8563_SS(Start);                                                  /* 开启PCF8563                  */   

}
/*********************************************************************************************************
** Function name:       main_rtc
** Descriptions:        PCF8563时钟
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void main_rtc (void)
{
	if(PCF8563_Read(&TimeAndDate)==FALSE){
		ExRTCInit();
	}
	if(TimeAndDate.year<=2000 || TimeAndDate.month >12 || TimeAndDate.month == 0 || 
		TimeAndDate.day >31 || TimeAndDate.day == 0 || TimeAndDate.hour >=24 || 
			TimeAndDate.minute >=60|| TimeAndDate.second >=60 )	{
		ExRTCInit();
	}
    rtcInit(&TimeAndDate);
}
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
