#ifndef __LPC_ADC_H__
#define __LPC_ADC_H__
#include "AllHand.h"


#define ADC_DEBUG           1
#define ADC_12BITS          1

#define ADC_DONE            0x80000000
#define ADC_OVERRUN         0x40000000
#define ADC_ADINT           0x00010000

#define ADC_NUM             10
#define ADC_CLK             400000                                          /* set to 400kHz                   */

#define ADC_CHN				5		                                        /*	转换用通道	                   */
#define ADC_TIM				100		                                        /*	采样次数	                   */


extern void ADCInit( uint32_t ADC_Clk );
extern uint8_t CP_Pin_Dector(void);


#endif