/**
 * --------------Smart_Car工程-----------------
 * @file Delay.c
 * @version 0.0
 * @date 2015-2-7
 *
 *
 * 版权所有:  北华航天工学院 第十届飞思卡尔  摄像头2队
 * 硬件平台:  MK60FX512
 * 
 */
#include "common.h"

/*
 * Delayms
 * 延时
 * 参数： 
 *    ms--毫秒
 *
 * 输出:
 *    无
 */
void Delayms(uint16 ms)
{
  uint16 i;
  while(ms--)
  {
    for(i=0; i<400; i++)
    {
      asm("nop");
    }
  }
}
/*
 * Delayus
 * 延时
 */
void Delayus(uint16 us)
{	
  while(us) 
    us--; 
}