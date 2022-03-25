/**
 * @file system_MK60.c
 * @version 1.2.1[By LPLD]
 * @date 2013-06-18
 * @brief MK60系列单片机系统配置文件
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

#include <stdint.h>
#include "common.h"

#define DISABLE_WDOG    1

/*----------------------------------------------------------------------------
  定义时钟相关值
 *----------------------------------------------------------------------------*/
#define CPU_XTAL_CLK_HZ                 50000000u       //外部有源晶振频率，单位Hz
#define CPU_XTAL32k_CLK_HZ              32768u          //外部32k时钟晶振频率，单位Hz    
#define CPU_INT_SLOW_CLK_HZ             32768u          //慢速内部振荡器的值，单位Hz
#define CPU_INT_FAST_CLK_HZ             4000000u        //快速内部振荡器的值，单位Hz
#define DEFAULT_SYSTEM_CLOCK            100000000u      //默认系统主频，单位Hz

/**
 * @brief 系统主频（单位Hz）
 */
uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

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
void SystemInit (void) {
  UART_InitTypeDef term_port_structure;
  //使能全部IO时钟
  SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK
              | SIM_SCGC5_PORTB_MASK
              | SIM_SCGC5_PORTC_MASK
              | SIM_SCGC5_PORTD_MASK
              | SIM_SCGC5_PORTE_MASK );
#if (DISABLE_WDOG)
  //禁用看门狗模块
  /* WDOG_UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = (uint16_t)0xC520u;     /* Key 1 */
  /* WDOG_UNLOCK : WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK  = (uint16_t)0xD928u;    /* Key 2 */
  /* WDOG_STCTRLH: ??=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = (uint16_t)0x01D2u;
#endif /* (DISABLE_WDOG) */
  
  //将中断向量表、需在RAM中运行的函数等数据拷贝到RAM中
  common_relocate();
  
  //初始化各部分时钟：系统内核主频、总线时钟、FlexBus时钟、Flash时钟
  LPLD_PLL_Setup(CORE_CLK_MHZ);
  
  //更新内核主频
  SystemCoreClockUpdate();
  
  //获取各部分时钟
  g_core_clock = SystemCoreClock;
  g_bus_clock = g_core_clock / ((uint32_t)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT)+ 1u);
  g_flexbus_clock =  g_core_clock / ((uint32_t)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV3_MASK) >> SIM_CLKDIV1_OUTDIV3_SHIFT)+ 1u);
  g_flash_clock =  g_core_clock / ((uint32_t)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT)+ 1u);
  
  //初始化用于打印调试信息的串口模块
  //TERM_PORT为UART通道，在k60_card.h中定义
  //TERMINAL_BAUD为UART波特率，在k60_card.h中定义
  term_port_structure.UART_Uartx = TERM_PORT;
  term_port_structure.UART_BaudRate = TERMINAL_BAUD;
  term_port_structure.UART_RxPin = UART_R_PORT;  //接收引脚
  term_port_structure.UART_TxPin = UART_T_PORT;  //发送引脚
  LPLD_UART_Init(term_port_structure);
  
  //打印系统调试信息
#ifdef DEBUG_PRINT     
 
#endif
  
}

/**
 * 更新SystemCoreClock
 *
 * @param  none
 * @return none
 *
 * @brief  更新全局变量SystemCoreClock的值，以便获取最新的系统内核频率。
 *         
 */
void SystemCoreClockUpdate (void) {
  uint32_t temp;
#ifdef CPU_MK60F15
  temp =  CPU_XTAL_CLK_HZ /((uint32_t)(MCG->C5 & MCG_C5_PRDIV0_MASK) + 1u );
  temp = (uint32_t)(temp*((uint32_t)(MCG->C6 & MCG_C6_VDIV0_MASK) +16u ));
  temp = temp/2;
#endif
  SystemCoreClock = temp;
}

/**
 * 硬件错误中断函数
 *
 * @param  none
 * @return none
 *
 * @brief  触发此中断的原因一般为：模块使用未初始化、错误的寻址空间等。
 *         
 */
void HardFault_Handler(void)
{
#ifdef DEBUG_PRINT 
   printf("\r\n****内核发生硬件错误*****\r\n");
#endif
   return;
}

/**
 * 默认中断函数
 *
 * @param  none
 * @return none
 *
 * @brief  未定义中断处理函数的中断源会进入此函数，并打印中断向量号。
 *         
 */
void DefaultISR(void)
{
   #define VECTORNUM                     (*(volatile uint8_t*)(0xE000ED04)) 
#ifdef DEBUG_PRINT 
   printf("\r\n****进入未定义中断,Interrupt Number %d*****\r\n",VECTORNUM-16);
#endif
   return;
}

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
void SystemTickInit (void)
{
  uint32  cpu_clk_freq;
  uint32  cnts;

  cpu_clk_freq = g_core_clock;  //获取SysTick时钟

#if (OS_VERSION >= 30000u)
  cnts  = cpu_clk_freq / (uint32)OSCfg_TickRate_Hz;    
#else
  cnts  = cpu_clk_freq / (uint32)OS_TICKS_PER_SEC;        
#endif

  OS_CPU_SysTickInit(cnts);     //初始化uCOS滴答定时器SysTick
}
#endif

