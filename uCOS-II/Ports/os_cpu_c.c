/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               os_cpu_c.c
** Last modified Date:      2007.01.18
** Last Version:            1.0
** Description:             The C functions that a uC/OS-II port requires    uC/OS-II移植所需要的C函数		 
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Steven Zhou 周绍刚
** Created date:            2007.01.18
** Version:                 1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao
** Modified date:           2007.10.29
** Version:                 1.1
** Description:             The second version 第二版
**
*********************************************************************************************************/
#define  OS_CPU_GLOBALS

#include "LPC177x_8x.h"
#include <includes.h>


/********************************************************************************************************
  Local variables 局部变量
********************************************************************************************************/
#if OS_TMR_EN > 0
    static INT16U OSTmrCtr;
#endif

/*********************************************************************************************************
** Function name:			OSInitHookBegin
** Descriptions:			The hook function called by OSInit() at the beginning of OSInit()
**                          OSInit()函数刚开始执行时调用的钩子函数    
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
    void OSInitHookBegin (void)
    {
        #if OS_TMR_EN > 0
            OSTmrCtr = 0;
        #endif
    }
#endif


/*********************************************************************************************************
** Function name:			OSInitHookEnd
** Descriptions:			The hook function called by OSInit() at the end of OSInit()
**                          OSInit()函数结束前调用的钩子函数    
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
    void OSInitHookEnd (void)
    {
    }
#endif

/*********************************************************************************************************
** Function name:			OSTaskCreateHook
** Descriptions:			The hook function called when a task is created
**                          初始化任务时调用的钩子函数
** Input parameters:		ptcb
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
    void OSTaskCreateHook (OS_TCB  *ptcb)
    {
	    (void)ptcb;                                                     /*  Prevent compiler warning    */
                                                                        /*  防止编译警告                */
    }
#endif

/*********************************************************************************************************
** Function name:			OSTaskDelHook
** Descriptions:			The hook function called when a task is deleted
**                          删除任务时调用的钩子函数
** Input parameters:		ptcb
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
    void  OSTaskDelHook (OS_TCB  *ptcb)
    {
        (void)ptcb;                                                     /*  Prevent compiler warning    */
                                                                        /*  防止编译警告                */
    }
#endif

/*********************************************************************************************************
** Function name:			OSTaskIdleHook
** Descriptions:			The hook function called by the idle task
**                          空闲任务调用的钩子函数
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
    void OSTaskIdleHook (void)
    {
     }
#endif

/*********************************************************************************************************
** Function name:			OSTaskStatHook
** Descriptions:			The hook function called every second by the statistics task 
**                          统计任务每秒钟调用的钩子函数
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
    void OSTaskStatHook (void)
    {
    }
#endif

/*********************************************************************************************************
** Function name:			SysTick_Handler
** Descriptions:			系统节拍定时器中断服务函数
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无        
** Created by:				Zhengbailiang
** Created Date:			2011.07.06
**--------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date: 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  SysTick_Handler(void)
{
    #if OS_CRITICAL_METHOD == 3
        OS_CPU_SR cpu_sr;
    #endif 

    OS_ENTER_CRITICAL();                         
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                                       /*  Call uC/OS-II's OSTimeTick()*/

    OSIntExit();                                 
}

/*********************************************************************************************************
** Function name:			sysTickInit
** Descriptions:			系统节拍定时器初始化函数
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无        
** Created by:				Zhengbailiang
** Created Date:			2011.07.06
**--------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date: 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  sysTickInit(void)
{
    SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);
}

/*********************************************************************************************************
** Function name:			OSTaskStkInit
** Descriptions:			This function is called by either OSTaskCreate() or OSTaskCreateExt() to 
**                          initialize the stack frame of the task being created.  
**                          This function is highly processor specific.
**                          OSTaskCreate() 和 OSTaskCreateExt()调用本函数初始化正被创建的任务的堆栈
**                          本函数和具体的处理器有关 
** Input parameters:		task：  A pointer to the task code   任务代码的指针
**                          p_arg： A pointer to a user supplied data area that will be passed to the task
**                                  when the task first executes.
**                                  用户提供的数据空间的指针，任务运行时会被传递给任务 
**                          ptos:   A pointer to the top of the stack. For this port, it's the highest  
**                                  valid address of the stack. 
**                                  栈顶的指针。对于本移植，它指向堆栈的最高有效地址。
**                          opt:    Specified options that can be used to alter the behavior of 
*                                   OSTaskStkInit().(see uCOS_II.H for OS_TASK_OPT_xxx).
**                                  特定选项，可以改变对OSTaskStkInit()的使用方法。
**                                  (参看 uCOS_II.H 中的 OS_TASK_OPT_xxx).
** Output parameters:		None 无
** Returned value:	        The new location of top-of-stack  新栈顶指针位置         
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
OS_STK *OSTaskStkInit (void  (*task)(void  *parg), void  *parg, OS_STK  *ptos, INT16U  opt)
{
    OS_STK *stk;


    (void)opt;                                                          /*  'opt' is not used, prevent  */
                                                                        /*  warning 没有用'opt'，       */
                                                                        /*  防止编译警告                */
                                                                            
    stk = ptos;                                                         /*  Load stack pointer          */
                                                                        /*  装载堆栈指针                */                                    

                                                                        /*  Registers stacked as if     */
                                                                        /*  auto-saved on exception     */             
	                                                                    /*  模拟成异常，自动把寄存器压栈*/

    *(stk) = (INT32U)0x01000000L;                                       /*  xPSR                        */ 
    *(--stk) = (INT32U)task;                                            /*  Entry Point of the task     */
                                                                        /*  任务入口地址                */
    *(--stk) = (INT32U)0xFFFFFFFEL;                                     /*  R14 (LR)  (init value will  */
                                                                        /*  cause fault if ever used)   */
                                                                           
    *(--stk) = (INT32U)0x12121212L;                                     /*  R12                         */
    *(--stk) = (INT32U)0x03030303L;                                     /*  R3                          */
    *(--stk) = (INT32U)0x02020202L;                                     /*  R2                          */
    *(--stk) = (INT32U)0x01010101L;                                     /*  R1                          */
    *(--stk) = (INT32U)parg;                                            /*  R0 : argument  输入参数     */
                                                                        /*  Remaining registers saved on*/
                                                                        /*  process stack               */   
	                                                                    /*  剩下的寄存器保存到堆栈      */

    *(--stk) = (INT32U)0x11111111L;                                     /*  R11                         */
    *(--stk) = (INT32U)0x10101010L;                                     /*  R10                         */
    *(--stk) = (INT32U)0x09090909L;                                     /*  R9                          */
    *(--stk) = (INT32U)0x08080808L;                                     /*  R8                          */
    *(--stk) = (INT32U)0x07070707L;                                     /*  R7                          */
    *(--stk) = (INT32U)0x06060606L;                                     /*  R6                          */
    *(--stk) = (INT32U)0x05050505L;                                     /*  R5                          */
    *(--stk) = (INT32U)0x04040404L;                                     /*  R4                          */

    return(stk);
}

/*********************************************************************************************************
** Function name:			OSTaskSwHook
** Descriptions:			The hook function called when a task switch is performed.
**                          任务切换时前调用的钩子函数    
** Input parameters:		None 无
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void OSTaskSwHook (void)
{

}

/*********************************************************************************************************
** Function name:			OSTCBInitHook
** Descriptions:			The hook function called by OS_TCBInit()
**                          OS_TCBInit()调用的钩子函数    
** Input parameters:		ptcb
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if (OS_CPU_HOOKS_EN > 0) && (OS_VERSION > 203)
    void OSTCBInitHook (OS_TCB  *ptcb)
    {
        (void)ptcb;                                                     /*  Prevent compiler warning    */
                                                                        /*  防止编译警告                */
    }
#endif

/*********************************************************************************************************
** Function name:			OSTimeTickHook
** Descriptions:			The hook function called every tick
**                          每个节拍调用的钩子函数    
** Input parameters:		ptcb
** Output parameters:		None 无
** Returned value:		    None 无
** Created by:				Steven Zhou 周绍刚
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao 倪力考
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if ((OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)) || (OS_VERSION < 283)
    void OSTimeTickHook (void)
    {
        #if OS_TMR_EN > 0
	        OSTmrCtr++;
		    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
                OSTmrCtr = 0;
                OSTmrSignal();
		    }
	    #endif
    }
#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

