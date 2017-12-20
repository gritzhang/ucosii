/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           it_isr.c
** Last modified Date:  2017-10-17
** Last Version:        V0.01
** Descriptions:        it_isr  
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-10-17
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
#include "AllHand.h"



/*********************************************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTC中断处理函数
* Input          : 无
* Output         : 无
* Return         : 无
*********************************************************************************************************/
void  RTC_IRQHandler (void)
{  	                                                 					
	static uint8_t tmp;
    uint32_t ulTimes;
    uint32_t ulDatas;
    
	ulTimes = LPC_RTC->CTIME0;                                               /* 读取完整的时钟寄存器         */
	ulDatas = LPC_RTC->CTIME1;

	RTCtimedate.year   	= (ulDatas >> 16) & 0xfff; 
	RTCtimedate.month  	= (ulDatas >> 8) & 0x0f; 
	RTCtimedate.day   	= ulDatas & 0x1f; 
	RTCtimedate.week 	= (ulTimes >> 24) & 0x07; 
	RTCtimedate.hour 	= (ulTimes >> 16) & 0x1f;
	RTCtimedate.minute	= (ulTimes >> 8) & 0x3f;
	RTCtimedate.second	= ulTimes & 0x3f;

	LPC_RTC->ILR = 0x01;													/* 清除中断标志                 */														   	
	
	time_flg |= SECFLG;//secflg = 1;   									   	//秒标志
	
    Time_Seconds++;
    
//    if((time_flg&PTL104_TIME_TO_HEART) == 0){
//        if(Time_Seconds%SECONDS_PER_HEART_BEAT == 0){
//            time_flg |= PTL104_TIME_TO_HEART;
//        }
//    }
//    if(time_flg&PTL104_HEART_WAIT_ACK){
//        Wait_Heart_Ack_Seconds++;
//        if(Wait_Heart_Ack_Seconds == TIMEOUT_HEART_W_A){
//            time_flg |= PTL104_TIME_TO_HEART;
//            //超时标识置1
//        }
//    }
//    if((time_flg&PTL104_EVENT_WAIT_ACK) == 0){
//        if(Time_Seconds%SECONDS_PER_EVENT_UPDATE == 0){
//            time_flg |= PTL104_TIME_TO_EVENT;
//        }
//    }
//    if(time_flg&PTL104_HEART_WAIT_ACK){
//        Wait_Event_Ack_Seconds++;
//        if(Wait_Event_Ack_Seconds == TIMEOUT_EVENT_W_A){
//            time_flg |= PTL104_TIME_TO_EVENT;//重发事件上报帧
//            //超时标识置1
//        }
//    }
    
    
//	if(MsaConFlag == 1){
//		UartDlyTim.GucHeartF1++;												//测试帧响应计时 
//		AckTime++;																//延时计时
//	}
//	if(sysupdatetimer)
//	{
//		sysupdatetimer --;
//	}	
//	if(sysupdatetimer == 0) SysUpdateFlag = 0;
//	if(PTL104State == 1 || PTL104State == 2)
//		PTL104StateCount++;
//	if(PTL104StateCount >= 10)
//	{
//		TcpConFlag=0;										                //重新登录主站	
//		MsaConFlag=0;
//		wirelessConnectState = 0;
//		PTL104State = 0;
//		PTL104StateCount = 0;
//	}
//	if(readCardInterval != 0)
//		readCardInterval--;
//	if(wirelessConnectState == 11)
//		gprsSisoCount++;
//	if(gprsSisoCount >= 150){
//		wirelessConnectState = 0;
//		gprsSisoCount = 0;
//	}
//	DspDlyNs++;															    //延时计时
//		
//	sSysOvrTim.ADCVALCNT++;	 												//AD采样异常计数
//	if(SysCtlWrod.status == WORK3 || SysCtlWrod.activech==1) 		
//		SysCtlWrod.time++;
//	if(tmp++%2){
//		SYSRUNON();
//	}
//	else {
//		SYSRUNOFF();
//	}

//	if(sysresetflag){
//		if(sysresetcnt++>30){			
//			wdt_entry();  								                    //看门狗复位系统
//			while(1);			
//			//zyReset(ZY_HARD_RESET);	 				                    //参数设置系统复位
//		}	
//	}

}


extern volatile uint8_t     I2C_sla;                                             /* I2C器件从地址                */
extern volatile uint32_t    I2C_suba;                                            /* I2C器件内部子地址            */
extern volatile uint8_t     I2C_suba_num;                                        /* I2C子地址字节数              */
extern volatile uint8_t     *I2C_buf;                                            /* 数据缓冲区指针               */
extern volatile uint32_t    I2C_num;                                             /* 要读取/写入的数据个数        */
extern volatile uint8_t     I2C_end;                                             /* I2C总线结束标志：结束总线是  */
                                                                          /* 置1                          */
extern volatile uint8_t     I2C_suba_en;         /*  子地址控制。
                                               0--子地址已经处理或者不需要子地址
                                               1--读取操作
                                               2--写操作
                                        */

/*********************************************************************************************************
** Function name:       I2C1_IRQHandler
** Descriptions:        硬件I2C中断服务程序。
** input parameters:    无
** Output parameters:   无
** Returned value:      注意处理子地址为2字节的情况。
*********************************************************************************************************/
void  I2C1_IRQHandler(void)
{   /*
     *读取I2C状态寄存器I2DAT,按照全局变量的设置进行操作及设置软件标志,清除中断逻辑,中断返回
     */
    switch (LPC_I2C1->STAT & 0xF8)
    {   /* 根据状态码进行相应的处理     */
        case 0x08:    /* 已发送起始条件,主发送和主接收都有,装入SLA+W或者SLA+R */
        if(I2C_suba_en == 1)/* SLA+R */                                 /* 指定子地址读                 */
        {   LPC_I2C1->DAT = I2C_sla & 0xFE;                                   /* 先写入地址                   */
        }
        else                                                            /* SLA+W                        */
        {   LPC_I2C1->DAT = I2C_sla;                                          /* 否则直接发送从机地址         */
        }
                                                                        /* 清零SI位                     */
        LPC_I2C1->CONCLR = (1 << 3)|                                    /* SI                           */
                           (1 << 5);                                    /* STA                          */
        break;

        case 0x10:    /*已发送重复起始条件 */                           /* 主发送和主接收都有           */
                                                                        /* 装入SLA+W或者SLA+R           */
        LPC_I2C1->DAT = I2C_sla;                                              /* 重起总线后，重发从地址       */
        LPC_I2C1->CONCLR = 0x28;                                              /* 清零SI,STA                   */
        break;

        case 0x18:
        case 0x28:                                                      /* 已发送I2DAT中的数据,已接收ACK*/
        if (I2C_suba_en == 0)
        {
            if (I2C_num > 0)
            {    LPC_I2C1->DAT = *I2C_buf++;
                LPC_I2C1->CONCLR = 0x28;                                      /* 清零SI,STA                   */
                I2C_num--;
            }
            else                                                        /* 没有数据发送了               */
            {                                                           /* 停止总线                     */
                LPC_I2C1->CONSET = (1 << 4);                                  /* STO                          */
                LPC_I2C1->CONCLR = 0x28;                                      /* 清零SI,STA                   */
                I2C_end = 1;                                            /* 总线已经停止                 */
            }
        }

        if(I2C_suba_en == 1)                                            /* 若是指定地址读,则重新启动总线*/
        {
            if (I2C_suba_num == 2)
            {   LPC_I2C1->DAT = ((I2C_suba >> 8) & 0xff);
                LPC_I2C1->CONCLR = 0x28;                                      /* 清零SI,STA                   */
                I2C_suba_num--;
                break;
            }

            if(I2C_suba_num == 1)
            {   LPC_I2C1->DAT = (I2C_suba & 0xff);
                LPC_I2C1->CONCLR = 0x28;                                      /* 清零SI,STA                   */
                I2C_suba_num--;
                break;
            }

            if (I2C_suba_num == 0)
            {
                LPC_I2C1->CONCLR = 0x08;
                LPC_I2C1->CONSET = 0x20;
                I2C_suba_en = 0;                                        /* 子地址己处理                 */
                break;
            }
        }

        if (I2C_suba_en == 2)                                           /* 指定子地址写,子地址尚未指定, */
                                                                        /* 则发送子地址                 */
        {
            if (I2C_suba_num > 0)
            {    if (I2C_suba_num == 2)
                {    LPC_I2C1->DAT = ((I2C_suba >> 8) & 0xff);
                    LPC_I2C1->CONCLR = 0x28;
                    I2C_suba_num--;
                    break;
                }
                if (I2C_suba_num == 1)
                {    LPC_I2C1->DAT    = (I2C_suba & 0xff);
                    LPC_I2C1->CONCLR = 0x28;
                    I2C_suba_num--;
                    I2C_suba_en  = 0;
                    break;
                }
            }
        }
        break;

        case 0x40:                                                      /* 已发送SLA+R,已接收ACK        */
        if (I2C_num <= 1)                                               /* 如果是最后一个字节           */
        {    LPC_I2C1->CONCLR = 1 << 2;                                       /* 下次发送非应答信号           */
        }
        else
        {    LPC_I2C1->CONSET = 1 << 2;                                       /* 下次发送应答信号             */
        }
        LPC_I2C1->CONCLR = 0x28;                                              /* 清零SI,STA                   */
        break;

        case 0x20:                                                      /* 已发送SLA+W,已接收非应答     */
        case 0x30:                                                      /* 已发送I2DAT中的数据，已接收  */
                                                                        /* 非应答                       */
        case 0x38:                                                      /* 在SLA+R/W或数据字节中丢失仲  */
                                                                        /* 裁                           */
        case 0x48:                                                      /* 已发送SLA+R,已接收非应答     */
        LPC_I2C1->CONCLR = 0x28;
        I2C_end = 0xFF;
        break;

        case 0x50:                                                      /* 已接收数据字节，已返回ACK    */
        *I2C_buf++ = LPC_I2C1->DAT;
        I2C_num--;
        if (I2C_num == 1)                                               /* 接收最后一个字节             */
        {   LPC_I2C1->CONCLR = 0x2C;                                          /* STA,SI,AA = 0                */
        }
        else
        {   LPC_I2C1->CONSET = 0x04;                                          /* AA=1                         */
            LPC_I2C1->CONCLR = 0x28;
        }
        break;

        case 0x58:                                                      /* 已接收数据字节，已返回非应答 */
        *I2C_buf++ = LPC_I2C1->DAT;                                           /* 读取最后一字节数据           */
        LPC_I2C1->CONSET = 0x10;                                              /* 结束总线                     */
        LPC_I2C1->CONCLR = 0x28;
        I2C_end = 1;
        break;

        default:
        break;
    }
}

/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

