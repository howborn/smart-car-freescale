/**
 * @file HW_TSI.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief TSI底层模块相关函数
 *
 * 更改建议:不建议修改代码，见注释可酌情修改部分值
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
#ifndef __HW_TSI_H__
#define __HW_TSI_H__

/********用户可修改值 开始***********/
//TSI电极阈值窗口值
//本值根据经验值设定，用户可以酌情修改
#define TSI_OVERRUN_VAL (0x30000)

//TSI电极触摸窗口值
//本值根据经验值设定，用户可以酌情修改
#define TSI_TOUCH_VAL   (0x800)

//TSI电极未触摸基础值
//本值根据经验值设定，如果开启初始化自检则无需此值
#define TSI_BASE_VAL    (0x800)
/********用户可修改值 结束***********/

//TSI模块中断回调函数类型
typedef void (*TSI_ISR_CALLBACK)(void);

//TSI通道掩码定义
typedef enum TsiChxEnum
{
  TSI_Ch0  = 0x0001,    //PTB0
  TSI_Ch1  = 0x0002,    //PTA0
  TSI_Ch2  = 0x0004,    //PTA1
  TSI_Ch3  = 0x0008,    //PTA2
  TSI_Ch4  = 0x0010,    //PTA3
  TSI_Ch5  = 0x0020,    //PTA4
  TSI_Ch6  = 0x0040,    //PTB1
  TSI_Ch7  = 0x0080,    //PTB2
  TSI_Ch8  = 0x0100,    //PTB3
  TSI_Ch9  = 0x0200,    //PTB16
  TSI_Ch10 = 0x0400,    //PTB17
  TSI_Ch11 = 0x0800,    //PTB18
  TSI_Ch12 = 0x1000,    //PTB19
  TSI_Ch13 = 0x2000,    //PTC0
  TSI_Ch14 = 0x4000,    //PTC1
  TSI_Ch15 = 0x8000     //PTC2
}TsiChxEnum_Type;

//TSI_InitTypeDef结构体TSI_ScanTriggerMode取值
#define TSI_SCAN_SOFT   0       //软件触发扫描
#define TSI_SCAN_PERIOD 1       //周期触发扫描
//TSI_InitTypeDef结构体TSI_EndScanOrOutRange取值
#define TSI_OUTOFRANGE_INT 0    //范围溢出中断
#define TSI_ENDOFDCAN_INT  1    //扫描结束中断

typedef struct 
{
  /*
    描述：
      选择要初始化的通道
    取值：
      TSI_Ch0~TSI_Ch15-对应的引脚见TsiChxEnum_Type枚举注释
    初始化：
      必须初始化
  */
  uint16 TSI_Chs;
  
  /*
    描述：
      选择触发扫描的模式
    取值：
      TSI_SCAN_SOFT-软件触发扫描
      TSI_SCAN_PERIOD-周期触发扫描
    初始化：
      不必须初始化，默认TSI_SCAN_SOFT
  */
  uint8 TSI_ScanTriggerMode;
  
  /*
    描述：
      选择扫描结束触发中断还是范围溢出触发中断
    取值：
      TSI_OUTOFRANGE_INT-范围溢出中断
      TSI_ENDOFDCAN_INT-扫描结束中断
    初始化：
      不必须初始化，默认TSI_OUTOFRANGE_INT
  */ 
  uint8 TSI_EndScanOrOutRangeInt;
  
  /*
    描述：
      使能错误中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */ 
  boolean TIS_ErrIntEnable;

  /*
    描述：
      扫描结束中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  TSI_ISR_CALLBACK TSI_EndScanIsr;

  /*
    描述：
      范围溢出中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  TSI_ISR_CALLBACK TSI_OutRangeIsr;

  /*
    描述：
      错误中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  TSI_ISR_CALLBACK TSI_ErrIsr;
  
  /*
    描述：
      是否使能初始化自调准
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */ 
  boolean TSI_IsInitSelfCal;
  
}TSI_InitTypeDef;

//清除扫描结束标志
#define LPLD_TSI_ClearEndScanFlag()   (TSI0->GENCS|=TSI_GENCS_EOSF_MASK)
//清除范围溢出标志
#define LPLD_TSI_ClearOutRangeFlag()  (TSI0->GENCS|=TSI_GENCS_OUTRGF_MASK)
//清除通道CHX的扫描接触标志，CHX取值TSI_Ch0~TSI_Ch15
#define LPLD_TSI_ClearEndScanFlagChx(CHX)   (TSI0->STATUS|=CHX)
//清除通道CHX的错误标志，CHX取值TSI_Ch0~TSI_Ch15
#define LPLD_TIS_ClrarErrorFlagChx(CHX)     (TSI0->STATUS|=(CHX<<16))
//通道CHX是否范围溢出，CHX取值TSI_Ch0~TSI_Ch15
#define LPLD_TSI_IsChxOutRange(CHX)   (TSI0->STATUS&CHX)
//通道CHX是否错误，CHX取值TSI_Ch0~TSI_Ch15
#define LPLD_TSI_IsChxError(CHX)      (TSI0->STATUS&(CHX<<16))
//使能TSI中断
#define LPLD_TSI_EnableIrq()     enable_irq(TSI0_IRQn)
//禁用TSI中断
#define LPLD_TSI_DisableIrq()    disable_irq(TSI0_IRQn)

//TSI通用初始化函数
uint8 LPLD_TSI_Init(TSI_InitTypeDef);
//TSI反初始化函数
void LPLD_TSI_Deinit(void);
//编号为ch_num的通道是否有触摸发生
boolean LPLD_TSI_IsChxTouched(uint8);

#endif /* __HW_TSI_H__ */