/**
 * @file HW_SYSTICK.h
 * @version 3.02[By LPLD]
 * @date 2013-11-29
 * @brief SYSTICK底层模块相关函数
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
#ifndef __HW_SYSTICK_H__
#define __HW_SYSTICK_H__
/* ----------------------------------------------------------------------------
   -- SYSTICK Access Layer
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SYSTICK_Access_Layer SYSTICK Access Layer
 * @{
 */

/** SYSTICK - Register Layout Typedef */
typedef struct {
  __IO uint32_t CSR;     /**< Systemtick Control and Status Register , offset: 0x0 */
  __IO uint32_t RVR;     /**< Systemtick Reload Value High, offset: 0x4 */
  __IO uint32_t CVR;     /**< Systemtick Current Value Register , offset: 0x8 */
  __IO uint32_t CALIB;   /**< Systemtick Calibration Value Register , offset: 0xC */
}SYSTICK_Type;

/* ----------------------------------------------------------------------------
   -- SYSTICK Register Masks
   ---------------------------------------------------------------------------- */

/**
 * @addtogroup SYSTICK_Register_Masks SYSTICK Register Masks
 * @{
 */
/* CSR Bit Fields */
#define SYSTICK_CSR_ENABLE_MASK                  0x1u
#define SYSTICK_CSR_ENABLE_SHIFT                 0
#define SYSTICK_CSR_TICKINT_MASK                 0x2u
#define SYSTICK_CSR_TICKINT_SHIFT                1
#define SYSTICK_CSR_CLKSOURCE_MASK               0x4u
#define SYSTICK_CSR_CLKSOURCE_SHIFT              2
#define SYSTICK_CSR_COUNTFLAG_MASK               0x10000u
#define SYSTICK_CSR_COUNTFLAG_SHIFT              16
/* RVR Bit Fields */
#define SYSTICK_RVR_RELOAD_MASK                  0xFFFFFFu
#define SYSTICK_RVR_RELOAD_SHIFT                 0
#define SYSTICK_RVR_RELOAD(x)                    (((uint32_t)(((uint32_t)(x))<<SYSTICK_RVR_RELOAD_SHIFT))&SYSTICK_RVR_RELOAD_MASK)
/* CVR Bit Fields */
#define SYSTICK_CVR_CURRENT_MASK                 0xFFFFFFu
#define SYSTICK_CVR_CURRENT_SHIFT                0
#define SYSTICK_CVR_CURRENT(x)                   (((uint32_t)(((uint32_t)(x))<<SYSTICK_CVR_CURRENT_SHIFT))&SYSTICK_CVR_CURRENT_MASK)
/* CALIB Bit Fields */
#define SYSTICK_CALIB_TENMS_MASK                 0xFFFFFFu
#define SYSTICK_CALIB_TENMS_SHIFT                0
#define SYSTICK_CALIB_TENMS(x)                   (((uint32_t)(((uint32_t)(x))<<SYSTICK_CALIB_TENMS_SHIFT))&SYSTICK_CALIB_TENMS_MASK)
#define SYSTICK_CALIB_SKEW_MASK                  0x40000000u
#define SYSTICK_CALIB_SKEW_SHIFT                 30
#define SYSTICK_CALIB_NOREF_MASK                 0x80000000u
#define SYSTICK_CALIB_NOREF_SHIFT                31
/**
 * @}
 */ /* end of group SYSTICK_Register_Masks */

/* SYSTICK - instance base addresses */
/** Peripheral SYSTICK base address */
#define SYSTICK_BASE (0xE000E010u)
/** Peripheral SYSTICK base pointer */
#define SYSTICK  ((SYSTICK_Type *)SYSTICK_BASE)

/**
 * @}
 */ /* end of group SYSTICK_Access_Layer */
typedef void (*SYSTICK_ISR_CALLBACK)(void);

#define SYSTICK_COUNTER_MAX (0xFFFFFF)
#define SYSTICK_UINT_US(x)  (x/1000000)
#define SYSTICK_UINT_MS(x)  (x/1000)
#define LPLD_SYSTICK_ISDONE ( SYSTICK->CSR & SYSTICK_CSR_COUNTFLAG_MASK )

typedef struct {
  /*
    描述：
      SYSTICK模块的周期，单位us
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，SYSTICK_PeriodUs、SYSTICK_PeriodMs、SYSTICK_PeriodS至少一个初始化
  */
  uint32 SYSTICK_PeriodUs;
  
  /*
    描述：
      SYSTICK模块的周期，单位ms
    取值：
      1~0xFFFFFFFF
    初始化：
      不必须初始化，SYSTICK_PeriodUs、SYSTICK_PeriodMs、SYSTICK_PeriodS至少一个初始化
  */
  uint32 SYSTICK_PeriodMs;

  /*
    描述：
      SYSTICK中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SYSTICK_ISR_CALLBACK SYSTICK_Isr; 
	
}SYSTICK_InitType;

//SYSTICK初始化函数
void LPLD_SYSTICK_Init(SYSTICK_InitType );
//SYSTICK反初始化函数
void LPLD_SYSTICK_DeInit(void);
//SYSTICK延时US函数
void LPLD_SYSTICK_DelayUs(uint32 );
//SYSTICK延时MS函数
void LPLD_SYSTICK_DelayMs(uint32 );
#endif