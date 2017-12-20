/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           lpc_adc.c
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
#include "lpc_adc.h"

/*****************************************************************************
** Function name:       ADCInit
**
** Descriptions:        initialize ADC channel
**
** parameters:          ADC clock rate
** Returned value:      None
** 
*****************************************************************************/
void ADCInit( uint32_t ADC_Clk )
{
    LPC_SC->PCONP |= (0x1<<12);         /* 使能ADC功率控制器                     */

    LPC_IOCON->P1_31 &= ~0x9F;
    LPC_IOCON->P1_31 |= 0x03;           /* ADC IN5                               */


    LPC_ADC->CR = ( 0x01 << ADC_CHN ) |   /* 选则ADC[5]为ADC输入                 */
    ((PeripheralClock/ADC_Clk-1)<<8) |  /* CLKDIV = Fpclk / ADC_Clk - 1      */ 
    ( 0 << 16 ) |                   /* BURST = 0, 软件控制转换               */
    ( 1 << 21 ) |                   /* PDN = 1,  处与正常模式                */
    ( 0 << 24 ) |                   /* START = 0  不启动AD转换               */
    ( 0 << 27 );                    /* EDGE = 0  上升沿启动                  */ 

    return;
}
/*****************************************************************************
** Function name:       ADCRead
**
** Descriptions:        Read ADC channel
**
** parameters:          Channel number
** Returned value:      Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead( uint8_t channelNum )
{
    uint32_t regVal, ADC_Data = 0;
/* 
 * channelNum大于ADC最大通道号 则清零channelNum 
 */
    if ( channelNum >= ADC_NUM ){      
        channelNum = 0;
        return 0xFFFFFF;                 /* 返回一个大于ADC采样最大值的数         */     
    }

    LPC_ADC->CR &= 0xF8FFFFFF;         /* 停止转换                             */
    regVal = LPC_ADC->DR[channelNum];  /* 读数据寄存器                         */

    if ( regVal & ADC_OVERRUN ) {
        return ( 0xFFFFFF );
    }
   
#if ADC_12BITS
    ADC_Data = ( regVal >> 4 ) & 0xFFF;
#else
    ADC_Data = ( regVal >> 6 ) & 0x3FF;
#endif

  return ( ADC_Data );
}
/*****************************************************************************
** Function name:       ADC_GetStat
**
** Descriptions:        获取A/D转换的状态
**
** parameters:          ChannelNum
** Returned value:      
** 
*****************************************************************************/
uint32_t ADC_GetStat( uint8_t channelNum )
{
    uint32_t stat;

    if( channelNum >= ADC_NUM) {
        channelNum = 0; 
    }
    stat = ( ( LPC_ADC->DR[channelNum] ) &ADC_DONE ) >>31;
    return (stat);
}
/*****************************************************************************
** Function name:       ADC_Start
**
** Descriptions:        启动ADC转换
**
** parameters:          ChannelNum
** Returned value:      Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
void ADC_Start( uint8_t channelNum )
{
    if( channelNum >= ADC_NUM) {
        channelNum = 0; 
    }
    LPC_ADC->CR |= (1<<channelNum)|
                    (1UL<<24);      /* START = 1  启动AD转换                 */
}


uint32_t AD0_IN5_Value(uint8_t meter_times)
{
    uint8_t i;
    uint32_t ulADCData,ulADCbuf,ret;
    ulADCData = 0;
    for(i = 0;i < meter_times; i++){
        ADC_Start(ADC_CHN);                                             		    /* 打开A/D转换                  */
        while(ADC_GetStat(ADC_CHN));                                    		    /* 等待转换完成                 */
        ulADCbuf=ADCRead(ADC_CHN);  
        ulADCData += ulADCbuf;
    }
    ret = (ulADCData / meter_times);                                    		    /* 采样n次进行虑波处理          */
    return ret;
}

uint8_t CP_Pin_Dector(void)
{
    uint32_t CP_Value;
    
    CP_Value = AD0_IN5_Value(ADC_TIM);
    
    if(CP_Value<MIN6V){

    }
    else if(CP_Value<MAX6V){
        if(!(Charge_Flag&CHANGE_GUN_CONN_BIT))
        {
            pwmDb1Init(PWMCNT);
            Charge_Flag |= CHANGE_GUN_CONN_BIT;          //车辆连接OK
        }
        Charge_Flag |= CHANGE_CAR_READY_BIT;             //车辆连接OK且可以充电
    }
    else if(CP_Value<MAX9V && CP_Value > MIN9V){
        if(!(Charge_Flag&CHANGE_GUN_CONN_BIT))
            pwmDb1Init(PWMCNT);
        Charge_Flag |= CHANGE_GUN_CONN_BIT;             //车辆连接OK
        
        if((Charge_Flag & CHANGE_ING_BIT) ){            //充电中车辆断开S2开关 电池已充满！
            Charge_Flag |= CHANGE_END_Full_BIT;         //电池充满           
            Charge_Flag &= ~(CHANGE_AUTHTIC_BIT | CHANGE_ING_BIT);
            Station_State = STATION_ChargeEND;
            DisplayWaitTimeSeconds = Time_Seconds;
            pwmDb1Init(HIVCNT);
            P1P16OFF();                              //220继电器输出控制  关闭充电枪220v输出  
            SET_CHG_JSSJ();
        }        
    }
    else if(CP_Value<MAX12V  && CP_Value > MIN12V){
               
        if((Charge_Flag & CHANGE_ING_BIT) ){        //充电中拔枪结束充电 结束充电 上传充电情况
            Charge_Flag |= CHANGE_END_Pull_BIT;              
            Charge_Flag &= ~(CHANGE_AUTHTIC_BIT | CHANGE_ING_BIT);
            Station_State = STATION_ChargeEND;
            DisplayWaitTimeSeconds = Time_Seconds;
            pwmDb1Init(HIVCNT);
            SET_CHG_JSSJ();
//            P1P16OFF();         //220继电器输出控制  关闭充电枪220v输出    
        }
        P1P16OFF();         //220继电器输出控制  关闭充电枪220v输出 
        Charge_Flag &= ~(CHANGE_GUN_CONN_BIT | CHANGE_CAR_READY_BIT);  // 
    }
    else{

    }    
}

/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

