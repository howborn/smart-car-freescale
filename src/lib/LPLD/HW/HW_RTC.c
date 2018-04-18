/**
 * @file HW_RTC.c
 * @version 3.01[By LPLD]
 * @date 2013-10-4
 * @brief RTC底层模块相关函数
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
#include "HW_RTC.h"

//用户自定义中断服务函数
RTC_ISR_CALLBACK RTC_ISR[3];

/*
 * LPLD_RTC_Init
 * RTC通用初始化函数,在该函数需要设置RTC秒计数器，如果需要报警功能
 * 需要使能报警中断，并设置报警寄存器
 * 
 * 参数:
 *    rtc_init_structure--RTC初始化结构体，
 *                        具体定义见RTC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_RTC_Init(RTC_InitTypeDef rtc_init_structure)
{
  int delay;
  uint32 seconds = rtc_init_structure.RTC_Seconds;
  uint32 alarm_time = rtc_init_structure.RTC_AlarmTime;
  RTC_ISR_CALLBACK invalid_isr_func = rtc_init_structure.RTC_InvalidIsr;
  RTC_ISR_CALLBACK overflow_isr_func = rtc_init_structure.RTC_OverflowIsr;
  RTC_ISR_CALLBACK alarm_isr_func = rtc_init_structure.RTC_AlarmIsr;

  SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
  //复位所有RTC寄存器 除了SWR位 RTC_WAR和RTC_RAR寄存器
  //在VBAT POR设置之后,用软件清除SWR
  //复位RTC寄存器
  RTC->CR  = RTC_CR_SWR_MASK; 
  //清除RTC复位标志  
  RTC->CR  &= ~RTC_CR_SWR_MASK;  
  //使能RTC 32.768 kHzRTC时钟源
  //使能之后要延时一段时间等待
  //等待时钟稳定后在配置RTC时钟计数器
  RTC->CR |= RTC_CR_OSCE_MASK;
  
  //等待32.768时钟起振
  for(delay = 0;delay < 0x600000 ;delay++);
  
  if(rtc_init_structure.RTC_InvalidIntEnable == TRUE)
  { 
    RTC->IER |= (RTC_INT_INVALID & 0x07);//开启RTC无效数据中断
    RTC_ISR[0] = invalid_isr_func;
  }
  else
  {
    RTC->IER &= ~RTC_INT_INVALID;
  }

  if(rtc_init_structure.RTC_OverflowIntEnable == TRUE)
  { 
    RTC->IER |= (RTC_INT_OVERFLOW & 0x07);//开启RTC计数器溢出中断
    RTC_ISR[1] = overflow_isr_func;
  }
  else
  {
    RTC->IER &= ~RTC_INT_OVERFLOW;
  }

  if(rtc_init_structure.RTC_AlarmIntEnable == TRUE)
  {   
    RTC->IER |= (RTC_INT_ALARM & 0x07);//开启RTC报警中断
    RTC_ISR[2] = alarm_isr_func;
  }
  else
  {
    RTC->IER &= ~RTC_INT_ALARM;
  }

  //设置时钟补偿寄存器
  RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);
  
  //配置秒计数器
  RTC->TSR = seconds;
    
  //配置报警寄存器
  RTC->TAR = alarm_time;
  
  //使能秒计数器
  RTC->SR |= RTC_SR_TCE_MASK;
  
  return 1;
}

/*
 * LPLD_RTC_Deinit
 * RTC反初始化函数，关闭RTCx并禁止RTC中断
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_RTC_Deinit(void)
{
  //禁止RTC模块中断
  disable_irq(RTC_IRQn);
  //关闭RTC
  RTC->SR &= (~RTC_SR_TCE_MASK);
  //关闭RTC总线时钟
  SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;  
}

/*
 * LPLD_RTC_IsRunning
 * 判断RTC是否运行函数
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    1，RTC计数器正在运行
 *    0，RTC计数器以复位
 */
uint8 LPLD_RTC_IsRunning(void)
{
  uint8 request;
  SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
  
  if(RTC->SR & RTC_SR_TIF_MASK)
  {
    request = 0;
  }
  else
  {
    request = 1;
  }
  return request;
}

/*
 * RTC中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void RTC_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  if((RTC->SR & RTC_SR_TIF_MASK)== 0x01)
  {
    //调用用户自定义中断服务
    RTC_ISR[0](); 
    //清除中断标志位
    RTC->SR |= RTC_SR_TIF_MASK;
    
  }	
  else if((RTC->SR & RTC_SR_TOF_MASK) == 0x02)
  {
    //调用用户自定义中断服务
    RTC_ISR[1]();  
    //清除中断标志位
    RTC->SR |= RTC_SR_TOF_MASK;
  }	 	
  else if((RTC->SR & RTC_SR_TAF_MASK) == 0x04)
  {
    //调用用户自定义中断服务
    RTC_ISR[2]();  
    //清除中断标志位
    RTC->SR |= RTC_SR_TAF_MASK;
  }	
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
