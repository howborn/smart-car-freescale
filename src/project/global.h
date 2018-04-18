/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Global.h
 * @version 0.0
 * @date 2015-2-7
 * @brief 关于该工程的描述
 *
 * 版权所有:北华航天工业学院 第十届飞思卡尔  摄像头2队 
 * 硬件平台:  MK60FX512
 * 
 */
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "common.h"


#define BUZZ_ON         LPLD_GPIO_Output_b(PTB,18,1)
#define BUZZ_OFF        LPLD_GPIO_Output_b(PTB,18,0) 

#define H              188                             /* 图像宽度 */
#define V              120                             /* 图像高度 */
#define Start_Row      23                             //图像处理开始列
#define End_Row        183                            //图像处理结束列
#define Start_Column   0                              //图像处理开始行
#define End_Column     120                             //图像处理结束行

#define BUFF_SIZE   512*45
extern uint8 buff[BUFF_SIZE];

extern uint8 Image[V][H];
extern uint8 Image_show[V][H];
extern uint32 Which_pic;
extern uint8 Flag_DispPhoto;
extern uint8 Stop_Flag;
extern uint8 printf_flag;
extern uint8 frame;
extern int16 D_Pwm_Center;                                //舵机参数已校正
extern int16 D_Kp_a;
extern int16 D_Kp_b;
extern int16 D_Kp;
extern int16 D_Kd;
extern int32 D_PWM;

extern int16 S_L_Kp;                                        //放大10倍
extern int16 S_L_Ki;
extern int16 S_L_Kd;
extern int16 S_R_Kp;                                    
extern int16 S_R_Ki;                                     
extern int16 S_R_Kd;

extern int32 S_L_PWM;
extern int32 S_R_PWM;

extern int16 Get_L_Speed; 
extern int16 Get_R_Speed;

extern int16 Speed;

extern int16 Straight_High_Speed;            //直道最高速度
extern int16 Straight_Low_Speed;             //直道最低速度

extern int16 S_C_High_Speed;                 //直道入弯最高速度
extern int16 S_C_Low_Speed;                  //直道入弯最低速度

extern int16 Curve_High_Speed;                //弯道最高速度
extern int16 Curve_Low_Speed;                 //弯道最低速度


extern int16 S_a;
extern int16 S_b;

extern int16 Set_L_Speed;
extern int16 Set_R_Speed;

extern uint16 Send_Data[4];                                 //监视数据

extern int16 Line_Array[End_Column-Start_Column][3];        //存储各行边线和中线-->格式：Line_Array[][0]--左边线     Line_Array[][1]--中线   Line_Array[][2]--右边线
extern int16 Center_Line;
extern int16 Active_Lines;
extern int16 Str_Cur_Offset; 
extern int8 Left_Flag;
extern int8 Right_Flag;
extern int8 Str_Flag;
extern uint8 Track_Type; 

#endif