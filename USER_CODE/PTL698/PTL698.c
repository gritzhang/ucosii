/****************************************Copyright (c)****************************************************
**                            ShangHai FuDian Technology Co.,LTD.
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL698.h
** Latest modified Date:       2014-5-20
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 AndyLau
** Created date:               2014-5-20
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2014-5-20
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include 	"..\config.h"

Struct_UpProgram 	Struct_UpProgram_True;
GX_DAT_FRM 			Tmp_Frm_GX;							   	//数据缓存
DATE				CurDATA;								//当前日期
TIME				CurTIME;								//当前时间
/****************************************************************************
  * 名	       称：uint8 Is_Time_Value(uint8 *RxBuff)
  * 功           能：判断命令是否在有效时间内
  * 入口参数：
  * 出口参数：1: 有效    0:时间无效
************************************ ****************************************/
uint8 Is_Time_Value(uint8 *RxBuff) 
{
	int32 tmp;

	if(CurDATA.day>Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY==0)
		return 1;
	
	if(CurDATA.day>Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[3])
		;
	else
	{
		tmp = Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[2]*60*60+ Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[1]*60 + Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[0];
		if(tmp>CurTIME.hour*60*60+CurTIME.min*60+CurTIME.sec-Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY)
			return 1;
	}
	return 0;
}

void F_Access_JZQ_GX(uint8 * RxBuff,uint8 * RetBuff,uint8 Mode)
{

 	switch(Tmp_Frm_GX.gx_jzq_dat.AFN)
	{
	  	case 0x0F:
		{
			switch(Tmp_Frm_GX.gx_jzq_dat.DT)
			{
					case F1:		//
						
					break;
					
					case F2:		//
						
					break;
					
					case F6:																//文件升级
					{		
							Struct_UpProgram_True.LimitTime = Struct_UpProgram_True_No1.LimitTime;	//取超时时间
							
							Struct_UpProgram_True.ChuangShangNUM[0] = *(RxBuff+mVar);		//读取厂商编号
							mVar+=1;
							Struct_UpProgram_True.ChuangShangNUM[1] = *(RxBuff+mVar);
							mVar+=1;
							Struct_UpProgram_True.ChuangShangNUM_Ext = *(RxBuff+mVar);
							mVar+=1;
							Struct_UpProgram_True.WorkCode = *(RxBuff+mVar);
							mVar+=1;
							memcpy((uint8 *)&Struct_UpProgram_True.ComNUM[0],RxBuff+mVar,4);
							mVar+=4;
	
							switch(Struct_UpProgram_True.WorkCode)
							{
									case 0x01:	//建立远程升级会话												
										    if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
										    {											
										  		Struct_UpProgram_True.UpGX_Result_OKorNO=1;  					//厂商编号相同
										    	Struct_UpProgram_True.UpGX_Result_CODE=0x00;    				//结果码 返回正确应答
												memcpy((uint8 *)&Struct_UpProgram_True.Mima[0],RxBuff+mVar,3); 	//暂时不判密码
												mVar+=3;														
												Struct_UpProgram_True.UpVersion = *(RxBuff+mVar);				//升级版本(升级方式)
												mVar+=1;
												Struct_UpProgram_True.LimitTime =*(RxBuff+mVar);				//超时时间
												mVar+=1;
												UpGX_program_Sta = 1;											//进入升级第一步
									   		}
											else
											{
												Struct_UpProgram_True.UpGX_Result_OKorNO=0;
												Struct_UpProgram_True.UpGX_Result_CODE=0;	 				
											}										
//确认/否认应答													
mreplyy:										   						
										   nVar+=3;				 //厂商编号&厂商功能扩展码 3bytes										   
										   *(RetBuff+nVar)=0; 	 //操作码1bytes
										   nVar++;				 										  
										   nVar+=4;				 //会话号 4bytes										   
										   *(RetBuff+nVar)=Struct_UpProgram_True.WorkCode; //对应操作码
										   nVar++;	
										   if(Struct_UpProgram_True.UpGX_Result_OKorNO==0)
										   		Struct_UpProgram_True_No1.LimitTime = 0;	//出错退出超时任务                                                 
										   *(RetBuff+nVar)=(Struct_UpProgram_True.UpGX_Result_OKorNO<<6)+Struct_UpProgram_True.UpGX_Result_CODE; 	//  结果码 
										   nVar++;											
	
										   if( (*(RxBuff+nVar+gx_ofs_jzq_seq)&0x80)==0x80 )  
										   {  	//带时标												   
	                                             memcpy( (uint8 *)(RetBuff+nVar+gx_ofs_jzq_ResultCode+1),(uint8 *)(RxBuff+nVar+gx_ofs_jzq_ResultCode+6),6);   //时标按原样返回          
	                                             nVar+= 6;
												 MyTp=1;
										   }
										   else 
										   {   	//无时标											   
	                                             MyTp=0 ;  //  nVar+= (gx_ofs_jzq_ResultCode);												
										   }											   
										  
										   F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  //打包发送										   
									
									break;
	
									case 0x11:  //只用于接收11 工作码 	//本次通话主要是获取   取得目录  取得文件名 
										if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									    {
									    	if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
									    	{												    													
										  		Struct_UpProgram_True.UpGX_Result_OKorNO=1;  		//厂商编号相同
										    	Struct_UpProgram_True.UpGX_Result_CODE=0x00;    	//结果码 返回正确应答
	
												memcpy((uint8 *)&Struct_UpProgram_True.FileRoad[0],(uint8 *)(RxBuff+mVar),64);  // 取得目录
												mVar+=64;
											    memcpy((uint8 *)&Struct_UpProgram_True.FileName[0],(uint8 *)(RxBuff+mVar),32);  // 取得文件名
											    mVar+=32;
											    Struct_UpProgram_True.LastFileVersion = *((uint8 *)(RxBuff+mVar));					// 取得升级文件之版本号
//andy 2009.04.22													
												if(Struct_UpProgram_True_No1.LastFileVersion == 0xff)
												{
													if(Struct_UpProgram_True.LastFileVersion != 0)
													{
														Struct_UpProgram_True.UpGX_Result_OKorNO=0x0;					
														Struct_UpProgram_True.UpGX_Result_CODE=0x0D;				//文件版本低 			
													}
												}
												else if(Struct_UpProgram_True_No1.LastFileVersion == Struct_UpProgram_True.LastFileVersion )
												{
													if(Return_Update_Status()==2)	//升级成功或者文件不存在需以更高版本升级
													{
														Struct_UpProgram_True.UpGX_Result_OKorNO=0x0;					
														Struct_UpProgram_True.UpGX_Result_CODE=0x0D;					//文件版本低 				
													}
												}
												else if(Struct_UpProgram_True_No1.LastFileVersion > Struct_UpProgram_True.LastFileVersion )
												{
													
													Struct_UpProgram_True.UpGX_Result_OKorNO=0x0;					
													Struct_UpProgram_True.UpGX_Result_CODE=0x0D;					//文件版本低 				
												}													
//andy 2009.04.22
												UpGX_program_Sta = 2;									//进入升级第一步
									    	}
											else
											{
												Struct_UpProgram_True.UpGX_Result_OKorNO=0;
												Struct_UpProgram_True.UpGX_Result_CODE=3;	 			//会话号错			
											}	
								   		}
										else
										{
											Struct_UpProgram_True.UpGX_Result_OKorNO=0;					
											Struct_UpProgram_True.UpGX_Result_CODE=0;					//厂商代码错 				
										}	
	            					    goto mreplyy;
	
									break;
	
	
								case 0x13:
								//只用于接收13 工作码 
								//请求升级文件属性   回答14										   
									   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									   {
										   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
										   {													
												nVar+=3;				 	//厂商编号&厂商功能扩展码 3bytes
											   
												*(RetBuff+nVar)=0x14; 	 	//操作码1bytes
												nVar++;				 
												  
												nVar+=4;					//会话号 4bytes
	
												*(RetBuff+nVar)=Struct_UpProgram_True_No1.LastFileVersion;  //原文件版本
												nVar++;
												
	                                            memcpy((uint8 *)(RetBuff+nVar),(uint8 *)&Struct_UpProgram_True_No1.FileSize,4); 	//文件大小
												nVar+=4;	
												
												memcpy((uint8 *)(RetBuff+nVar),(uint8 *)&Struct_UpProgram_True_No1.LastUpFileSta[0],2);  //升级状态														
												nVar+=2;
												
												item = Struct_UpProgram_True_No1.LastTotalBlock%8;				//掩码字节数
												
												if(item==0)
													item = Struct_UpProgram_True_No1.LastTotalBlock/8;	
												else
													item = Struct_UpProgram_True_No1.LastTotalBlock/8 + 1;
												
												if(item>0 && item<128)
												{
													memcpy((uint8 *)(RetBuff+nVar),(uint8 *)&Struct_UpProgram_True_No1.LastUpMask[0],item );  //要初始化
													nVar+=item; 
												}
												
												if( (*(RxBuff+nVar+gx_ofs_jzq_seq)&0x80)==0x80 )  
												{  //带时标														   
													memcpy( (uint8 *)(RetBuff+nVar+gx_ofs_jzq_ResultCode+1),(uint8 *)(RxBuff+nVar+gx_ofs_jzq_ResultCode+6),6);   //时标按原样返回          
													nVar+= 6;
													MyTp=1;
												}
												else 
												{   //无时标
													MyTp=0 ;  //  nVar+= (gx_ofs_jzq_ResultCode);												
												}
	
												F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  //打包发送
												
	
											    UpGX_program_Sta = 3;								   //进入升级下一步
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;
											   Struct_UpProgram_True.UpGX_Result_CODE=3;			   //会话号错	
											   goto mreplyy;
										   }   
									   }
									   else
									   {
										   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
										   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //厂商代码错 		
										   goto mreplyy;
									   }  
						 		break;
	                                    												
								case 0x15: //只用于接收15 工作码  //设置块属性									   
	                                   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									   {
										   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
										   {	
										   		
												Struct_UpProgram_True_No1.bak[0] = 0;
										   		memcpy((uint8 *)&Struct_UpProgram_True_No1.FileSize,RxBuff+mVar,4);
												mVar+=4;
												//读取块总数														
												Struct_UpProgram_True_No1.LastTotalBlock=*(RxBuff+mVar+1)*256+*(RxBuff+mVar);														
												Struct_UpProgram_True_No1.LastTotalBlock&=0x3ffc;
												Struct_UpProgram_True_No1.LastTotalBlock>>=2;   	   // 块总数	
												if((*(RxBuff+mVar+1)>>6)>0)
												{	//全新传输,置新的升级状态
													Struct_UpProgram_True_No1.k = 0x4000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//升级状态:2	升级成功
													Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
													Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	
													memset((uint8 *)&Struct_UpProgram_True_No1.LastUpMask[0],0,128);
												}
												else
												{
													Struct_UpProgram_True_No1.k = 0x4000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//升级状态:2	升级成功
													Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
													Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	
												}
												mVar += 2;
												
												//设置文件版本
												Struct_UpProgram_True_No1.LastFileVersion = *(RxBuff+mVar);														
												mVar+=1;																							
	
												UpGX_program_Sta = 4;								   //进入升级下一步
												
												Struct_UpProgram_True.UpGX_Result_OKorNO=1;			   //正确
												Struct_UpProgram_True.UpGX_Result_CODE=0x00;			
												goto mreplyy;
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;
											   Struct_UpProgram_True.UpGX_Result_CODE=3;			   //会话号错	
											   goto mreplyy;
										   }   
									   }
									   else
									   {
										   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
										   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //厂商代码错 		
										   goto mreplyy;
									   }  
									   break;											 
	
								case 0x17: //只用于接收17 工作码 //接收升级数据包											  
								 
									   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									   {
										   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
										   {
												  Struct_UpProgram_True_No1.k = *(RxBuff+26) +  *(RxBuff+27) * 256; 	//数据大小&文件块编号
												  Struct_UpProgram_True_No1.k &= 0x0fff;		//当前块编号
												  Struct_UpProgram_True_No1.k += 1;				//当前帧序号+1

												  Struct_UpProgram_True.k  = *(RxBuff+29+254)+*(RxBuff+29+254+1)*256;											   
												  mTtemp = PSW_CRC_FILE((BYTE *)(RxBuff+29),1,Struct_UpProgram_True.k,254,1234);											   
												  if(mTtemp==0)
												  {
												   	   sprintf( (char *)ZhuanFa_Buff,"CRC ERR-%04d-%02x:%02x:%02x\n\r ",Struct_UpProgram_True_No1.k,BcdCurTime.hour,BcdCurTime.min,BcdCurTime.sec);																										  
													   ZhuanFa_Flag = 0x01;
													   ZhuanFa_Len = 32;
													   F_SendData_BY_CHB(ZhuanFa_Buff,ZhuanFa_Len);
												
													   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
													   Struct_UpProgram_True.UpGX_Result_CODE=0x0b;				   //CRC错	
													   goto mreplyy;
												  } 												
										  												  
												  Struct_UpProgram_True_No1.k = *(RxBuff+26) +  *(RxBuff+27) * 256; //数据大小&文件块编号
												  KaiNum = Struct_UpProgram_True_No1.k & 0x0fff; 												  
												  Struct_UpProgram_True_No1.k = *(RxBuff+27) +  *(RxBuff+28) * 256; //数据大小&文件块编号
												  item = Struct_UpProgram_True_No1.k/16;													  
												  Struct_UpProgram_True_No1.k = KaiNum;
												  if(item>256)	 													 // 块字节数据不能大于256byte			  
												  {														  		
//												  		sprintf((char *)ZhuanFa_Buff,"Frame E1!!! %04d-%02d:%02x:%02x:%02x\n\r ",Struct_UpProgram_True_No1.k,item,BcdCurTime.hour,BcdCurTime.min,BcdCurTime.sec);																	 
//														ZhuanFa_Flag = 0x01;
//														ZhuanFa_Len = 32;
//														F_SendData_BY_CHB(ZhuanFa_Buff,ZhuanFa_Len);							//CHB 级联	传输	
//														//OSTimeDly(OS_TICKS_PER_SEC*2);																
////														SysResetEN = 1;
														return;																
														item = 256;
												  }	
												  else
												  {
													    sprintf( (char *)ZhuanFa_Buff,"%04d-%02x:%02x:%02x\n\r ",Struct_UpProgram_True_No1.k+1,BcdCurTime.hour,BcdCurTime.min,BcdCurTime.sec);																										  
													    ZhuanFa_Flag = 0x01;
													    ZhuanFa_Len = 32;
													    F_SendData_BY_CHB(ZhuanFa_Buff,ZhuanFa_Len);	
												  }
												  
												  memcpy((uint8 *)&Struct_UpProgram_True_No1.Updata[Struct_UpProgram_True_No1.k][0],RxBuff+29,item); 	//数据
												  memcpy((uint8 *)&Struct_UpProgram_True_No1.CrC[0],RxBuff+29+item,2); 				//crc
	
												  Struct_UpProgram_True_No1.bak[0]++;		//实际接收块计数器
												  
												  item = KaiNum%8;
												  KaiNum = KaiNum/8;														  
												  item = 1<<item;
												  if(KaiNum>=128)
												  {
													    SysResetEN = 1;
													    return;
												  }
												  	
												  Struct_UpProgram_True_No1.LastUpMask[KaiNum] |= item; 				// 改变块MASK   
									 
												  Struct_UpProgram_True_No1.k = *(RxBuff+26) +  *(RxBuff+27) * 256; 	//数据大小&文件块编号
												  Struct_UpProgram_True_No1.k &= 0x0fff;		//当前块编号
												  Struct_UpProgram_True_No1.k += 1;				//当前帧序号+1
	
												  
	
												
												Struct_UpProgram_True_No1.bak[8]=0;
												for(i=0;i<Struct_UpProgram_True_No1.LastTotalBlock;i++)
												{
													j = i/8;
													k = 1<<(i%8);
													if((Struct_UpProgram_True_No1.LastUpMask[j] & k)==0)
														break;
												}																	
												if(i == Struct_UpProgram_True_No1.LastTotalBlock)
												{	
													Struct_UpProgram_True_No1.bak[8]=0xee;	//收到所有升级文件数据帧
												}
												else if(Struct_UpProgram_True_No1.k==Struct_UpProgram_True_No1.LastTotalBlock)
												{																
													Struct_UpProgram_True_No1.bak[8]=0xff;	//主站发送完毕，但未收全所有升级文件数据帧
												}
	
												UpGX_program_Sta=5;  //改变状态					
												return;		 
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;
											   Struct_UpProgram_True.UpGX_Result_CODE=3;			   //会话号错	
											   goto mreplyy;
										   }   
									   }
									   else
									   {
										   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
										   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //厂商代码错 		
										   goto mreplyy;
									   }  
									   break;	
	
										   		   
									case 0x18: ////只用于接收18工作码//关闭文件传输
										   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
										   {
											   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
											   {
											   		if(Struct_UpProgram_True_No1.bak[8]!=0xee)
											   		{
													   	Struct_UpProgram_True.UpGX_Result_OKorNO=0;
													   	Struct_UpProgram_True.UpGX_Result_CODE=5;			//05H该文件不能升级
	
														F_Save_update_File();								//保存整个升级文件及其参数
	
														SysResetEN = 1;
											
													   	goto mreplyy;
												    }

											   		UpGX_program_Sta=6;	 //改变状态															
											   		Struct_UpProgram_True.UpGX_Result_OKorNO=1;
												   	Struct_UpProgram_True.UpGX_Result_CODE=0;			   //正确
												   	goto mreplyy;
															
											   }
											   else
											   {
												   	Struct_UpProgram_True.UpGX_Result_OKorNO=0;
												   	Struct_UpProgram_True.UpGX_Result_CODE=3;			   //会话号错	
												   	goto mreplyy;
											   }   
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
											   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //厂商代码错 		
											   goto mreplyy;
										   }  
								   	break;	
									
									case 0x02: //只用于接收02 工作码 //关闭升级会话											     
										  if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
											{
												if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
												{
												 	   UpGX_program_Sta=0;  //改变状态	
												 
													   Struct_UpProgram_True.UpGX_Result_OKorNO=1;
													   Struct_UpProgram_True.UpGX_Result_CODE=0;				//正确 
													   nVar+=3;				 //厂商编号&厂商功能扩展码 3bytes
													   
													   *(RetBuff+nVar)=0; 	 //操作码1bytes
													   nVar++;				 
													  
													   nVar+=4;				 //会话号 4bytes
													   
													   *(RetBuff+nVar)=Struct_UpProgram_True.WorkCode; //对应操作码
													   nVar++;												   
													                                                        
													   *(RetBuff+nVar)=(Struct_UpProgram_True.UpGX_Result_OKorNO<<6)+Struct_UpProgram_True.UpGX_Result_CODE; 	//  结果码 
													   nVar++;											

													   if( (*(RxBuff+nVar+gx_ofs_jzq_seq)&0x80)==0x80 )  
													   {  	//带时标												   
		                                                     memcpy( (uint8 *)(RetBuff+nVar+gx_ofs_jzq_ResultCode+1),(uint8 *)(RxBuff+nVar+gx_ofs_jzq_ResultCode+6),6);   //时标按原样返回          
		                                                     nVar+= 6;
															 MyTp=1;
													   }
													   else 
													   {   	//无时标											   
		                                                     MyTp=0 ;  //  nVar+= (gx_ofs_jzq_ResultCode);												
													   }
													   //F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  //打包发送	

													   if(Struct_UpProgram_True_No1.bak[8]==0xff)
													   {
													   		Struct_UpProgram_True.UpGX_Result_OKorNO=0;				//结果:失败
													   		Struct_UpProgram_True.UpGX_Result_CODE=5;				//原因:文件无法升级
													   		F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  		//打包发送	
															
													   		
															//接收完毕但有帧漏掉，升级状态置0:失败
															//升级状态-- 上次升级结果 0:传输失败/2:成功/1:正在传/3:文件不存在
															Struct_UpProgram_True_No1.k = 0x4000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//升级状态:2	升级成功
															Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
															Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	
												
														    F_Save_update_File();		//保存整个升级文件及其参数
													   }
													   else if(Struct_UpProgram_True_No1.bak[8]==0xee)
													   {		
															F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  			//打包发送

															//接收到所有数据帧，升级状态置2:成功
															//升级状态-- 上次升级结果 2:成功/0:传输失败/1:正在传/3:文件不存在
															Struct_UpProgram_True_No1.k = 0x8000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//升级状态:2	升级成功
															Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
															Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	

															F_Save_Para_update_File();	//保存升级文件参数
															
														    F_Update();					//执行升级任务
													   }

													   return;		 
												}
												else
												{
													 Struct_UpProgram_True.UpGX_Result_OKorNO=0;
													 Struct_UpProgram_True.UpGX_Result_CODE=3;				//会话号错	 
													 goto mreplyy;
												}	
											}
											else
											{
												Struct_UpProgram_True.UpGX_Result_OKorNO=0; 				
												Struct_UpProgram_True.UpGX_Result_CODE=0;					//厂商代码错		 
												goto mreplyy;
											}  
									 break; 
									 
									 default:
									 break;
								}
					}
					break;
					default:
					break;
			}
		}
		break;
		default:
		break;
	}
}

void  Attand_ZhuZhan_HB_Command(uint8 *RxBuff,uint8 *RetBuff,uint8 Mode)
{	  
	  //int32     tmp;
	  uint8		ret;  
	  
	  Tmp_Frm_GX.gx_dat_len = *(RxBuff+gx_ofs_jzq_len)/4 + *(RxBuff+gx_ofs_jzq_len+1)*256/4;
	  Tmp_Frm_GX.gx_frm_ctl = *(RxBuff+gx_ofs_jzq_ctl);
	  
	  Tmp_Frm_GX.gx_jzq_adr.A1 = *(RxBuff+gx_ofs_jzq_addr)+*(RxBuff+gx_ofs_jzq_addr+1)*256;
	  Tmp_Frm_GX.gx_jzq_adr.A2 = *(RxBuff+gx_ofs_jzq_addr+2)+*(RxBuff+gx_ofs_jzq_addr+3)*256;
	  Tmp_Frm_GX.gx_jzq_adr.A3 = *(RxBuff+gx_ofs_jzq_addr+4);	  
	  Tmp_Frm_GX.gx_jzq_dat.AFN = *(RxBuff+gx_ofs_jzq_afn);
	  Tmp_Frm_GX.gx_jzq_dat.SEQ = *(RxBuff+gx_ofs_jzq_seq);
	  Tmp_Frm_GX.gx_jzq_dat.DA = *(RxBuff+gx_ofs_jzq_dat)+*(RxBuff+gx_ofs_jzq_dat+1)*256;
	  Tmp_Frm_GX.gx_jzq_dat.DT = *(RxBuff+gx_ofs_jzq_dat+2)+*(RxBuff+gx_ofs_jzq_dat+3)*256;
	  Tmp_Frm_GX.gx_jzq_dat.PW	= *(RxBuff+Tmp_Frm_GX.gx_dat_len-1)*256+*(RxBuff+Tmp_Frm_GX.gx_dat_len-2);
	  Tmp_Frm_GX.gx_jzq_dat.EC	= *(RxBuff+Tmp_Frm_GX.gx_dat_len-1)*256+*(RxBuff+Tmp_Frm_GX.gx_dat_len-2);
	  ret = Tmp_Frm_GX.gx_jzq_dat.SEQ & 0x80;	  
	  if(ret)
	  {
	  		Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.PFC = *(RxBuff+Tmp_Frm_GX.gx_dat_len);
			memcpy(Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM,RxBuff+Tmp_Frm_GX.gx_dat_len+1,4);
	  		Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY = *(RxBuff+Tmp_Frm_GX.gx_dat_len+ 5);
			if(Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY != 0)
			{
				ret = Is_Time_Value(RxBuff);
		 		if(ret==0)
		 			return;	
			}
	  }	

	  F_Access_JZQ_GX(RxBuff,RetBuff,Mode);


	  return;	  

}

/*********************************************************************************************************
**											END OF FILE												   	**
**********************************************************************************************************/
