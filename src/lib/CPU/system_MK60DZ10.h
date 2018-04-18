/**
 * @file system_MK60DZ10.h
 * @version 1.2.1[By LPLD]
 * @date 2013-06-18
 * @brief MK60DZ10系列单片机系统配置文件
 *
 * 更改建议:禁止修改
 *
 * 该代码提供系统配置函数以及一个储存系统主频的全局变量。
 * 配置函数主要负责建立系统各模块的时钟。
 * 代码还实现常见的系统中断函数。
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

#ifndef SYSTEM_MK60DZ10_H_
#define SYSTEM_MK60DZ10_H_                   

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
  
/**
 * @brief 系统主频（单位Hz）
 */
extern uint32_t SystemCoreClock;

/**
 * 初始化系统
 *
 * @param  none
 * @return none
 *
 * @brief  初始化MCU硬件系统，包括开放IO时钟、初始化主频、初始化调试串口，
 *         打印调试信息。SystemInit由MCU启动文件调用。
 *         
 */
extern void SystemInit (void);

/**
 * 更新SystemCoreClock
 *
 * @param  none
 * @return none
 *
 * @brief  更新全局变量SystemCoreClock的值，以便获取最新的系统内核频率。
 *         
 */
void SystemCoreClockUpdate (void);

/**
 * 硬件错误中断函数
 *
 * @param  none
 * @return none
 *
 * @brief  触发此中断的原因一般为：模块使用未初始化、错误的寻址空间等。
 *         
 */
void HardFault_Handler (void);

/**
 * 默认中断函数
 *
 * @param  none
 * @return none
 *
 * @brief  未定义中断处理函数的中断源会进入此函数，并打印中断向量号。
 *         
 */
void DefaultISR (void);

/**
 * 初始化操作系统的滴答定时器(OS Tick)
 *
 * @param  none
 * @return none
 *
 * @brief  该函数仅在uC/OS等系统中调用，切必须在OSStart()和处理器初始化后调用
 *         
 */
#if UCOS_II > 0u
extern void SystemTickInit (void);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #if !defined(SYSTEM_MK60DZ10_H_) */
