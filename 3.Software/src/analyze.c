/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Analyze.c
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

float32 w1 = 1.0,w2 = 1.5,w3 = 1.0;                                 //中线加权因子
int8 Adjust[30]={2,15,27,36,44,50,55,61,65,68,71,74,76,78,80,       //畸变校正
                 82,84,86,88,90,91,92,93,94,95,96,97,98,99,100};

/***************************两数差的绝对值**************************/
int16 ABS(int16 x, int16 y)
{
  if((x-y)>0)
    return (x-y);
  else
    return (y-x);
}
/************************赛道黑色引导线提取***************************/
void Find_Line(uint8 image[][H])
{
    int16 l_line = Start_Row,l_line_old = Start_Row,r_line_old=End_Row,r_line=End_Row,center_line = 90,center_line_old=90;
    int16 track_width = 110,track_width_old = 110;                              //赛道宽度
    int16 llbound = Start_Row, lrbound = center_line, rlbound = center_line, rrbound = End_Row;
    int16 i,j,Cur_Flag;
    uint8 r_line_flag = 0,l_line_flag = 0;
    uint8 lose_line_flag=0;
    Left_Flag = 0;
    Right_Flag = 0;
    Str_Flag = 0;
    for(i=End_Column-20;i>=Start_Column;i--)                                      //逐行扫描，共扫描100行
    {
      r_line = End_Row;
      l_line = Start_Row;
      r_line_flag=0;
      l_line_flag=0;
      for(j=rlbound;j<rrbound;j++)
      {
        if((image[i][j]==0x00) && (image[i][j-1]!=0x00) && (image[i][j-2]!=0x00))
        {
          r_line_flag = 1;                           //搜索到边线置1
          r_line = j;             
          break;
        }
       //边线限幅和未搜到线的情况
       if((r_line_old != 0) && (ABS(r_line,r_line_old)>6) && (r_line_flag == 1))   
          r_line = r_line_old;
      }
      for(j=lrbound;j>=llbound;j--)
      {
        if((image[i][j]==0x00) && (image[i][j+1]!=0x00) && (image[i][j+2]!=0x00))
        {
          l_line_flag = 1;                          //搜索到边线置1
          l_line = j;         
          break;
        }
       //边线限幅和未搜到线的情况
       if((l_line_old != 0) && (ABS(l_line,l_line_old)>6) && (l_line_flag == 1))
          l_line = l_line_old;
      }
      if((r_line_flag == 0) && (l_line_flag == 1))
      {
        if(i>2)
        {
           if(((r_line-l_line)<(Line_Array[i-1][2]- Line_Array[i-1][0])) && ((Line_Array[i-1][2]- Line_Array[i-1][0])<(Line_Array[i-2][2]- Line_Array[i-2][0])) && (Line_Array[i-1][2] == End_Row))
             lose_line_flag = 1;                              //右边丢线
        } 
        center_line = l_line + track_width_old/2;
      } 
      else if((r_line_flag == 1) && (l_line_flag == 0))
      {
        if(i>2) 
        {
           if(((r_line-l_line)<(Line_Array[i-1][2]- Line_Array[i-1][0])) && ((Line_Array[i-1][2]- Line_Array[i-1][0])<(Line_Array[i-2][2]- Line_Array[i-2][0])) && (Line_Array[i-1][0] == Start_Row))
             lose_line_flag = 2;                              //左边丢线
        }
        center_line = r_line - track_width_old/2;
      }
      else if(r_line_flag == 1 && l_line_flag == 1) 
      {
        center_line = (l_line+r_line) >>1;                   //计算中线
        track_width = r_line-l_line;                         //记录赛道宽度
      }
      if((lose_line_flag == 1 && (r_line_flag == 1 || center_line > 160)) || (lose_line_flag == 2 && (l_line_flag == 1 || center_line < 27)))
        break;                                             //本帧结束找线
      //边缘跟踪范围确定
      if(l_line < Start_Row + 8)                            //左线的左右边界,左边界不能超过图像最左边
      {
        lrbound = center_line;
        llbound = Start_Row;
      }
      else
      {
        lrbound = l_line + 8;
        llbound = l_line - 8;
      }
      if(r_line > End_Row - 8)                            //右线的左右边界，右边界不能超过图像最右边
      {
        rrbound = End_Row;
        rlbound = center_line;
      }
      else
      {
        rrbound = r_line + 8;
        rlbound = r_line - 8;
      }
      //赛道类型识别
      if((center_line-center_line_old) > 1)                //左弯
        Left_Flag++;
      else if((center_line-center_line_old) < -1)         //右弯
        Right_Flag++;
      else                                                //直道
        Str_Flag++;
      //搬移数据
      r_line_old = r_line;
      l_line_old = l_line; 
      center_line_old = center_line; 
      track_width_old = track_width;
      //存储本行左、中、右线
      Line_Array[i][0] = l_line;
      Line_Array[i][1] = center_line;
      Line_Array[i][2] = r_line; 
      Image[i][r_line] = 0x11;                            //标记边线，TFT显示中线  
      Image[i][l_line] = 0x11;          
      Image[i][center_line] = 0x12;                       //标记中线，TFT显示中线   
      if(PTC1_I==0)                                       //标记边缘跟踪范围
      {
        Image_show[i][llbound] = 0x13; 
        Image_show[i][lrbound] = 0x13;
        Image_show[i][rrbound] = 0x13;                         
        Image_show[i][rlbound] = 0x13;
      }
    }
    //赛道类型
    Cur_Flag = Left_Flag + Right_Flag;                     //弯道量
    Active_Lines = Cur_Flag + Str_Flag;                    //有效行
    Str_Cur_Offset = Str_Flag - Cur_Flag;                  //直道-弯道
}
/************************赛道中心线计算***************************/
void Get_Center_Line()
{
  int32 sum = 0;
  int8 number1 = 0,number2 = 0,number3 = 0;
  for(uint8 i=0;i>30;i++)
  {
    if(Adjust[i]<Active_Lines)                                   //小于有效行才有效
    {
      if(Adjust[i]<50)                                           //加权
      {
        sum += w1*Line_Array[End_Column-20-Adjust[i]][1];
        number1++; 
      }
      else if(Adjust[i]>=50 && Adjust[i]<=90)
      {
        sum += w2*Line_Array[End_Column-20-Adjust[i]][1];
        number2++; 
      }
      else
      {
        sum += w3*Line_Array[End_Column-20-Adjust[i]][1];
        number3++; 
      }
    }
  }
  Center_Line = (int16)(sum/(w1*number1+w2*number2+w3*number3));  //求平均
}
/*************************赛道类型识别***************************/
void Recognition_Type()
{
  if(Active_Lines>85 && Str_Cur_Offset>30)                       //直道
    Track_Type = 1;                                         
  else if (Active_Lines<60 && Str_Cur_Offset<-15)
    Track_Type = 2;                                              //直道入弯
  else if(Active_Lines<60 && (Str_Cur_Offset>-15 && Str_Cur_Offset<20))
    Track_Type = 3;                                              //弯道
  else 
    Track_Type = 0;                                              //其他
}