/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               zy_if.c
** Latest modified Date:    2009-07-23
** Latest Version:          1.00
** Descriptions:            用户编写的环境接口函数,必须在特权模式运行
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
#include "config.h"
#include "zy_if.h"
#include <string.h>

/*********************************************************************************************************
  消息通道信息
*********************************************************************************************************/
struct __zy_if_msg {
    INT32U    ulMsgSize;                                                /*  消息大小                    */
    OS_EVENT *poeQ;                                                     /*  数据队列指针                */
    OS_MEM   *pomMsg;                                                   /*  内存管理指针                */
};

typedef  struct __zy_if_msg     __ZY_IF_MSG;

/*********************************************************************************************************
** Function name:           zyIfInit
** Descriptions:            接口初始化
** input parameters:        none
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIfInit (void)
{
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyReset
** Descriptions:            系统复位
** input parameters:        uiMode: ZY_POWER_RESET: 上电复位
**                                  ZY_HARD_RESET:  硬件复位
**                                  ZY_SOFT_RESET:  软件复位
**                                  其它:           与系统相关
** output parameters:       none
** Returned value:          none
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void zyReset (unsigned int uiMode)
{
    switch (uiMode) {

    case ZY_POWER_RESET:                                                /*  此系统上电复位等同硬件复位   */

#if 0
        break;
#endif                                                                  /*  0                           */

    case ZY_HARD_RESET:
        changeToSYSMode();
        AITCR = (0x05fa << 16) + 4;
        break;

    case ZY_SOFT_RESET:
        changeToSYSMode();
        AITCR = (0x05fa << 16) + 1;
        break;
    
    default:                                                            /*  参数不正确不复位            */
        break;
    }
}

/*********************************************************************************************************
** Function name:           zyIrqDisable
** Descriptions:            禁止中断
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIrqDisable (void)
{
    OS_ENTER_CRITICAL();
    return 0;
}

/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            允许中断
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIrqEnable (void)
{
    OS_EXIT_CRITICAL();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyIsrSet
** Descriptions:            IO系统设置中断服务程序
** input parameters:        uiChannel:  中断通道号
**                          ulFunction: 中断服务程序地址
**                          uiPrio:     中断优先级
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIsrSet (unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();

    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
   
        ((INT32U *)0xE000E100)[uiTmp1] = 1ul << uiTmp2;
        ((INT8U *)0xE000E400)[uiTmp3]  = uiPrio;
    } else {
        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;

        default:
            break;
        }
        if (uiChannel >= MMI) {
            uiTmp3 = uiChannel - MMI;
            ((INT8U *)0xE000ED18)[uiTmp3]  = uiPrio;
        }
    }

#if VECTOR_TABLE_IN_FLASH == 0
    ((unsigned long *)VTOR)[uiChannel] = ulFunction;
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zdIsrClr
** Descriptions:            IO系统清除中断服务程序
** input parameters:        uiChannel:  中断通道号
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S __s_zyIsrClr (unsigned int uiChannel)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();
    
    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
    
        ((INT32U *)0xE000E180)[uiTmp1] = 1ul << uiTmp2;
        ((INT8U *)0xE000E400)[uiTmp3]  = 0;

    } else {

        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR & ~(1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR & ~(1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR & ~(1 << 18);
            break;

        default:
            break;
        }
        if (uiChannel >= MMI) {
            uiTmp3 = uiChannel - MMI;
            ((INT8U *)0xE000ED18)[uiTmp3]  = 0;
        }
    }

#if VECTOR_TABLE_IN_FLASH == 0
    ((unsigned long *)VTOR)[uiChannel] = 0;
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyIsrDisable
** Descriptions:            禁止指定中断
** input parameters:        uiChannel:  中断通道号
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S __s_zyIsrDisable (unsigned int uiChannel)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }
    
    if (uiChannel < 16) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();    

    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
    
        ((INT32U *)0xE000E180)[uiTmp1] = 1ul << uiTmp2;

    } else {

        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR & ~(1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR & ~(1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR & ~(1 << 18);
            break;

        default:
            break;
        }
    }

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyIsrEnable
** Descriptions:            允许指定中断
** input parameters:        uiChannel:  中断通道号
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIsrEnable (unsigned int uiChannel)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();

    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
        ((INT32U *)0xE000E100)[uiTmp1] = 1ul << uiTmp2;

    } else {

        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;

        default:
            break;
        }
    }

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyHeapMalloc
** Descriptions:            堆分配内存
** input parameters:        ulSize: 内存大小
** output parameters:       none
** Returned value:          内存地址,NULL为不成功
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void *zyHeapMalloc (INT32U ulSize)
{
    void *pvRt = NULL;                                                  /*  返回值                      */

    zyIrqDisable();
    pvRt = malloc((size_t)ulSize);
    zyIrqEnable();
    return pvRt;
}

/*********************************************************************************************************
** Function name:           zyHeapFree
** Descriptions:            堆释放内存
** input parameters:        pvPrt: 要释放的内存
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyHeapFree (void *pvPrt)
{
    zyIrqDisable();
    free(pvPrt);
    zyIrqEnable();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyThreadCreate
** Descriptions:            建立线程
** input parameters:        pvThreedMain: 线程主程序
**                          pvArg:        给线程的参数
**                          uiPrio:       线程优先级
**                          ulSize:       堆栈大小
** output parameters:       none
** Returned value:          线程ID,0为不成功
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zyThreadCreate (void *pvThreedMain, void *pvArg, unsigned int uiPrio, INT32U ulSize)
{
    OS_STK       *pstpTmp1;
    OS_TCB      **pptcpTmp2;
    unsigned long ulRt;
    
    pstpTmp1 = (OS_STK *)zyHeapMalloc(ulSize * sizeof(OS_STK));
    if (pstpTmp1 == NULL) {
        return 0;
    }

    OSSchedLock();
    
    if (OSTaskCreate((void (*) (void *))pvThreedMain,
                     pvArg,
#if OS_STK_GROWTH == 1
                     pstpTmp1 + ulSize - 1,
#else
                     pstpTmp1,
#endif                                                                  /*  OS_STK_GROWTH               */
                     (INT8U)uiPrio) != OS_NO_ERR) {
        OSSchedUnlock();
        return 0;
    }    

    pptcpTmp2 = OSTCBPrioTbl;
    ulRt      = (unsigned long)pptcpTmp2[uiPrio];
    
    OSSchedUnlock();
    
    return ulRt;
}

/*********************************************************************************************************
** Function name:           zyThreadDel
** Descriptions:            删除线程
** input parameters:        ulThreadId: 线程ID
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadDel (unsigned long ulThreadId)
{
    OS_TCB *ptcbTask;                                                   /*  任务TCB                     */
    void   *pvTmp1;

    ptcbTask = (OS_TCB *)ulThreadId;

    if (zyThreadGetCurId() == ulThreadId) {
        zyIrqDisable();
        pvTmp1 = (void *)(ptcbTask->OSTCBStkPtr);
        zyHeapFree(pvTmp1);
        OSTaskDel(ptcbTask->OSTCBPrio);
        zyIrqEnable();
        return ZY_OK;
    }

    OSSchedLock();

    pvTmp1 = (void *)(ptcbTask->OSTCBStkPtr);
    zyHeapFree(pvTmp1);
    OSTaskDel(ptcbTask->OSTCBPrio);

    OSSchedUnlock();
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyThreadSuspendSelf
** Descriptions:            挂起线程自身
** input parameters:        none
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadSuspendSelf (void)
{
    OSTaskSuspend(OS_PRIO_SELF);
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyThreadResume
** Descriptions:            恢复任务
** input parameters:        ulThreadId: 线程ID
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadResume (unsigned long ulThreadId)
{
    OS_TCB *ptcbTask;                                                   /*  任务TCB                     */

    ptcbTask = (OS_TCB *)ulThreadId;

    OSTaskResume(ptcbTask->OSTCBPrio);
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyThreadGetCurId
** Descriptions:            获得当前线程ID
** input parameters:        none
** output parameters:       none
** Returned value:          当前线程ID
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zyThreadGetCurId (void)
{
    return (unsigned long)OSTCBCur;
}

/*********************************************************************************************************
** Function name:           __zyThreadSuspendCheck
** Descriptions:            检查线程是否确实被挂起
** input parameters:        ulThreadId: 线程ID
** output parameters:       none
** Returned value:          zy_OK:      挂起
**                          -ZY_NOT_OK: 未挂起
**                          其它负数:   错误,绝对值参考zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadSuspendCheck (unsigned long ulThreadId)
{
    OS_TCB *ptcbTask;                                                   /*  任务TCB                     */

    ptcbTask = (OS_TCB *)ulThreadId;
    if(((ptcbTask->OSTCBStat) & OS_STAT_SUSPEND) != OS_STAT_RDY) {
        return ZY_OK;
    }

    return -ZY_NOT_OK;
}

/*********************************************************************************************************
** Function name:           zyThreadDelay
** Descriptions:            当前线程延时
** input parameters:        ulTimes: 等待时间(毫秒)
** output parameters:       none
** Returned value:          zy_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadDelay (unsigned long ulTimes)
{
    if (ulTimes == 0) {
        return -ZY_PARAMETER_ERR;
    }

    OSTimeDly((INT16U)((ulTimes * OS_TICKS_PER_SEC + 999) / 1000));
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zySemCreate
** Descriptions:            创建信号量
** input parameters:        ulCnt:  信号量初始值
** output parameters:       none
** Returned value:          信号量标识
**                          0: 不成功
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zySemCreate (INT32S ulCnt)
{
    return (unsigned long)OSSemCreate((INT16U)ulCnt);
}

/*********************************************************************************************************
** Function name:           zySemDel
** Descriptions:            删除信号量
** input parameters:        ulSem: zySemCreate返回值
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemDel (unsigned long ulSem)
{
    INT8U ucErr;                                                        /*  错误代码                    */

#if OS_ARG_CHK_EN == 0    
    /*
     *  参数检查
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    OSSemDel((OS_EVENT *)ulSem, OS_DEL_ALWAYS, &ucErr);
    
    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_ERR_DEL_ISR:
        return -ZY_NOT_OK;

    case OS_NO_ERR:
        return 0;
    
    default:
        return -ZY_NOT_OK;
    }
}

/*********************************************************************************************************
** Function name:           zySemAccept
** Descriptions:            无等待获得信号量
** input parameters:        ulSem: zySemCreate返回值
** output parameters:       none
** Returned value:          >=0:   信号量当前值
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemAccept (unsigned long ulSem)
{
#if OS_ARG_CHK_EN == 0    
    /*
     *  参数检查
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    return OSSemAccept((OS_EVENT *)ulSem);
}

/*********************************************************************************************************
** Function name:           zySemPend
** Descriptions:            等待信号量
** input parameters:        ulSem: zySemCreate返回值
**                          ulDly: 最长等待时间,毫秒为单位
** output parameters:       none
** Returned value:          >=0:   信号量当前值
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemPend (unsigned long ulSem, INT32U ulDly)
{
    INT8U ucErr;                                                        /*  错误代码                    */
    
#if OS_ARG_CHK_EN == 0    
    /*
     *  参数检查
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    OSSemPend((OS_EVENT *)ulSem, (INT16U)((ulDly * OS_TICKS_PER_SEC + 999) / 1000), &ucErr);
    
    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_TIMEOUT:
        return -ZY_TIME_OUT;

    case OS_NO_ERR:
        return 0;
    
    default:
        return -ZY_NOT_OK;
    }
}

/*********************************************************************************************************
** Function name:           zySemPost
** Descriptions:            发送信号量
** input parameters:        ulSem: zySemCreate返回值
**                          ucMod: 发送模式,一般为0
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemPost (unsigned long ulSem, INT8U ucMod)
{
#if OS_ARG_CHK_EN == 0    
    /*
     *  参数检查
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    switch (OSSemPost((OS_EVENT *)ulSem)) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_SEM_OVF:
        return -ZY_NOT_OK;

    case OS_NO_ERR:
        return 0;
    
    default:
        return -ZY_NOT_OK;
    }
}

/*********************************************************************************************************
** Function name:           zyMsgCreate
** Descriptions:            创建消息通道
** input parameters:        ulMsgs:    可以缓存消息数目
**                          ulMsgSize: 消息大小
** output parameters:       none
** Returned value:          消息通道标识
**                          0: 不成功
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zyMsgCreate (INT32S ulMsgs, INT32U ulMsgSize)
{
    INT32U       ulSize;                                                /*  需要的内存空间              */
    INT8U        ucErr;                                                 /*  错误代码                    */
    __ZY_IF_MSG *pimTmp1;
    INT8U       *pucTmp2;
    
    /*
     *  参数校验
     */
    if (ulMsgs == 0) {
        return 0;
    }

    if (ulMsgSize == 0) {
        return 0;
    }
    
    /*
     *  分配存储空间
     */
    ulSize  = sizeof(__ZY_IF_MSG) + ulMsgs * sizeof(void *) + ulMsgs * ulMsgSize;
    pucTmp2 = (INT8U *)zyHeapMalloc(ulSize);
    if (pucTmp2 == NULL) {
        return 0;
    }

    pimTmp1 = (__ZY_IF_MSG  *)pucTmp2;

    /*
     *  创建消息队列
     */
    pimTmp1->poeQ = OSQCreate((void **)(pucTmp2 + sizeof(__ZY_IF_MSG)), (INT16U)ulMsgs);
    if (pimTmp1->poeQ == NULL) {
        zyHeapFree(pucTmp2);
        return 0;
    }
    
    /*
     *  创建内存管理单元
     */
    pimTmp1->pomMsg = OSMemCreate((void *)(pucTmp2 + sizeof(__ZY_IF_MSG) * ulMsgs),
                                  (INT16U)ulMsgs,
                                  ulMsgSize,
                                  &ucErr
                                  );


    if (pimTmp1->pomMsg == NULL) {
        OSQDel(pimTmp1->poeQ, OS_DEL_ALWAYS, &ucErr);
        zyHeapFree(pucTmp2);
        return 0;
    }
    
    pimTmp1->ulMsgSize = ulMsgSize;

    return (unsigned long)pimTmp1;
}

/*********************************************************************************************************
** Function name:           zyMsgDel
** Descriptions:            删除消息通道
** input parameters:        ulMsg:  zyMsgCreate返回值
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgDel (unsigned long ulMsg)
{
    __ZY_IF_MSG *pimThis;                                               /*  消息通道信息                */
    INT8U        ucErr;                                                 /*  错误代码                    */

    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
    
    pimThis = (__ZY_IF_MSG *)ulMsg;

    if (pimThis->poeQ != NULL) {
        OSQDel(pimThis->poeQ, OS_DEL_ALWAYS, &ucErr);
    }
    zyHeapFree((void *)pimThis);
    return -ZY_NOT_OK;
}

/*********************************************************************************************************
** Function name:           zyMsgAccept
** Descriptions:            无等待获得消息
** input parameters:        ulMsg:  zyMsgCreate返回值
** output parameters:       pucMsg: 保存获得的消息
** Returned value:          ZY_OK:  成功
**                          负数:   错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgAccept (unsigned long ulMsg, INT8U *pucMsgRt)
{
    __ZY_IF_MSG *pimThis;                                               /*  消息通道信息                */
    void        *pvTmp;

    pimThis = (__ZY_IF_MSG *)ulMsg;

    /*
     *  参数检查
     */
    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
#if OS_ARG_CHK_EN == 0    
    if (pimThis->poeQ == NULL) {
        return -ZY_PARAMETER_ERR;
    }
    if (pimThis->pomMsg == NULL) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    pvTmp = OSQAccept(pimThis->poeQ);
    if (pvTmp == NULL) {
        return -ZY_NOT_OK;
    }

    memcpy(pucMsgRt, pvTmp, (size_t)(pimThis->ulMsgSize));              /*  拷贝数据                    */
    OSMemPut(pimThis->pomMsg, pvTmp);                                   /*  归还缓冲区                  */
    
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyMsgPend
** Descriptions:            等待消息
** input parameters:        ulMsg:  zyMsgCreate返回值
**                          ulDly: 最长等待时间,毫秒为单位
** output parameters:       pucMsgRt: 保存获得的消息
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgPend (unsigned long ulMsg, INT8U *pucMsgRt, INT32U ulDly)
{
    __ZY_IF_MSG *pimThis;                                               /*  消息通道信息                */
    INT8U        ucErr;                                                 /*  错误代码                    */
    void        *pvTmp;

    pimThis = (__ZY_IF_MSG *)ulMsg;

    /*
     *  参数检查
     */
    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
#if OS_ARG_CHK_EN == 0    
    if (pimThis->poeQ == NULL) {
        return -ZY_PARAMETER_ERR;
    }
    if (pimThis->pomMsg == NULL) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    pvTmp = OSQPend(pimThis->poeQ, (INT16U)((ulDly * OS_TICKS_PER_SEC + 999) / 1000), &ucErr);

    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_TIMEOUT:
        return -ZY_TIME_OUT;

    case OS_NO_ERR:
        break;
    
    default:
        return -ZY_NOT_OK;
    }

    memcpy(pucMsgRt, pvTmp, (size_t)(pimThis->ulMsgSize));              /*  拷贝数据                    */
    OSMemPut(pimThis->pomMsg, pvTmp);                                   /*  归还缓冲区                  */
    
    return ZY_OK;
}

/*********************************************************************************************************
** Function name:           zyMsgPost
** Descriptions:            发送消息
** input parameters:        ulMsg:      zyMsgCreate返回值
**                          pucMsgSend: 要发送的消息
**                          ucMod:      发送模式,一般为0
** output parameters:       none
** Returned value:          ZY_OK: 成功
**                          负数:  错误,绝对值参考zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgPost (unsigned long ulMsg, INT8U *pucMsgSend, INT8U ucMod)
{
    __ZY_IF_MSG *pimThis;                                               /*  消息通道信息                */
    INT8U        ucErr;                                                 /*  错误代码                    */
    void        *pvTmp;

    pimThis = (__ZY_IF_MSG *)ulMsg;

    /*
     *  参数检查
     */
    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
#if OS_ARG_CHK_EN == 0    
    if (pimThis->poeQ == NULL) {
        return -ZY_PARAMETER_ERR;
    }
    if (pimThis->pomMsg == NULL) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    /*
     *  申请缓冲区
     */
    pvTmp = OSMemGet(pimThis->pomMsg, &ucErr);

    switch (ucErr) {

    case OS_MEM_INVALID_PMEM:
        return -ZY_NO_FIND_OBJECT;

    case OS_MEM_NO_FREE_BLKS:
        return -ZY_NO_MEMORY;

    case OS_NO_ERR:
        break;
    
    default:
        return -ZY_NOT_OK;
    }

    /*
     *  发送消息
     */
    memcpy(pvTmp, pucMsgSend, (size_t)(pimThis->ulMsgSize));            /*  拷贝数据                    */
    ucErr = OSQPostOpt(pimThis->poeQ, pvTmp, ucMod);                    /*  保存消息存储位置            */
    
    if (ucErr == OS_NO_ERR) {
        return ZY_OK;
    }

    OSMemPut(pimThis->pomMsg, pvTmp);                                   /*  归还缓冲区                  */

    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_Q_FULL:
        return -ZY_NO_MEMORY;

    default:
        return -ZY_NOT_OK;
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
