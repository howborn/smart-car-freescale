/**
 * @file HW_PDB.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief PDB底层模块相关函数
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
#ifndef __HW_PDB_H__
#define __HW_PDB_H__
#include "common.h"

//PDB模块中断回调函数类型
typedef void (*PDB_ISR_CALLBACK)(void);

//PDB_InitTypeDef->PDB_TriggerInputSourceSel变量取值定义：触发模式
#define TRIGGER_EXTERNAL        0
#define TRIGGER_CMP0            1
#define TRIGGER_CMP1            2
#define TRIGGER_CMP2            3
#define TRIGGER_PIT0            4
#define TRIGGER_PIT1            5
#define TRIGGER_PIT2            6
#define TRIGGER_PIT3            7
#define TRIGGER_FTM0            8
#define TRIGGER_FTM1            9
#define TRIGGER_FTM2            10
#define TRIGGER_RESERVED        11
#define TRIGGER_RTCALARM        12
#define TRIGGER_RTCSECONDS      13
#define TRIGGER_LPTMR           14
#define TRIGGER_SOFTWARE        15

//PDB_InitTypeDef->PDB_LoadModeSel变量取值定义：LDMOD加载模式
#define LOADMODE_0       0
#define LOADMODE_1       1
#define LOADMODE_2       2
#define LOADMODE_3       3

//LPLD_PDB_AdcTriggerCfg函数cfg形参取值定义：使能ADCx通道CHn、是否延时输出、是否BB模式
#define PRETRIG_EN_A            PDB_C1_EN(0x01)
#define PRETRIG_EN_B            PDB_C1_EN(0x02)
#define PRETRIG_DLY_A           PDB_C1_TOS(0x01)
#define PRETRIG_DLY_B           PDB_C1_TOS(0x02)
#define PRETRIG_BB_A            PDB_C1_BB(0x01)
#define PRETRIG_BB_B            PDB_C1_BB(0x02)

//LPLD_PDB_DacTriggerCfg函数cfg形参取值定义：使能DACx间隔触发、是否外部触发
#define EXT_TRIG_EN             (PDB_INTC_EXT_MASK<<PDB_INTC_EXT_SHIFT)
#define INT_TRIG_EN             (PDB_INTC_TOE_MASK<<PDB_INTC_TOE_SHIFT)


//PDB模块初始化结构体，用于配置PDB各项参数
typedef struct
{  
  /*
    描述：
      设置PDBx计数器溢出周期，单位us
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，PDB_CounterPeriodUs、PDB_CounterPeriodMs、PDB_CounterPeriodS至少一个初始化
  */
  uint32 PDB_CounterPeriodUs;
  
  /*
    描述：
      设置PDBx计数器溢出周期，单位ms
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，PDB_CounterPeriodUs、PDB_CounterPeriodMs、PDB_CounterPeriodS至少一个初始化
  */
  uint32 PDB_CounterPeriodMs;
  
  /*
    描述：
      设置PDBx计数器溢出周期，单位s
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，PDB_CounterPeriodUs、PDB_CounterPeriodMs、PDB_CounterPeriodS至少一个初始化
  */
  uint32 PDB_CounterPeriodS;
  
  /*
    描述：
      选择PDBx模块MOD、IDLY、CHnDLYm、INTx和POyDLY寄存器的加载模式选择
    取值：
      LOADMODE_0-模式0，当LDOK写1后立即加载数值到上述寄存器内
      LOADMODE_1-模式1，当LDOK写1后且PDBx计数器值等于MOD寄存器值时加载数值到上述寄存器内
      LOADMODE_2-模式2，当LDOK写1后且一个输入触发事件被检测到时加载数值到上述寄存器内
      LOADMODE_3-模式3，当LDOK写1后且(PDBx计数器值等于MOD寄存器值 或 一个输入触发事件被检测到)时加载数值到上述寄存器内
    初始化：
      不必须初始化，默认LOADMODE_0
  */
  uint8 PDB_LoadModeSel;
  
  /*
    描述：
      设置PDBx计数器开始工作的输入触发源
    取值：
      TRIGGER_EXTERNAL        -外部触发
      TRIGGER_CMP0            -CMP0模块触发
      TRIGGER_CMP1            -CMP1模块触发
      TRIGGER_CMP2            -CMP2模块触发
      TRIGGER_PIT0            -PIT0模块触发
      TRIGGER_PIT1            -PIT1模块触发
      TRIGGER_PIT2            -PIT2模块触发
      TRIGGER_PIT3            -PIT3模块触发
      TRIGGER_FTM0            -FTM0模块触发
      TRIGGER_FTM1            -FTM1模块触发
      TRIGGER_FTM2            -FTM2模块触发
      TRIGGER_RESERVED        -保留
      TRIGGER_RTCALARM        -RTC Alarm触发
      TRIGGER_RTCSECONDS      -RTC Seconds触发
      TRIGGER_LPTMR           -LPTMR模块触发
      TRIGGER_SOFTWARE        -软件触发
    初始化：
      不必须初始化，默认TRIGGER_EXTERNAL
  */
  uint8 PDB_TriggerInputSourceSel;
  
  /*
    描述：
      使能PDBx连续工作模式，使能后PDBx将在第一次触发后连续工作
    取值：
      TRUE-使能连续
      FALSE-禁用连续
    初始化：
      不必须初始化，默认FALSE
  */
  boolean PDB_ContinuousModeEnable;
  
  /*
    描述：
      使能PDBx模块DMA请求
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean PDB_DmaEnable;
  
  /*
    描述：
      使能PDBx模块中断，时能后当PDBx计数器计数时间等于PDB_Delay后触发中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean PDB_IntEnable;
    
  /*
    描述：
      使能PDBx模块序列错误中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */  
  boolean PDB_SeqErrIntEnable;
  
  /*
    描述：
      设置PDBx延时寄存器溢出时间，单位us
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化
  */
  uint32 PDB_DelayUs;
  
  /*
    描述：
      设置PDBx延时寄存器溢出时间，单位ms
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化
  */
  uint32 PDB_DelayMs;
  
  /*
    描述：
      设置PDBx延时寄存器溢出时间，单位s
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化
  */
  uint32 PDB_DelayS;

  /*
    描述：
      PDBx中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  PDB_ISR_CALLBACK PDB_Isr; 

  /*
    描述：
      PDBx序列错误中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  PDB_ISR_CALLBACK PDB_SeqErrIsr; 
  
} PDB_InitTypeDef;

//使能PDBx中断
#define LPLD_PDB_EnableIrq()    enable_irq(PDB0_IRQn)
//禁用PDBx中断
#define LPLD_PDB_DisableIrq()   disable_irq(PDB0_IRQn)
//软件触发PDBx计数器开始工作
#define LPLD_PDB_SoftwareTrigger()      (PDB0->SC|=PDB_SC_SWTRIG_MASK)

//PDBx模块通用初始化函数，初始化PDBx计数器周期、工作模式、触发源、中断等配置
uint8 LPLD_PDB_Init(PDB_InitTypeDef);
//PDBx模块反初始化函数，禁用PDBx所有功能
void LPLD_PDB_Deinit(void);
//PDBx触发ADCx模块配置函数
uint8 LPLD_PDB_AdcTriggerCfg(ADC_Type *, uint32, uint16);
//PDBx触发DACx模块配置函数
uint8 LPLD_PDB_DacTriggerCfg(DAC_Type *, uint32, uint16);

#endif /* __HW_PDB_H__ */