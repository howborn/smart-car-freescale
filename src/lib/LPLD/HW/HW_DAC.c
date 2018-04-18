/**
 * @file HW_DAC.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief DAC底层模块相关函数
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
#include "HW_DAC.h"

//用户自定义中断服务函数数组
DAC_ISR_CALLBACK DAC_RDPTBOT_ISR[2];
DAC_ISR_CALLBACK DAC_RDPTTOP_ISR[2];
DAC_ISR_CALLBACK DAC_WATERMK_ISR[2];


/*
 * LPLD_DAC_Init
 * DAC初始化函数，使能DAC各项功能
 * 
 * 参数:
 *    dac_init_struct--DAC初始化结构体，
 *                        具体定义见DAC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_DAC_Init(DAC_InitTypeDef dac_init_struct)
{
  uint8 x;
  DAC_Type *dacx = dac_init_struct.DAC_Dacx;
  uint8 work_mode = dac_init_struct.DAC_BufferWorkMode;
  uint8 watermark = dac_init_struct.DAC_BufferWatermarkSel;
  uint8 upper = dac_init_struct.DAC_BufferUpperLimit;
  DAC_ISR_CALLBACK rdptbot_isr = dac_init_struct.DAC_ReadPointerBottomIsr;
  DAC_ISR_CALLBACK rdpttop_isr = dac_init_struct.DAC_ReadPointerTopIsr;
  DAC_ISR_CALLBACK watermk_isr = dac_init_struct.DAC_BufferWatermarkIsr;
  
  upper = (upper==NULL?1:upper);
    
  //参数检查
  ASSERT( work_mode <= BUFFER_MODE_ONETIMESCAN );  //判断模式选择
  ASSERT( watermark <= WATERMARK_4WORDS );      //判断缓冲区水印选择
  ASSERT( upper <= 16);         //判断缓冲区上限数值
    
  //配置DAC时钟
  if(dacx == DAC0)
  {
    x = 0;
    SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;   //开启DAC0时钟
  }
  else if(dacx == DAC1)
  {
    x = 1;
    SIM->SCGC2 |= SIM_SCGC2_DAC1_MASK;   //开启DAC1时钟
  }
  else 
  {
    return 0;
  }
  
  //配置C2寄存器
  dacx->C2 = DAC_C2_DACBFUP(upper - 1);     //配置缓冲区上限数值
  
  //配置C1寄存器
  dacx->C1 = 0;
  if(dac_init_struct.DAC_BufferEnable == TRUE)
  {
    dacx->C1 |= DAC_C1_DACBFEN_MASK;    //使能缓冲区 
  }
  dacx->C1 |= DAC_C1_DACBFMD(work_mode);        //缓冲区工作模式配置
  dacx->C1 |= DAC_C1_DACBFWM(watermark);        //缓冲区水印选择配置
  if(dac_init_struct.DAC_DmaEnable == TRUE)
  {
    dacx->C1 |= DAC_C1_DMAEN_MASK;      //使能DMA
  }
  
  //配置C0寄存器
  dacx->C0 = 0;
  if(dac_init_struct.DAC_ReadPointerBottomIntEnable == TRUE && rdptbot_isr != NULL)
  {
    dacx->C0 |= DAC_C0_DACBBIEN_MASK;   //使能缓冲区底部中断
    DAC_RDPTBOT_ISR[x] = rdptbot_isr;
  }
  if(dac_init_struct.DAC_ReadPointerTopIntEnable == TRUE && rdpttop_isr != NULL)
  {
    dacx->C0 |= DAC_C0_DACBTIEN_MASK;   //使能缓冲区顶部中断
    DAC_RDPTTOP_ISR[x] = rdpttop_isr;
  }
  if(dac_init_struct.DAC_BufferWatermarkIntEnable == TRUE && watermk_isr != NULL)
  {
    dacx->C0 |= DAC_C0_DACBWIEN_MASK;   //使能缓冲区水印中断
    DAC_WATERMK_ISR[x] = watermk_isr;
  }
  if(dac_init_struct.DAC_SoftTrgEnable == TRUE)
  {
    dacx->C0 |= DAC_C0_DACTRGSEL_MASK;  //使能软件触发缓冲区
  }
  dacx->C0 |= DAC_C0_DACRFS_MASK;       //使能DACREF_2(VDDA)为参考电压
  
  //使能DAC模块，可编程参考发生器开始工作
  dacx->C0 |= DAC_C0_DACEN_MASK;
  
  return 1;
}

/*
 * LPLD_DAC_Deinit
 * DAC反初始化函数，禁用DAC模块
 * 
 * 参数:
 *    dac_init_struct--DAC初始化结构体，
 *                        具体定义见DAC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_DAC_Deinit(DAC_InitTypeDef dac_init_struct)
{
  uint8 x;
  DAC_Type *dacx = dac_init_struct.DAC_Dacx;

  //关闭DAC模块
  dacx->C0 &= ~(DAC_C0_DACEN_MASK);
  
  //关闭DAC时钟
  if(dacx == DAC0)
  {
    x = 0;
    SIM->SCGC2 &= ~(SIM_SCGC2_DAC0_MASK);   
  }
  else if(dacx == DAC1)
  {
    x = 1;
    SIM->SCGC2 &= ~(SIM_SCGC2_DAC1_MASK);   
  }
  else 
  {
    return 0;
  }

  disable_irq((IRQn_Type)(DAC0_IRQn + x));
  
  return 1;
}

/*
 * LPLD_DAC_EnableIrq
 * 使能DACx中断
 * 
 * 参数:
 *    dac_init_struct--DAC初始化结构体，
 *                        具体定义见DAC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_DAC_EnableIrq(DAC_InitTypeDef dac_init_struct)
{
  uint8 x;
  DAC_Type *dacx = dac_init_struct.DAC_Dacx;
  
  if(dacx == DAC0)
    x = 0;
  else if(dacx == DAC1)
    x = 1;
  else
    return 0;

  enable_irq((IRQn_Type)(DAC0_IRQn + x));
  
  return 1;
}

/*
 * LPLD_DAC_DisableIrq
 * 禁用DACx中断
 * 
 * 参数:
 *    dac_init_struct--DAC初始化结构体，
 *                        具体定义见DAC_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_DAC_DisableIrq(DAC_InitTypeDef dac_init_struct)
{
  uint8 x;
  DAC_Type *dacx = dac_init_struct.DAC_Dacx;
  
  if(dacx == DAC0)
    x = 0;
  else if(dacx == DAC1)
    x = 1;
  else
    return 0;

  disable_irq((IRQn_Type)(DAC0_IRQn + x));
  
  return 1;
}

/*
 * LPLD_DAC_SetBufferDataN
 * 设置DAC缓存区单个数据n
 * 
 * 参数:
 *    dacx--DACx模块号
 *      |__DAC0          --DAC0
 *      |__DAC1          --DAC1
 *    data--缓冲区输出数据
 *      |__0~4095        --12位数据
 *    num--指定缓冲区位置
 *      |__1~16          --缓冲区第1至16个字
 *
 * 输出:
 *    无
 */
void LPLD_DAC_SetBufferDataN(DAC_Type *dacx, uint16 data, uint8 num)
{
  //参数检查
  ASSERT( (num <= 16)&&(num > 0) );  //判断数据序号

  dacx->DAT[num-1].DATL = data & 0x0FF;
  dacx->DAT[num-1].DATH = (data & 0xF00)>>8;
}

/*
 * LPLD_DAC_SetBufferData
 * 设置DAC缓存区所有数据
 * 
 * 参数:
 *    dacx--DACx模块号
 *      |__DAC0          --DAC0
 *      |__DAC1          --DAC1
 *    *data--缓冲区输出数据数组指针
 *      |__*0~4095       --12位数据数组
 *    len--数据数组长度
 *      |__1~16          --长度为1~16
 *
 * 输出:
 *    无
 */
void LPLD_DAC_SetBufferData(DAC_Type *dacx, uint16 *data, uint8 len)
{
  //参数检查
  ASSERT( (len <= 16)&&(len > 0) );  //判断数据长度
  
  for(uint8 i=0; i < len; i++)
  {
    dacx->DAT[i].DATL = data[i] & 0x0FF;
    dacx->DAT[i].DATH = (data[i] & 0xF00)>>8;
  }
}

/*
 * LPLD_DAC_SoftwareTrigger
 * 软件触发DAC缓冲区工作
 * 
 * 参数:
 *    dacx--DACx模块号
 *      |__DAC0          --DAC0
 *      |__DAC1          --DAC1
 *
 * 输出:
 *    无
 */
void LPLD_DAC_SoftwareTrigger(DAC_Type *dacx)
{
  dacx->C0 |= DAC_C0_DACSWTRG_MASK;
}

/*
 * DAC0--DAC1中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */

void DAC0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //是否为指针底部中断
  if((DAC0->SR & DAC_SR_DACBFRPBF_MASK) && (DAC0->C0 & DAC_C0_DACBBIEN_MASK))
  {
    //调用用户自定义中断服务
    DAC_RDPTBOT_ISR[0]();  
    DAC0->SR &= ~(DAC_SR_DACBFRPBF_MASK);
  }
  //是否为指针顶部中断
  if((DAC0->SR & DAC_SR_DACBFRPTF_MASK) && (DAC0->C0 & DAC_C0_DACBTIEN_MASK))
  {
    //调用用户自定义中断服务
    DAC_RDPTTOP_ISR[0]();  
    DAC0->SR &= ~(DAC_SR_DACBFRPTF_MASK);
  }
  //是否为水印中断
  if((DAC0->SR & DAC_SR_DACBFWMF_MASK) && (DAC0->C0 & DAC_C0_DACBWIEN_MASK))
  {
    //调用用户自定义中断服务
    DAC_WATERMK_ISR[0]();  
    DAC0->SR &= ~(DAC_SR_DACBFWMF_MASK);
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

void DAC1_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //是否为指针底部中断
  if((DAC1->SR & DAC_SR_DACBFRPBF_MASK) && (DAC1->C0 & DAC_C0_DACBBIEN_MASK))
  {
    //调用用户自定义中断服务
    DAC_RDPTBOT_ISR[1]();  
    DAC1->SR &= ~(DAC_SR_DACBFRPBF_MASK);
  }
  //是否为指针顶部中断
  if((DAC1->SR & DAC_SR_DACBFRPTF_MASK) && (DAC1->C0 & DAC_C0_DACBTIEN_MASK))
  {
    //调用用户自定义中断服务
    DAC_RDPTTOP_ISR[1]();  
    DAC1->SR &= ~(DAC_SR_DACBFRPTF_MASK);
  }
  //是否为水印中断
  if((DAC1->SR & DAC_SR_DACBFWMF_MASK) && (DAC1->C0 & DAC_C0_DACBWIEN_MASK))
  {
    //调用用户自定义中断服务
    DAC_WATERMK_ISR[1]();  
    DAC1->SR &= ~(DAC_SR_DACBFWMF_MASK);
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}