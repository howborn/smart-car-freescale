/**
 * --------------Smart_Car工程-----------------
 * @file Interrupt.c
 * @version 0.0
 * @date 2015-2-7
 *
 *
 * 版权所有:  北华航天工学院 第十届飞思卡尔  摄像头2队
 * 硬件平台:  MK60FX512
 * 
 */
#include "common.h"
#include "global.h"

int8 Flag_VS = 0;  

/*-----------------------------------------------------------------------
* Stop_isr: 停车
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Stop_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin3))
  {
    if(Stop_Flag == 0)
      Stop_Flag=1;
  }
}
/*-----------------------------------------------------------------------
* VS_isr: 摄像头场中断
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void VS_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin9))
  {
     //检测到场开始信号，加载目的地址
     LPLD_DMA_LoadDstAddr(DMA_CH0, Image); 
     //清行中断标志，防止进入无效行中断
     LPLD_GPIO_ClearIntFlag(PORTA);
     enable_irq(PORTA_IRQn);
  }
}
/*-----------------------------------------------------------------------
* HS_isr: 摄像头行中断
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void HS_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTA, GPIO_Pin26))
  {       
    //检测到行开始信号，使能DMA请求
     if(Flag_VS < V)
     {  
       LPLD_DMA_EnableReq(DMA_CH0);        
     }
     //行数采集已满，关闭中断
    else
    {
      //关GPIO中断 
      disable_irq(PORTA_IRQn);
      disable_irq(PORTB_IRQn);
      Flag_DispPhoto = 1;              //可以显示图像
      Flag_VS = 0;     
    } 
    Flag_VS ++;
  }
}
/*-----------------------------------------------------------------------
* Picture_get: 图像预处理
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Picture_get(void)
{
  Flag_DispPhoto = 0;
  
  LPLD_GPIO_ClearIntFlag(PORTB);      //清摄像头场中断标志
  enable_irq(PORTB_IRQn);             //使能摄像头场中断   
  
  while(Flag_VS<2);
  
  for(int16 i=0;i<V;i++)
  {
    if(i<V-5)
      while(i>Flag_VS-3);
    if(i>=V-5)
      while(!Flag_DispPhoto);
  
    for(int16 h=0;h<H;h++)
    {
      buff[i*H+h]=Image[i][h];
    }
  }
}