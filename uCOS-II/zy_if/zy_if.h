/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               zy_if.h
** Latest modified Date:    2009-07-23
** Latest Version:          1.00
** Descriptions:            环境接口函数,必须在特权模式运行
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Chenmingji
** Created date:            2009-07-23
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
#ifndef __ZY_IF_H
#define __ZY_IF_H

#include "includes.h"

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  返回值定义
*********************************************************************************************************/
#define     ZY_OK               0                                       /*  操作成功                    */
#define     ZY_NOT_OK           1                                       /*  操作失败                    */
#define     ZY_PARAMETER_ERR    2                                       /*  参数错误                    */
#define     ZY_NO_FIND_FUNCTION 6                                       /*  没有发现指定函数            */
#define     ZY_NO_MEMORY        12                                      /*  内存不足                    */
#define     ZY_TIME_OUT         13                                      /*  超时                        */
#define     ZY_NO_FIND_OBJECT   16                                      /*  没有发现对象                */

/*********************************************************************************************************
  64位变量相关定义
*********************************************************************************************************/
typedef long long   INT64S;                                             /*  64位有符号数                */

#define zyLlAdd(a, b, c) a = (INT64S)(b) + (INT64S)(c)                  /*  64位有符号加法a=b+c         */
#define zyLlSub(a, b, c) a = (INT64S)(b) - (INT64S)(c)                  /*  64位有符号减法a=b-c         */
#define zyLlMul(a, b, c) a = (INT64S)(b) * (INT64S)(c)                  /*  64位有符号乘法a=b*c         */
#define zyLlDiv(a, b, c) a = (INT64S)(b) / (INT64S)(c)                  /*  64位有符号除法a=b/c         */
#define zyLlMod(a, b, c) a = (INT64S)(b) % (INT64S)(c)                  /*  64位有符号除法a=b/c         */
#define zyLlSet(a, b)    a = (INT64S)(b)                                /*  64位有符号赋值              */
#define zyLlIsLess(a, b) ((INT64S)(a) < (INT64S)(b))                    /*  64位有符号小于判定          */
#define zyLlSet32(a, b)  a = (INT32U)(b)                                /*  64位有符号赋值给32位无符号数*/
#define zyLlSet32s(a, b) a = (INT32S)(b)                                /*  64位有符号赋值给32位有符号数*/

/*********************************************************************************************************
  复位模式
*********************************************************************************************************/
#define ZY_POWER_RESET      0                                           /*  上电复位                    */
#define ZY_HARD_RESET       1                                           /*  硬件复位                    */
#define ZY_SOFT_RESET       2                                           /*  软件复位                    */

/*********************************************************************************************************
** Function name:           zyIfInit
** Descriptions:            接口初始化
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIfInit(void);

/*********************************************************************************************************
** Function name:           zyReset
** Descriptions:            系统复位
** input parameters:        uiMode: ZY_POWER_RESET: 上电复位
**                                  ZY_HARD_RESET:  硬件复位
**                                  ZY_SOFT_RESET:  软件复位
**                                  其它:           与系统相关
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
extern void zyReset(unsigned int uiMode);

/*********************************************************************************************************
** Function name:           zyIrqDisable
** Descriptions:            禁止中断
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIrqDisable(void);

/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            允许中断
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIrqEnable(void);

/*********************************************************************************************************
** Function name:           zyIsrSet
** Descriptions:            设置中断服务程序
** input parameters:        uiChannel:  中断通道号
**                          ulFunction: 中断服务程序地址
**                          uiPrio:     中断优先级
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrSet(unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio);

/*********************************************************************************************************
** Function name:           zyIsrClr
** Descriptions:            清除中断服务程序
** input parameters:        uiChannel:  中断通道号
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrClr(unsigned int uiChannel);

/*********************************************************************************************************
** Function name:           zyIsrDisable
** Descriptions:            禁止指定中断
** input parameters:        uiChannel:  中断通道号
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrDisable(unsigned int uiChannel);

/*********************************************************************************************************
** Function name:           zyIsrEnable
** Descriptions:            允许指定中断
** input parameters:        uiChannel:  中断通道号
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrEnable(unsigned int uiChannel);

/*********************************************************************************************************
** Function name:           zyHeapMalloc
** Descriptions:            堆分配内存
** input parameters:        ulSize: 内存大小
** output parameters:       none
** Returned value:          内存地址,NULL为不成功
*********************************************************************************************************/
extern void *zyHeapMalloc(INT32U ulSize);

/*********************************************************************************************************
** Function name:           zyHeapFree
** Descriptions:            堆释放内存
** input parameters:        pvPrt: 要释放的内存
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyHeapFree(void *pvPrt);

/*********************************************************************************************************
** Function name:           zyThreadCreate
** Descriptions:            建立线程
** input parameters:        pvThreedMain: 线程主程序
**                          pvArg:        线程主程序参数
**                          uiPrio:       线程优先级
**                          ulSize:       堆栈大小
** output parameters:       none
** Returned value:          线程ID,0为不成功
*********************************************************************************************************/
extern unsigned long zyThreadCreate(void *pvThreedMain, void *pvArg, unsigned int uiPrio, INT32U ulSize);

/*********************************************************************************************************
** Function name:           zyThreadDel
** Descriptions:            删除线程
** input parameters:        ulThreadId: 线程ID
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadDel(unsigned long ulThreadId);

/*********************************************************************************************************
** Function name:           zyThreadSuspendSelf
** Descriptions:            挂起线程自身
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadSuspendSelf(void);

/*********************************************************************************************************
** Function name:           zyThreadResume
** Descriptions:            恢复线程
** input parameters:        ulThreadId: 线程ID
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadResume(unsigned long ulThreadId);

/*********************************************************************************************************
** Function name:           zyThreadGetCurId
** Descriptions:            获得当前线程ID
** input parameters:        none
** output parameters:       none
** Returned value:          线程ID
*********************************************************************************************************/
extern unsigned long zyThreadGetCurId(void);

/*********************************************************************************************************
** Function name:           zyThreadSuspendCheck
** Descriptions:            检查线程是否确实被挂起
** input parameters:        ulThreadId: 线程ID
** output parameters:       none
** Returned value:          zy_OK:      挂起
**                          -ZY_NOT_OK: 未挂起
**                          其它负数:   错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadSuspendCheck(unsigned long ulThreadId);

/*********************************************************************************************************
** Function name:           zyThreadDelay
** Descriptions:            当前线程延时
** input parameters:        ulTimes: 以毫秒为单位等待时间
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadDelay(unsigned long ulTimes);

/*********************************************************************************************************
** Function name:           zySemCreate
** Descriptions:            创建信号量
** input parameters:        ulCnt:  信号量初始值
** output parameters:       none
** Returned value:          信号量标识
**                          0: 不成功
*********************************************************************************************************/
extern unsigned long zySemCreate(INT32S ulCnt);

/*********************************************************************************************************
** Function name:           zySemDel
** Descriptions:            删除信号量
** input parameters:        ulSem: zySemCreate返回值
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zySemDel(unsigned long ulSem);

/*********************************************************************************************************
** Function name:           zySemAccept
** Descriptions:            无等待获得信号量
** input parameters:        ulSem: zySemCreate返回值
** output parameters:       none
** Returned value:          >=0:   信号量当前值
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zySemAccept(unsigned long ulSem);

/*********************************************************************************************************
** Function name:           zySemPend
** Descriptions:            等待信号量
** input parameters:        ulSem: zySemCreate返回值
**                          ulDly: 最长等待时间,毫秒为单位
** output parameters:       none
** Returned value:          >=0:   信号量当前值
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zySemPend(unsigned long ulSem, INT32U ulDly);

/*********************************************************************************************************
** Function name:           zySemPost
** Descriptions:            允许指定中断
** input parameters:        ulSem: zySemCreate返回值
**                          ucMod: 发送模式,一般为0
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zySemPost(unsigned long ulSem, INT8U ucMod);

/*********************************************************************************************************
** Function name:           zyMsgCreate
** Descriptions:            创建消息通道
** input parameters:        ulMsgs:    可以缓存消息数目
**                          ulMsgSize: 消息大小
** output parameters:       none
** Returned value:          消息通道标识
**                          0: 不成功
*********************************************************************************************************/
extern unsigned long zyMsgCreate(INT32S ulMsgs, INT32U ulMsgSize);

/*********************************************************************************************************
** Function name:           zyMsgDel
** Descriptions:            删除消息通道
** input parameters:        ulMsg:  zyMsgCreate返回值
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgDel(unsigned long ulMsg);

/*********************************************************************************************************
** Function name:           zyMsgAccept
** Descriptions:            无等待获得消息
** input parameters:        ulMsg:  zyMsgCreate返回值
** output parameters:       pucMsg: 保存获得的消息
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgAccept(unsigned long ulMsg, INT8U *pucMsgRt);

/*********************************************************************************************************
** Function name:           zyMsgPend
** Descriptions:            等待消息
** input parameters:        ulMsg:  zyMsgCreate返回值
**                          ulDly: 最长等待时间,毫秒为单位
** output parameters:       pucMsgRt: 保存获得的消息
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgPend(unsigned long ulMsg, INT8U *pucMsgRt, INT32U ulDly);

/*********************************************************************************************************
** Function name:           zyMsgPost
** Descriptions:            发送消息
** input parameters:        ulMsg:      zyMsgCreate返回值
**                          pucMsgSend: 要发送的消息
**                          ucMod:      发送模式,一般为0
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgPost(unsigned long ulMsg, INT8U *pucMsgSend, INT8U ucMod);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __ZY_IF_H                   */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
