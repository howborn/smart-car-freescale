/**
 * @file HW_FTM.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief FTM底层模块相关函数
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
#include "HW_FTM.h"

//用户自定义中断服务函数数组
#if (defined(CPU_MK60DZ10))
FTM_ISR_CALLBACK FTM_ISR[3];
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
FTM_ISR_CALLBACK FTM_ISR[4];
#endif

static uint8 LPLD_FTM_PWM_Init(FTM_InitTypeDef);
static uint8 LPLD_FTM_IC_Init(FTM_InitTypeDef);
static uint8 LPLD_FTM_QD_Init(FTM_InitTypeDef);
static uint8 LPLD_FTM_DEC_Init(FTM_InitTypeDef);
static uint8 LPLD_FTM_PinInit(FTM_Type *, FtmChnEnum_Type, PortPinsEnum_Type);
static uint8 LPLD_FTM_PinDeinit(FTM_Type *, FtmChnEnum_Type);


/*
 * LPLD_FTM_Init
 * FTM模块通用初始化，可初始化为PWM、输入捕获、正交解码、双边捕获模式
 *
 * 参数:
 *    ftm_init_structure--FTM初始化结构体，
 *                        具体定义见FTM_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_Init(FTM_InitTypeDef ftm_init_structure)
{
  uint8 result, i;
  //参数检查
  ASSERT( ftm_init_structure.FTM_Mode & 
         (FTM_MODE_PWM|FTM_MODE_IC|FTM_MODE_QD|FTM_MODE_DEC));  //判断模式选择
  
  // 使能FTM时钟模块
  if(ftm_init_structure.FTM_Ftmx == FTM0)
  {
    i=0; 
    SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
  }
  else if(ftm_init_structure.FTM_Ftmx == FTM1)
  {
    i=1;
    SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
  }
  else if(ftm_init_structure.FTM_Ftmx == FTM2)
  {
    i=2;
    SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
  }
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(ftm_init_structure.FTM_Ftmx == FTM3)
  {
    i=3;
    SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
  }
#endif
  else
  {
    return 0;
  }
  
  if(ftm_init_structure.FTM_Mode & FTM_MODE_PWM)
  {
    result = LPLD_FTM_PWM_Init(ftm_init_structure);
  }
  else if(ftm_init_structure.FTM_Mode & FTM_MODE_IC)
  {
    result = LPLD_FTM_IC_Init(ftm_init_structure);
  }
  else if(ftm_init_structure.FTM_Mode & FTM_MODE_QD)
  {
    result = LPLD_FTM_QD_Init(ftm_init_structure);
  }
  else if(ftm_init_structure.FTM_Mode & FTM_MODE_DEC)
  {
    result = LPLD_FTM_DEC_Init(ftm_init_structure);
  }
  
  if(result == 1)
  {    
    //判断是否开启溢出中断
    if(ftm_init_structure.FTM_Isr!=NULL)
    {      
      FTM_ISR[i] = ftm_init_structure.FTM_Isr;
      if(ftm_init_structure.FTM_ToiEnable == TRUE)
      {
        ftm_init_structure.FTM_Ftmx->SC |= FTM_SC_TOIE_MASK;
      }
    }
  }
  
  return result;
}

/*
 * LPLD_FTM_Deinit
 * FTM模块通用反初始化
 *
 * 参数:
 *    ftm_init_structure--FTM初始化结构体，
 *                        具体定义见FTM_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_Deinit(FTM_InitTypeDef ftm_init_structure)
{
  if(ftm_init_structure.FTM_Ftmx == FTM0)
  {
    SIM->SCGC6 &= ~SIM_SCGC6_FTM0_MASK;
  }
  else if(ftm_init_structure.FTM_Ftmx == FTM1)
  {
    SIM->SCGC6 &= ~SIM_SCGC6_FTM1_MASK;
  }
  else if(ftm_init_structure.FTM_Ftmx == FTM2)
  {
    SIM->SCGC3 &= ~SIM_SCGC3_FTM2_MASK;
  }
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(ftm_init_structure.FTM_Ftmx == FTM3)
  {
    SIM->SCGC3 &= ~SIM_SCGC3_FTM3_MASK;
  }
#endif
  else
  {
    return 0;
  }
  
  return LPLD_FTM_DisableIrq(ftm_init_structure);
}

/*
 * LPLD_FTM_PWM_Enable
 * FTM模块PWM模式输出使能，配置输出通道、占空比、指定对应的引脚、对齐方式
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *    duty--PWM输出占空比
 *      |__0~10000--占空比0.00%~100.00%
 *    pin--FTMx通道对应的引脚
 *      FTM0
 *       FTM_Ch0-PTA3\PTC1
 *       FTM_Ch1-PTA4\PTC2
 *       FTM_Ch2-PTA5\PTC3
 *       FTM_Ch3-PTA6\PTC4
 *       FTM_Ch4-PTA7\PTD4
 *       FTM_Ch5-PTA0\PTD5
 *       FTM_Ch6-PTA1\PTD6
 *       FTM_Ch7-PTA2\PTD7
 *      FTM1
 *       FTM_Ch0-PTA8\PTA12\PTB0
 *       FTM_Ch1-PTA9\PTA13\PTB1
 *      FTM2
 *       FTM_Ch0-PTA10\PTB18
 *       FTM_Ch1-PTA11\PTB19
 *      <注:只有MK60F系列含有FTM3>
 *      FTM3
 *       FTM_Ch0-PTE5\PTD0
 *       FTM_Ch1-PTE6\PTD1
 *       FTM_Ch2-PTE7\PTD2
 *       FTM_Ch3-PTE8\PTD3
 *       FTM_Ch4-PTE9\PTC8
 *       FTM_Ch5-PTE10\PTC9
 *       FTM_Ch6-PTE11\PTC10
 *       FTM_Ch7-PTE12\PTC11
 *    align--脉冲对齐方式
 *      |__ALIGN_LEFT    --左对齐
 *      |__ALIGN_RIGHT   --右对齐
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_PWM_Enable(FTM_Type *ftmx, FtmChnEnum_Type chn, uint32 duty, PortPinsEnum_Type pin, uint8 align)
{
  uint32 cv;
  vuint32 mod;
  
  //参数检查
  ASSERT( duty <= 10000 );                  //判断占空比
  
  if(!LPLD_FTM_PinInit(ftmx, chn, pin))
    return 0;
  
  //如果是右对齐，100%-占空比
  if(align == ALIGN_RIGHT)
  {
    duty = 10000 - duty;
  }
  
  //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)
  mod = ftmx->MOD;
  cv = (duty*(mod-0+1)+0)/10000;
  
  // 配置FTM通道控制寄存器 
  // 通道模式 MSB:MSA-1X, 通道边缘选择 左对齐 ELSB:ELSA-10
  // 通道模式 MSB:MSA-1X, 通道边缘选择 右对齐 ELSB:ELSA-X1
  ftmx->CONTROLS[chn].CnSC = align;
  // 配置FTM通道值
  ftmx->CONTROLS[chn].CnV  = cv;
  
  return 1;
}

/*
 * LPLD_FTM_PWM_ChangeDuty
 * 变更FTM模块PWM模式指定通道的输出占空比
 *
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *    duty--PWM输出占空比
 *      |__0~10000--占空比0.00%~100.00%
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_PWM_ChangeDuty(FTM_Type *ftmx, FtmChnEnum_Type chn, uint32 duty)
{
  uint32 cv;
  vuint32 mod;
  
  //参数检查
  ASSERT( duty <= 10000 );                  //判断占空比
    
  //如果是右对齐，100%-占空比
  if(ftmx->CONTROLS[chn].CnSC & FTM_CnSC_ELSA_MASK)
  {
    duty = 10000 - duty;
  }
  
  //占空比 = (CnV-CNTIN)/(MOD-CNTIN+1)
  mod = ftmx->MOD;
  cv = (duty*(mod-0+1)+0)/10000;
 
  // 配置FTM通道值
  ftmx->CONTROLS[chn].CnV = cv;
  
  return 1;
}

/*
 * LPLD_FTM_DisableChn
 * 禁用FTM模块指定通道的输出、输入
 *
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_DisableChn(FTM_Type *ftmx, FtmChnEnum_Type chn)
{
  LPLD_FTM_PinDeinit(ftmx, chn);
  ftmx->CONTROLS[chn].CnSC = 0;
  ftmx->CONTROLS[chn].CnV  = 0;
  
  return 1;
}



/*
 * LPLD_FTM_IC_Enable
 * FTM模块输入捕获模式输出使能，配置输入通道、指定对应的引脚、捕获边缘
 *
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *    duty--PWM输出占空比
 *      |__0~10000--占空比0.00%~100.00%
 *    pin--FTMx通道对应的引脚
 *      FTM0
 *       FTM_Ch0-PTA3\PTC1
 *       FTM_Ch1-PTA4\PTC2
 *       FTM_Ch2-PTA5\PTC3
 *       FTM_Ch3-PTA6\PTC4
 *       FTM_Ch4-PTA7\PTD4
 *       FTM_Ch5-PTA0\PTD5
 *       FTM_Ch6-PTA1\PTD6
 *       FTM_Ch7-PTA2\PTD7
 *      FTM1
 *       FTM_Ch0-PTA8\PTA12\PTB0
 *       FTM_Ch1-PTA9\PTA13\PTB1
 *      FTM2
 *       FTM_Ch0-PTA10\PTB18
 *       FTM_Ch1-PTA11\PTB19
 *      <注:只有MK60F系列含有FTM3>
 *      FTM3
 *       FTM_Ch0-PTE5\PTD0
 *       FTM_Ch1-PTE6\PTD1
 *       FTM_Ch2-PTE7\PTD2
 *       FTM_Ch3-PTE8\PTD3
 *       FTM_Ch4-PTE9\PTC8
 *       FTM_Ch5-PTE10\PTC9
 *       FTM_Ch6-PTE11\PTC10
 *       FTM_Ch7-PTE12\PTC11
 *    capture_edge--捕获边缘设置
 *      |__CAPTURE_RI    --上升沿捕获
 *      |__CAPTURE_FA    --下降沿捕获
 *      |__CAPTURE_RIFA  --上升下降沿捕获
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_IC_Enable(FTM_Type *ftmx, FtmChnEnum_Type chn, PortPinsEnum_Type pin, uint8 capture_edge)
{
  if(!LPLD_FTM_PinInit(ftmx, chn, pin))
    return 0;
  
  ftmx->CONTROLS[chn].CnSC = 0x00;
  
  ftmx->CONTROLS[chn].CnSC |= capture_edge;        
  
  ftmx->CONTROLS[chn].CnSC &= (~FTM_CnSC_CHF_MASK);
  ftmx->CONTROLS[chn].CnSC |= FTM_CnSC_CHIE_MASK;         //使能通道捕获输入中断
  
  ftmx->CONTROLS[chn].CnSC &= (~FTM_CnSC_MSB_MASK);
  ftmx->CONTROLS[chn].CnSC &= (~FTM_CnSC_MSA_MASK);       //配置成Input capture模式
  
  ftmx->CONTROLS[chn].CnSC &= (~FTM_CnSC_DMA_MASK);       //关闭DMA
  
  return 1;
}

/*
 * LPLD_FTM_IsTOF
 * 判断FTMx是否产生计数溢出中断标志
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *
 * 输出:
 *    TRUE、FALSE
 *
 */
__INLINE boolean LPLD_FTM_IsTOF(FTM_Type *ftmx)
{
  return (boolean)((ftmx->SC & FTM_SC_TOF_MASK)>>FTM_SC_TOF_SHIFT);
}

/*
 * LPLD_FTM_ClearTOF
 * 清除FTMx计数器溢出中断标志
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *
 * 输出:
 *    无
 *
 */
__INLINE void LPLD_FTM_ClearTOF(FTM_Type *ftmx)
{
  ftmx->SC = ftmx->SC & ~FTM_SC_TOF_MASK;
}

/*
 * LPLD_FTM_IsCHnF
 * 判断通道n是否产生中断标志
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *
 * 输出:
 *    TRUE、FALSE
 *
 */
__INLINE boolean LPLD_FTM_IsCHnF(FTM_Type *ftmx, FtmChnEnum_Type chn)
{
  return (boolean)((ftmx->CONTROLS[chn].CnSC & FTM_CnSC_CHF_MASK)>>FTM_CnSC_CHF_SHIFT);
}

/*
 * LPLD_FTM_ClearCHnF
 * 清除通道n中断标志
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *
 * 输出:
 *    无
 *
 */
__INLINE void LPLD_FTM_ClearCHnF(FTM_Type *ftmx, FtmChnEnum_Type chn)
{
  ftmx->CONTROLS[chn].CnSC = ftmx->CONTROLS[chn].CnSC & ~FTM_CnSC_CHF_MASK;
}

/*
 * LPLD_FTM_GetChVal
 * 获取FTMx通道n捕获的FTMx计数值
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *    chn--PWM输出通道
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM_Ch0          --FTMx通道0(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch1          --FTMx通道1(FTM0\FTM1\FTM2\FTM3)
 *      |__FTM_Ch2          --FTMx通道2(FTM0\FTM3)
 *      |__FTM_Ch3          --FTMx通道3(FTM0\FTM3)
 *      |__FTM_Ch4          --FTMx通道4(FTM0\FTM3)
 *      |__FTM_Ch5          --FTMx通道5(FTM0\FTM3)
 *      |__FTM_Ch6          --FTMx通道6(FTM0\FTM3)
 *      |__FTM_Ch7          --FTMx通道7(FTM0\FTM3)
 *
 * 输出:
 *    0x1~0xFFFF
 *
 */
__INLINE uint16 LPLD_FTM_GetChVal(FTM_Type *ftmx, FtmChnEnum_Type chn)
{
  return ftmx->CONTROLS[chn].CnV;    //获取相应通道捕获到的FTM计数器值
}

/*
 * LPLD_FTM_GetClkDiv
 * 获取FTMx时钟分频系数
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *
 * 输出:
 *    1、2、4、8、16、32、64、128
 *
 */
__INLINE uint8 LPLD_FTM_GetClkDiv(FTM_Type *ftmx)
{
  return 1u<<((ftmx->SC & FTM_SC_PS_MASK)>>FTM_SC_PS_SHIFT);    //获得时钟分频系数
}

/*
 * LPLD_FTM_GetCounter
 * 获取FTMx计数器值
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *
 * 输出:
 *    无
 *
 */
__INLINE uint16 LPLD_FTM_GetCounter(FTM_Type *ftmx)
{
  return ftmx->CNT;        //获取FTMx计数器值
}

/*
 * LPLD_FTM_ClearCounter
 * 清空FTMx计数器
 * 
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM0          --FTM0
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *      <注:只有MK60F系列含有FTM3>
 *      |__FTM3          --FTM3
 *
 * 输出:
 *    无
 *
 */
__INLINE void LPLD_FTM_ClearCounter(FTM_Type *ftmx)
{
  ftmx->CNT = 0;        //清空FTMx计数器值
}

/*
 * LPLD_FTM_EnableIrq
 * 使能FTMx中断
 * 
 * 参数:
 *    ftm_init_structure--FTM初始化结构体，
 *                        具体定义见FTM_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_FTM_EnableIrq(FTM_InitTypeDef ftm_init_structure)
{
  uint8 i;
  FTM_Type *ftmx = ftm_init_structure.FTM_Ftmx;
  
  if(ftmx == FTM0)
    i=0;
  else if(ftmx == FTM1)
    i=1;
  else if(ftmx == FTM2)
    i=2;
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(ftmx == FTM3)
    i=3;
#endif
  else
    return 0;

  enable_irq((IRQn_Type)(FTM0_IRQn + i));
  
  return 1;
}

/*
 * LPLD_FTM_DisableIrq
 * 禁用FTMx中断
 * 
 * 参数:
 *    ftm_init_structure--FTM初始化结构体，
 *                        具体定义见FTM_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_FTM_DisableIrq(FTM_InitTypeDef ftm_init_structure)
{
  uint8 i;
  FTM_Type *ftmx = ftm_init_structure.FTM_Ftmx;
  
  if(ftmx == FTM0)
    i=0;
  else if(ftmx == FTM1)
    i=1;
  else if(ftmx == FTM2)
    i=2;
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(ftmx == FTM3)
    i=3;
#endif
  else
    return 0;

  disable_irq((IRQn_Type)(FTM0_IRQn + i));
  
  return 1;
}

/*
 * LPLD_FTM_QD_Enable
 * FTM模块正交解码模式输入使能，配置输入通道PHA和PHB
 *
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *    pha--PHA输入通道引脚
 *      FTM1
 *        PTA8\PTA12\PTB0
 *      FTM2
 *        PTA10\PTB18
 *    phb--PHB输入通道引脚
 *      FTM1
 *        PTA9\PTA13\PTB1
 *      FTM2
 *        PTA11\PTB19
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_QD_Enable(FTM_Type *ftmx, PortPinsEnum_Type pha, PortPinsEnum_Type phb)
{
  if(!LPLD_FTM_PinInit(ftmx, FTM_PhA, pha))
    return 0;
  if(!LPLD_FTM_PinInit(ftmx, FTM_PhB, phb))
    return 0;
  
  return 1;
}

/*
 * LPLD_FTM_QD_Disable
 * FTM模块正交解码模式输入禁用
 *
 * 参数:
 *    ftmx--FTMx模块号
 *      |__FTM1          --FTM1
 *      |__FTM2          --FTM2
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_FTM_QD_Disable(FTM_Type *ftmx)
{
  if(!LPLD_FTM_PinDeinit(ftmx, FTM_PhA))
    return 0;
  if(!LPLD_FTM_PinDeinit(ftmx, FTM_PhB))
    return 0;
  
  return 1;
}

/*
 * LPLD_FTM_PWM_Init
 * FTM模块PWM模式初始化，内部调用
 */
static uint8 LPLD_FTM_PWM_Init(FTM_InitTypeDef ftm_init_structure)
{
  uint32 bus_clk_hz;
  uint32 mod, mod2;
  uint8 ps;
  uint32 freq = ftm_init_structure.FTM_PwmFreq;
  uint32 dt_en = ftm_init_structure.FTM_PwmDeadtimeCfg;
  uint8 dt_div = ftm_init_structure.FTM_PwmDeadtimeDiv;
  uint8 dt_val = ftm_init_structure.FTM_PwmDeadtimeVal;
  FTM_Type *ftmx = ftm_init_structure.FTM_Ftmx;
  
  //参数检查
  ASSERT( freq );                  //判断频率
  ASSERT( dt_val<=63 );            //判断死区插入值
  
  bus_clk_hz = g_bus_clock;
  
  if(freq>bus_clk_hz) return 0;
  
  if((mod=bus_clk_hz/(freq*128)) < 0xFFFFu)
  {
    ps = 7;
    mod2=mod;
    if((mod=bus_clk_hz/(freq*64)) < 0xFFFFu)
    {
      ps = 6;  
      mod2=mod;  
      if((mod=bus_clk_hz/(freq*32)) < 0xFFFFu)
      {
        ps = 5;  
        mod2=mod;
        if((mod=bus_clk_hz/(freq*16)) < 0xFFFFu)
        {
          ps = 4;  
          mod2=mod;   
          if((mod=bus_clk_hz/(freq*8)) < 0xFFFFu)
          {
            ps = 3;
            mod2=mod;
            if((mod=bus_clk_hz/(freq*4)) < 0xFFFFu)
            {
              ps = 2;
              mod2=mod;
              if((mod=bus_clk_hz/(freq*2)) < 0xFFFFu)
              {
                ps = 1;
                mod2=mod;
                if((mod=bus_clk_hz/(freq*1)) < 0xFFFFu)
                {
                  ps = 0;
                  mod2=mod;
                }
              }
            }
          }
        }  
      }
    }
  }
  else
  {
    return 0;
  }
  
  ftmx->SC = 0;
  
  // 设置PWM周期及占空比
  //    PWM周期 = (MOD-CNTIN+1)*FTM时钟周期 :
  // 配置FTM计数初始值
  ftmx->CNT = 0;
  ftmx->CNTIN = 0;
  // 配置FTM计数MOD值
  ftmx->MOD = mod2;
  
  ftmx->DEADTIME = FTM_DEADTIME_DTPS(dt_div) | FTM_DEADTIME_DTVAL(dt_val);
  ftmx->COMBINE = dt_en;        //使能死区
  
  // 配置FTM控制寄存器
  // 禁用中断, 加计数模式, 时钟源:System clock（Bus Clk）, 分频系数:8
  // 假设SysClk = 50MHz, SC_PS=3, FTM Clk = 50MHz/2^3 = 6.25MHz
  ftmx->SC = FTM_SC_CLKS(1)|FTM_SC_PS(ps);
  
  return 1;
}

/*
 * LPLD_FTM_IC_Init
 * FTM模块输入捕获模式初始化，内部调用
 */
static uint8 LPLD_FTM_IC_Init(FTM_InitTypeDef ftm_init_structure)
{
  uint8 i;
  uint8 ps = ftm_init_structure.FTM_ClkDiv;
  FTM_ISR_CALLBACK isr_func = ftm_init_structure.FTM_Isr;
  FTM_Type *ftmx = ftm_init_structure.FTM_Ftmx;
  
  //参数检查
  ASSERT( ps <= 7);             //时钟分频系数
  
  ftmx->CONF=FTM_CONF_BDMMODE(0x3);
  
  ftmx->SC = 0;
  
  ftmx->CNT = 0;
  ftmx->CNTIN = 0;
  ftmx->MOD = 0;                           
  ftmx->QDCTRL = (~FTM_QDCTRL_QUADEN_MASK); //关闭正交解码
  ftmx->FILTER = 0x00;                      //关过虑器
  
  // 配置FTM控制寄存器
  // 将FTM Counter配置成Free Counter
  // 禁用中断, 加计数模式, 时钟源:System clock（Bus Clk）, 分频系数:PS
  // 假设SysClk = 50MHz, SC_PS=3, FTM Clk = 50MHz/2^3 = 6.25MHz
  ftmx->SC |= FTM_SC_CLKS(1)|FTM_SC_PS(ps);
  //ftmx->SC |= FTM_SC_TOIE_MASK;             //使能计数溢出中断
  ftmx->SC &= (~FTM_SC_CPWMS_MASK);         //FTM加计数

  //设置中断函数入口地址并开启中断
  if(isr_func!=NULL)
  {
    if(ftmx == FTM0)
      i=0;
    else if(ftmx == FTM1)
      i=1;
    else if(ftmx == FTM2)
      i=2;
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
    else if(ftmx == FTM3)
      i=3;
#endif   
    else
      return 0;
    FTM_ISR[i] = isr_func;
  }

  return 1;
}

/*
 * LPLD_FTM_PinInit
 * 初始化通道n指定引脚的FTM复用功能，内部调用
 */
static uint8 LPLD_FTM_PinInit(FTM_Type *ftmx, FtmChnEnum_Type chn, PortPinsEnum_Type pin)
{ 
  //根据ftmx使能相应pin的ftm功能
  if(ftmx == FTM0)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if(pin == PTA3)
        PORTA->PCR[3] = PORTA->PCR[3] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTC1)
        PORTC->PCR[1] = PORTC->PCR[1] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch1:
      if(pin == PTA4)
        PORTA->PCR[4] = PORTA->PCR[4] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTC2)
        PORTC->PCR[2] = PORTC->PCR[2] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch2:
      if(pin == PTA5)
        PORTA->PCR[5] = PORTA->PCR[5] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTC3)
        PORTC->PCR[3] = PORTC->PCR[3] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch3:
      if(pin == PTA6)
        PORTA->PCR[6] = PORTA->PCR[6] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTC4)
        PORTC->PCR[4] = PORTC->PCR[4] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch4:
      if(pin == PTA7)
        PORTA->PCR[7] = PORTA->PCR[7] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTD4)
        PORTD->PCR[4] = PORTD->PCR[4] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch5:
      if(pin == PTA0)
        PORTA->PCR[0] = PORTA->PCR[0] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTD5)
        PORTD->PCR[5] = PORTD->PCR[5] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch6:
      if(pin == PTA1)
        PORTA->PCR[1] = PORTA->PCR[1] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTD6)
        PORTD->PCR[6] = PORTD->PCR[6] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch7:
      if(pin == PTA2)
        PORTA->PCR[2] = PORTA->PCR[2] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTD7)
        PORTD->PCR[7] = PORTD->PCR[7] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    default:
      return 0;     
    }
  }
  else if(ftmx == FTM1)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if(pin == PTA8)
        PORTA->PCR[8] = PORTA->PCR[8] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTA12)
        PORTA->PCR[12] = PORTA->PCR[12] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else if(pin == PTB0)
        PORTB->PCR[0] = PORTB->PCR[0] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_Ch1:
      if(pin == PTA9)
        PORTA->PCR[9] = PORTA->PCR[9] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTA13)
        PORTA->PCR[13] = PORTA->PCR[13] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else if(pin == PTB1)
        PORTB->PCR[1] = PORTB->PCR[1] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_PhA:
      if(pin == PTA8)
        PORTA->PCR[8] = PORTA->PCR[8] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTA12)
        PORTA->PCR[12] = PORTA->PCR[12] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(7);
      else if(pin == PTB0)
        PORTB->PCR[0] = PORTB->PCR[0] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6);
      else
        return 0;
      break;
    case FTM_PhB:
      if(pin == PTA9)
        PORTA->PCR[9] = PORTA->PCR[9] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTA13)
        PORTA->PCR[13] = PORTA->PCR[13] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(7);
      else if(pin == PTB1)
        PORTB->PCR[1] = PORTB->PCR[1] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6);
      else
        return 0;
    default:
      return 0;   
    }
  }
  else if(ftmx == FTM2)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if(pin == PTA10)
        PORTA->PCR[10] = PORTA->PCR[10] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTB18)
        PORTB->PCR[18] = PORTB->PCR[18] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_Ch1:
      if(pin == PTA11)
        PORTA->PCR[11] = PORTA->PCR[11] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3); 
      else if(pin == PTB19)
        PORTB->PCR[19] = PORTB->PCR[19] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_PhA:
      if(pin == PTA10)
        PORTA->PCR[10] = PORTA->PCR[10] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTB18)
        PORTB->PCR[18] = PORTB->PCR[18] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6);
      else
        return 0;
      break;
    case FTM_PhB:
      if(pin == PTA11)
        PORTA->PCR[11] = PORTA->PCR[11] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTB19)
        PORTB->PCR[19] = PORTB->PCR[19] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6);
      else
        return 0;
      break;
    default:
      return 0;   
    }
  }
  
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(ftmx == FTM3)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if(pin == PTE5)
        PORTE->PCR[5] = PORTE->PCR[5] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTD0)
        PORTD->PCR[0] = PORTD->PCR[0] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch1:
      if(pin == PTE6)
        PORTE->PCR[6] = PORTE->PCR[6] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTD0)
        PORTD->PCR[1] = PORTD->PCR[1] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch2:
      if(pin == PTE7)
        PORTE->PCR[7] = PORTE->PCR[7] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTD0)
        PORTD->PCR[2] = PORTD->PCR[2] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch3:
      if(pin == PTE8)
        PORTE->PCR[8] = PORTE->PCR[8] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTD0)
        PORTD->PCR[3] = PORTD->PCR[3] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(4);
      else
        return 0;
      break;
    case FTM_Ch4:
      if(pin == PTE9)
        PORTE->PCR[9] = PORTE->PCR[9] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTC8)
        PORTC->PCR[8] = PORTC->PCR[8] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_Ch5:
      if(pin == PTE10)
        PORTE->PCR[10] = PORTE->PCR[10] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTC9)
        PORTC->PCR[9] = PORTC->PCR[9] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_Ch6:
      if(pin == PTE11)
        PORTE->PCR[11] = PORTE->PCR[11] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTC10)
        PORTC->PCR[10] = PORTC->PCR[10] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    case FTM_Ch7:
      if(pin == PTE12)
        PORTE->PCR[12] = PORTE->PCR[12] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(6); 
      else if(pin == PTC11)
        PORTC->PCR[11] = PORTC->PCR[11] & ~PORT_PCR_MUX_MASK | PORT_PCR_MUX(3);
      else
        return 0;
      break;
    default:
      return 0;     
    }
  }
#endif
  else
  {
    return 0;
  }
  return 1;
}

/*
 * LPLD_FTM_PinInit
 * 反初始化通道n指定引脚的FTM复用功能，内部调用
 */
static uint8 LPLD_FTM_PinDeinit(FTM_Type *ftmx, FtmChnEnum_Type chn)
{
  //根据ftmx禁用相应pin的ftm功能，回复到默认功能
  if(ftmx == FTM0)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if((PORTA->PCR[3]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[3] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[1]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTC->PCR[1] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch1:
      if((PORTA->PCR[4]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[4] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[2]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTC->PCR[2] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch2:
      if((PORTA->PCR[5]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[5] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[3]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTC->PCR[3] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch3:
      if((PORTA->PCR[6]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[6] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[4]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTC->PCR[4] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch4:
      if((PORTA->PCR[7]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[7] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[4]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[4] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch5:
      if((PORTA->PCR[0]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[0] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[5]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[5] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch6:
      if((PORTA->PCR[1]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[1] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[6]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[6] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch7:
      if((PORTA->PCR[2]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[2] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[7]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[7] = PORT_PCR_MUX(0);      
      break;
    default:
      return 0;     
    }
  }
  else if(ftmx == FTM1)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if((PORTA->PCR[8]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[8] = PORT_PCR_MUX(0); 
      if((PORTA->PCR[12]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[12] = PORT_PCR_MUX(0);
      if((PORTB->PCR[0]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTB->PCR[0] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch1:
      if((PORTA->PCR[9]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[9] = PORT_PCR_MUX(0); 
      if((PORTA->PCR[13]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[13] = PORT_PCR_MUX(0);
      if((PORTB->PCR[1]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTB->PCR[1] = PORT_PCR_MUX(0);
      break;
    case FTM_PhA:
      if((PORTA->PCR[8]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTA->PCR[8] = PORT_PCR_MUX(0); 
      if((PORTA->PCR[12]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(7))
        PORTA->PCR[12] = PORT_PCR_MUX(0);
      if((PORTB->PCR[0]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTB->PCR[0] = PORT_PCR_MUX(0);
      break;
    case FTM_PhB:
      if((PORTA->PCR[9]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTA->PCR[9] = PORT_PCR_MUX(0); 
      if((PORTA->PCR[13]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(7))
        PORTA->PCR[13] = PORT_PCR_MUX(0);
      if((PORTB->PCR[1]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTB->PCR[1] = PORT_PCR_MUX(0);
      break;
    default:
      return 0;   
    }
  }
  else if(ftmx == FTM2)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if((PORTA->PCR[10]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[10] = PORT_PCR_MUX(0); 
      if((PORTB->PCR[18]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTB->PCR[18] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch1:
      if((PORTA->PCR[11]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTA->PCR[11] = PORT_PCR_MUX(0); 
      if((PORTB->PCR[19]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTB->PCR[19] = PORT_PCR_MUX(0);
      break;
    case FTM_PhA:
      if((PORTA->PCR[10]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTA->PCR[10] = PORT_PCR_MUX(0); 
      if((PORTB->PCR[18]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTB->PCR[18] = PORT_PCR_MUX(0);
      break;
    case FTM_PhB:
      if((PORTA->PCR[11]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTA->PCR[11] = PORT_PCR_MUX(0); 
      if((PORTB->PCR[19]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTB->PCR[19] = PORT_PCR_MUX(0);
      break;
    default:
      return 0;   
    }
  }
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(ftmx == FTM3)
  {
    switch(chn)
    {
    case FTM_Ch0:
      if((PORTE->PCR[5]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[5] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[0]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[0] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch1:
      if((PORTE->PCR[6]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[6] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[1]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[1] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch2:
      if((PORTE->PCR[7]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[7] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[2]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[2] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch3:
      if((PORTE->PCR[8]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[8] = PORT_PCR_MUX(0); 
      if((PORTD->PCR[3]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(4))
        PORTD->PCR[3] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch4:
      if((PORTE->PCR[9]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[9] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[8]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTC->PCR[8] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch5:
      if((PORTE->PCR[10]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[10] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[9]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTC->PCR[9] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch6:
      if((PORTE->PCR[11]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[11] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[10]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTC->PCR[10] = PORT_PCR_MUX(0);
      break;
    case FTM_Ch7:
      if((PORTE->PCR[12]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(6))
        PORTE->PCR[12] = PORT_PCR_MUX(0); 
      if((PORTC->PCR[11]& PORT_PCR_MUX_MASK) == PORT_PCR_MUX(3))
        PORTC->PCR[11] = PORT_PCR_MUX(0);
      break;
    default:
      return 0;     
    }
  }
#endif
  else
  {
    return 0;
  }
  return 1;
}

/*
 * LPLD_FTM_QD_Init
 * FTM模块正交解码模式初始化，内部调用
 */
static uint8 LPLD_FTM_QD_Init(FTM_InitTypeDef ftm_init_structure)
{
  uint8 mode = ftm_init_structure.FTM_QdMode;
  FTM_Type *ftmx = ftm_init_structure.FTM_Ftmx;
 
  ftmx->MODE |= FTM_MODE_FTMEN_MASK;    //FTM2EN=1   
  
  ftmx->CNTIN = 0;//FTM0计数器初始值为0  
  ftmx->CNT=0;  
  ftmx->MOD = 0xFFFF;//结束值  
  
  ftmx->QDCTRL |= mode;     //解码模式选择
  ftmx->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;       //使能正交解码模式 

  return 1;
}

static uint8 LPLD_FTM_DEC_Init(FTM_InitTypeDef ftm_init_structure)
{
  
  return 0;
}

/*
 * FTM0--FTM1中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void FTM0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  FTM_ISR[0]();
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

void FTM1_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  FTM_ISR[1]();
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

void FTM2_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  FTM_ISR[2]();
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
void FTM3_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  FTM_ISR[3]();
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
#endif