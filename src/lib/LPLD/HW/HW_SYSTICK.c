/**
 * @file HW_SYSTICK.c
 * @version 3.02[By LPLD]
 * @date 2013-11-29
 * @brief SYSTICK底层模块相关函数
 *
 * 更改建议:不建议修改
 *
 * 版权所有:北京拉普兰德电子技术有限公司
 * http://www.lpld.cn
 * mail:support@lpld.cn
 *
 * @par
 * 本代码由拉普兰德[LPLD]开发并维护，并向所有使用者开放源代码。
 * 开发者可以随意修使用或改源代码。但本段及以上注释应予以保留。
 * 不得更改或删除原版权所有者姓名，二次开发者可以加注二次版权所有者。
 * 但应在遵守此协议的基础上，开放源代码、不得出售代码本身。
 * 拉普兰德不负责由于使用本代码所带来的任何事故、法律责任或相关不良影响。
 * 拉普兰德无义务解释、说明本代码的具体原理、功能、实现方法。
 * 除非拉普兰德[LPLD]授权，开发者不得将本代码用于商业产品。
 */
#include "common.h"
#include "HW_SYSTICK.h"

/*
*   SyStick是一款Cotex-M4内核的定时器
*   SyStick采用减计数的方式，如果减到零，则设置COUNTFLAG标志位，如果使能中断，则产生中断
*   然后继续加载RVR中的值，继续计数；
*   具体配置参考Cortex-M4 Reference Manual
*/
SYSTICK_ISR_CALLBACK SYSTICK_ISR;
/*
* LPLD_SYSTICK_Init
* SYSTICK初始化函数，启动SYStick定时器，设置时钟源为内核时钟
* >如果在结构体初始化的时候定义回调函数，在LPLD_SYSTICK_Init中开启中断功能
* >如果没有用到中断，可以通过循环判断LPLD_SYSTICK_ISDONE的方式获得SYSTICK是否计数完成
* 
* 参数:
*    systick_init_structure--SYSTICK初始化结构体，
*                        具体定义见SYSTICK_InitType
*
* 输出：
*   无
*/
void LPLD_SYSTICK_Init(SYSTICK_InitType systick_init_structure)  
{      
  //如果内核时钟是100Mhz，最大定时167ms
  uint32 ldval = systick_init_structure.SYSTICK_PeriodUs * SYSTICK_UINT_US(g_core_clock)
             + systick_init_structure.SYSTICK_PeriodMs * SYSTICK_UINT_MS(g_core_clock);
  //计数寄存器的值不能超过0xFFFFFF           
  if( ldval > SYSTICK_COUNTER_MAX)
  {
    return;
  }
  
  SYSTICK->CSR = 0 & (~SYSTICK_CSR_ENABLE_MASK)      //禁止SYSTICK
                    | SYSTICK_CSR_CLKSOURCE_MASK     //选择内核时钟作为时钟源
                    | SYSTICK_CSR_COUNTFLAG_MASK;    //清除标志位
  if(systick_init_structure.SYSTICK_Isr != NULL)
  {
    SYSTICK->CSR |= SYSTICK_CSR_TICKINT_MASK;  //使能systick中断
    SYSTICK_ISR = systick_init_structure.SYSTICK_Isr;
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    enable_irq(SysTick_IRQn);                 //使能SYSTICK中断
  }
  SYSTICK->RVR = ldval - 1;  
  SYSTICK->CVR = 0x00;  
  SYSTICK->CSR |= SYSTICK_CSR_ENABLE_MASK;  //使能SYSTICK
}

/*
* LPLD_SYSTICK_Init
* SYSTICK反初始化函数，关闭SYStick定时器
* 关闭中断，清空计数器
* 
* 参数:
*   无
*
* 输出：
*   无
*/
void LPLD_SYSTICK_DeInit(void)
{
  SYSTICK->CSR = 0 & (~SYSTICK_CSR_ENABLE_MASK)   //禁止SYSTICK
                   & (~SYSTICK_CSR_TICKINT_MASK)  //禁止产生内核SYSTICK异常
                   & (~SYSTICK_CSR_CLKSOURCE_MASK)//选择内核时钟作为时钟源
                   | SYSTICK_CSR_COUNTFLAG_MASK;  //清除标志位                     
  disable_irq(SysTick_IRQn);         //禁止SYSTICK中断       
  SYSTICK->CVR = 0x00;  
}

/*
* LPLD_SYSTICK_DelayUs
*
* 利用SYSTICK产生Us延时,已在函数内部初始化，直接调用该函数既可以产生延时
* 启动SYStick定时器，设置定时器的计数间隔为1/g_core_clock us
* 设置加载寄存器，加载寄存器的值为1/g_core_clock us 的period_us整数倍
*
* 参数:
*    period_us--延时的微秒值
*
* 输出:
*   无
*/
void LPLD_SYSTICK_DelayUs(uint32 period_us)
{  
  uint32 i;
  
  SYSTICK->CSR = 0 & (~SYSTICK_CSR_ENABLE_MASK)      //禁止SYSTICK
                 & (~SYSTICK_CSR_TICKINT_MASK)       //禁止产生内核SYSTICK异常
                 | SYSTICK_CSR_CLKSOURCE_MASK        //选择内核时钟作为时钟源
                 | SYSTICK_CSR_COUNTFLAG_MASK;       //清除标志位
                  
  i = period_us * SYSTICK_UINT_US(g_core_clock) - 1; 
  //计数寄存器的值不能超过0xFFFFFF
  if( i > SYSTICK_COUNTER_MAX)
  {
    return;
  }
  SYSTICK->RVR = i;  
  SYSTICK->CVR = 0x00;  
  SYSTICK->CSR |= SYSTICK_CSR_ENABLE_MASK;  
  do  
  {  
    i = SYSTICK->CSR;  
  }  
  while(i & SYSTICK_CSR_ENABLE_MASK \
        && !(i & SYSTICK_CSR_COUNTFLAG_MASK)); 
   
  SYSTICK->CSR = 0 & (~SYSTICK_CSR_ENABLE_MASK)     //禁止SYSTICK
                 & (~SYSTICK_CSR_TICKINT_MASK)      //禁止产生内核SYSTICK异常
                 | SYSTICK_CSR_CLKSOURCE_MASK       //选择内核时钟作为时钟源
                 | SYSTICK_CSR_COUNTFLAG_MASK;      //清除标志位  
                 
  SYSTICK->CVR = 0x00;  
}  

/*
* LPLD_SYSTICK_DelayMs
*
* 利用SYSTICK产生Ms延时,已在函数内部初始化，直接调用该函数既可以产生延时
* 启动SYStick定时器，设置定时器的计数间隔为1/g_core_clock ms
* 设置加载寄存器，加载寄存器的值为1/g_core_clock ms 的period_ms整数倍
*
* 参数:
*    period_ms--延时的毫秒值
*
* 输出:
*   无
*/
void LPLD_SYSTICK_DelayMs(uint32 period_ms)
{  
  uint32 i;
  
  SYSTICK->CSR = 0 & (~SYSTICK_CSR_ENABLE_MASK)    //禁止SYSTICK
               & (~SYSTICK_CSR_TICKINT_MASK)       //禁止产生内核SYSTICK异常
               | SYSTICK_CSR_CLKSOURCE_MASK        //选择内核时钟作为时钟源
               | SYSTICK_CSR_COUNTFLAG_MASK;       //清除标志位
               
  i = period_ms * SYSTICK_UINT_MS(g_core_clock) - 1;
  //计数寄存器的值不能超过0xFFFFFF
  if( i > SYSTICK_COUNTER_MAX)
  {
    return;
  }
  SYSTICK->RVR = i;  
  SYSTICK->CVR = 0x00;  
  SYSTICK->CSR |= SYSTICK_CSR_ENABLE_MASK;  
  do  
  {  
    i = SYSTICK->CSR;  
  }  
  while(i & SYSTICK_CSR_ENABLE_MASK \
        && !(i & SYSTICK_CSR_COUNTFLAG_MASK)); 
   
  SYSTICK->CSR = 0 & (~SYSTICK_CSR_ENABLE_MASK)    //禁止SYSTICK
                 & (~SYSTICK_CSR_TICKINT_MASK)     //禁止产生内核SYSTICK异常
                 | SYSTICK_CSR_CLKSOURCE_MASK      //选择内核时钟作为时钟源
                 | SYSTICK_CSR_COUNTFLAG_MASK;     //清除标志位  
                 
  SYSTICK->CVR = 0x00;  
} 

#if (UCOS_II == 0u)
void OS_CPU_SysTickHandler(void)
{
  SYSTICK_ISR();
}
#endif
