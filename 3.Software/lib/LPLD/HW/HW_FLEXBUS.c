/**
 * @file HW_FLEXBUS.c
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

#include "common.h"
#include "HW_FLEXBUS.h"

const uint32* FB_PORT[32] = {(uint32*)&PORTD->PCR[6], (uint32*)&PORTD->PCR[5], (uint32*)&PORTD->PCR[4], (uint32*)&PORTD->PCR[3],
                             (uint32*)&PORTD->PCR[2], (uint32*)&PORTC->PCR[10], (uint32*)&PORTC->PCR[9], (uint32*)&PORTC->PCR[8],
                             (uint32*)&PORTC->PCR[7], (uint32*)&PORTC->PCR[6], (uint32*)&PORTC->PCR[5], (uint32*)&PORTC->PCR[4],
                             (uint32*)&PORTC->PCR[2], (uint32*)&PORTC->PCR[1], (uint32*)&PORTC->PCR[0], (uint32*)&PORTB->PCR[18],
                             (uint32*)&PORTB->PCR[17], (uint32*)&PORTB->PCR[16], (uint32*)&PORTB->PCR[11], (uint32*)&PORTB->PCR[10],
                             (uint32*)&PORTB->PCR[9], (uint32*)&PORTB->PCR[8], (uint32*)&PORTB->PCR[7], (uint32*)&PORTB->PCR[6],
                             (uint32*)&PORTC->PCR[15], (uint32*)&PORTC->PCR[14], (uint32*)&PORTC->PCR[13], (uint32*)&PORTC->PCR[12],
                             (uint32*)&PORTB->PCR[23], (uint32*)&PORTB->PCR[22], (uint32*)&PORTB->PCR[21], (uint32*)&PORTB->PCR[20]};

/*
 * LPLD_FlexBus_Init
 * FlexBux通用初始化函数
 * 
 * 参数:
 *    fb_init_struct--FlexBus初始化结构体，
 *                        具体定义见FB_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FlexBus_Init(FB_InitTypeDef fb_init_struct)
{
  FBx Fbx = fb_init_struct.FB_Fbx;
  uint32 base_addr = fb_init_struct.FB_ChipSelAddress;
  uint32 addr_space = fb_init_struct.FB_AddressSpace;
  uint8 size = fb_init_struct.FB_PortSize;
  boolean justied = fb_init_struct.FB_IsRightJustied;
  boolean aa = fb_init_struct.FB_AutoAckEnable;
  uint8 ws = fb_init_struct.FB_WateStates;
  uint8 rah = fb_init_struct.FB_ReadAddrHold; 
  uint8 wah = fb_init_struct.FB_WriteAddrHold;
  uint32* pcr;
  uint8 n=0;
  
  //参数检查
  ASSERT( Fbx <= 5);    //判断FB通道号
  ASSERT( base_addr >= 0x60000000 && base_addr <= 0xDFFFFFFF);    //判断基地址合法性
  ASSERT( addr_space != 0);    //寻址空间大小
  ASSERT( size <= 3);    //判断数据位宽
  ASSERT( rah <= 3);     //判断读地址保持时间
  ASSERT( wah <= 3);     //判断写地址保持时间
  
  FB->CS[Fbx].CSMR = 0;
      
  //设置基地址
  FB->CS[Fbx].CSAR = base_addr;
  //端口位宽
  FB->CS[Fbx].CSCR  = FB_CSCR_PS(size);      
  
  if(justied)
  {
    FB->CS[Fbx].CSCR |= FB_CSCR_BLS_MASK;   // 数据右对齐
  }
      
  if(aa)    
  {
    FB->CS[Fbx].CSCR |= FB_CSCR_AA_MASK;   //使能内部应答
  }
  
  FB->CS[Fbx].CSCR |= FB_CSCR_WS(ws)    // 在内部应答信号之前加入ws个WS
                     | FB_CSCR_RDAH(rah)
                     | FB_CSCR_WRAH(wah);
  
  //根据寻址空间大小计算基地址掩码
  addr_space = (addr_space-1)>>16;
  FB->CS[Fbx].CSMR = FB_CSMR_BAM(addr_space)  //设置基地址掩码为128K地址空间 =2^n(n=BAM+16)
                | FB_CSMR_V_MASK;    //使能CS信号
  
  //使能复用功能的FB_CSn及引脚复用
  if(Fbx == FB0)
  {
    PORTD->PCR[1]  = PORT_PCR_MUX(5);       // FB_CS0     
  }
  else if(Fbx == FB1)
  {
    FB->CSPMCR &= ~(FB_CSPMCR_GROUP1_MASK);
    FB->CSPMCR |= FB_CSPMCR_GROUP1(1);      
    PORTD->PCR[0]  = PORT_PCR_MUX(5);       // FB_CS1  
  }
  else if(Fbx == FB2)
  {
    FB->CSPMCR &= ~(FB_CSPMCR_GROUP4_MASK);
    FB->CSPMCR |= FB_CSPMCR_GROUP4(1);     
    PORTC->PCR[18]  = PORT_PCR_MUX(5);       // FB_CS2
  }
  else if(Fbx == FB3)
  {
    FB->CSPMCR &= ~(FB_CSPMCR_GROUP5_MASK);
    FB->CSPMCR |= FB_CSPMCR_GROUP5(1);     
    PORTC->PCR[19]  = PORT_PCR_MUX(5);       // FB_CS3
  }
  else if(Fbx == FB4)
  {
    FB->CSPMCR &= ~(FB_CSPMCR_GROUP2_MASK);
    FB->CSPMCR |= FB_CSPMCR_GROUP2(0);      
    PORTC->PCR[17]  = PORT_PCR_MUX(5);       // FB_CS4
  }
  else if(Fbx == FB5)
  {
    FB->CSPMCR &= ~(FB_CSPMCR_GROUP3_MASK);
    FB->CSPMCR |= FB_CSPMCR_GROUP3(0);     
    PORTC->PCR[16]  = PORT_PCR_MUX(5);       // FB_CS5
  }
  
  //使能总线地址、数据引脚复用功能
  //先使能低16位地址总线
  for(uint8 i=0; i<16; i++)
  {
    pcr = (uint32 *)FB_PORT[i];
    *pcr = PORT_PCR_MUX(5);    
  }
  //使能其余的地址总线
  n=16;
  for(uint8 i=0; i<16; i++)
  {
    if((addr_space>>i)&0x01)
    {
      pcr = (uint32 *)FB_PORT[n++];
      *pcr = PORT_PCR_MUX(5); 
    }
  }
  //如果是32位宽，则使能高16位
  n=0;
  if(size == 0)
  {   
    n=16;
  }else if(justied==0 && size == 0x10)
  {
    n=16;
  }else if(justied==0 && size == 0x01)
  {
    n=8;
  }
  if(n!=0)
  {
    for(uint8 i=31; i>31-n; i--)
    {
      pcr = (uint32 *)FB_PORT[i];
      *pcr = PORT_PCR_MUX(5);    
    }
  }
  
  //控制信号线
  PORTB->PCR[19] = PORT_PCR_MUX(5);          // fb_oe_b
  PORTC->PCR[11] = PORT_PCR_MUX(5);          // fb_rw_b  
              
  return 1;
}