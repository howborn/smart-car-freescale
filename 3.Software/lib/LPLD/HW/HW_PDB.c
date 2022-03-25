/**
 * @file HW_PDB.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief PDB底层模块相关函数
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
#include "HW_PDB.h"

//用户自定义中断服务函数数组
PDB_ISR_CALLBACK PDB_ISR[1];
PDB_ISR_CALLBACK PDB_SE_ISR[1];

//PDB时钟预分频系数乘数
const uint8 pdb_sc_mults[4] = {1, 10, 20, 40};


/*
 * LPLD_PDB_Init
 * PDBx模块通用初始化函数，初始化PDBx计数器周期、工作模式、触发源、中断等配置
 * 
 * 参数:
 *    pdb_init_struct--PDBx初始化结构体，
 *                        具体定义见PDB_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_PDB_Init(PDB_InitTypeDef pdb_init_struct)
{
  uint32 us = pdb_init_struct.PDB_CounterPeriodUs
            + pdb_init_struct.PDB_CounterPeriodMs*1000
            + pdb_init_struct.PDB_CounterPeriodS*1000000;
  uint32 delayus = pdb_init_struct.PDB_DelayUs
            + pdb_init_struct.PDB_DelayMs*1000
            + pdb_init_struct.PDB_DelayS*1000000;
  uint8 loadmode = pdb_init_struct.PDB_LoadModeSel;
  uint8 trriger = pdb_init_struct.PDB_TriggerInputSourceSel;
  uint32 bus_clk = g_bus_clock/1000000;
  uint8 prescaler, mult;
  uint32 mod;
  float32 temp;
  
  //参数检查
  ASSERT( loadmode <= LOADMODE_3 );       //加载模式选择
  ASSERT( trriger <= TRIGGER_SOFTWARE );  //触发输入源选择
  
/*
 *************************************************
  【LPLD注解】PDB关键系数
  prescaler(总线时钟预分频系数): 2^n(n:0~7)
  mult(prescaler的倍数): 1、10、20、40
  mod(计数器模数，影响计数器周期): 0~0xFFFF
  计数器周期 = (prescaler*mult*mod)/bus_clk
 *************************************************
 */
  
  //根据期望的计数器周期自动设定prescaler、mult和mod的值
  for(uint8 i=0; i<4; i++)
  {
    mult = i;
    for(uint8 j=0; j<8; j++)
    {
      prescaler = j;
      mod = (bus_clk*us)/((1<<j)*pdb_sc_mults[i]);
      if(mod <= 0xFFFFu)
        break;
    }
    if(mod <= 0xFFFFu)
      break;
    else if(i == 3)
      return 0;         //PDB计数周期设置的过小，计算错误
  }
  
  //使能PDB时钟
  SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;
  
  //初始化SC寄存器
  PDB0->SC = 0x00;
  //使能PDB
  PDB0->SC |= PDB_SC_PDBEN_MASK;
  PDB0->SC |= PDB_SC_MULT(mult);
  PDB0->SC |= PDB_SC_PRESCALER(prescaler);
  //配置MOD、IDLY、CHnDLYm、INTx和POyDLY寄存器的加载模式
  PDB0->SC |= PDB_SC_LDMOD(loadmode); 
  //配置触发输入源
  PDB0->SC |= PDB_SC_TRGSEL(trriger);
  
  //是否使能连续模式
  if(pdb_init_struct.PDB_ContinuousModeEnable == TRUE)
    PDB0->SC |= PDB_SC_CONT_MASK;
  else
    PDB0->SC &= ~(PDB_SC_CONT_MASK);
  
  //是否使能DMA
  if(pdb_init_struct.PDB_DmaEnable == TRUE)
    PDB0->SC |= PDB_SC_DMAEN_MASK;
  else
    PDB0->SC &= ~(PDB_SC_DMAEN_MASK);
  
  if(pdb_init_struct.PDB_Isr != NULL)
  {
    PDB_ISR[0] = pdb_init_struct.PDB_Isr;
    //是否使能PDB中断
    if(pdb_init_struct.PDB_IntEnable == TRUE)
      PDB0->SC |= PDB_SC_PDBIE_MASK;
    else
      PDB0->SC &= ~(PDB_SC_PDBIE_MASK);
  }
  
  if(pdb_init_struct.PDB_SeqErrIsr != NULL)
  {
    PDB_SE_ISR[0] = pdb_init_struct.PDB_SeqErrIsr;
    //是否使能PDB序列错误中断
    if(pdb_init_struct.PDB_SeqErrIntEnable == TRUE)
      PDB0->SC |= PDB_SC_PDBEIE_MASK;
    else
      PDB0->SC &= ~(PDB_SC_PDBEIE_MASK);
  }
  
  //设置PDB中断延时时间
  if( delayus <= us )
  {
    temp = (float32)mod/(float32)us;
    PDB0->IDLY = (uint32)(delayus*temp);
  }
  
  //加载值
  PDB0->SC |= PDB_SC_LDOK_MASK;
  
  return 1;
}

/*
 * LPLD_PDB_Deinit
 * PDBx模块反初始化函数，禁用PDBx所有功能
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_PDB_Deinit(void)
{
 
  PDB0->CH[0].C1 = 0;
  PDB0->CH[1].C1 = 0;
  
  PDB0->DAC[0].INTC = 0;
  PDB0->DAC[1].INTC = 0;
  
  PDB0->SC = 0;
  
  //禁用PDB时钟
  SIM->SCGC6 &= ~(SIM_SCGC6_PDB_MASK);
  
  disable_irq(PDB0_IRQn);
}


/*
 * LPLD_PDB_AdcTriggerCfg
 * PDBx触发ADCx模块配置函数
 * 
 * 参数:
 *    adcx--ADCx模块号
 *      |__ADC0          --ADC0
 *      |__ADC1          --ADC1
 *    cfg--ADCx预触发功能配置，可多个值合并配置(eg. cfg=PRETRIG_EN_A|PRETRIG_DLY_A;)
 *      ADCx A组配置
 *      |__PRETRIG_EN_A  --使能ADCx的A组预触发功能
 *      |__PRETRIG_DLY_A --使能ADCx的A组延时输出预触发功能
 *      |__PRETRIG_BB_A  --使能ADCx的A组“Back to Back”模式
 *      ADCx B组配置
 *      |__PRETRIG_EN_B  --使能ADCx的B组预触发功能
 *      |__PRETRIG_DLY_B --使能ADCx的B组延时输出预触发功能
 *      |__PRETRIG_BB_B  --使能ADCx的B组“Back to Back”模式
 *    delay--ADCx A或B组的延时输出预触发时间，单位us
 *      |__0~0xFFFF      --0~4095us
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_PDB_AdcTriggerCfg(ADC_Type *adcx, uint32 cfg, uint16 delay)
{
  uint8 n,m;
  uint8 prescaler, mult;
  uint32 bus_clk = g_bus_clock/1000000;

  if(adcx == ADC0)
    n = 0;
  else if(adcx == ADC1)
    n = 1;
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
  else if(adcx == ADC2)
    n = 2;
  else if(adcx == ADC3)
    n = 3;  
#endif 
  else
    return 0;
    
  if(cfg & 0x01)
    m = 0;
  else if(cfg & 0x02)
    m = 1;
  else
    return 0;
  
  prescaler = 1<<((PDB0->SC & PDB_SC_PRESCALER_MASK) >> PDB_SC_PRESCALER_SHIFT);
  mult = pdb_sc_mults[(PDB0->SC & PDB_SC_MULT_MASK) >> PDB_SC_MULT_SHIFT];
  
  PDB0->CH[n].C1 |= cfg;        //根据参数配置BB模式、选择预触发输出、使能预触发
  PDB0->CH[n].DLY[m] = (bus_clk * delay)/(prescaler * mult);    //配置ADCn的m组预触发延时值
  
  PDB0->SC |= PDB_SC_LDOK_MASK; //加载值
  
  return 1;
}

/*
 * LPLD_PDB_DacTriggerCfg
 * PDBx触发DACx模块配置函数
 * 
 * 参数:
 *    dacx--DACx模块号
 *      |__DAC0          --DAC0
 *      |__DAC1          --DAC1
 *    cfg--DACx触发功能配置
 *      |__EXT_TRIG_EN   --使能外部触发
 *      |__EINT_TRIG_EN  --使能内部间隔触发
 *    int_delay--DACx间隔触发时间，单位us
 *      |__0~0xFFFF      --0~4095us
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_PDB_DacTriggerCfg(DAC_Type *dacx, uint32 cfg, uint16 int_delay)
{
  uint8 n;
  uint8 prescaler, mult;
  uint32 bus_clk = g_bus_clock/1000000;
  
  if(dacx == DAC0)
    n = 0;
  else if(dacx == DAC1)
    n = 1;
  else
    return 0;
  
  prescaler = 1<<((PDB0->SC & PDB_SC_PRESCALER_MASK) >> PDB_SC_PRESCALER_SHIFT);
  mult = pdb_sc_mults[(PDB0->SC & PDB_SC_MULT_MASK) >> PDB_SC_MULT_SHIFT];
  
  PDB0->DAC[n].INTC |= cfg;     //配置使能外部触发或者内部间隔触发
  PDB0->DAC[n].INT = (bus_clk * int_delay)/(prescaler * mult);    //配置DACn的间隔触发时间
  
  PDB0->SC |= PDB_SC_LDOK_MASK; //加载值
  
  return 1;
}

/*
 * PDB中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void PDB_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //是否为PDB中断
  if((PDB0->SC & PDB_SC_PDBIF_MASK) && (PDB0->SC & PDB_SC_PDBIE_MASK))
  {
    //调用用户自定义中断服务
    PDB_ISR[0]();  
    //清除中断标志位  
    PDB0->SC &= ~PDB_SC_PDBIF_MASK;
  }
  
  //是否为PDB序列错误中断 CH0
  if((PDB0->CH[0].S & PDB_S_ERR(1)) && (PDB0->SC & PDB_SC_PDBEIE_MASK))
  {
    //调用用户自定义中断服务
    PDB_SE_ISR[0]();  
    PDB0->CH[0].S |= PDB_S_ERR(1);
  }
  
  //是否为PDB序列错误中断 CH1
  if((PDB0->CH[1].S & PDB_S_ERR(2)) && (PDB0->SC & PDB_SC_PDBEIE_MASK))
  {
    //调用用户自定义中断服务
    PDB_SE_ISR[0]();  
    PDB0->CH[1].S |= PDB_S_ERR(2);
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}