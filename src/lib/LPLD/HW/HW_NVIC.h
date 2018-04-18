/**
 * @file HW_NVIC.h
 * @version 3.02[By LPLD]
 * @date 2013-11-29
 * @brief 内核NVIC模块相关函数
 *
 * 更改建议:不建议修改
 *
 * 版权所有:北京拉普兰德电响应式技术有限公司
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

#ifndef __HW_NVIC_H__
#define __HW_NVIC_H__

#include "common.h"


//定义NVIC中断分组
#define NVIC_PriorityGroup_0         ((uint32_t)0x07) /*!< 0 bits for pre-emption priority
                                                            4 bits for subpriority */
#define NVIC_PriorityGroup_1         ((uint32_t)0x06) /*!< 1 bits for pre-emption priority
                                                            3 bits for subpriority */
#define NVIC_PriorityGroup_2         ((uint32_t)0x05) /*!< 2 bits for pre-emption priority
                                                            2 bits for subpriority */
#define NVIC_PriorityGroup_3         ((uint32_t)0x04) /*!< 3 bits for pre-emption priority
                                                            1 bits for subpriority */
#define NVIC_PriorityGroup_4         ((uint32_t)0x03) /*!< 4 bits for pre-emption priority
                                                            0 bits for subpriority */
//断言变量是否存在
#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PriorityGroup_0) || \
                                       ((GROUP) == NVIC_PriorityGroup_1) || \
                                       ((GROUP) == NVIC_PriorityGroup_2) || \
                                       ((GROUP) == NVIC_PriorityGroup_3) || \
                                       ((GROUP) == NVIC_PriorityGroup_4))

#define IS_NVIC_PREEMPTION_PRIORITY(PRIORITY)  ((PRIORITY) < 0x10)

#define IS_NVIC_SUB_PRIORITY(PRIORITY)  ((PRIORITY) < 0x10)

typedef struct
{
  /*
    描述：
      中断IRQn_Type号，可在MK60DZ10.h中查到
    取值：
      在枚举IRQn_Type中定义了中断号，选取对应通道的IRQn_Type号
    初始化：
      必须初始化
  */
  IRQn_Type  NVIC_IRQChannel; 
  
  /*
    描述：
      NVIC 中断分组
    取值：
      NVIC_PriorityGroup_0
        没有抢占式优先级（NVIC_IRQChannelPreemptionPriority = 0），最多16个响应式优先级（NVIC_IRQChannelSubPriority）
      NVIC_PriorityGroup_1
        最多2个抢占式优先级（NVIC_IRQChannelPreemptionPriority） ，最多8个响应式优先级（NVIC_IRQChannelSubPriority）
        其中（抢占式优先级0 优先级高于 抢占式优先级1）
      NVIC_PriorityGroup_2
        最多4个抢占式优先级（NVIC_IRQChannelPreemptionPriority） ，最多4个响应式优先级（NVIC_IRQChannelSubPriority）
      NVIC_PriorityGroup_3
        最多8个抢占式优先级（NVIC_IRQChannelPreemptionPriority） ，最多2个响应式优先级（NVIC_IRQChannelSubPriority）
      NVIC_PriorityGroup_4
        最多16个抢占式优先级（NVIC_IRQChannelPreemptionPriority），没有响应式优先级（NVIC_IRQChannelSubPriority = 0）
    初始化：
      必须初始化
  */  
  uint32  NVIC_IRQChannelGroupPriority;
  
  /*
    描述：
      NVIC抢占式优先级，根据中断分组中的描述而设定
    取值：
      最大15,取值方式见NVIC 中断分组描述
    初始化：
      必须初始化
  */                                                   
  uint32  NVIC_IRQChannelPreemptionPriority; 
  
  /*
    描述：
      NVIC响应式优先级，根据中断分组中的描述而设定
    取值：
      最大15,取值方式见NVIC 中断分组描述
    初始化：
      必须初始化
   */    
  uint32  NVIC_IRQChannelSubPriority;  
  
  /*
    描述：
      在NVIC中使能或者禁止IRQn_Type通道
      该功能可以在LPLD_XXX_EnableIRQ（）或者LPLD_XXX_DisableIRQ（）中实现
    取值：
      TRUE  使能
      FALSE 禁止
    初始化：
      默认禁止
   */  
  boolean NVIC_IRQChannelEnable;      
  
} NVIC_InitTypeDef;
//NVIC 初始化函数
uint8 LPLD_NVIC_Init(NVIC_InitTypeDef );
#endif