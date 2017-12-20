/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           hardware_interface.c
** Last modified Date:  2013-08-08
** Last Version:        V0.01
** Descriptions:        
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
#include "hardware_interface.h"
#include "ucos_ii.h"

volatile uint8_t     I2C_sla;                                             /* I2C器件从地址                */
volatile uint32_t    I2C_suba;                                            /* I2C器件内部子地址            */
volatile uint8_t     I2C_suba_num;                                        /* I2C子地址字节数              */
volatile uint8_t     *I2C_buf;                                            /* 数据缓冲区指针               */
volatile uint32_t    I2C_num;                                             /* 要读取/写入的数据个数        */
volatile uint8_t     I2C_end;                                             /* I2C总线结束标志：结束总线是  */
                                                                          /* 置1                          */
volatile uint8_t     I2C_suba_en;         /*  子地址控制。
                                               0--子地址已经处理或者不需要子地址
                                               1--读取操作
                                               2--写操作
                                        */

// rf_FM1702 & FM25Cl64 SPI接口
void SSP0_Init(void)
{
    uint8_t uiRegVal = 0;

    LPC_SC->PCONP |= (1 << 21);                                           /* 开启SSP0外设                 */

	/* 
	 * 设定外设分频值
	 */
	uiRegVal = LPC_SC->PCLKSEL;
	if (uiRegVal < 4) {
			LPC_SC->PCLKSEL = 4;
	}
	
	LPC_IOCON->P0_15 &= ~0x07;
	LPC_IOCON->P0_15 |=  0x02;                                              /* SSP CLK                      */
	//LPC_IOCON->P0_16 &= ~0x07;    
	//LPC_IOCON->P0_16 |=  0x02;                                            /* SSP SSEL                     */
	LPC_IOCON->P0_17 &= ~0x07;
	LPC_IOCON->P0_17 |=  0x02;                                              /* SSP MISO                     */
	LPC_IOCON->P0_18 &= ~0x07;    
	LPC_IOCON->P0_18 |=  0x02;                                              /* SSP MOSI                     */
		
	LPC_IOCON->P0_16 &= ~0x07;                                              /* P0.16为SSP0的CS管脚          */
	LPC_GPIO0->DIR |= (0x1<<16);
	LPC_GPIO0->SET |= (0x1<<16);

    LPC_SSP0->CR0  = 0x0807;												/* 设置数据长度为8位，帧格式SPI，SCK高有效，*/
    LPC_SSP0->CPSR = 0x02;			    		                            /* 设置SSP从PCLK获得的分频  */
                                                                            /* 值,主模式下有效,         */
                                                                            /* 最小值为0x02	            */
    LPC_SSP0->CR1  = 0x02;		   			                                /* 设置SSP为主机模式,       */	                                                                        /* 主模式下有效, 最小值为0x02*/   
}

void SSP_Hardeare_SendData(LPC_SSP_TypeDef* SSPx, uint16_t Data)
{
    while ((SSPx->SR & 0x02) == 0);	    	                            /* 等待发送FIFO留出空间         */   
	SSPx->DR = Data;
    while ((SSPx->SR & 0x10) == 0x10);	 	                            /* 等待数据帧发送完毕	        */ 
    return;																// 清除SPIF标志
}

uint16_t SSP_Hardeare_ReceiveData(LPC_SSP_TypeDef* SSPx)
{
	uint8_t tmp ;
    while ((SSPx->SR & 0x04) == 0x04)
    {
    	tmp = SSPx->DR ;
    }	    				                    							/* 清空接收FIFO留出空间   	*/
    
    while ((SSPx->SR & 0x01) == 0);	    				                    /* 清空发送FIFO留出空间   	*/
	SSPx->DR = 0xff ;
    while ((SSPx->SR & 0x10) == 0x10);	 				                    /* 等待数据帧发送完毕		*/

    while ((SSPx->SR & 0x04) == 0x00);	 				                    /* 等待数据帧接收完毕		*/

	tmp = SSPx->DR ;

    return tmp;   
}

// W5100 SPI接口	
void SSP1_Init(void)
{
    uint8_t uiRegVal = 0;

    LPC_SC->PCONP |= (0x1<<10);                                         /* 开启SSP1外设                 */

    /* 
    * 设定外设分频值
    */
    uiRegVal = LPC_SC->PCLKSEL;
    if ( uiRegVal < 4 ) {
        LPC_SC->PCLKSEL = 4;
    }

    LPC_IOCON->P0_7 &= ~0x07;
    LPC_IOCON->P0_7 |=  0x02;                                       /* SSP CLK                      */
    LPC_IOCON->P0_8 &= ~0x07;
    LPC_IOCON->P0_8 |=  0x02;                                       /* SSP MISO                     */
    LPC_IOCON->P0_9 &= ~0x07;    
    LPC_IOCON->P0_9 |=  0x02;                                       /* SSP MOSI                     */

    LPC_IOCON->P0_6 &= ~0x07;                                  /* P0.6为SSP1的CS管脚           */
    LPC_GPIO0->DIR |= (0x1<<6);
    LPC_GPIO0->SET |= (0x1<<6);

    LPC_SSP1->CR0  = 0x0807; 				/* 设置数据长度为8位，帧格式SPI，SCK高有效，第一个时钟沿采样，位速率为默认值 	*/
    LPC_SSP1->CPSR = 0x02;			    /* 设置SSP从PCLK获得的分频值,主模式下有效, 最小值为0x02 */
    LPC_SSP1->CR1  = 0x02;		   		/* 设置SSP为主机模式,       */	 
}    

void SSP1_IO_Init(void)
{    
    PINSEL_ConfigPin ( 0, 6, 0);
	LPC_GPIO0->DIR |= SSP1_SS_PIN;

	PINSEL_ConfigPin ( 0, 7, 0);
	LPC_GPIO0->DIR |= SSP1_SCLK_PIN;
	PINSEL_ConfigPin ( 0, 9, 0); 
	LPC_GPIO0->DIR |= SSP1_MOSI_PIN;
    
	PINSEL_ConfigPin ( 0, 8, 0);
	LPC_GPIO0->DIR &= ~SSP1_MISO_PIN;
    
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_SS_PIN,1);
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_SCLK_PIN,0);
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_MOSI_PIN,1);
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_MISO_PIN,1);
}

// 初始化I2C1
void i2c1Init(uint32_t ulFi2c)
{
    LPC_SC->PCONP |= (0x01 << 19);                                      /* 打开I2C1模块功率控制位       */
    LPC_SC->PCONP |= (0x01 << 15);                                      /* 打开GPIO模块功率控制位       */
    LPC_IOCON->P0_0 = 0x03 | (0x00 << 3) | (0x01 << 10);                /* P0.0配置为SDA1功能          */
    LPC_IOCON->P0_1 = 0x03 | (0x00 << 3) | (0x01 << 10);                /* P0.1配置为SCL1功能          */

    if (ulFi2c>400000) ulFi2c = 400000;

    LPC_I2C1->SCLH = (PeripheralClock / ulFi2c) / 2;                   /* 设置I2C时钟为fi2c            */
    LPC_I2C1->SCLL = (PeripheralClock / ulFi2c) / 2;

    NVIC_EnableIRQ(I2C1_IRQn);                                          /* 使能I2C中断                  */
    NVIC_SetPriority(I2C1_IRQn, 11);                                     /* 设置I2C1中断优先级并使能中断 */

    LPC_I2C1->CONCLR = 0x2C;
    LPC_I2C1->CONSET = 0x40;                                            /* 使能主I2C                    */  
}
   
/*********************************************************************************************************
** Function name:           Wait_I2c_End
** Descriptions:            软件延时，提供给I2C总线等待超时使用
** input parameters:        dly        延时参数，值越大，延时越久
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
uint8_t  Wait_I2c_End(uint32_t  dly)
{  uint32_t  i;

    if( I2C_end==1 ) return (1);
    for(; dly>0; dly--)
    for(i=0; i<5000; i++)
    {
        if( I2C_end==1 ) return (1);
    }
    return (0);
}


/*********************************************************************************************************
** Function name:       I2C_ReadNByte
** Descriptions:        从有子地址器件任意地址开始读取N字节数据
** input parameters:    sla         器件地址
**                      suba_type   子地址结构    1－单字节地址    2－8+X结构    2－双字节地址
**                      suba        器件子地址
**                      s           数据接收缓冲区指针
**                      num         读取的个数
** Output parameters:   返回值为0时表示出错，为1时表示操作正确。
** Returned value:      NONE
*********************************************************************************************************/
uint8_t I2C_ReadNByte (uint8_t sla, uint32_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
    if (num > 0)                                                        /* 判断num个数的合法性          */
    {                                                                   /* 参数设置                     */
        if (suba_type == 1)
        {                                                               /* 子地址为单字节               */
            I2C_sla         = sla + 1;                                  /* 读器件的从地址，R=1          */
            I2C_suba        = suba;                                     /* 器件子地址                   */
            I2C_suba_num    = 1;                                        /* 器件子地址为1字节            */
        }
        if (suba_type == 2)
        {                                                               /* 子地址为2字节                */
            I2C_sla         = sla + 1;                                  /* 读器件的从地址，R=1          */
            I2C_suba        = suba;                                     /* 器件子地址                   */
            I2C_suba_num    = 2;                                        /* 器件子地址为2字节            */
        }
        if (suba_type == 3)
        {                                                               /* 子地址结构为8+X              */
            I2C_sla         = sla + ((suba >> 7 )& 0x0e) + 1;           /* 读器件的从地址，R=1          */
            I2C_suba        = suba & 0x0ff;                             /* 器件子地址                   */
            I2C_suba_num    = 1;                                        /* 器件子地址为8+x              */
        }
        I2C_buf     = s;                                                /* 数据接收缓冲区指针           */
        I2C_num     = num;                                              /* 要读取的个数                 */
        I2C_suba_en = 1;                                                /* 有子地址读                   */
        I2C_end     = 0;

                                                                        /* 清除STA,SI,AA标志位          */
        LPC_I2C1->CONCLR = (1 << 2)|                                          /* AA                           */
                           (1 << 3)|                                          /* SI                           */
                           (1 << 5);                                          /* STA                          */

                                                                        /* 置位STA,启动I2C总线          */
        LPC_I2C1->CONSET = (1 << 5)|                                          /* STA                          */
                           (1 << 6);                                          /* I2CEN                        */

                                                                        /* 等待I2C操作完成              */
        return( Wait_I2c_End(20));
    }
    return (FALSE);
} 

/*********************************************************************************************************
** Function name:       I2C_WriteNByte
** Descriptions:        向有子地址器件写入N字节数据
** input parameters:    sla         器件地址
**                      suba_type   子地址结构    1－单字节地址    2－8+X结构    2－双字节地址
**                      suba        器件子地址
**                      s           将要写入的数据的指针
**                      num         将要写入的数据的个数
** Output parameters:   返回值为0时表示出错，为1时表示操作正确。
** Returned value:      NONE
*********************************************************************************************************/
uint8_t I2C_WriteNByte(uint8_t sla, uint8_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
    if (num > 0)                                                        /* 如果读取的个数为0，则返回错  */
                                                                        /* 误                           */
    {                                                                   /* 设置参数                     */
        if (suba_type == 1)
        {                                                               /* 子地址为单字节               */
            I2C_sla         = sla;                                      /* 读器件的从地址               */
            I2C_suba        = suba;                                     /* 器件子地址                   */
            I2C_suba_num    = 1;                                        /* 器件子地址为1字节            */
        }
        if (suba_type == 2)
        {                                                               /* 子地址为2字节                */
            I2C_sla         = sla;                                      /* 读器件的从地址               */
            I2C_suba        = suba;                                     /* 器件子地址                   */
            I2C_suba_num    = 2;                                        /* 器件子地址为2字节            */
        }
        if (suba_type == 3)
        {                                                               /* 子地址结构为8+X              */
            I2C_sla         = sla + ((suba >> 7 )& 0x0e);               /* 读器件的从地址               */
            I2C_suba        = suba & 0x0ff;                             /* 器件子地址                   */
            I2C_suba_num    = 1;                                        /* 器件子地址为8+X              */
        }

        I2C_buf     = s;                                                /* 数据                         */
        I2C_num     = num;                                              /* 数据个数                     */
        I2C_suba_en = 2;                                                /* 有子地址，写操作             */
        I2C_end     = 0;

                                                                        /* 清除STA,SI,AA标志位          */
        LPC_I2C1->CONCLR = (1 << 2)|                                          /* AA                           */
                           (1 << 3)|                                          /* SI                           */
                           (1 << 5);                                          /* STA                          */

                                                                        /* 置位STA,启动I2C总线          */
        LPC_I2C1->CONSET = (1 << 5)|                                          /* STA                          */
                           (1 << 6);                                          /* I2CEN                        */

                                                                        /* 等待I2C操作完成              */
        return( Wait_I2c_End(20));
                /*while (I2C_end == 0)
        {
         //
        }
        if (I2C_end == 1)
            return (TRUE);
        else
            return (FALSE);            */
    }
    return (FALSE);
}


/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/





