/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           ethernet.c
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
#include "ethernet.h"

//uint8_t Src_IP_Add[4] =         {192,168,1,10};
//uint8_t Subnet_Mask_Add[4] =    {255,255,255,0};
//uint8_t Gateway_Add[4] =        {192,168,1,1};//路由器ip211.157.135.58

//uint8_t Src_MAC_Add[6] =        {0,0,0,5,122,0};
//uint8_t Sn_DIP_Add[4] =         {192,168,1,14};
//uint8_t Sn_Port[2] =            {0x09,0x66};//2406

//连接测试服务器
uint8_t Src_IP_Add[4] =         {192,168,1,140};
uint8_t Subnet_Mask_Add[4] =    {255,255,255,0};
uint8_t Gateway_Add[4] =        {192,168,1,1};

uint8_t Src_MAC_Add[6] =        {0,0,0,5,122,0};
uint8_t Sn_DIP_Add[4] =         {47,93,185,40};
uint8_t Sn_Port[2] =            {0x09,0x66};//2406

uint8_t RX_Buffer[RX_Size];
uint8_t TX_Buffer[TX_Size];        

uint8_t PTL104_State;           //bit7 login bit6 activation bit5 time bit4 JF_strategy 
                                //bit3 Tx  bit2 Rx 

uint8_t Log_In_Frame[25] = {0x68,0x01,0x02,0x84,0x01,0x01,0x10,0x11,0x11,0x22,
                            0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0xae,0x21,0x02,0x17,};                       
  
                            
/*****************************************************************
程序名: W5100_Initialization
输入: 无
输出: 无
返回: 无
说明：先对W5100初始化，然后检查网关，最后分别初始化4个端口
*****************************************************************/
void W5100_Initialization(void)
{
//	S0_State = 0 ; 
//	W5100_Load_Parameters() ; 	// 载入以太网参数
//	W5100_Init();				// 初始化以太网参数
//	OSTimeDly(10) ; 
//	Detect_Gateway();			// 检查网关服务器 
//	OSTimeDly(10) ; 
//	Socket_Init(0);				// 端口0 
//	OSTimeDly(10) ; 
}
                            
void Ethernet_Init(void)
{
	if(Term_Para1.CHGMode == MODE_GPRS)
    {
//		GPRSPWReset();
    }
	if(Term_Para1.CHGMode == MODE_WIFI)
    {
//		wifienterATmode();
    }
	if(Term_Para1.CHGMode == MODE_4G)
    {
//		Module4GInit();
    }
	if(Term_Para1.CHGMode == MODE_RJ45)   
    {   
        PTL104_State = 0;
        iinchip_init();
    }
}    
  
uint8_t Ethernet_config(void)
{
    uint8_t read_buffer[8];
    OSTimeDly(OS_TICKS_PER_SEC/10);
    setGAR(Term_Para1.GateWay); 
    getGAR(read_buffer);
    
    setSUBR(Term_Para1.SelfMASK);
    setSHAR(Term_Para1.SelfMAC);
    setSIPR(Term_Para1.SelfIP);
    getSIPR(read_buffer);
    
    sysinit(0x55,0x55);
    
    setRTR(0xd007);
    setRCR(0x03);
    setIMR(0xcf);
    
    OSTimeDly(OS_TICKS_PER_SEC/20);;
    
    if(!socket(0,Sn_MR_TCP,Term_Para1.SelfPort,0x20))//(5369  0x14f9 ) (4658,0x1232)
    {
//        GPIO_OutputValue(LED_PORT,LED_PIN,0); 
    }
    setSn_DIPR(0,Term_Para1.ServerIP);
}


uint8_t			Ethernet_ReCnc_Ctr= 0;      //网络重连计数

void Ethrenet_Connecting(uint8_t Value)
{
    uint8_t SN_SR;
    SN_SR = getSn_SR(0);
    if(Value&Sn_IR_CON)												/* 在TCP模式下,Socket0成功连接 */
    {
        if(SN_SR == SOCK_ESTABLISHED){                
            Ethernet_State = ETHERNET_CONNECT_OK; 
            Ethernet_ReCnc_Ctr = 0;
            PTL104_State = 0;
            time_flg &= 0x80;
            putISR(0,Value&(~Sn_IR_CON));
        }
        else
            Ethernet_State = ETHERNET_CONNECTING;
        return;
    }
    if(Value&Sn_IR_TIMEOUT)
    {
        Ethernet_ReCnc_Ctr++;
        if(Ethernet_ReCnc_Ctr == 2){
             Ethernet_State = ETHERNET_FAILURE;
        } 
        putISR(0,Value&(~Sn_IR_TIMEOUT));
    }
    if(Value&Sn_IR_DISCON)
    {
        putISR(0,Value&(~Sn_IR_DISCON));
    }
    switch(SN_SR)
    {
        case SOCK_CLOSED:
            connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort);
            break;
        case SOCK_CLOSE_WAIT:
            close(0);
            break;        
    }
    return;
}

//保存待确认帧  充电桩发送的 需要服务器确认的 帧一段时间未收到确认 则重新发送该帧
uint8_t Save_NoAck_Frame(uint8_t const *sbuf,uint8_t len)
{
    
}

//删除已确认帧   确认以后删除保存的该帧
void Del_Ack_Frame(uint16_t Txsn)
{
    
}


//发送登录帧
uint8_t Sent_Login_Frame(uint8_t *pNo)
{
    uint8_t len,sBuf[30];
    len = F104_LOGIN_PACK(sBuf,pNo,0);
//    Save_NoAck_Frame(sBuf,len);
    return send(0,sBuf,len);
}

//发送心跳帧
void Sent_Heart_Frame(void)
{
    uint8_t FRM_HEART[]   = {0x68,0x04,0x43,0x00,0x00,0x00};
//    Save_NoAck_Frame(FRM_HEART,6);
    send(0,FRM_HEART,6);
}

//发送S确认帧
void Sent_S_Ack(uint16_t Txsn)
{
    uint8_t FRM_S_Ack[]   = {0x68,0x04,0x01,0x00,0x00,0x00};
    FRM_S_Ack[4] = Txsn%256;
    FRM_S_Ack[5] = Txsn/256;
    send(0,FRM_S_Ack,6);
}


//uint8_t FRM_ACTIVE[]  = {0x68,0x04,0x07,0x00,0x00,0x00};
//发送激活确认帧   FRM_RACTIVE
void Sent_RACTIVE(void)
{
    uint8_t FRM_RACTIVE[] = {0x68,0x04,0x0B,0x00,0x00,0x00};
    send(0,FRM_RACTIVE,6);
}

//发送时间同步确认
void Sent_Rsettime(unsigned short Rxsn,unsigned short Sdsn)
{
    uint8_t	  len ,dbuf[32];
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson = 0x07;
    sPTL_104_I_SND.Sdsn = Rxsn;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);//发送总召确认帧	
}

//发送事件状态上报帧
void Sent_Event_Frame(void)
{
    
}

//发送总召唤确认
void R_Call_Ack(unsigned short Rxsn,unsigned short Sdsn)
{
    uint8_t	  len ,dbuf[32];
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson = 0x07;
    sPTL_104_I_SND.Sdsn = Rxsn;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);//发送总召确认帧											
//  F104_ACK_SAVE(dbuf,len);    
}

void Charge_State_Update(void)
{
    memcpy(sPTL_104_I_100.CDZBH,Term_Para1.ChargeNo,8);						//终端机器编码（即充电桩编号）

    switch(Station_State)
    {
        case STATION_SETUP:                     
        case STATION_Idle:                  //等待扫码充电 显示二维码      
        case STATION_Connect:               //已连接充电枪等待充电鉴权  
        case STATION_Authtic:               //已签权等待充电枪连接
            sPTL_104_I_100.GZZT = 0x02;
            break;             
        case STATION_Charge:                //充电中 显示充电情况信息
            sPTL_104_I_100.GZZT = 0x03;
            break;        
        case STATION_ChargeEND:             //充电结束 显示扣款及充电订单结算信息            
            sPTL_104_I_100.GZZT = 0x04;
            break;            
        default : 
            sPTL_104_I_100.GZZT = 0x01;
            break;
    }

    switch(Charge_Flag&0x60)
    {
        case 0x0:
            sPTL_104_I_100.LJQRKGZT = 0x00;
            break;
        case 0x40:
            break;
        case 0x20:
            sPTL_104_I_100.LJQRKGZT = 0x01;
            break;
        case 0x60:
            sPTL_104_I_100.LJQRKGZT = 0x02;
            break;
        default:
            break;
    }   
    if(PWRPINSTS )//读状态 上拉电阻 高电平继电器断开
        sPTL_104_I_100.SCJDQZT = 0x00;
    else
        sPTL_104_I_100.SCJDQZT = 0x01;
}

//发送充电桩状态
void Charge_State_Ack(unsigned short const Rxsn,unsigned short const Sdsn)
{
    uint8_t	  len ,dbuf[50];    
    sPTL_104_I_SND.Type = 0x86;
    sPTL_104_I_SND.Rson = 0x00;

    Charge_State_Update();//充电桩信息更新
    len = F104_ZZHSJ_PACK(&sPTL_104_I_SND);
    sPTL_104_I_SND.Flen = 10+len;
    sPTL_104_I_SND.Sdsn = Rxsn+2;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);							
    send(0,dbuf,len);	   												//发送总召数据帧
//  F104_ACK_SAVE(dbuf,len);
}

//发送总召唤结束
void R_Call_Over(unsigned short const Rxsn,unsigned short const Sdsn)
{
    uint8_t	  len ,dbuf[32];      
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson = 0x0a;
    sPTL_104_I_SND.Sdsn = Rxsn+4;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);													//发送总召结束帧
//  F104_ACK_SAVE(dbuf,len);	
//  sPTL_104_I_SND.Sdsn += 2;
}

//回复服务器总召唤 此处保存发送接收序列号，等待相应的s确认帧 确认，如果等待超时，响应的帧重发
void Answer_R_Call(unsigned short const Rxsn,unsigned short const Sdsn)
{
    R_Call_Ack(Rxsn,Sdsn);//发送总召唤确认帧
    Charge_State_Ack(Rxsn,Sdsn);//发送充电桩状态
    R_Call_Over(Rxsn,Sdsn);//发送总召唤结束      
}

//更新系统时间
void Set_Sys_Time(void )
{  	
	RTCtimedate.second = (sPTL_104_I_RCV.Dobj[1]+sPTL_104_I_RCV.Dobj[2]*256)/1000;
    RTCtimedate.minute 	= sPTL_104_I_RCV.Dobj[3];
	RTCtimedate.hour	= sPTL_104_I_RCV.Dobj[4];
	RTCtimedate.day		= sPTL_104_I_RCV.Dobj[5]&0x1f;
	RTCtimedate.week	= sPTL_104_I_RCV.Dobj[5]>>5;
	RTCtimedate.month	= sPTL_104_I_RCV.Dobj[6]&0x0f;
	RTCtimedate.year	= sPTL_104_I_RCV.Dobj[7]+2000;

	rtcInit(&RTCtimedate);		//配置内部RTC
	ExRTCSet(&RTCtimedate);		//配置外部RTC
}


//MD5_ADD                
//MD5_BAK_ADD            
//CHARGE_STRATEGY_ADD    
//CHARGE_STRATEGY_BAK_ADD
//保存计费策略
void Save_Charge_Strategy(uint16_t len)
{ 
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3],MD5_ADD,16);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+16],CHARGE_STRATEGY_RECODE_NUM_ADD,1);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+17],CHARGE_STRATEGY_ADD,96);   

//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3],MD5_BAK_ADD,16);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+16],CHARGE_STRATEGY_RECODE_NUM_BAK_ADD,1);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+17],CHARGE_STRATEGY_BAK_ADD,96);     
}    



uint8_t Authtic_Success[120];
//鉴权帧处理  保存交易流水号 保存鉴权信息 到全局鉴权数据中
void Authtic_Deal_With(uint16_t len)
{
    if(len == 0x66)
        PTL104_JQXXWithOrder_unpack(&sPTL_104_I_RCV);		//鉴权响应解析	长度为0x66        
    else if(len == 0x76)
        PTL104_JQXXWithOrderAndLogicNumber_unpack(&sPTL_104_I_RCV);   //鉴权响应解析 长度为0x76
    if(DspJQFHerr(sPTL_104_I_DW_JQSJ.JQSBYY)==0)        //鉴权成功 
    {
        Wait_Gun_Insert_Seconds = Time_Seconds;
        Charge_Flag |= CHANGE_AUTHTIC_BIT;

        if(GUNPINSTS)       //枪拔下来为 0 枪在上面打开锁
        {
            P3P0OFF();          //打开枪锁
            P3P1ON();
            OSTimeDly(OS_TICKS_PER_SEC);
            P3P1OFF();   
        }        
        
        memcpy(Authtic_Success,&sPTL_104_I_RCV.Dobj[0],len-12);
    }
}

//发送遥控确认帧  unsigned short Rxsn,unsigned short Sdsn
void SendRemoteCtrlAsk(unsigned short const Rxsn,unsigned short const Sdsn)
{
    uint8_t	  len ,dbuf[32];      
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson =   0x07;
    sPTL_104_I_SND.Sdsn = Rxsn;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    sPTL_104_I_SND.Apdu += 1;
    sPTL_104_I_SND.Dobj[6] = 0x01;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);   
}


//遥控停止帧处理
void RemoteCtrlFrameDeal(unsigned short const Rxsn,unsigned short const Sdsn)
{
    //停止充电 发送 结算订单  上传计费策略充电情况 
    SendRemoteCtrlAsk(Rxsn,Sdsn);    //发送遥控确认帧
    
    pwmDb1Init(HIVCNT);
    P1P16OFF();         //220继电器输出控制  关闭充电枪220v输出
    SET_CHG_JSSJ();     //设置充电结束时间
    Charge_Flag |= CHANGE_END_Ctrl_BIT;         //APP充电结束             
    Charge_Flag &= ~CHANGE_AUTHTIC_BIT;         //签权位清0
    Station_State = STATION_ChargeEND;          //更改充电桩状态为充电结束状态
    DisplayWaitTimeSeconds = Time_Seconds;

}

void PayForDile(uint8_t len)
{
    PTL104_KKXX_With_LogicNumber_unpack(&sPTL_104_I_RCV);
}

//下行数据解析 分析接收的数据
void PTL104DataAnalysis(uint8_t const * rx_data,uint16_t len)
{
	uint8_t data = 0,frame_len = 0,minus_len  = 0;
    
    data = *rx_data;
    if(data == 0x68)	//判断是否为帧头
    {
        uint16_t sent_temp,rece_temp;
        sent_temp = *(rx_data+2)+*(rx_data+3)*256;
        rece_temp = *(rx_data+4)+*(rx_data+5)*256;
        
        memcpy(&sPTL_104_I_RCV,rx_data,len);//memcpy(&sPTL_104_I_RCV,rx_data,sizeof(sPTL_104_I_RCV));
        frame_len = *(rx_data+1);
        minus_len = frame_len+2;    //当前帧长度
        switch(frame_len){
            case 0x01://为登录返回帧
                PTL104_State |= PTL104_LOGIN;
                PTL104_State &= ~PTL104_LOGIN_SENT;
                minus_len = 25;
                break;
            case 0x04://激活帧,心跳确认帧，S确认帧 
                data = *(rx_data+2);
                if(data == 0x07){//激活帧 回复激活确认
                    Sent_RACTIVE();
                    PTL104_State |= PTL104_ACTV;
                }
                else if(data == 0x83){//心跳确认帧 
                    time_flg &= ~PTL104_HEART_WAIT_ACK;
                }
                else if(data == 0x01){//S确认帧 
//                    Del_Ack_Frame(*(rx_data+2)+*(rx_data+3)*256-2);
                }
                break;
            case 0x0e://为总召唤 或 遥控帧              
                data = *(rx_data+6);
                if(data == 0x64){//为总召唤 
                    Answer_R_Call(sent_temp,rece_temp);
                }
                else if(data == 0x45){//为遥控帧 处理  
                    RemoteCtrlFrameDeal(sent_temp,rece_temp);
                }
                break; 
            case 0x12://为时间同步 
                data = *(rx_data+6);
                if(data == 0x67)                //为时间同步 
                {
                    Set_Sys_Time();//同步时间 设置时间
                    PTL104_State |= PTL104_SETTIME;
                    Sent_Rsettime(sent_temp,rece_temp);     //发送时间同步确认帧 
                }
                break;
            case 0x4e://为扣款            
                PayForDile(frame_len);//显示扣款信息
                break;            
            case 0x72://二维码下发                               
                break;            
            case 0x7e://为计费策略   回复S确认帧    
                Save_Charge_Strategy(len);//保存计费策略
                PTL104_State |= PTL104_JF_STRGY;
                Sent_S_Ack(sent_temp+2);
                break;
            case 0x56:
            case 0x66:
            case 0x76://为鉴权返回  回复S确认帧     0x66
                Authtic_Deal_With(frame_len);                        //根据鉴权信息打开充电桩充电 或者 不准许充电
                Sent_S_Ack(*(rx_data+2)+*(rx_data+3)*256+2);
                break;               
            default: 
                break;	
        }
        if(len > minus_len)
        {
            PTL104DataAnalysis(rx_data+minus_len,len - minus_len);
        }
    }
}

extern CHASTRATEGY_UP_PARA ChargeStrategyUP;
void Sent_StrateChargeInfo_Frame(sPtl_104_i * pPtl104)
{
	uint8_t len;
	uint8_t sBuf[BUFSIZE];
	pPtl104->Type = 0x79;
	pPtl104->Lmtd = 0x01;
	pPtl104->Rson = 0x02;
	pPtl104->Apdu = 0x00;
	SET_JFCLSJ(&ChargeStrategyUP);
	len = F104_JFCLCDQK_UP_PACK(pPtl104);
	pPtl104->Flen = 10+len;
	len = F104_INF_PACK(sBuf,pPtl104);
	send(0,sBuf,len);
//	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;	    
}

void Sent_OnlineChargeInfo_Frame(sPtl_104_i * pPtl104)
{
	uint8_t len;
	uint8_t sBuf[BUFSIZE];
	pPtl104->Type = 0x82;
	pPtl104->Lmtd = 0x01;
	pPtl104->Rson = 0x03;
	pPtl104->Apdu = 0x00;
	SET_ZXTZSJ();
	len = F104_ZXSJSJ_PACK(pPtl104);
	pPtl104->Flen = 10+len;
	len = F104_INF_PACK(sBuf,pPtl104);
	send(0,sBuf,len);
//	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;	
//	isOnlinePayBack = 1;
}


//与服务器连接建立以后业务执行
void Ethernet_Work(uint8_t Value)
{
    uint8_t SN_SR;
    SN_SR = getSn_SR(0); 
    if(Value & Sn_IR_DISCON){                             //网络断开
        Ethernet_State = ETHERNET_CONNECTING;
        return;
    }    
    if(SN_SR == SOCK_ESTABLISHED){                          //连接状态
        if(Value & Sn_IR_RECV){                             //接收到数据
            uint16_t len = getSn_RX_RSR(0);
            if(len < RX_Size){
                recv(0,RX_Buffer,len);
                PTL104DataAnalysis(RX_Buffer,len);          //下行数据解析 分析处理接收的数据
            }
            putISR(0,Value&(~Sn_IR_RECV));
        }
        if(!(PTL104_State&PTL104_LOGIN)){               //设备未登录，发送登录帧 等待登录确认
            if(!(PTL104_State&PTL104_LOGIN_SENT) || \
                ((PTL104_State&PTL104_LOGIN_SENT)&&  \
            (Time_Seconds-Wait_Ack_Seconds>TIMEOUT_LOGIN_W_A))){
                
                if(Sent_Login_Frame(Term_Para1.ChargeNo) == 0)
                {
                    Ethernet_State = ETHERNET_CONNECTING;   //发送失败端口连接关闭需要重新连接
                }
                PTL104_State |= PTL104_LOGIN_SENT;
                Wait_Ack_Seconds = Time_Seconds;
            }
        }
        
        if(PTL104_State>=0xe0)//登录成功 充电桩激活 时间同步 完毕
        {
            if((Time_Seconds != Wait_Heart_Ack_Seconds) && \
                (!(Time_Seconds%SECONDS_PER_HEART_BEAT) || \
                (time_flg&PTL104_TIME_TO_HEART))){   //发送心跳时间到
                Sent_Heart_Frame();
                time_flg &= ~PTL104_TIME_TO_HEART;
                time_flg |= PTL104_HEART_WAIT_ACK;
                Wait_Heart_Ack_Seconds = Time_Seconds;
            }
            
            if(time_flg&PTL104_HEART_WAIT_ACK){
                if(Time_Seconds-Wait_Heart_Ack_Seconds >= TIMEOUT_HEART_W_A){
                    time_flg |= PTL104_TIME_TO_HEART;
                    //超时标识置1
                }
            }
            
            if((Time_Seconds != Wait_Event_Ack_Seconds) && \
                (!(Time_Seconds%SECONDS_PER_EVENT_UPDATE) \
                || (time_flg&PTL104_TIME_TO_EVENT))){//发送事件状态时间到
//                Sent_Event_Frame();
                time_flg &= ~PTL104_TIME_TO_EVENT;
                time_flg |= PTL104_EVENT_WAIT_ACK; 
                Wait_Event_Ack_Seconds = Time_Seconds;
                
            }
            
            if(time_flg&PTL104_EVENT_WAIT_ACK){
                if(Time_Seconds-Wait_Event_Ack_Seconds >= TIMEOUT_EVENT_W_A){
                    time_flg |= PTL104_TIME_TO_EVENT;//重发事件上报帧
                    //超时标识置1
                }
            }   
            //发送计费策略充电情况 以及在线充电记录上传
            if((Charge_Flag&CHANGE_END_Ctrl_BIT)||(Charge_Flag&CHANGE_END_Full_BIT)||(Charge_Flag&CHANGE_END_Pull_BIT))    
            {
                Charge_Flag &= ~(CHANGE_END_Ctrl_BIT|CHANGE_END_Full_BIT|CHANGE_END_Pull_BIT);
                Charge_Flag |= CHANGE_END_PAY;
                Sent_StrateChargeInfo_Frame(&sPTL_104_I_SND);       //计费策略充电情况
                Sent_OnlineChargeInfo_Frame(&sPTL_104_I_SND);       //在线充电情况    
            }
            //发送鉴权帧 刷卡事件发生时发送
        }
        
    }
    else
        Ethernet_State = ETHERNET_CONNECTING;
}
/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/




                            