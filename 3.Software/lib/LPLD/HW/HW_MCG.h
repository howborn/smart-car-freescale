/**
 * @file HW_MCG.h
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
#ifndef __HW_MCG_H__
#define __HW_MCG_H__

#include "common.h"
   

typedef enum PllOptionEnum
{
  PLL_48=48u,    //如果使用USB模块，必须选择48的倍数主频
  PLL_50=50u,
  PLL_96=96u,    //如果使用USB模块，必须选择48的倍数主频
  PLL_100=100u,  //100MHz是MK60D(Z)10的额定最高主频，再往上飞思卡尔可不质保 
  PLL_120=120u,  //120MHz是MK60F12的额定最高主频
  PLL_150=150u,  //150MHz是MK60F15的额定最高主频
  PLL_180=180u,
  PLL_200=200u   //如果超到200，发热很正常，烧了别找我>_>
  //高于200Mhz的内核频率不建议使用，MK60到不了煎蛋的水平^_^ 
} PllOptionEnum_Type;

//初始化内核时钟及其他系统时钟
extern uint8 LPLD_PLL_Setup(PllOptionEnum_Type);
//设置系统时钟分频
RAMFUNC void LPLD_Set_SYS_DIV(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4);
   

extern uint32 g_core_clock;
extern uint32 g_bus_clock;
extern uint32 g_flash_clock;
extern uint32 g_flexbus_clock;   


#endif /* __HW_MCG_H__ */