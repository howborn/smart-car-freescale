/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Gui.c
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
#include "TFT.h"
#include "analyze.h" 
#include "control.h"

uint8 Key_Value = 0;                  //按键值
uint8 Page_Value = 1;                   //显示页
uint8 Which_Number_2 = 0;              //第二页所要操作的目标
uint8 Which_Number_3 = 0;              //第三页所要操作的目标
uint8 Ok_Time = 0;                    //按确定键次数
int16 Which_Row = End_Column-1;        //图像中的指定行

void GUI_Init()
{
  TFT_Init();
  TFT_ClearScreen(COLOR_White);                                  //清屏
}
/*
 * 显示第一页
 * 
 * 图像和赛道基本信息显示
 */
void GUI_ShowPage1(uint8 image[][H])
{
  TFT_DrawU8Gray(0,0,H-28-1,End_Column-1,image);                       //显示采集的图像   
  
  TFT_FillBackground(0,End_Column,159,128,COLOR_White);                        //清屏指定区域 
  
  TFT_ShowString(80,120,"C_L",COLOR_Blue,COLOR_White);
  TFT_ShowNum(95,120,Center_Line,COLOR_Black,COLOR_White); 
  TFT_ShowString(40,120,"W_O",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(55,120,Which_Row,COLOR_Red,COLOR_White);
  TFT_ShowString(0,120,"W_P",COLOR_Blue,COLOR_White);
  TFT_ShowNum(15,120,Which_pic,COLOR_Black,COLOR_White); 
  
  TFT_DrawLine(0,Which_Row,159,Which_Row,COLOR_Blue);              //摄像头显示上边界*/
  TFT_DrawLine(80,End_Column-6,80,End_Column-1,COLOR_Magenta);       //屏幕中心
  TFT_DrawLine(80,Start_Column,80,Start_Column+5,COLOR_Magenta);     //屏幕中心  
}
/*
 * 显示第二页
 * 
 * 转向控制的参数设定和状态监测
 */
static void GUI_ShowPage2()
{
  TFT_ClearScreen(COLOR_White); 
  
  TFT_ShowString(60,5,"Direction",COLOR_Magenta,COLOR_White);

  TFT_ShowString(10,20,"Kp_a",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(35,20,D_Kp_a,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(65,20,"Kp_b",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(90,20,D_Kp_b,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(125,20,"Kd",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(140,20,D_Kd,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(10,30,"D_C",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(35,30,D_Pwm_Center,COLOR_Red,COLOR_White);
  
  //状态参数显示
  TFT_ShowString(10,75,"Kp",COLOR_Blue,COLOR_White);
  TFT_ShowNum(35,75,D_Kp,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(65,75,"PWM",COLOR_Blue,COLOR_White);
  TFT_ShowNum(90,75,D_PWM,COLOR_Black,COLOR_White);   
  
  //菜单指示
  switch(Which_Number_2)
  {
  case 1:
    TFT_ShowChar(5,20,'*',COLOR_Black,COLOR_White);
    break;
  case 2:
    TFT_ShowChar(60,20,'*',COLOR_Black,COLOR_White);
    break; 
  case 3:
    TFT_ShowChar(120,20,'*',COLOR_Black,COLOR_White);   
    break;
  case 4:
    TFT_ShowChar(5,30,'*',COLOR_Black,COLOR_White);   
    break;
  default:break;
  }
}
/*
 * 显示第三页
 * 
 * 速度控制的参数设定和状态监测
 */
static void GUI_ShowPage3()
{
  TFT_ClearScreen(COLOR_White); 

  TFT_ShowString(70,5,"Speed",COLOR_Magenta,COLOR_White);
  
  TFT_ShowString(10,20,"L_Kp",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(35,20,S_L_Kp,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(65,20,"L_Ki",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(90,20,S_L_Ki,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(120,20,"L_Kd",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(145,20,S_L_Kd,COLOR_Red,COLOR_White);
  
  TFT_ShowString(10,30,"R_Kp",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(35,30,S_R_Kp,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(65,30,"R_Ki",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(90,30,S_R_Ki,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(120,30,"R_Kd",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(145,30,S_R_Kd,COLOR_Red,COLOR_White);
  
  TFT_ShowString(10,40,"Straight_H",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(100,40,Straight_High_Speed,COLOR_Red,COLOR_White);
    
  TFT_ShowString(10,50,"Straight_L",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(80,50,Straight_Low_Speed,COLOR_Red,COLOR_White);
  
  TFT_ShowString(10,60,"Curve_H_S",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(60,60,Curve_High_Speed,COLOR_Red,COLOR_White);
  
  TFT_ShowString(85,60,"Curve_L_S",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(135,60,Curve_Low_Speed,COLOR_Red,COLOR_White);
  
  TFT_ShowString(10,70,"S_a",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(35,70,S_a,COLOR_Red,COLOR_White);
  
  TFT_ShowString(85,70,"S_b",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(110,70,S_b,COLOR_Red,COLOR_White);
  
  //状态参数显示
  TFT_ShowString(10,90,"Speed",COLOR_Blue,COLOR_White);
  TFT_ShowNum(40,90,Speed,COLOR_Black,COLOR_White);
 
  TFT_ShowString(60,90,"G_L_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(90,90,Get_L_Speed,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(110,90,"G_R_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(140,90,Get_R_Speed,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(10,100,"S_L_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(40,100,Set_L_Speed,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(60,100,"S_R_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(90,100,Set_R_Speed,COLOR_Black,COLOR_White); 
  
  //菜单指示
  switch(Which_Number_3)
  {
  case 1:
    TFT_ShowChar(5,20,'*',COLOR_Black,COLOR_White);
    break;
  case 2:
    TFT_ShowChar(60,20,'*',COLOR_Black,COLOR_White);
    break; 
  case 3:
    TFT_ShowChar(115,20,'*',COLOR_Black,COLOR_White);   
    break;
  case 4:
    TFT_ShowChar(5,30,'*',COLOR_Black,COLOR_White);    
    break;
  case 5:
    TFT_ShowChar(60,30,'*',COLOR_Black,COLOR_White);   
    break;
  case 6:
    TFT_ShowChar(116,30,'*',COLOR_Black,COLOR_White);
    break;
  case 7:
    TFT_ShowChar(5,40,'*',COLOR_Black,COLOR_White);
    break;
  case 8:
    TFT_ShowChar(5,50,'*',COLOR_Black,COLOR_White);
    break; 
  case 9:
    TFT_ShowChar(5,60,'*',COLOR_Black,COLOR_White);
    break; 
  case 10:
    TFT_ShowChar(80,60,'*',COLOR_Black,COLOR_White);
    break; 
  case 11:
    TFT_ShowChar(5,70,'*',COLOR_Black,COLOR_White);
    break;
  case 12:
    TFT_ShowChar(80,70,'*',COLOR_Black,COLOR_White);
    break;
  default:break;
  }
}
/*
 * 菜单
 * 
 * 设定参数
 */
void GUI_Menu(uint8 image[][H])
{

   if(PTE28_I == 0 && PTC1_I==1)
   {
     Page_Value ++;
     if(Page_Value > 3)
       Page_Value = 1;
   }
   if(Page_Value == 1)                     //第一位薄码开关--换显示页
   {
     if(PTA7_I == 0)                       //实时显示指定行的数据
     {
       if(Which_Row > 0)
          Which_Row -= 1;
     }
     if(PTE28_I == 0)                       //实时显示指定行的数据
     {
       if(Which_Row < 120)
          Which_Row += 1;
     }
     if(PTA13_I == 0)
     {
       if(Which_pic<=frame)
         Which_pic++;
       Which_Row=119;
     }
     if(PTA5_I == 0)
     {
       if(Which_pic>0)
         Which_pic--;
       Which_Row=119;
     }
     if(PTA6_I == 0 && PTC1_I==0)
     {
       printf_flag=0;
     }
     GUI_ShowPage1(image);   
   }
  else if(Page_Value == 2)              //显示第2页，转向控制信息
  {
    if(PTA12_I == 0)
    {
      Ok_Time++;
      if(Ok_Time > 1)
        Ok_Time = 0;
    }
    if(PTA7_I == 0  && Ok_Time == 0)    //← →
    {
      Which_Number_2++;
      if(Which_Number_2 > 4)
        Which_Number_2 = 1;
    }
    if(PTA13_I == 0 && Ok_Time == 1)   //↑ 
    {
      switch(Which_Number_2)
      {
        case 1:
          D_Kp_a += 1;
          break;
        case 2:
          D_Kp_b += 1;
          break;
        case 3:
          D_Kd += 1;
          break; 
        case 4:
          D_Pwm_Center += 1;
          break; 
        default:break;
      }
    }
    if(PTA5_I == 0 && Ok_Time == 1)   //↓
    {
      switch(Which_Number_2)
      {
         case 1:
          D_Kp_a -= 1;
          break;
        case 2:
          D_Kp_b -= 1;
          break;
        case 3:
          D_Kd -= 1;
          break;
        case 4:
          D_Pwm_Center -= 1;
          break;
        default:break;
      }
    }
  GUI_ShowPage2();
  }
  else if(Page_Value == 3)
  {
    if(PTA12_I == 0)
    {
      Ok_Time++;
      if(Ok_Time > 1)
        Ok_Time = 0;
    }
    if(PTA7_I == 0  && Ok_Time == 0)    //← →
    {
      Which_Number_3++;
      if(Which_Number_3 > 12)
        Which_Number_3 = 1;
    }
    if(PTA13_I == 0 && Ok_Time == 1)   //↑ 
    {
      switch(Which_Number_3)
      {
        case 1:
          {
           S_L_Kp += 10;
           S_R_Kp += 10;
          }
          break;
        case 2:
          {
           S_L_Ki += 10;
           S_R_Ki += 10;
          }
          break;
        case 3:
          {
          S_L_Kd += 10;
          S_R_Kd += 10;
          }
          break;  
        case 4:
          S_R_Kp += 10;
          break;
        case 5:
          S_R_Ki += 10;
          break;
        case 6:
          S_R_Kd += 10;
          break; 
        case 7:
          Straight_High_Speed += 1;
          break;
        case 8:
          Straight_Low_Speed += 1;
          break;
        case 9:
          Curve_High_Speed += 1;
          break;  
        case 10:
          Curve_Low_Speed += 1;
          break;
        case 11:
          S_a += 1;
          break;
        case 12:
          S_b += 1;
          break;     
        default:break;
      }
    }
    if(PTA5_I == 0 && Ok_Time == 1)   //↓
    {
      switch(Which_Number_3)
      {
         case 1:
           {
            S_L_Kp -= 10;
            S_R_Kp -= 10;
           }
          break;
        case 2:
          {
          S_L_Ki -= 10;
          S_R_Ki -= 10;
          }
          break;
        case 3:
          {
          S_L_Kd -= 10;
          S_R_Kd -= 10;
          }
          break;  
        case 4:
          S_R_Kp -= 10;
          break;
        case 5:
          S_R_Ki -= 10;
          break;
        case 6:
          S_R_Kd -= 10;
          break; 
        case 7:
          Straight_High_Speed -= 1;
          break;
        case 8:
          Straight_Low_Speed -= 1;
          break;
        case 9:
          Curve_High_Speed -= 1;
          break;  
        case 10:
          Curve_Low_Speed -= 1;
          break;
        case 11:
          S_a -= 1;
          break;
        case 12:
          S_b -= 1;
          break;     
        default:break;
      }
    }
    GUI_ShowPage3();
  }
}