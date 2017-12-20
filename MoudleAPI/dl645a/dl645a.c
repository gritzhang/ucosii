/****************************************Copyright (c)****************************************************
**                            		xxx Co.,LTD.
**
**                                 http://www.xxx.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               myfun.c
** Latest modified Date:    2013-09-05
** Latest Version:          1.00
** Descriptions:            DL645协议处理函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AndyLau
** Created date:            2013-09-05
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "dl645a.h"

//DL645-1997规约帧打包 ,返回打包后的帧长度，数据加0x33
uint8_t FramePack_645_1997(uint16_t IDSymbol,uint8_t *MeterNo,uint8_t *DataPtr,uint8_t DataLen,uint8_t *SendBuff,uint8_t Ctlw)  
{
		  uint8_t 	ii;
		  uint8_t 	CCs;
		  uint8_t 	*pSendBuff;

		  pSendBuff = SendBuff;
		  
		  *pSendBuff++ = 0xFE;  				 		//引导字符
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;
		  
		  *pSendBuff++ = 0x68;
		  memcpy(pSendBuff,MeterNo,6);
		  pSendBuff+=6;
		  *pSendBuff++ = 0x68;
		  *pSendBuff++ = Ctlw;
		  *pSendBuff++ = DataLen; 						//数据长度

		  if(DataLen>=2) 
		  {
		  		*pSendBuff++ = (IDSymbol%256)+0x33;	   		//数据标识
		  		*pSendBuff++ = (IDSymbol>>8)%256+0x33;
              	for(ii=0;ii<(DataLen-2);ii++) 
		   		{
                   *pSendBuff++ = 0x33 + *DataPtr++; 	//数据内容
		  		}
		  }
		  
		  CCs=0;
		  for(ii=4;ii<(DataLen+14);ii++)
		  {
              CCs=CCs+*(SendBuff+ii);
		  }		  
		  *pSendBuff++ = CCs;
		  
		  *pSendBuff++ = 0x16;

		  return (DataLen+16);
}

//DL645-2007规约帧打包 ,返回打包后的帧长度，数据加0x33
uint8_t FramePack_645_2007(uint8_t *MeterNo,uint8_t Ctlw,uint32_t IDSymbol,uint8_t *DataPtr,uint8_t DataLen,uint8_t *SendBuff) 
{  
		  uint8_t 	ii;
		  uint8_t 	CCs;
		  uint8_t 	*pSendBuff;

		  pSendBuff = SendBuff;
		  
		  *pSendBuff++ = 0xFE;  				 		//引导字符
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;
		  *pSendBuff++ = 0xFE;
		  
		  *pSendBuff++ = 0x68;
		  memcpy((uint8_t *)&pSendBuff[0],(uint8_t *)&MeterNo[0],6);
		  pSendBuff+=6;
		  *pSendBuff++ = 0x68;
		  *pSendBuff++ = Ctlw;
		  *pSendBuff++ = DataLen; 						//数据长度
		  
		  if(DataLen>=4) 
		  {
				*pSendBuff++ = (IDSymbol%256)+0x33;  			//数据标识
				*pSendBuff++ = (IDSymbol>>8)%256+0x33;
				*pSendBuff++ = (IDSymbol>>16)%256+0x33;
				*pSendBuff++ = (IDSymbol>>24)%256+0x33;
              	for(ii=0;ii<(DataLen-4);ii++) 
		   		{
                   *pSendBuff++ = 0x33 + *DataPtr++; 	//数据内容
		  		}
		  }
		  
		  CCs=0;
		  for(ii=4;ii<(DataLen+14);ii++)
		  {
              CCs=CCs+*(SendBuff+ii);
		  }		  
		  *pSendBuff++ = CCs;
		  
		  *pSendBuff++ = 0x16;

		  return (DataLen+16);
}

//DL645-2007规约帧检测 ,通过返回帧首地址偏移量，并还原数据，失败返回MAXFRMLEN
uint16_t  FrameCheck_645_2007( uint8_t *RcvBuff ,uint8_t *MeterNo,uint32_t IDSymbol)
{
		uint32_t 	id;
		uint16_t  k,mk;
		uint8_t	Temp,CheckSum;
		
		for(mk=0;mk<MAXFRMLEN;mk++)								
		{	
			   if(mk>MAXFRMLEN-12)
			   		return MAXFRMLEN;									   
			   if(RcvBuff[mk]==0x68)							//0x68
			   {
//			   	   if(memcmp(RcvBuff+mk+1,MeterNo,6) != 0)
//				   		continue;
				   if(RcvBuff[mk+7] != 0x68)continue; 			//0x68
				   Temp = RcvBuff[8+mk] & 0x80;					//respone ?	
				   if(Temp == 0)continue; 						//ctl
				   Temp = RcvBuff[8+mk] & 0x40;					//error ?	
				   if(Temp != 0)continue; 						//ctl
				   
				   Temp = RcvBuff[8+mk] & 0x0f;
				   if(Temp == 0x01 || Temp == 0x02)				//respone read command
				   {
					   if(RcvBuff[mk+9] <= 4)continue;			//len

					   Temp = RcvBuff[9+mk]+10;					//cs data len			   
					   CheckSum = 0;
					   for(k=0;k<Temp;k++)
							   CheckSum += RcvBuff[mk+k];					   
					   if(CheckSum!=RcvBuff[mk+Temp])continue;
					   
					   if(RcvBuff[mk+Temp+1]!=0x16)continue; 

					   Temp = RcvBuff[9+mk];					//data len 
					   for(k=0;k<Temp;k++)							   
						   RcvBuff[DATAOFFSET+mk+k] = RcvBuff[DATAOFFSET+mk+k] - 0x33;		//还原数据
					   id = 0;
					   for(k=4;k>0;k--){
					   	  id <<= 8;
					   	  id += *(RcvBuff+mk+10+k-1);
					   }			
				   	   if(id != IDSymbol)
					   		continue;
				   }
				   else if(Temp == 0x04)						//respone write command
				   {
				   		 if(RcvBuff[mk+9] != 0)continue;		//result
				   		 Temp = 10;
				   		 CheckSum = 0;
					   	 for(k=0;k<Temp;k++)
							   CheckSum += RcvBuff[mk+k];
						 if(CheckSum!=RcvBuff[mk+Temp])continue;	
						 if(RcvBuff[mk+Temp+1]!=0x16)continue; 
				   }
				   break;
			   }											   
		}
		
		return mk;
}

//DL645-1997规约帧检测 ,通过返回帧首地址偏移量，并还原数据，失败返回MAXFRMLEN
uint16_t FrameCheck_645_1997( uint8_t *RcvBuff ,uint8_t *MeterNo,uint8_t *IDSymbol)
{
	uint16_t 	k,mk;
	uint8_t	Temp,CheckSum;
	
	for(mk=0;mk<MAXFRMLEN;mk++)								
	{
	   if(mk>MAXFRMLEN-12)
			return MAXFRMLEN;													   
	   if(RcvBuff[mk]==0x68)						//寻找帧头
	   {
//			if(memcmp(RcvBuff+mk+1,MeterNo,6) != 0)
//				continue; 
			if(RcvBuff[mk+7] != 0x68)continue; 		//分隔符
			if(RcvBuff[mk+8] & 0x80 == 0)continue; 	//方向正确
			if(RcvBuff[mk+8] & 0x40 == 0)continue; 	//应答正常
			if(RcvBuff[mk+9] < 3)continue;			//无有效数据
			
			Temp = RcvBuff[9+mk]+10;				//cs data len	
			CheckSum = 0;
			for(k=0;k<Temp;k++)
			   CheckSum += RcvBuff[mk+k];
			
			if(CheckSum!=RcvBuff[mk+Temp])
				continue;
			
			if(RcvBuff[mk+Temp+1]!=0x16)
				continue;

			Temp = RcvBuff[9+mk];					//data len 
			for(k=0;k<Temp;k++)
			    RcvBuff[DATAOFFSET+mk+k] = RcvBuff[DATAOFFSET+mk+k] - 0x33;			//还原数据

			if(memcmp(RcvBuff+mk+10,IDSymbol,2) != 0)
				continue; 
	   }											   
	}
	return mk;
}
