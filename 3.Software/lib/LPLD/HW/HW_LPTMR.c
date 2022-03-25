/**
 * @file HW_LPTMR.c
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

#include "common.h"
#include "HW_LPTMR.h"

LPTMR_ISR_CALLBACK LPTMR_ISR[1];

/*
 * LPLD_LPTMR_Init
 * LPTMR初始化函数
 * 
 * 参数:
 *    lptmr_init_structure--LPTMR初始化结构体，
 *                         具体定义见LPTMR_InitTypeDef 
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_LPTMR_Init(LPTMR_InitTypeDef lptmr_init_structure)
{
  uint8  mode = lptmr_init_structure.LPTMR_Mode; 
  uint16 period_ms = lptmr_init_structure.LPTMR_PeriodMs;
  uint8  pluseacc_input = lptmr_init_structure.LPTMR_PluseAccInput;
  LPTMR_ISR_CALLBACK isr_func = lptmr_init_structure.LPTMR_Isr;

  ASSERT(mode <= LPTMR_MODE_PLACC );
  ASSERT(pluseacc_input <= LPTMR_ALT2 );

  //初始化LPTMR时钟
  SIM->SCGC5|=SIM_SCGC5_LPTIMER_MASK;  
   
  //清空寄存器
  LPTMR0->CSR = 0x00;                   
  LPTMR0->PSR = 0x00;
  LPTMR0->CMR = 0x00;
   
  if(mode == LPTMR_MODE_TIMER)          // 配置成低功耗定时器功能
  {    
    /* 配置LPTMR */
    // LPO 1Khz
    //设置比较的值
    //使用内部时钟 分频系数Prescale为 2^(X+1)
    //定时器的一个计数周期=时钟源(Clk_Src)/ 预分频值(Prescale)
    //定时器的定时值=设置的比较值(compare_value)/定时器的一个计数周期
    LPTMR0->CMR = LPTMR_CMR_COMPARE(period_ms);                 //设置比较值
    LPTMR0->PSR = LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;       //采用LPO时钟旁路使能
        
  }
  else if(mode == LPTMR_MODE_PLACC)             //配置成脉冲捕获模式
  {
    
    if(pluseacc_input == LPTMR_ALT1)            //PTA19引脚
    {
      PORTA->PCR[19] = PORT_PCR_MUX(0x6);       //设置PTA19为脉冲累加
    }
    else if(pluseacc_input == LPTMR_ALT2)       //PTC5引脚
    {
      PORTC->PCR[5] = PORT_PCR_MUX(0x4);        //设置PTC5为脉冲累加  
    }

    //与所用的内部时钟无关
    //外部输入的信号作为计数时钟
    LPTMR0->PSR = LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
    LPTMR0->CSR = LPTMR_CSR_TPS(pluseacc_input);
    LPTMR0->CSR |= LPTMR_CSR_TMS_MASK; //捕获模式
    LPTMR0->CSR |= LPTMR_CSR_TFC_MASK; //自由计数模式
  }
  
  if(lptmr_init_structure.LPTMR_IntEnable == TRUE && isr_func != NULL)
  {
    LPTMR_ISR[0] = isr_func;
    LPTMR0->CSR |= LPTMR_CSR_TIE_MASK;
  }

  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK; //开启LPTMR
  return 1;
}

/*
 * LPLD_LPTMR_DeInit
 * LPTMR反初始化函数，关闭lptmrx
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_LPTMR_Deinit(void)
{
  //清空CSR寄存器
  LPTMR0->CSR=0x00;  
  //清空PSR寄存器
  LPTMR0->PSR=0x00;
  //清空CMR寄存器
  LPTMR0->CMR=0x00;
  //禁止LPTimer中断
  disable_irq(LPTimer_IRQn);
  
   //关闭LPTMR时钟
  SIM->SCGC5 &= ~(SIM_SCGC5_LPTIMER_MASK);  
  
  return 1;
}

/*
 *  LPLD_LPTMR_Reset
 *  低功耗定时器复位，复位以后清空低功耗定时器Counter
 * 
 *  参数:
 *      无
 *  输出:
 *      无
 */
void LPLD_LPTMR_ResetCounter(void)
{
  LPTMR0->CSR&=(~LPTMR_CSR_TEN_MASK);       //Reset LPTMR0 Counter
}
/*
 * LPLD_LPTMR_EnableIrq
 * 使能LPTMR中断功能，在该函数中使能TIE位和LPTMR模块中断
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_LPTMR_EnableIrq(void)
{
  enable_irq(LPTimer_IRQn);
  return 1;
}

/*
 * LPLD_LPTMR_DisableIrq
 * 禁止LPTMR中断
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_LPTMR_DisableIrq(void)
{
  disable_irq(LPTimer_IRQn); 
  return 1;
}

/*
 *  LPLD_LPTMR_GetPulseAcc
 *  得到脉冲累加的值
 * 
 *  参数:
 *      无
 *  输出:
 *      脉冲累加值
 */
uint16 LPLD_LPTMR_GetPulseAcc(void)
{
   return (uint16)LPTMR0->CNR;
}

/*
 *  LPLD_LPTMR_DelayMs
 *  低功耗定时器延时n个ms
 * 
 * 参数:
 *    period_ms--设定定时器的定时值 单位ms
 *    最大65535ms
 *
 *  输出:
 *      无
 */
void LPLD_LPTMR_DelayMs(uint16 period_ms)
{
  if(!period_ms)
    return;
  
  /* 开启LPTMR时钟 */
  SIM->SCGC5|=SIM_SCGC5_LPTIMER_MASK; 
  
  /* 设置毫秒延时计数 */
  LPTMR0->CMR = period_ms; 
  
  /* 设置LPTMR不分频采用1kHz LPO为时钟源 */
  LPTMR0->PSR = 0|LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK; 
  
  /* 开启定时器 */
  LPTMR0->CSR = LPTMR_CSR_TEN_MASK; 

  /* 等待计数达到设定值 */
  while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));
  
  /*清除定时比较标志 */
  LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
  
}


/*
 * LPTMR中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void LPT_IRQHandler(void)
{
  LPTMR0->CSR|=LPTMR_CSR_TCF_MASK;  //清除LPT比较标志
  LPTMR_ISR[0]();
}
