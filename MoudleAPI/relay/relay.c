/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           relay.c
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
#include "lpc177x_8x_gpio.h"
#include "relay.h"

void All_Relay_Init(void)
{
    P1P16OFF();         //220继电器输出控制             引脚 P1.16
    PWRPININT();        //220输出继电器的控制状态读取   引脚 P2.9
    GUNPININT();        // P1.9 初始化 充电枪状态检测端口
    
	RLY1OFF();          //12V输出                       引脚 P2.15
	RLY2OFF();          //12V输出                       引脚 P2.14
	PWMCOFF();          //cp 控制连接寄存器             引脚 P1.5
    P3P25ON();          //cp 输出+-12V                  引脚 p3.25
	CCRCOFF();          //CC继电器控制                  引脚 P3.2

    
	LedLRL();		 							//左边红色LED
	LedLGH();					 				
	LedLDH();									//左边
    
    LedLCL();
	LedLCH();

					 				
	LedLDL();									//左边
     
    LedLCL();
	LedLCH();
  
    
	LedRRL();                                   //右边绿色LED
	LedRGH();
	LedRDH(); 									//右边  
    
    LedRCL();
	LedRCH();
    LedRCL();
	LedRCH();
    LedRCL();
	LedRCH();
}

void LEDBlinkFlow(void)
{
    uint8_t delay;
	LedLRL();		 							//左边红色LED
	LedLGH();	
	LedRRL();                                   //右边绿色LED
	LedRGH();  
    for(delay=0;delay<8;delay++)
    {
        
    }
}


/*********************************************************************************************************
** Function name:       pwmDb1Init
** Descriptions:        初始化双边沿PWM输出
** input parameters:    无
** output parameters:   无 
** Returned value:      无
*********************************************************************************************************/
void pwmDb1Init (uint8_t tim)
{
    uint32_t	pwmrval;
	if(tim==HIVCNT){
//		pwmstatus = 2;

//		PWMCON();
//		CCRCON();
	}
	else if(tim==PWMCNT){
//		pwmstatus = 1;
        
//		PWMCON();
//		CCRCON();
	}
	else{
//		pwmstatus = 0;

		PWMCOFF();
		CCRCOFF();
	}

//	if(tim==PWMCNT)
//		tim=bcd2hex(Term_Para1.DutyCycle);							 	//导入占空比

	LPC_SC->PCONP |= (0x01 << 6);                                       /* 打开PWM模块功率控制位       */  
	LPC_IOCON->P3_25  &= ~0x07;										    /* 功能选择                    */
	LPC_IOCON->P3_25  |= 2;                                             /* P3.25作PWM1[2]输出           */
	pwmrval     = PeripheralClock/10000/2;
	
	LPC_PWM1->PR  = 0x09;                                               /* FPCLK_FREQ分频，PWM计数频率  */
	                                                                    /* 为FPCLK_FREQ/10000           */
	LPC_PWM1->MCR = 0x02;                                               /* PWMMR0与PWMTC匹配时复位PWMTC */
	LPC_PWM1->PCR = (1 << 10);                                        	/* 使能PWM2输出                 */
	LPC_PWM1->MR0 = pwmrval;                                            /* PWM速率控制/计数初值         */
	LPC_PWM1->MR1 = (pwmrval / HIVCNT) * tim;                                           
	LPC_PWM1->MR2 = (pwmrval / HIVCNT) * tim;    
	                                                                    /* 设置边沿脉冲的脉宽和位置     */
	LPC_PWM1->LER = 0x7F;                                               /* 锁存所有PWM匹配值            */
	LPC_PWM1->TCR = 0x02;                                               /* 复位PWMTC                    */
	LPC_PWM1->TCR = 0x09;                                               /* 使能PWM                      */
}









/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

