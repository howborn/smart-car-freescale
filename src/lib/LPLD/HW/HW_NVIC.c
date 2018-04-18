/**
 * @file HW_NVIC.c
 * @version 3.02[By LPLD]
 * @date 2013-11-29
 * @brief 内核NVIC模块相关函数
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
#include "HW_NVIC.h"

/*
 * LPLD_NVIC_Init
 *
 * 在该函数中配置IRQn_Type通道优先级，并根据需要在NVIC中使能IRQn_Type
 * 在函数中调用了core_m4.h中的NVIC函数管理M4内核的16个中断优先级
 * 16个中断优先级通过分组的形式进行管理，将NVIC中的优先级分为5组，NVIC_PriorityGroup_0 -- 5
 * 每组包含抢占式优先级（NVIC_IRQChannelPreemptionPriority）和响应式优先级（NVIC_IRQChannelSubPriority）
 *
 * 优先级仲裁:
 * >1 PreemptionPriority优先级高的才能抢占PreemptionPriority优先级低的中断异常;
 * (即NVIC_IRQChannelPreemptionPriority值越小优先级越高);
 * >2 PreemptionPriority优先级相同的中断异常之间不能相互抢占;
 * >3 如果PreemptionPriority优先级相等，再比较SubPriority响应优先级，SubPriority响应优先级值越小中断优先级越高;
 *
 * NVIC中的优先级分组划分见NVIC_InitTypeDef中的NVIC 中断分组
 * 
 * 参数:
 *    NVIC_InitStructure--NVIC初始化结构体，
 *                        具体定义见NVIC_InitTypeDef
 * 输出:
 *    0:配置失败
 *    1:配置成功
 *
*/
uint8 LPLD_NVIC_Init(NVIC_InitTypeDef NVIC_InitStructure)
{
  uint8  request = 1;
  IRQn_Type int_id = NVIC_InitStructure.NVIC_IRQChannel;
  uint32 nvic_priority_group = NVIC_InitStructure.NVIC_IRQChannelGroupPriority; 
  uint32 nvic_preemption_priority = NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority;
  uint32 nvic_sub_priority = NVIC_InitStructure.NVIC_IRQChannelSubPriority;
  boolean status = NVIC_InitStructure.NVIC_IRQChannelEnable;
  uint32 temp;
  //参数检查
  ASSERT(IS_NVIC_PRIORITY_GROUP(nvic_priority_group));
  ASSERT(IS_NVIC_PREEMPTION_PRIORITY(nvic_preemption_priority));
  ASSERT(IS_NVIC_SUB_PRIORITY(nvic_sub_priority));
  //根据 NVIC中断分组 配置抢占优先级
  switch(nvic_priority_group)
  {
      case NVIC_PriorityGroup_0: NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
              if(nvic_preemption_priority > 0 || nvic_sub_priority >15)
                request = 0;
              break;
      case NVIC_PriorityGroup_1: NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
              if(nvic_preemption_priority > 1 || nvic_sub_priority >7)
                request = 0;
              break;
      case NVIC_PriorityGroup_2: NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
              if(nvic_preemption_priority > 3 || nvic_sub_priority >3)
                request = 0;
              break;
      case NVIC_PriorityGroup_3: NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
              if(nvic_preemption_priority > 7 || nvic_sub_priority >1)
                request = 0;
              break;
      case NVIC_PriorityGroup_4: NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
              if(nvic_preemption_priority > 15 || nvic_sub_priority >0)
                request = 0;
              break;
      default:return 0;
  }
  //根据中断分抢占设置中断抢占优先级
  //根据中断分抢占设置中断响应优先级
  temp = NVIC_EncodePriority(nvic_priority_group,\
                             nvic_preemption_priority,\
                             nvic_sub_priority);
  //设置中断向量号,并在M4内核中设置NVIC优先级
  NVIC_SetPriority(int_id,temp);
  
  if(status == TRUE)
  {
    NVIC_EnableIRQ(int_id);
  }
  else
  {
    NVIC_DisableIRQ(int_id);
  }
  return request;
}
