/**
 * @file HW_WDOG.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief WDOG底层模块相关函数
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
#include "HW_WDOG.h"

static void LPLD_WDOG_Unlock(void);

/*
 * LPLD_WDOG_Init
 * 看门狗初始化
 * 
 * 参数:
 *    period_ms--看门狗复位周期
 *      |__单位为毫秒
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_WDOG_Init(uint32 period_ms)
{
  uint8 presc = 3;
  uint32 bus_khz = g_bus_clock / 1000;
  uint32 value;
  
  if(period_ms==0)
  {
    return 0;
  }
    
  value=(period_ms*bus_khz)/(presc+1);
    
  //先解锁寄存器
  LPLD_WDOG_Unlock();
  //设置分频
  WDOG->PRESC = WDOG_PRESC_PRESCVAL(presc);
  //设置计时溢出值
  WDOG->TOVALH = (value&0xFFFF0000)>>16;
  WDOG->TOVALL = (value&0x0000FFFF)>>0;  
  //使能WDOG
  WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;
  
  return 1;
}

/*
 * LPLD_WDOG_Enable
 * 看门狗使能
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_WDOG_Enable(void)
{
  //先解锁寄存器
  LPLD_WDOG_Unlock();
  //使能WDOG
  WDOG->STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;
}

/*
 * LPLD_WDOG_Disable
 * 看门狗禁用
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_WDOG_Disable(void)
{
  //先解锁寄存器
  LPLD_WDOG_Unlock();
  //禁用WDOG
  WDOG->STCTRLH &= ~(WDOG_STCTRLH_WDOGEN_MASK);
}

/*
 * LPLD_WDOG_Feed
 * 喂狗
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_WDOG_Feed(void)
{
  WDOG->REFRESH = 0xA602;
  WDOG->REFRESH = 0xB480;
}

/*
 * LPLD_WDOG_Unlock
 * WDOG解锁函数，内部调用
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
static void LPLD_WDOG_Unlock(void)
{
  //以下代码必须在20个时钟周期内完成，否则会复位
  DisableInterrupts;
  
  //写0xC520来解锁寄存器
  WDOG->UNLOCK = 0xC520;
  
  //接下来写0xD928完成解锁
  WDOG->UNLOCK = 0xD928;
  	
  EnableInterrupts;
}