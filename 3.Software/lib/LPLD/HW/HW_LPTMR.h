/**
 * @file HW_LPTMR.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief LPTMR底层模块相关函数
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
#ifndef __HW_LPTMR_H__
#define __HW_LPTMR_H__
/********************************************************************/

//定义LPTMR定时器模式
#define LPTMR_MODE_TIMER    0
#define LPTMR_MODE_PLACC    1

//定义LPTMR脉冲累加通道
#define CMP0_OUTPUT         0
#define LPTMR_ALT1          1
#define LPTMR_ALT2          2

//LPTMR模块中断回调函数类型
typedef void (*LPTMR_ISR_CALLBACK)(void);

typedef struct 
{
  /*
    描述：
      选择LPTMR模式
    取值：
      LPTMR_MODE_TIMER--低功耗定时器模式
      LPTMR_MODE_PLACC--脉冲累加模式
    初始化：
      必须初始化 
  */
  uint8  LPTMR_Mode;
  
  /*
    描述：
      设定定时器的定时值 单位ms
    取值：
      1~65535--1ms<= 定时周期  <=65535ms
    初始化：
      如果选择LPTMR_MODE_PLACC脉冲累加模式，不必初始化 
      如果选择LPTMR_MODE_TIMER低功耗定时器模式，需要设置定时器周期
  */
  uint16 LPTMR_PeriodMs;
  
  /*
    描述：
      选择LPTMR脉冲累加模式下的输入IO
    取值：
      CMP0_OUTPUT-CMP0模块输出
      LPTMR_ALT1-PTA19引脚 
      LPTMR_ALT2-PTC5引脚
    初始化：
      不必须初始化、默认CMP0_OUTPUT
  */
  uint8  LPTMR_PluseAccInput;
    
  /*
    描述：
      使能LPTMR中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认值FALSE
  */
  boolean LPTMR_IntEnable;
  
  /*
    描述：
      LPTMR中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  LPTMR_ISR_CALLBACK LPTMR_Isr;

}LPTMR_InitTypeDef;

//初始化LPTMR工作模式，脉冲输入通道
uint8 LPLD_LPTMR_Init(LPTMR_InitTypeDef);
//LPTMR反初始化函数
uint8 LPLD_LPTMR_Deinit(void);
//LPTMR复位Counter
void LPLD_LPTMR_ResetCounter(void);
//LPTMR中断使能函数
uint8 LPLD_LPTMR_EnableIrq(void);
//LPTMR禁止使能函数
uint8 LPLD_LPTMR_DisableIrq(void);
//LPTMR获得捕获值函数
uint16 LPLD_LPTMR_GetPulseAcc(void);
//LPTMR ms延时函数
void LPLD_LPTMR_DelayMs(uint16);

#endif /* __HW_LPTMR_H__ */
