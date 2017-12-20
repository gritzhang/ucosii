#ifndef __RELAY_H__
#define __RELAY_H__

#include "global.h"
//220v导通继电器		            
//	控制输出	P180	P1[16]
//			
//	读取状态	P132	P2[9]


//			
//			
//CP			
//	电压检测	P40	P1[31]
//			
//	控制继电器	P156	P1[5]
//			
//	控制输出+-12V	P56	P3[25]      
//			
//			
//CC(未用)			
//	检测	P185	P1[2]
//			
//	控制继电器	P207	P3[2]
//					
//12V输出继电器			
//	控制输出1	P91	P2[14]
//			
//	输出2	P99	P2[15]
//			
//			
//充电枪锁			
//			
//	检测(5v旁边in)	P188	P1[9]

#define		HIVCNT		100			//输出高电平
#define		PWMSTR	   	27	   		//16A (系数0.6)
#define		PWMCNT	   	53			//32A (系数0.6)
#define		PWMLOW	   	0			//输出负压

void All_Relay_Init(void);
void pwmDb1Init (uint8_t tim);


#endif