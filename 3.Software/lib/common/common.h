/**
 * @file common.h
 * @version 3.1[By LPLD]
 * @date 2015-01-11
 * @brief 包含所有需要用到的头文件以及相关宏定义
 *
 * 更改建议:建议备份后修改
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
 * 
 * 基于LPLD的MK60DZ10库进行的更改，作为MK60F15固件库。
 * @ 樊浩柏
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/***********************************************************************/
/*
 * 基础数据类型定义
 */
typedef unsigned char	uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef signed char		int8;   /*  8 bits */
typedef short int	        int16;  /* 16 bits */
typedef int		        int32;  /* 32 bits */

typedef volatile int8		vint8;  /*  8 bits */
typedef volatile int16	vint16; /* 16 bits */
typedef volatile int32	vint32; /* 32 bits */

typedef volatile uint8	vuint8;  /*  8 bits */
typedef volatile uint16	vuint16; /* 16 bits */
typedef volatile uint32	vuint32; /* 32 bits */

typedef float   		float32; /*  32 bits */
typedef double   		float64; /*  64 bits */

typedef unsigned char   boolean;      /* 8-bit*/

/*
 * 其他定义
 */
#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	0

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	1

#ifndef NULL
#define NULL    0
#endif

/********************************************************************/

/*
 * RAMFUNC关键字定义，用于告诉编译器函数在RAM中运行
 */
#ifndef __IAR_SYSTEMS_ICC__
  #warning OSKineits仅支持IAR编译器[http://www.lpld.cn]
  #define RAMFUNC 
#else
  #define RAMFUNC __ramfunc
#endif

/*
 * 打印调试信息定义
 */
#define DEBUG_PRINT

/*
 * 使用断言判断参数合法性
 */
#define DEBUG_ASSERT
   
/* 
 * 包含开发平台指定的头文件
 * 如果使用拉普兰德开发板需定义LPLD_K60
 * 如果提示"未定义K60开发平台"，需在工程选项中添加定义
 */
#if (defined(LPLD_K60))
 #include "k60_card.h"
#else
  #error "未定义K60开发平台"
#endif
   
/* 
 * 包含CPU指定的头文件
 * CPU型号应该在"开发平台指定的头文件"中定义
 */
#ifdef CPU_MK60F15
  #include "MK60F15.h"
#endif

/* 
 * 包含通用功能函数
 */
#include "assert.h"
#include "io.h"
#include "uif.h"
#include "stdlib.h"
#include "relocate.h"
#if (defined(__IAR_SYSTEMS_ICC__))
	#include "intrinsics.h"
#endif

/* 
 * 包含拉普兰德K60固件库头文件
 */
#include "LPLD_Drivers.h"

/* 
 * 包含实时系统头文件
 */
#if (UCOS_II > 0u)
#include  <ucos_ii.h>
#endif

/* 
 * 包含文件系统头文件
 */
#if (USE_FATFS > 0u)
#include  <ff.h>
#endif

/***********************************************************************/
/* 
 * 中断相关函数定义
 */
//使能全局中断宏定义
#define EnableInterrupts __enable_irq()
//禁用全局中断宏定义
#define DisableInterrupts  __disable_irq()
//使能指定中断向量号的中断
#define enable_irq(IRQn)    NVIC_EnableIRQ(IRQn)
//禁用指定中断向量号的中断
#define disable_irq(IRQn)    NVIC_DisableIRQ(IRQn)
//更改中断向量表地址，写VTOR寄存器
#define write_vtor(vector_addr) SCB->VTOR = (uint32_t)vector_addr;
/***********************************************************************/

/* 
 * 用户入口函数声明
 */
void main(void);


/********************************************************************/

#endif /* _COMMON_H_ */
