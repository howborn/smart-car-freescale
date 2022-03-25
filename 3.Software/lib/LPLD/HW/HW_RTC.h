/**
 * @file HW_RTC.h
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
#ifndef __HW_RTC_H__
#define __HW_RTC_H__

//RTC模块中断回调函数类型
typedef void (*RTC_ISR_CALLBACK)(void);

//定义RTC中断方式
#define RTC_INT_DIS           (0)
#define RTC_INT_INVALID       (1)
#define RTC_INT_OVERFLOW      (2)
#define RTC_INT_ALARM         (4)

typedef struct 
{
  /*
    描述：
      设置RTC秒计数器起始值
    取值：
      如果计数器设置为0，在不复位的情况下可计数2的32次方秒，约136年
    初始化：
      必须初始化
  */
  uint32 RTC_Seconds; 
  
  /*
    描述：
      设置报警值
    取值：
      当报警值等于RTC_TSR，设置TAF标志位，可触发中断
    初始化：
      如果需要报警功能，必须初始化
  */
  uint32 RTC_AlarmTime; 
  
  /*
    描述：
      开启RTC 无效数据中断
    取值：
      TRUE - 开启RTC无效数据中断
      FALSE - 关闭RTC无效数据中断
    初始化：
      不必须初始化，默认值FLASE
  */
  boolean  RTC_InvalidIntEnable; 
  
  /*
    描述：
      开启RTC 计数器溢出中断
    取值：
      TRUE - 开启RTC计数器溢出中断
      FALSE - 关闭RTC计数器溢出中断
    初始化：
      不必须初始化，默认值FLASE
  */
  boolean  RTC_OverflowIntEnable;
  
  /*
    描述：
      开启RTC 报警中断
    取值：
      TRUE - 开启RTC计数器溢出中断
      FALSE - 关闭RTC计数器溢出中断
    初始化：
      不必须初始化，默认值RTC_INT_DIS 关闭RTC中断
  */
  boolean  RTC_AlarmIntEnable; 
  
  /*
    描述：
      无效数据中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  RTC_ISR_CALLBACK RTC_InvalidIsr;
  
  /*
    描述：
      计数溢出中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  RTC_ISR_CALLBACK RTC_OverflowIsr;
  
  /*
    描述：
      报警中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  RTC_ISR_CALLBACK RTC_AlarmIsr;

}RTC_InitTypeDef;

//RTC使能模块中断
#define LPLD_RTC_EnableIrq()    enable_irq(RTC_IRQn)
//RTC禁止模块中断
#define LPLD_RTC_DisableIrq()   disable_irq(RTC_IRQn)
//RTC获得实时秒计数函数
#define LPLD_RTC_GetRealTime()  (RTC->TSR)
//RTC重新设置秒计数器函数
#define LPLD_RTC_SetRealTime(TIME)     (RTC->TSR=TIME)
//RTC重新设置报警寄存器函数
#define LPLD_RTC_SetAlarmTime(TIME)    (RTC->TAR=TIME)
//RTC关闭函数
#define LPLD_RTC_Stop()         (RTC->SR&=(~RTC_SR_TCE_MASK))


//RTC初始化函数
uint8 LPLD_RTC_Init(RTC_InitTypeDef );
//RTC反初始化函数，关闭选择的RTCx
void LPLD_RTC_Deinit(void );
//判断RTC是否运行函数
uint8 LPLD_RTC_IsRunning(void);     
#endif /* __HW_RTC_H__ */
