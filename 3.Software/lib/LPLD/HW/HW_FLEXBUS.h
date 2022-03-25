/**
 * @file HW_FLEXBUS.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief FLEXBUS底层模块相关函数
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
#ifndef __HW_FLEXBUS_H__
#define __HW_FLEXBUS_H__

//FlexBus模块号定义
typedef enum FBx
{
  FB0=0,
  FB1=1,
  FB2=2,
  FB3=3,
  FB4=4,
  FB5=5
}FBx;

//FB_InitTypeDef结构体FB_PortSize取值
#define FB_SIZE_8BIT    0x01
#define FB_SIZE_16BIT   0x02
#define FB_SIZE_32BIT   0x00
//FB_InitTypeDef结构体FB_AddressSpace取值
#define FB_SPACE_KB(n)  (n*1024)

typedef struct 
{    
  /*
    描述：
      选择FBx
    取值：
      FB0、FB1、FB2、FB3、FB4、FB5
    初始化：
      必须初始化
  */
  FBx FB_Fbx;
  
  /*
    描述：
      配置FBx的片选地址，即寻址基地址
    取值：
      0x60000000~0xDFFFFFFF-必须在FlexBux寻址范围内
    初始化：
      必须初始化
  */
  uint32 FB_ChipSelAddress;
  
  /*
    描述：
      配置FBx总线的寻址空间大小
    取值：
      FB_SPACE_KB(n)-即nKB大小
    初始化：
      必须初始化
  */
  uint32 FB_AddressSpace;
  
  /*
    描述：
      配置FBx总线的数据位宽
    取值：
      FB_SIZE_8BIT-8位宽
      FB_SIZE_16BIT-16位宽
      FB_SIZE_32BIT-32位宽
    初始化：
      不必须初始化，默认FB_SIZE_8BIT
  */
  uint8 FB_PortSize;
  
  /*
    描述：
      配置FBx总线的数据是否为右对齐
    取值：
      TRUE-右对齐
      FALSE-左对齐
    初始化：
      不必须初始化，默认左对齐
  */
  boolean FB_IsRightJustied;
    
  /*
    描述：
      配置FBx总线是否自动产生内部应答
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认左对齐
  */  
  boolean FB_AutoAckEnable; 
  
  /*
    描述：
      配置FBx总线在产生内部应答前加入的等待个数
    取值：
      0~63
    初始化：
      不必须初始化，默认0
  */ 
  uint8 FB_WateStates;
  
  /*
    描述：
      配置FBx总线的读地址信号在CS使能之前保持时间
    取值：
      0-如果FB_AutoAckEnable禁用为1个周期，否则0个周期
      1-如果FB_AutoAckEnable禁用为2个周期，否则1个周期
      2-如果FB_AutoAckEnable禁用为3个周期，否则2个周期
      3-如果FB_AutoAckEnable禁用为4个周期，否则3个周期
    初始化：
      不必须初始化，默认0
  */ 
  uint8 FB_ReadAddrHold;
  
  /*
    描述：
      配置FBx总线的写地址信号在CS使能之前保持时间
    取值：
      0-1个周期
      1-2个周期
      2-3个周期
      3-4个周期
    初始化：
      不必须初始化，默认0
  */ 
  uint8 FB_WriteAddrHold;
  
}FB_InitTypeDef;
  
//FlexBux通用初始化函数
uint8 LPLD_FlexBus_Init(FB_InitTypeDef);

#endif /* __HW_FLEXBUS_H__ */
