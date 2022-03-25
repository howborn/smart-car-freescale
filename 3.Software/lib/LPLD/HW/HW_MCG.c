/**
 * @file HW_MCG.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief MCG底层模块相关函数
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
#include "HW_MCG.h"


uint32 g_core_clock = -1ul;
uint32 g_bus_clock = -1ul;
uint32 g_flash_clock = -1ul;
uint32 g_flexbus_clock = -1ul;

/*
 * LPLD_PLL_Setup
 * 初始化内核时钟及其他系统时钟
 * 
 * 参数:
 *    core_clk_mhz--期望内核时钟频率
 *      |__PLLx--参见HAL_MCG.h中的PLL_option定义
 *
 * 输出:
 *    内核频率，单位MHz
 */
uint8 LPLD_PLL_Setup(PllOptionEnum_Type core_clk_mhz)
{
  uint8 pll_freq;
  uint8 prdiv, vdiv;
  uint8 core_div, bus_div, flexbus_div, flash_div;
#if (defined(CPU_MK60DZ10))
/*
 *************************************************
  【LPLD注解】MCG关键系数
  prdiv(PLL分频系数): 0~31(1~32)
  vdiv(PLL倍频系数): 0~31(24~55)
  PLL参考时钟范围: 2MHz~4MHz
  PLL参考时钟 = 外部参考时钟(CPU_XTAL_CLK_HZ)/prdiv
  CoreClk = PLL参考时钟 x PLL倍频系数 /OUTDIV1
 *************************************************
 */
 
  // 对于MK60DZ10来说，core_clk_mhz建议不要超过100，这里限制为最高200
  core_clk_mhz = (PllOptionEnum_Type)(core_clk_mhz>200u?200u:core_clk_mhz);
  
  // 根据期望主频选择分频和倍频系数
  switch(core_clk_mhz)
  {
  case PLL_48:
    prdiv = 24u;
    vdiv = 0u;
    break;
  case PLL_50:
    prdiv = 24u;
    vdiv = 1u;
    break;
  case PLL_96:
    prdiv = 24u;
    vdiv = 24u;
    break;
  case PLL_100:
    prdiv = 24u;
    vdiv = 26u;
    break;
  case PLL_120:
    prdiv = 19u;
    vdiv = 24u;
    break;
  case PLL_150:
    prdiv = 15u;
    vdiv = 24u;
    break;
  case PLL_180:
    prdiv = 14u;
    vdiv = 30u;
    break;
  case PLL_200:
    prdiv = 12u;
    vdiv = 28u;
    break;
  default:
    return LPLD_PLL_Setup(PLL_96);
  }
#elif (defined(CPU_MK60D10)) 
  //TODO:
  
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
 /*
 *************************************************
  【LPLD注解】MCG关键系数
  prdiv(PLL分频系数): 0~7(1~8)
  vdiv(PLL倍频系数): 0~31(16~47)
  PLL参考时钟范围: 8MHz~16MHz
  PLL参考时钟 = 外部参考时钟(CPU_XTAL_CLK_HZ)/prdiv
  PLL输出时钟范围: 90MHz~180MHz
  PLL输出时钟 = PLL参考时钟 x vdiv（PLL倍频系数）/2
  CoreClk = PLL输出时钟 /OUTDIV1
 *************************************************
*/
  
  // 对于MK60F12来说，core_clk_mhz建议不要超过120，这里限制为最高200
  // 对于MK60F15来说，core_clk_mhz建议不要超过150，这里限制为最高200
  core_clk_mhz = (PllOptionEnum_Type)(core_clk_mhz>200u?200u:core_clk_mhz);
  // 根据期望主频选择分频和倍频系数
  switch(core_clk_mhz)
  {
  case PLL_100:
    prdiv = 4u;
    vdiv = 4u;
    break;
  case PLL_120:
    prdiv = 4u;
    vdiv = 8u;
    break;
  case PLL_150:
    prdiv = 4u;
    vdiv = 14u;
    break;
  case PLL_180:
    prdiv = 4u;
    vdiv = 20u;
    break;
  case PLL_200:
    prdiv = 3u;
    vdiv = 0u;
    break;
  default:
    return LPLD_PLL_Setup(PLL_120);
  } 
#endif
  
  pll_freq = core_clk_mhz * 1;
  core_div = 0;
  if((bus_div = (uint8)(core_clk_mhz/BUS_CLK_MHZ - 1u)) == (uint8)-1)
  {
    bus_div = 0;
  }
  else if(core_clk_mhz/(bus_div+1) > BUS_CLK_MHZ)
  {
    bus_div += 1;
  }
  if((flexbus_div = (core_clk_mhz/FLEXBUS_CLK_MHZ - 1u)) == (uint8)-1)
  {
    flexbus_div = 0;
  }
  else if(core_clk_mhz/(flexbus_div+1) > FLEXBUS_CLK_MHZ)
  {
    flexbus_div += 1;
  }
  if((flash_div = (core_clk_mhz/FLASH_CLK_MHZ - 1u)) == (uint8)-1)
  {
    flash_div = 0;
  }
  else if(core_clk_mhz/(flash_div+1) > FLASH_CLK_MHZ)
  {
    flash_div += 1;
  }
 
#if (defined(CPU_MK60DZ10)) 
  // 这里假设复位后 MCG 模块默认为 FEI 模式
  // 首先移动到 FBE 模式
  MCG->C2 = 0;
  // 振荡器初始化完成后,释放锁存状态下的 oscillator 和 GPIO 
  SIM->SCGC4 |= SIM_SCGC4_LLWU_MASK;
  LLWU->CS |= LLWU_CS_ACKISO_MASK;
  // 选择外部 oscilator 、参考分频器 and 清零 IREFS 启动外部osc
  // CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);  
  while (MCG->S & MCG_S_IREFST_MASK){}; // 等待参考时钟清零
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // 等待时钟状态显示为外部参考时钟(ext ref clk)
  // 进入FBE模式
  // 配置 PLL 参考分频器, PLLCLKEN=0, PLLSTEN=0, PRDIV=5
  // 用晶振频率来选择 PRDIV 值. 仅在有频率晶振的时候支持
  // 产生 2MHz 的参考时钟给 PLL.
  MCG->C5 = MCG_C5_PRDIV(prdiv); // 设置 PLL 匹配晶振的参考分频数 
  // 确保MCG_C6处于复位状态,禁止LOLIE、PLL、和时钟控制器,清PLL VCO分频器
  MCG->C6 = 0x0;
  //设置系统时钟分频系数
  LPLD_Set_SYS_DIV(core_div, bus_div, flexbus_div, flash_div);  
  //设置倍频系数
  MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(vdiv); 
  while (!(MCG->S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
  while (!(MCG->S & MCG_S_LOCK_MASK)){}; // Wait for LOCK bit to set
  // 已经进入PBE模式
  // 清零CLKS 进入PEE模式
  MCG->C1 &= ~MCG_C1_CLKS_MASK;
  // Wait for clock status bits to update
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
  // 已经进入PEE模式
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
  if (PMC->REGSC &  PMC_REGSC_ACKISO_MASK)
        PMC->REGSC |= PMC_REGSC_ACKISO_MASK;
  /*注意：PLL初始化不会配置系统时钟分频，
    所以系统时钟分频必须在PLL之前调用*/ 
  SIM->CLKDIV1 = ( 0
                  | SIM_CLKDIV1_OUTDIV1(core_div)   //设置内核分频
                  | SIM_CLKDIV1_OUTDIV2(bus_div)   //设置外设分频
                  | SIM_CLKDIV1_OUTDIV3(flexbus_div)   //设置FlexBus分频
                  | SIM_CLKDIV1_OUTDIV4(flash_div) );//设置FLASH分频
  //初始化各部分时钟：系统内核主频、总线时钟、FlexBus时钟、Flash时钟
  // 这里假设复位后 MCG 模块默认为 FEI 模式
  // 首先移动到 FBE 模式
  MCG->C2 = MCG_C2_RANGE0(1);
  // 选择外部 oscilator 、参考分频器 and 清零 IREFS 启动外部osc
  // CLKS=2, FRDIV=5, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(5);
  while (MCG->S & MCG_S_IREFST_MASK){}; // 等待参考时钟清零
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // 等待时钟状态显示为外部参考时钟(ext ref clk)
  // 进入FBE模式
  // 配置 PLL 参考分频器, PLLCLKEN=0, PLLSTEN=0, PRDIV
  // 用晶振频率来选择 PRDIV 值. 仅在有频率晶振的时候支持
  // 产生 10MHz 的参考时钟给 PLL.
  MCG->C6 |= MCG_C6_CME0_MASK;
  MCG->C5 = MCG_C5_PRDIV0(prdiv); // 设置 PLL 匹配晶振的参考分频数
  MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(vdiv); 
  while (!(MCG->S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
  while (!(MCG->S & MCG_S_LOCK0_MASK)){}; // Wait for LOCK bit to set
  // 已经进入PBE模式
  // 清零CLKS 进入PEE模式
  MCG->C1 &= ~MCG_C1_CLKS_MASK;
  // Wait for clock status bits to update
  while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
  // 已经进入PEE模式
#endif  
  return pll_freq;
} 

/*
 * LPLD_Set_SYS_DIV
 * 设置系统始终分频
 *
 * 说明:
 * 这段代码必须放置在RAM中，目的是防止程序跑飞，详见官方文档errata e2448.
 * 当Flash时钟分频改变的时候，Flash预读取必须禁用.
 * 禁止从Flash中运行以下代码.
 * 在预读取被重新使能之前必须在时钟分频改变后有一段小的延时.
 *
 * 参数:
 *    outdiv1~outdiv4--分别为core, bus, FlexBus, Flash时钟分频系数
 */
RAMFUNC void LPLD_Set_SYS_DIV(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
{
  uint32 temp_reg;
  uint8 i;
  
  temp_reg = FMC->PFAPR; // 备份 FMC_PFAPR 寄存器
  
  // 设置 M0PFD 到 M7PFD 为 1 禁用预先读取
  FMC->PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
             | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
             | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
  
  // 设置时钟分频为期望值  
  SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2) 
              | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);

  // 延时一小段时间等待改变
  for (i = 0 ; i < outdiv4 ; i++)
  {}
  
  FMC->PFAPR = temp_reg; // 回复原先的 FMC_PFAPR 寄存器值
  
  return;
} // set_sys_dividers

/*******************************************
*         MK60F12 & 15
*  MCGOUT = PLL output frequency/2
*  PLL = (Crystal / PRDIV0 ) * VDIV0
********************************************
PRDIV0 Divide Factor
********************************************
PRDIV0  Va
********************************************
000     1
001     2
010     3
011     4
100     5
101     6
110     7
111     8
********************************************
VDIV0 MultiplyFactor
********************************************
VDIV0 Va VDIV0 Va VDIV0 Va VDIV0 Va
********************************************
00000 16 01000 24 10000 32 11000 40
00001 17 01001 25 10001 33 11001 41
00010 18 01010 26 10010 34 11010 42
00011 19 01011 27 10011 35 11011 43
00100 20 01100 28 10100 36 11100 44
00101 21 01101 29 10101 37 11101 45
00110 22 01110 30 10110 38 11110 46
00111 23 01111 31 10111 39 11111 47
********************************************/

/********************************************
*             MK60DZ10
*  MCGOUT = PLL output frequency
*  PLL = (Crystal / PRDIV0 ) * VDIV0
*********************************************
PRDIV DivideFactor
*********************************************
PRDIV0 Va PRDIV0 Va PRDIV0 Va PRDIV0 Va
*********************************************
00000  1  01000   9 10000  17 11000  25
00001  2  01001  10 10001  18 11001  Reserved
00010  3  01010  11 10010  19 11010  Reserved
00011  4  01011  12 10011  20 11011  Reserved
00100  5  01100  13 10100  21 11100  Reserved
00101  6  01101  14 10101  22 11101  Reserved
00110  7  01110  15 10110  23 11110  Reserved
00111  8  01111  16 10111  24 11111  Reserved
*********************************************
VDIV MultiplyFactor
*********************************************
VDIV0 Va VDIV0 Va VDIV0 Va VDIV0 Va
*********************************************
00000 24 01000 32 10000 40 11000 48
00001 25 01001 33 10001 41 11001 49
00010 26 01010 34 10010 42 11010 50
00011 27 01011 35 10011 43 11011 51
00100 28 01100 36 10100 44 11100 52
00101 29 01101 37 10101 45 11101 53
00110 30 01110 38 10110 46 11110 54
00111 31 01111 39 10111 47 11111 55
*********************************************/
