/**
 * --------------Smart_Car工程-----------------
 * @file Control.c
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

int16 D_Error = 0;
int16 D_Last_Error = 0;

int16 S_L_Error = 0;
int16 S_L_DError = 0;
int16 S_L_DDError = 0;
int16 S_L_Pre_Error = 0;
int16 S_L_Pre_DError = 0;

int16 S_R_Error = 0;
int16 S_R_DError = 0;
int16 S_R_DDError = 0;
int16 S_R_Pre_Error = 0;
int16 S_R_Pre_DError = 0;

int8  S_PWM_Dead = 4;
int8  S_PID_Bang = 50;                                               //PID和bangbang控制切换阈值
int16 H_Speed;
int16 L_Speed;

int16 S_PWM_Max = 7000;
int16 S_PWM_Min = -7000;

uint8 Turn_Flag = 1;

float32 P1 = 0.009569,P2 = 2.962,P3 = 1000.0,Speed_Ratio = 1.0;    //前面三个系数放大了1000倍
/*-----------------------------------------------------------------------
* Get_Speed: 获取当前速度
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Get_Speed()
{
  Get_L_Speed = -(LPLD_FTM_GetCounter(FTM1));         //获取左轮速度(脉冲个数)
  Get_R_Speed = LPLD_FTM_GetCounter(FTM2);           //获取右轮速度
  LPLD_FTM_ClearCounter(FTM1);                       //清空计数器
  LPLD_FTM_ClearCounter(FTM2); 
} 

/*-----------------------------------------------------------------------
* Set_Speed: 设定当前速度
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Set_Speed()
{
  //速度策略
  if(Track_Type == 1)                               //直道
  {
    H_Speed = Straight_High_Speed;
    L_Speed = Straight_Low_Speed;
  }
  else if(Track_Type == 2)                         //直道入弯
  {
    H_Speed = S_C_High_Speed;
    L_Speed = S_C_Low_Speed;
  }
  else if(Track_Type == 3)                        //弯道
  {
    H_Speed = Curve_High_Speed;
    L_Speed = Curve_Low_Speed;
  }
  Speed = (int32)(H_Speed-(H_Speed-L_Speed)*(D_Error*D_Error)/8836.0);   //设定整车速度
  //计算左右轮设定速度
  if(Turn_Flag == 1)                                 //左转
  {
     Set_R_Speed = Speed;
     Set_L_Speed = (int32)(Set_R_Speed * (1.0/Speed_Ratio));
  }
  else                                              //右转
  {
     Set_L_Speed = Speed;
     Set_R_Speed = (int32)(Set_L_Speed * (1.0/Speed_Ratio));
  }
}

/*-----------------------------------------------------------------------
* BangBangBack: 棒棒刹车
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Speed_L_Back(void) 
{
  S_L_PWM =-8000; 
}

/*-----------------------------------------------------------------------
* BangBangGo: 棒棒启动
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Speed_L_Go(void) 
{
  S_L_PWM =8000; 
}
/*-----------------------------------------------------------------------
* BangBangBack: 棒棒刹车
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Speed_R_Back(void) 
{
  S_R_PWM =-8000;
}

/*-----------------------------------------------------------------------
* BangBangGo: 棒棒启动
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Speed_R_Go(void) 
{
  S_R_PWM =8000;
}

/*-----------------------------------------------------------------------
* Speed_L_Control: 左轮速度控制
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
static void Speed_L_Control()
{
  int32 pwm_p,pwm_i,pwm_d,s_pwm;
  
  S_L_Error = Set_L_Speed - Get_L_Speed;                          //e(k)
  S_L_DError = S_L_Error - S_L_Pre_Error;                         //e(k)-e(k-1)
  S_L_DDError = S_L_DError - S_L_Pre_DError;                      //e(k)-e(k-1)-(e(k-1)-e(k-2))
  S_L_Pre_Error = S_L_Error;                                      //挪动数据
  S_L_Pre_DError = S_L_DError;  
 
  if(S_L_Error > S_PID_Bang)           Speed_L_Go();               //BangBang
  else if(S_L_Error < -S_PID_Bang)    Speed_L_Back();             //BangBang
  else                                                            //PID              
  {
    if((S_L_Error < S_PWM_Dead) && (S_L_Error > -S_PWM_Dead))     //设置调节死区
      s_pwm = 0;
    else
    {
      if(Get_L_Speed<(0.2*Set_L_Speed)) 
        pwm_i = 0;                                              //积分分离防止震荡
      else
        pwm_i = S_L_Ki * S_L_Error;                            //积分项
      
      pwm_p = S_L_Kp * S_L_DError;                             //比例项
      pwm_d = S_L_Kd * S_L_DDError;                            //微分项
      s_pwm = (int32)(pwm_p + pwm_i + pwm_d);
    }
    S_L_PWM += s_pwm;
    
    if(S_L_PWM >= S_PWM_Max)            S_L_PWM = S_PWM_Max;         //限幅
    else if(S_L_PWM <= S_PWM_Min)      S_L_PWM = S_PWM_Min;
  }
}
/*-----------------------------------------------------------------------
* Speed_R_Control: 右轮速度控制
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
static void Speed_R_Control()
{
  int32 pwm_p,pwm_i,pwm_d,s_pwm;
  
  S_R_Error = Set_R_Speed - Get_R_Speed;                          //e(k)
  S_R_DError = S_R_Error - S_R_Pre_Error;                         //e(k)-e(k-1)
  S_R_DDError = S_R_DError - S_R_Pre_DError;                      //e(k)-e(k-1)-(e(k-1)-e(k-2))
  S_R_Pre_Error = S_R_Error;                                      //挪动数据
  S_R_Pre_DError = S_R_DError;  
  
  if(S_R_Error > S_PID_Bang)           Speed_R_Go();              //BangBang
  else if(S_R_Error < -S_PID_Bang)    Speed_R_Back();            //BangBang
  else                                                           //PID 
  {
    if((S_R_Error < S_PWM_Dead) && (S_R_Error > -S_PWM_Dead))     //设置调节死区
      s_pwm = 0;              
    else
    {
      if(Get_R_Speed<(0.2*Set_R_Speed)) 
        pwm_i = 0;                                             //积分分离防止震荡
      else
        pwm_i = S_R_Ki * S_R_Error;                            //积分项
      pwm_p = S_R_Kp * S_R_DError;                             //比例项
      
      pwm_d = S_R_Kd * S_R_DDError;                            //微分项
      s_pwm = (int32)(pwm_p + pwm_i + pwm_d);
    }
    S_R_PWM += s_pwm;
    
    if(S_R_PWM >= S_PWM_Max)            S_R_PWM = S_PWM_Max;      //限幅
    else if(S_R_PWM <= S_PWM_Min)      S_R_PWM = S_PWM_Min;
   }
}

/*-----------------------------------------------------------------------
* Stop_car: 刹车
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Stop_car()
{
  int16 speed;
  speed=(Get_R_Speed+Get_L_Speed)/2;
  if(speed > 20)
  {
    Speed_R_Back();
    Speed_L_Back();
  }
  else
  {
    S_L_PWM =0; 
    S_R_PWM =0;
  }
}

/*-----------------------------------------------------------------------
* Speed_PWM_Update: 输出速度
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Speed_PWM_Update()
{ 
  if(S_L_PWM < 0)
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch2,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch1,(-S_L_PWM));   
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch1,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch2,S_L_PWM);
  } 
  if(S_R_PWM < 0)
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch4,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch3,(-S_R_PWM));   
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch3,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch4,S_R_PWM);
  }
}
/*-----------------------------------------------------------------------
* Speed_Control: 速度控制
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Speed_Control()
{
  Get_Speed();
  if(Stop_Flag == 0)
  {
    Set_Speed();
    Speed_L_Control();
    Speed_R_Control();
  }
  else
    Stop_car(); 
  Speed_PWM_Update();
}

/*-----------------------------------------------------------------------
* Direction_Control: 位置式PID-舵机控制
*
*                           舵机参数
*
*                       左      中     右
*                      1665   1865   2065
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Direction_Control()
{
  //位置式PID
  int32 pwm_p,pwm_d,d_pwm;
  float32 p1,p2;
  D_Last_Error = D_Error;
  D_Error = Center_Line - 94;
  D_Kp = (int16)((D_Error*D_Error)/D_Kp_a+D_Kp_b);
  pwm_p = D_Kp * D_Error;                                            //计算比例项
  pwm_d = D_Kd * (D_Error - D_Last_Error);
  d_pwm = (int32)((pwm_p + pwm_d)/10.0);                            //前面参数放大了10倍，故此处需要缩小10倍                     

  if(d_pwm < -200)                                                 //限幅
    d_pwm = -200;
  else if(d_pwm > 200)
    d_pwm = 200;
  
  D_PWM =D_Pwm_Center - d_pwm; 
  
  if(d_pwm < 0)                            //左转
  {
    Turn_Flag = 1;
    d_pwm = -d_pwm;
  }
  else                                    //右转
  {
    Turn_Flag = 0;
  }
  //差速计算
  p1 = P1*d_pwm*d_pwm;
  p2 = P2*d_pwm;
  Speed_Ratio=(p1 + p2 + P3)/1000.0;
  
  LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, D_PWM);                 //输出给舵机
}
