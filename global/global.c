

#define	__GLOBAL_C__

#include	"global.h"



//全局变量初始化
void GolbalVarInit(void)
{
//	OS_ENTER_CRITICAL();	
//	memset(&SysCtlWrod,0,sizeof(sSysCtl));
//	memset(Chgcard,0,sizeof(Chgcard));
//	memset((uint8_t *)&energy_data,0,sizeof(energy_data));
//	memset((uint8_t *)&real_power,0,sizeof(real_power));
//	memset((uint8_t *)&sSysOvrTim,0,sizeof(sSysOvrTim));
//	memset((uint8_t *)&Term_Para1,0,sizeof(Term_Para1));
//	OS_EXIT_CRITICAL();
}

//读取终端参数及充电参数
void SystemParaInit(void)
{
	uint8_t cs;	
	//static uint32_t nn ;
	
	//nn = sizeof(Term_Para1) ;
	//nn = sizeof(sSysCtl);
	
//	ReadJYLSH();  													    //读取交易流水号
//	Readlxjycs();											            //读取交易记录参数	
//				
	FRAMRead((uint8_t *)&Term_Para1, TERMINAL_PARA_ADD, sizeof(Term_Para1));
//	cs	= CheckSum1((uint8_t *)&Term_Para1, &Term_Para1.cs);
//	if(cs != Term_Para1.cs){
//		FRAMRead((uint8_t *)&Term_Para1, TERMINAL_PARA_ADD_BAK, sizeof(Term_Para1));
//		cs	= CheckSum1((uint8_t *)&Term_Para1, &Term_Para1.cs);
//		if(cs != Term_Para1.cs){
//			Term_Para1.ParaFlag = 0;
//		}
//		else{			
//			FRAMWrite((uint8_t *)&Term_Para1, TERMINAL_PARA_ADD, sizeof(Term_Para1));
//		}
//	}	
	if(Term_Para1.ParaFlag != VALID_PARA)
    { 						        /* 复位参数								*/
		memset(&Term_Para1, 0x00 , sizeof(Term_Para1)) ;
		Term_Para1.ChargeNo[0]       	    = 0x01 ;					/* 充电桩编号		0000000000000001	*/
		Term_Para1.TerminalNo[0]	  	    = 0x01	;					/* 充电桩设备编号	0000000000000001	*/
		Term_Para1.CHGMode		 			= MODE_RJ45 ;			    /* 上行通道：GPRS/WIFI/RJ45/RS485		*/
		Term_Para1.CHGProtocol				= 0x01 ;					/* 充电桩协议 01 国网信通104  			*/

		Term_Para1.ServerIP[0]  			= 100;						/* 主站IP*/	
		Term_Para1.ServerIP[1]  			= 1;			
		Term_Para1.ServerIP[2]  			= 168;			
		Term_Para1.ServerIP[3]  			= 192;
		Term_Para1.ServerPort   			= 2406 ;					/* 主站端口*/	

		Term_Para1.GateWay[0]  				= 1;						/* 充电桩网关 */	
		Term_Para1.GateWay[1]  				= 1;			
		Term_Para1.GateWay[2]  				= 168;			
		Term_Para1.GateWay[3]  				= 192;			

		Term_Para1.SelfIP[0]  				= 185;						/* 充电桩IP*/	
		Term_Para1.SelfIP[1]  				= 1;			
		Term_Para1.SelfIP[2]  				= 168;			
		Term_Para1.SelfIP[3]  				= 192;		
		Term_Para1.SelfPort   				= 9000 ;					/* 充电桩端口*/		

		Term_Para1.SelfMASK[0]  			= 0 ;						/* 子网掩码 */
		Term_Para1.SelfMASK[1]  			= 255 ;			
		Term_Para1.SelfMASK[2]  			= 255 ;			
		Term_Para1.SelfMASK[3]  			= 255 ;			

		Term_Para1.SelfMAC[0]  				= 0x00 ;					/* 充电桩MAC地址 */	
		Term_Para1.SelfMAC[1]  				= 0x00 ;			
		Term_Para1.SelfMAC[2]  				= 0x10 ;			
		Term_Para1.SelfMAC[3]  				= 0x00 ;			
		Term_Para1.SelfMAC[4]  				= 0x00 ;			
		Term_Para1.SelfMAC[5]  				= 0x00 ;			
	
		Term_Para1.RatedVoltage 	 		= 0x2200 ;                  /* 额定电压 */
		Term_Para1.RatedCurrent 	 		= 0x3200 ;                  /* 额定电流 */
		
		Term_Para1.Overload 		 		= 0x0110 ;				    /* 过流 1.10 过流恢复 1.00 */
		Term_Para1.OverloadRecovery 	    = 0x0105 ;

		Term_Para1.Overvoltage 		 		= 0x0110 ;				    /* 过压 1.10 过压恢复 1.05 */
		Term_Para1.OvervoltagRecovery       = 0x0105 ;

		Term_Para1.Undervoltage 		 	= 0x0090 ;				    /* 欠压 0.8  欠压恢复 0.95 */
		Term_Para1.UndervoltagRecovery      = 0x0095 ;

		Term_Para1.MinCurrent 		 		= 0x0200 ;				    /* 最小电流 5% 1.6A 单相最大电流 36 A */
		Term_Para1.MaxCurrent 				= 0x9900 ;

		Term_Para1.AdminPassword 			= 0x12345611 ;		        /* 管理员密码 */
		Term_Para1.JudgeTime 				= 0x01 ;				    /* 过压过流欠压判定时间 */	
		Term_Para1.HeartBeat 		 		= 0x01 ;				    /* 心跳周期*/
		Term_Para1.ComTimeOut 		 		= 0x01 ;					/* 通讯超时时间*/
		Term_Para1.RepeatTime 		 		= 0x03 ;					/* 重发次数 */
		Term_Para1.LedStart 		 		= 0x1800 ;				    /* LED 开启时间 */
		Term_Para1.LedStop 			 		= 0x0600 ;				    /* LED 关闭时间 */
		Term_Para1.SoftVer 		 			= 0x1208 ;				    /* PCB 丝印日期*/
		Term_Para1.HardwareVer 				= 0x2014 ;	
		Term_Para1.DutyCycle                = 0x53;						/* 占空比	  */
		 
		memcpy((uint8_t *)&Term_Para1.APN,"CMNET",sizeof("CMNET"));
		memcpy((uint8_t *)&Term_Para1.WifiParas,"at+wifi_conf=ANDY,auto,wifimima\r\n",sizeof("at+wifi_conf=ANDY,auto,wifimima\r\n"));
		Term_Para1.ParaFlag           = VALID_PARA;	
		
//		Term_Para1.cs	= CheckSum1((uint8_t *)&Term_Para1, &Term_Para1.cs);
//		FRAMWrite((uint8_t *)&Term_Para1, TERMINAL_PARA_ADD, sizeof(Term_Para1));
//		FRAMWrite((uint8_t *)&Term_Para1, TERMINAL_PARA_ADD_BAK, sizeof(Term_Para1));
	}
#if LOCAL_DEBUG    
    
                      //debug
//                    Term_Para1.ServerIP[0]  			= 100;						/* 主站IP*/	
//                    Term_Para1.ServerIP[1]  			= 1;			
//                    Term_Para1.ServerIP[2]  			= 168;			
//                    Term_Para1.ServerIP[3]  			= 192;  
 
                    Term_Para1.ServerIP[0]  			= 40;						/* 主站IP*/	
                    Term_Para1.ServerIP[1]  			= 185;			
                    Term_Para1.ServerIP[2]  			= 93;			
                    Term_Para1.ServerIP[3]  			= 47;  
    
//                    Term_Para1.SelfIP[0]  				= 185;						/* 充电桩IP*/	
//                    Term_Para1.SelfIP[1]  				= 1;			
//                    Term_Para1.SelfIP[2]  				= 168;			
//                    Term_Para1.SelfIP[3]  				= 192;	    
#endif    
    
    Term_Para1.ServerPort   			= 2406 ;
    
//	FRAMRead((uint8_t *)&SysCtlWrod, CHGPARA_ADD, sizeof(SysCtlWrod));
//	cs	= CheckSum1((uint8_t *)&SysCtlWrod, &SysCtlWrod.cs);
//	if(cs != SysCtlWrod.cs)
//	{
//		FRAMRead((uint8_t *)&SysCtlWrod, CHGPARA_ADD_BAK, sizeof(SysCtlWrod));
//		cs	= CheckSum1((uint8_t *)&SysCtlWrod, &SysCtlWrod.cs) ;
//		if(cs != SysCtlWrod.cs){
//			SysCtlWrod.ParaFlag = 0 ; 
//		}
//		else{
//			FRAMWrite((uint8_t *)&SysCtlWrod, CHGPARA_ADD, sizeof(SysCtlWrod));
//		}
//	}	
//	if(SysCtlWrod.ParaFlag != VALID_PARA)
//	{		
//		memset(&SysCtlWrod , 0x00 , sizeof(SysCtlWrod)) ;	
//		SysCtlWrod.ParaFlag = VALID_PARA;
//		SysCtlWrod.cs	= CheckSum1((uint8_t *)&SysCtlWrod, &SysCtlWrod.cs);
//		FRAMWrite((uint8_t *)&SysCtlWrod, CHGPARA_ADD, sizeof(SysCtlWrod));
//		FRAMWrite((uint8_t *)&SysCtlWrod, CHGPARA_ADD_BAK, sizeof(SysCtlWrod));
//	} 
//	
	
    FRAMRead(MD5, MD5_ADD ,16);
    FRAMRead(&RecodeNum, CHARGE_STRATEGY_RECODE_NUM_ADD ,1);
    FRAMRead(Strategy, CHARGE_STRATEGY_ADD , 24*4);

//	
//	if(SysCtlWrod.card1pwdok!=1 || SysCtlWrod.status != WORK3){
//	//if(SysCtlWrod.card1pwdok!=1 || SysCtlWrod.status == WORK11 ||SysCtlWrod.status == WORK8){
//		dspwelcom();								  					//待机界面	
//	}			
}



void FillQrCode(char * QrCode)
{
	uint8_t  i;
	uint8_t	dspbuf[20];
    uint8_t	datbuf[8] = {0};
    
    memcpy(datbuf,Term_Para1.ChargeNo,8);
    bcdtwo2bcdone(datbuf,dspbuf,8);
    for(i=0;i<16;i++)
        QrCode[38+i] = dspbuf[i] + '0';
}

#undef	__GLOBAL_C__

