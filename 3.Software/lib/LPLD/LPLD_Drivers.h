/**
 * @file LPLD_Drivers.h
 * @version 3.1[By LPLD]
 * @date 2015-01-11
 * @brief LPLD Kinetis驱动库通用定义配置
 *
 * 更改建议:不建议修改
 *
 * 该代码包含拉普兰德K60固件库的所有头文件、
 * 定义所有PORT（引脚）的枚举值
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
#ifndef __LPLD_DRIVERS_H__
#define __LPLD_DRIVERS_H__

//底层库版本定义
#define OSKinetis_Version     "3.1 b1"

//端口引脚枚举定义
typedef enum PortPinsEnum
{
  //PortA
  PTA0=0,    PTA1=1,    PTA2=2,    PTA3=3,    
  PTA4=4,    PTA5=5,    PTA6=6,    PTA7=7,    
  PTA8=8,    PTA9=9,    PTA10=10,  PTA11=11,
  PTA12=12,  PTA13=13,  PTA14=14,  PTA15=15,
  PTA16=16,  PTA17=17,  PTA18=18,  PTA19=19,
  /********* MK60DZ10无PTA20~23 ************/
  PTA24=24,  PTA25=25,  PTA26=26,  PTA27=27,
  PTA28=28,  PTA29=29,
  /********* MK60DZ10无PTA30~31 ************/
  
  //PortB
  PTB0=32,   PTB1=33,   PTB2=34,   PTB3=35,
  PTB4=36,   PTB5=37,   PTB6=38,   PTB7=39,
  PTB8=40,   PTB9=41,   PTB10=42,  PTB11=43,
  /********* MK60DZ10无PTB12~15 ************/
  PTB16=48,  PTB17=49,  PTB18=50,  PTB19=51,
  PTB20=52,  PTB21=53,  PTB22=54,  PTB23=55,
  /********* MK60DZ10无PTB24~27 ************/
  /********* MK60DZ10无PTB28~31 ************/
  
  //PortC
  PTC0=60,   PTC1=61,   PTC2=62,   PTC3=63,
  PTC4=64,   PTC5=65,   PTC6=66,   PTC7=67,
  PTC8=68,   PTC9=69,   PTC10=70,  PTC11=71,
  PTC12=72,  PTC13=73,  PTC14=74,  PTC15=75,
  PTC16=76,  PTC17=77,  PTC18=78,  PTC19=79,
  /********* MK60DZ10无PTC20~23 ************/
  /********* MK60DZ10无PTC24~27 ************/
  /********* MK60DZ10无PTC28~31 ************/
  
  //PortD
  PTD0=92,   PTD1=93,   PTD2=94,   PTD3=95,
  PTD4=96,   PTD5=97,   PTD6=98,   PTD7=99,
  PTD8=100,  PTD9=101,  PTD10=102, PTD11=103,
  PTD12=104, PTD13=105, PTD14=106, PTD15=107,
  /********* MK60DZ10无PTD16~19 ************/
  /********* MK60DZ10无PTD20~23 ************/
  /********* MK60DZ10无PTD24~27 ************/
  /********* MK60DZ10无PTD28~31 ************/
  
  //PortE
  PTE0=124,  PTE1=125,  PTE2=126,  PTE3=127,
  PTE4=128,  PTE5=129,  PTE6=130,  PTE7=131,
  PTE8=132,  PTE9=133,  PTE10=134, PTE11=135,
#if defined(CPU_MK60F15)
  PTE12=136,
#endif
  /********* MK60DZ10无PTE12~15 ************/
  /********* MK60DZ10无PTE16~19 ************/
  /********* MK60DZ10无PTE20~23 ************/
  PTE24=148, PTE25=149, PTE26=150, PTE27=151,
  PTE28=152
  /********* MK60DZ10无PTE29~31 ************/
} PortPinsEnum_Type;


#include "HW_MCG.h"
#include "HW_WDOG.h"
#include "HW_UART.h"
#include "HW_GPIO.h"
#include "HW_PIT.h"
#include "HW_FTM.h"
#include "HW_ADC.h"
#include "HW_DAC.h"
#include "HW_PDB.h"
#include "HW_LPTMR.h"
#include "HW_FLASH.h"
#include "HW_I2C.h"
#include "HW_CAN.h"
#include "HW_SDHC.h"
#include "HW_DMA.h"
#include "HW_RTC.h"
#include "HW_TSI.h"
#include "HW_ENET.h"
#include "HW_SPI.h"
#include "HW_USB.h"
#include "HW_SYSTICK.h"
#include "HW_NVIC.h"

#endif /* __LPLD_DRIVERS_H__ */