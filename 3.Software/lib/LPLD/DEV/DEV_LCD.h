/**
 * @file DEV_LCD.h
 * @version 0.1[By LPLD]
 * @date 2013-09-24
 * @brief LCD设备驱动程序
 *
 * 更改建议:可根据实际硬件修改
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
#ifndef __DEV_LCD_H__
#define __DEV_LCD_H__

#include "common.h"

/********用户可修改值 开始***********/
//LCD窗口显示方向：1为横向；0为纵向
#define HORIZONTAL  1    
//LCD指令总线基地址
#define LCD_CMD_ADDRESS   (0x60000000)
//LCD数据总线基地址
#define LCD_DATA_ADDRESS  (0x60010000)
//LCD复位引脚
#define LCD_SET_RST       PTC19_O=1
#define LCD_CLR_RST       PTC19_O=0
//写寄存器间隔延时
#define LCD_DELAY_TIME    2
/********用户可修改值 结束***********/
#if  ( HORIZONTAL == 1 )
#define  MAX_X  320
#define  MAX_Y  240   
#else
#define  MAX_X  240
#define  MAX_Y  320   
#endif

/* LCD color */
#define COLOR_White          0xFFFF
#define COLOR_Black          0x0000
#define COLOR_Gray           0xF7DE
#define COLOR_Blue           0x001F
#define COLOR_Blue2          0x051F
#define COLOR_Red            0xF800
#define COLOR_Magenta        0xF81F
#define COLOR_Green          0x07E0
#define COLOR_Cyan           0x7FFF
#define COLOR_Yellow         0xFFE0

//R、G、B转16位RGB
#define RGB565CONVERT(red, green, blue)\
(uint16)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

//LCD设备信息结构体
typedef struct 
{
  uint16 Lcd_Id;    //LCD设备ID
  uint16 Write_GRAM;  //写GRAM地址
  uint16 X_GRAM;  //GRAM 水平地址
  uint16 Y_GRAM;  //GRAM 垂直地址
      
}LPLD_LCD_Info_t;

extern uint16 *LCD_GRAM;

//LCD初始化
uint16 LPLD_LCD_Init(void);
//填充LCD背景色
void LPLD_LCD_FillBackground(uint16);
//填充LCD背景色到LCD_GRAM
void LPLD_LCD_FillBackgroundGRAM(uint16 *);

//取得LCD某点的16位RGB值
uint16 LPLD_LCD_GetPoint(uint16, uint16);
//设置LCD某点的16位RGB值
void LPLD_LCD_SetPoint(uint16, uint16, uint16);

//在指定坐标显示字符
void LPLD_LCD_ShowChar(uint16, uint16, uint8, uint16, uint16);
//在指定坐标显示数字
void LPLD_LCD_ShowNum(uint16, uint16, uint16, uint8, uint16, uint16);
//在指定坐标显示字符串
void LPLD_LCD_ShowString(uint16, uint16, uint8 *,uint16, uint16);
//另起一行显示字符串
void LPLD_LCD_PrintStringLine(uint8 *, uint16, uint16);
//接着当前行显示字符串
void LPLD_LCD_PrintString(uint8 *,uint16, uint16);
//另起一行显示数字
void LPLD_LCD_PrintNumLine(uint16, uint8, uint16, uint16);
//接着当前行显示数字
void LPLD_LCD_PrintNum(uint16, uint8, uint16, uint16);

//在指定坐标显示直线
void LPLD_LCD_DrawLine(uint16, uint16, uint16, uint16 , uint16);
//在指定坐标显示正方形
void LPLD_LCD_DrawSquare(uint16, uint16, uint16, uint16, uint16);
//在指定坐标显示矩形
void LPLD_LCD_DrawRectangle(uint16, uint16, uint16, uint16, uint16, uint16);
//显示一幅指定大小的U8灰度图像
void LPLD_LCD_DrawU8Gray(uint8 *, uint16, uint16);

//填充指定颜色的矩形到LCD_GRAM
void LPLD_LCD_GRAMFillRec(uint16, uint16, uint16, uint16, uint16);
//填充指定颜色的横线到LCD_GRAM
void LPLD_LCD_GRAMDrawHLine(uint16, uint16, uint16, uint16);

//BGR颜色转RGB颜色
uint16 LPLD_LCD_BGR2RGB(uint16);
//U8灰度颜色转为RGB颜色
uint16 LPLD_LCD_U8GRAY2RGB(uint8);

//在指定坐标显示横线
#define LPLD_LCD_DrawHLine(x0,y0,len,color) LPLD_LCD_DrawLine(x0,y0,x0+len-1,y0,color)
//在指定坐标显示竖线
#define LPLD_LCD_DrawVLine(x0,y0,len,color) LPLD_LCD_DrawLine(x0,y0,x0,y0+len-1,color)
//设置LCD_GRAM某点的16位RGB值
#define LPLD_LCD_SetPointGRAM(Xpos,Ypos,color) LCD_GRAM[MAX_X*MAX_Y-(Ypos-1)*MAX_Y-(Xpos)]=color

#endif 








