/**
 * --------------Smart_Car工程-----------------
 * @file Global.c
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

uint8 buff[BUFF_SIZE];

uint8 Image[V][H];                   //图像
uint8 Image_show[V][H];            //图像
uint32 Which_pic = 0;                //要处理的第几帧图片856
uint8 printf_flag;       
uint8 frame;

uint8 Flag_DispPhoto = 0;
uint8 Stop_Flag = 1;

int16 D_Pwm_Center = 1865;              //舵机参数已校正
int16 D_Kp_a = 180;
int16 D_Kp_b = 28;
int16 D_Kp;                     
int16 D_Kd = 100;                      
int32 D_PWM;                          //舵机PWM输入值

int16 S_L_Kp = 300;                  
int16 S_L_Ki = 10;                     
int16 S_L_Kd = 20;                    

int16 S_R_Kp = 300;                  
int16 S_R_Ki = 10;                    
int16 S_R_Kd = 20;                     

int32 S_L_PWM = 0;                    //左电机输入PWM值
int32 S_R_PWM = 0;                    //右电机输入PWM值

int16 Get_L_Speed = 0;                //左轮实际速度
int16 Get_R_Speed = 0;                //右轮实际速度

int16 Speed;

int16 Straight_High_Speed = 300;        //直道最高速度
int16 Straight_Low_Speed = 250;         //直道最低速度

int16 S_C_High_Speed = 250;             //直道入弯最高速度
int16 S_C_Low_Speed = 155;              //直道入弯最低速度

int16 Curve_High_Speed = 200;           //弯道最高速度
int16 Curve_Low_Speed = 125;            //弯道最低速度

int16 S_a;                             //弯道速度二次关系的二次项
int16 S_b;                             //弯道速度二次关系的常数项

int16 Set_L_Speed = 0;                 //期望速度
int16 Set_R_Speed = 0; 

uint16 Send_Data[4] = {0};                                 //监视数据

int16 Line_Array[End_Column-Start_Column][3];              //存储各行边线和中线-->格式：Line_Array[][0]--左边线     Line_Array[][1]--中线   Line_Array[][2]--右边线
int16 Center_Line = 94;                                   //中线 
int16 Active_Lines = 0;                                    //有效行数
int16 Str_Cur_Offset = 0;
int8 Left_Flag = 0;
int8 Right_Flag = 0;
int8 Str_Flag = 0;
uint8 Track_Type = 0;                                      //赛道类型
