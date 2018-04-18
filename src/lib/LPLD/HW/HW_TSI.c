/**
 * @file HW_TSI.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief TSI底层模块相关函数
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
#include "HW_TSI.h"


//用户自定义中断服务函数数组
TSI_ISR_CALLBACK TSI_ISR[2];

//通道基准值
uint16 TSI_ChxBaseVal[16];

const uint16* TSI_CNTR[16]={(uint16*)&TSI0->CNTR1, (uint16*)(&TSI0->CNTR1)+1,
                            (uint16*)&TSI0->CNTR3, (uint16*)(&TSI0->CNTR3)+1,
                            (uint16*)&TSI0->CNTR5, (uint16*)(&TSI0->CNTR5)+1,
                            (uint16*)&TSI0->CNTR7, (uint16*)(&TSI0->CNTR7)+1,
                            (uint16*)&TSI0->CNTR9, (uint16*)(&TSI0->CNTR9)+1,
                            (uint16*)&TSI0->CNTR11, (uint16*)(&TSI0->CNTR11)+1,
                            (uint16*)&TSI0->CNTR13, (uint16*)(&TSI0->CNTR13)+1,
                            (uint16*)&TSI0->CNTR15, (uint16*)(&TSI0->CNTR15)+1}; 
 
const uint32* TSI_PORT[32]={(uint32*)&PORTB->PCR[0], (uint32*)&PORTA->PCR[0], (uint32*)&PORTA->PCR[1], (uint32*)&PORTA->PCR[2],
                            (uint32*)&PORTA->PCR[3], (uint32*)&PORTA->PCR[4], (uint32*)&PORTB->PCR[1], (uint32*)&PORTB->PCR[2], 
                            (uint32*)&PORTB->PCR[3], (uint32*)&PORTB->PCR[16], (uint32*)&PORTB->PCR[17], (uint32*)&PORTB->PCR[18], 
                            (uint32*)&PORTB->PCR[19], (uint32*)&PORTC->PCR[0], (uint32*)&PORTC->PCR[1], (uint32*)&PORTC->PCR[2]};

/*
 * LPLD_TSI_Init
 * TSI通用初始化函数
 * 
 * 参数:
 *    tsi_init_struct--TSI初始化结构体，
 *                        具体定义见TSI_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_TSI_Init(TSI_InitTypeDef tsi_init_struct)
{
  uint16 chs = tsi_init_struct.TSI_Chs;
  uint8 mode = tsi_init_struct.TSI_ScanTriggerMode;
  uint8 esor = tsi_init_struct.TSI_EndScanOrOutRangeInt;
  boolean err = tsi_init_struct.TIS_ErrIntEnable;
   
  //参数检查
  ASSERT( chs != 0);                  //判断通道
  ASSERT( mode <= TSI_SCAN_PERIOD);   //判断中断模式
  ASSERT( esor <= TSI_ENDOFDCAN_INT);//判断中断模式
  
  //开启TSI模块时钟
  SIM->SCGC5 |= (SIM_SCGC5_TSI_MASK); 
  
  //这里使能相关通道的引脚复用功能
  for(uint8 i=0; i<16; i++)
  {
    if((chs>>i)&0x1L)
    {
      *((uint32 *)TSI_PORT) = PORT_PCR_MUX(0); 
    }
  }
  
  //配置通用控制寄存器
  TSI0->GENCS = ((TSI_GENCS_NSCN(10))  //每个电极每次扫描(10)次、
                  |(TSI_GENCS_PS(3)));  //电极晶振(2^3)分频
  //配置扫描寄存器
  TSI0->SCANC = ((TSI_SCANC_EXTCHRG(3))        //0.8pF内部参考电容
                  |(TSI_SCANC_REFCHRG(31))      //32uA充电电流
#if defined(CPU_MK60DZ10)
                  |(TSI_SCANC_DELVOL(7))        //600mV间隔电压
#endif
                  |(TSI_SCANC_SMOD(0))          //扫描周期mod
                  |(TSI_SCANC_AMPSC(0)));       //输入时钟(2^0)分频
  //使能通道
  TSI0->PEN = chs;
  
  //使能相关中断
  //TSI_ENDOFDCAN_INT和TSI_OUTOFRANGE_INT只能选择其一
  if(tsi_init_struct.TSI_EndScanIsr!=NULL && esor==TSI_ENDOFDCAN_INT)
  {
    TSI0->GENCS |= TSI_GENCS_ESOR_MASK;
    TSI_ISR[0] = tsi_init_struct.TSI_EndScanIsr;
    TSI0->GENCS |= TSI_GENCS_TSIIE_MASK;   
  }
  else if(tsi_init_struct.TSI_OutRangeIsr!=NULL && esor==TSI_OUTOFRANGE_INT)
  {
    TSI0->GENCS &= ~(TSI_GENCS_ESOR_MASK);
    TSI_ISR[0] = tsi_init_struct.TSI_OutRangeIsr;
    TSI0->GENCS |= TSI_GENCS_TSIIE_MASK;  
  }
  if(tsi_init_struct.TSI_ErrIsr!=NULL && err==TRUE)
  {
    TSI0->GENCS |= TSI_GENCS_ERIE_MASK;
    TSI_ISR[1] = tsi_init_struct.TSI_EndScanIsr;
    TSI0->GENCS |= TSI_GENCS_TSIIE_MASK;  
  }
    
  //使能TSI模块  
  TSI0->GENCS |= (TSI_GENCS_TSIEN_MASK); 
 
  //是否进行基准值校验
  if(tsi_init_struct.TSI_IsInitSelfCal == TRUE)
  {
    //使能软件触发
    TSI0->GENCS |= TSI_GENCS_SWTS_MASK;
    //等待扫描结束
    while(!(TSI0->GENCS&TSI_GENCS_EOSF_MASK)){};
    for(uint16 i=0; i<5000; i++)
    {
      for(uint8 j=0; j<200; j++)
      {
        asm("NOP");
      }
    }
    //填充基准值数组、阈值寄存器
    for(uint8 i=0; i<16; i++)
    {
      if((chs>>i)&0x1L)
      {
        TSI_ChxBaseVal[i] = *((uint16 *)TSI_CNTR[i]);
#if (defined(CPU_MK60DZ10))
        TSI0->THRESHLD[i] = TSI_ChxBaseVal[i] + TSI_OVERRUN_VAL;
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
        TSI0->THRESHOLD = TSI_ChxBaseVal[i] + TSI_OVERRUN_VAL;
#endif
      }
    }  
  }
  else
  {
    for(uint8 i=0; i<16; i++)
    {
      TSI_ChxBaseVal[i] = TSI_BASE_VAL;
    }
  }
  
  //周期扫描模式or软件触发模式
  if(mode == TSI_SCAN_PERIOD)
  {
    TSI0->GENCS |= TSI_GENCS_STM_MASK;
  }
  else
  {
    TSI0->GENCS &= ~(TSI_GENCS_STM_MASK);
  }
  
  return 1;
}

/*
 * LPLD_TSI_Deinit
 * TSI反初始化函数
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    无
 */
void LPLD_TSI_Deinit(void)
{
  LPLD_TSI_DisableIrq();
#if (defined(CPU_MK60DZ10))
  TSI0->STATUS = 0xFFFFFFFF;
#endif
  TSI0->PEN = 0;
  TSI0->SCANC = 0;
  TSI0->GENCS = 0;
  //关闭TSI模块时钟
  SIM->SCGC5 &= ~(SIM_SCGC5_TSI_MASK); 
}

/*
 * LPLD_TSI_IsChxTouched
 * 通道x的电极是否被触摸
 * 
 * 参数:
 *    ch_num--通道x的数字
 *      |_0~15
 *
 * 输出:
 *    TRUE-有触摸
 *    FALSE-未触摸
 */
boolean LPLD_TSI_IsChxTouched(uint8 ch_num)
{ 
  if(TSI_ChxBaseVal[ch_num]<*((uint16 *)TSI_CNTR[ch_num]))
    return TRUE;
  return FALSE;
}

/*
 * TSI中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void TSI_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  if(TSI0->GENCS&(TSI_GENCS_ESOR_MASK|TSI_GENCS_EOSF_MASK) &&
     TSI_ISR[0] != NULL)
  {
    TSI_ISR[0]();
    TSI0->GENCS |= TSI_GENCS_OUTRGF_MASK;
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
#if (defined(CPU_MK60DZ10))
    TSI0->STATUS = 0x0000FFFF;
#endif
  }
#if (defined(CPU_MK60DZ10))
  else if(TSI0->STATUS != 0 && TSI_ISR[1] != NULL)
  {
    TSI_ISR[1]();
    TSI0->STATUS = 0xFFFF0000;
  }
#endif   
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}