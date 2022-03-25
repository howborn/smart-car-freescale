/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Init.c
 * @version 0.0
 * @date 2015-2-7
 * @brief 关于该工程的描述
 *
 * 版权所有:北华航天工业学院 第十届飞思卡尔  摄像头2队 
 * 硬件平台:  MK60FX512
 * 
 */
#include "common.h"
#include "global.h"
#include "gui.h"
#include "interrupt.h"
#include "mt9v034.h"

/* 结构体声明 */
GPIO_InitTypeDef gpio_init_struct;    /* GPIO结构体 */
GPIO_InitTypeDef pta_init_struct;     /* 摄像头行中断 */
GPIO_InitTypeDef ptb_init_struct;     /* 摄像头像素信号 */ 
DMA_InitTypeDef dma_init_struct;      /* DMA */
FTM_InitTypeDef ftm_init_struct;      /* FTM */
FTM_InitTypeDef qd_init_struct;       /* 正交解码 */
/*-----------------------------------------------------------------------
* init_gpio    : GPIO初始化
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
static void GPIO_Init()
{
  gpio_init_struct.GPIO_PTx = PTC;                                                 
  gpio_init_struct.GPIO_Pins = GPIO_Pin3;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_DIS|IRQC_RI;
  gpio_init_struct.GPIO_Isr = Stop_isr;
  LPLD_GPIO_Init(gpio_init_struct);
  //按键
  gpio_init_struct.GPIO_PTx = PTE;                                                //PTE28--KEY1
  gpio_init_struct.GPIO_Pins = GPIO_Pin28;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
  gpio_init_struct.GPIO_PTx = PTA;                                               //PTA5--KEYB下  PTA13--KEYC上  PTA7-左  PTA14--KEYA PTA6--start PTA12--OK
  gpio_init_struct.GPIO_Pins = GPIO_Pin5 | GPIO_Pin6 | GPIO_Pin7| GPIO_Pin12 | GPIO_Pin13;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
  //薄码开关
  gpio_init_struct.GPIO_PTx = PTC;                                                                                
  gpio_init_struct.GPIO_Pins = GPIO_Pin0 | GPIO_Pin1 | GPIO_Pin2 | GPIO_Pin7 | GPIO_Pin11 | GPIO_Pin13 | GPIO_Pin15 | GPIO_Pin16;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
  //LED
  gpio_init_struct.GPIO_PTx = PTE;
  gpio_init_struct.GPIO_Pins = GPIO_Pin24;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_H;
  gpio_init_struct.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
  //BUZZ
  gpio_init_struct.GPIO_PTx = PTB;
  gpio_init_struct.GPIO_Pins = GPIO_Pin18;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_L;
  gpio_init_struct.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct); 
  //摄像头数据口初始化：PTB0~PTB7
  gpio_init_struct.GPIO_PTx = PTB;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_Pins = GPIO_Pin0_7;
  gpio_init_struct.GPIO_PinControl = IRQC_DIS|INPUT_PULL_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
  //摄像头场信号接口初始化：PTB9
  gpio_init_struct.GPIO_PTx = PTB;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_Pins = GPIO_Pin9;
  gpio_init_struct.GPIO_PinControl = IRQC_RI|INPUT_PULL_DOWN;      
  gpio_init_struct.GPIO_Isr = VS_isr;
  LPLD_GPIO_Init(gpio_init_struct); 
  //摄像头行信号接口初始化：PTA26,注意重新定义一个结构体
  pta_init_struct.GPIO_PTx = PTA;
  pta_init_struct.GPIO_Dir = DIR_INPUT;
  pta_init_struct.GPIO_Pins = GPIO_Pin26;
  pta_init_struct.GPIO_PinControl = IRQC_RI|INPUT_PULL_DOWN;
  pta_init_struct.GPIO_Isr = HS_isr;
  LPLD_GPIO_Init(pta_init_struct); 
  //摄像头PCLK信号接口初始化：PTB11,注意重新定义一个结构体
  ptb_init_struct.GPIO_PTx = PTB;
  ptb_init_struct.GPIO_Dir = DIR_INPUT;
  ptb_init_struct.GPIO_Pins = GPIO_Pin11;
  ptb_init_struct.GPIO_PinControl = IRQC_DMARI|INPUT_PULL_DIS;  
  LPLD_GPIO_Init(ptb_init_struct); 
  
  LPLD_GPIO_ClearIntFlag(PORTB);      //清PORTB中断标志
  enable_irq(PORTB_IRQn);             //使能PORTB中断
  enable_irq(PORTC_IRQn);
}
/*-----------------------------------------------------------------------
* ftm_init   : FTM初始化
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
static void FTM_Init()
{
  //初始化FTM电机PWM
  ftm_init_struct.FTM_Ftmx = FTM3;	        //使能FTM3通道
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//使能PWM模式
  ftm_init_struct.FTM_PwmFreq = 10000;	        //PWM频率10000Hz
  LPLD_FTM_Init(ftm_init_struct);
  //初始化FTM舵机PWM
  ftm_init_struct.FTM_Ftmx = FTM0;	        //使能FTM0通道
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//使能PWM模式
  ftm_init_struct.FTM_PwmFreq = 120;	        //PWM频率120Hz
  LPLD_FTM_Init(ftm_init_struct);
  /*               
  *电机参数             
  *            左             右
  *        PTE6  PTE7    PTE8   PTE9
  * 正转    0    pwm      0      pwm
  * 反转   pwm    0      pwm      0
  *
  */
  LPLD_FTM_PWM_Enable(FTM3,                     //使用FTM3
                      FTM_Ch1,                  //使能Ch1通道
                      0,                        //占空比
                      PTE6,                     //使用Ch1通道的PTE6引脚
                      ALIGN_LEFT                //脉宽左对齐
                      ); 
  LPLD_FTM_PWM_Enable(FTM3,                     //使用FTM3
                      FTM_Ch2,                  //使能Ch2通道
                      0,                        //占空比                              
                      PTE7,                     //使用Ch2通道的PTE7引脚
                      ALIGN_LEFT                //脉宽左对齐
                      );    
  LPLD_FTM_PWM_Enable(FTM3,                     //使用FTM3
                      FTM_Ch3,                  //使能Ch3通道
                      0,                        //占空比                              
                      PTE8,                     //使用Ch3通道的PTE8引脚
                      ALIGN_LEFT                //脉宽左对齐
                      );   
  LPLD_FTM_PWM_Enable(FTM3,                     //使用FTM3
                      FTM_Ch4,                  //使能Ch4通道
                      0,                        //占空比                              
                      PTE9,                     //使用Ch4通道的PTE9引脚
                      ALIGN_LEFT                //脉宽左对齐
                      );   
  LPLD_FTM_PWM_Enable(FTM0,                     //使用FTM0
                      FTM_Ch7,                  //使能Ch7通道
                      1865,                     //占空比，此时舵机摆正           
                                                                                  /* 左    中    右       
                                                                                   1665  1865  2065 */
                      PTD7,                     //使用Ch7通道的PTD7引脚
                      ALIGN_LEFT                //脉宽左对齐
                      );  
}
/*-----------------------------------------------------------------------
* dam_init   : DMA初始化
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
static void DMA_Init()
{
  //DMA参数配置
  dma_init_struct.DMA_CHx = DMA_CH0;                           //CH0通道
  dma_init_struct.DMA_Req = PORTB_DMAREQ;                      //PORTB为请求源
  dma_init_struct.DMA_MajorLoopCnt = H;                        //主循环计数值：行采集点数，宽度
  dma_init_struct.DMA_MinorByteCnt = 1;                        //次循环字节计数：每次读入1字节
  dma_init_struct.DMA_SourceAddr = (uint32)&PTB->PDIR;         //源地址：PTB0~7
  dma_init_struct.DMA_DestAddr = (uint32)Image;                //目的地址：存放图像的数组
  dma_init_struct.DMA_DestAddrOffset = 1;                      //目的地址偏移：每次读入增加1
  dma_init_struct.DMA_AutoDisableReq = TRUE;                   //自动禁用请求
  //初始化DMA
  LPLD_DMA_Init(dma_init_struct);
}
/*-----------------------------------------------------------------------
* qd_init   : 正交解码初始化
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
static void QD_Init()
{
  //配置正交解码功能参数
  qd_init_struct.FTM_Ftmx = FTM1;              //只有FTM1和FTM2有正交解码功能
  qd_init_struct.FTM_Mode = FTM_MODE_QD;       //正交解码功能
  qd_init_struct.FTM_QdMode = QD_MODE_CNTDIR;  //计数和方向解码模式
  //初始化FTM
  LPLD_FTM_Init(qd_init_struct);
  //使能AB相输入通道
  //PTA8引脚接A相输入、PTA9引脚接B相输入
  LPLD_FTM_QD_Enable(FTM1, PTA8, PTA9);
  //配置正交解码功能参数
  qd_init_struct.FTM_Ftmx = FTM2;              //只有FTM1和FTM2有正交解码功能
  qd_init_struct.FTM_Mode = FTM_MODE_QD;       //正交解码功能
  qd_init_struct.FTM_QdMode = QD_MODE_CNTDIR;  //计数和方向解码模式
  //初始化FTM
  LPLD_FTM_Init(qd_init_struct);
  //使能AB相输入通道
  //PTA10引脚接A相输入、PTA11引脚接B相输入
  LPLD_FTM_QD_Enable(FTM2, PTA10, PTA11);
}
/*-----------------------------------------------------------------------
* Init   : 系统初始化
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Init()
{
  MT9V034_Init();
  GPIO_Init();       //GPIO初始化
  FTM_Init();        //FTM初始化
  DMA_Init();        //DMA初始化
  QD_Init();         //正交解码初始化
  GUI_Init();
}