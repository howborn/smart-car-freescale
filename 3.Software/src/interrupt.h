/**
 * --------------Smart_Car工程-----------------
 * @file Interrupt.h
 * @version 0.0
 * @date 2015-2-7
 *
 *
 * 版权所有:  北华航天工学院 第十届飞思卡尔  摄像头2队
 * 硬件平台:  MK60FX512
 * 
 */
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

void VS_isr();
void HS_isr();
void Stop_isr();
void Picture_get();

#endif