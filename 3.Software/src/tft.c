/**
 * --------------Smart_Car工程-----------------
 * @file TFT.c
 * @version 0.0
 * @date 2015-2-7
 *
 *
 * 版权所有:  北华航天工学院 第十届飞思卡尔  摄像头2队
 * 硬件平台:  MK60FX512
 * 
 */
#include "common.h"
#include "delay.h"
#include "font_ASC.h"
#include "tft.h"
#include "global.h"

/* TFT的SPI引脚定义 */
#define TFT_SPIX    (SPI0)

/* 选择TFT MOSI所对应的IO */
#define TFT_MOSI    (PTA16)
/* 选择TFT MISO所对应的IO */
#define TFT_MISO    (PTA17)        //未用
/* 选择TFT SCK所对应的IO */
#define TFT_SCK     (PTA15)
/* 选择TFT CS所对应的IO */
#define TFT_PCS0    (PTA14)       //未用

/* 选择TFT RESET所对应的IO */
#define TFT_RESET_PTX     (PTE)
#define TFT_RESET_IOX     (GPIO_Pin27)
#define TFT_RESET_SET()    PTE27_O = 1 
#define TFT_RESET_CLR()    PTE27_O = 0

/* 选择TFT A0所对应的IO */
#define TFT_A0_PTX        (PTE)
#define TFT_A0_IOX        (GPIO_Pin26)
#define TFT_A0_SET()       PTE26_O = 1 
#define TFT_A0_CLR()       PTE26_O = 0

/*
 * TFT_IO_Init
 * TFT所用IO初始化
 * 
 * 参数： 
 *    无
 *
 * 输出:
 *    无
 */
static void TFT_IO_Init(void)
{
  GPIO_InitTypeDef tft_init;
  SPI_InitTypeDef spi_init;
  //RESET 
  tft_init.GPIO_PTx = TFT_RESET_PTX;
  tft_init.GPIO_Pins = TFT_RESET_IOX;
  tft_init.GPIO_Dir = DIR_OUTPUT;
  tft_init.GPIO_Output = OUTPUT_H;
  tft_init.GPIO_PinControl = IRQC_DIS; 
  LPLD_GPIO_Init(tft_init); 
  //A0
  tft_init.GPIO_PTx = TFT_A0_PTX;
  tft_init.GPIO_Pins = TFT_A0_IOX;
  tft_init.GPIO_Dir = DIR_OUTPUT;
  tft_init.GPIO_Output = OUTPUT_L;
  tft_init.GPIO_PinControl = IRQC_DIS; 
  LPLD_GPIO_Init(tft_init); 
  //SPI
  spi_init.SPI_SPIx = TFT_SPIX;
  spi_init.SPI_SckDivider = SPI_SCK_DIV_4;          //4分频
  spi_init.SPI_Pcs0Pin = TFT_PCS0;
  spi_init.SPI_SckPin  = TFT_SCK;
  spi_init.SPI_MosiPin = TFT_MOSI;
  spi_init.SPI_MisoPin = TFT_MISO;
  spi_init.SPI_TxCompleteIntIsr = NULL;
  spi_init.SPI_QueueEndIntIsr = NULL;
  spi_init.SPI_TxFIFO_FillIntIsr = NULL;
  spi_init.SPI_TxFIFO_UnderflowIntIsr = NULL;
  spi_init.SPI_RxFIFO_DrainIntIsr = NULL;
  spi_init.SPI_RxFIFO_OverflowIntIsr = NULL;
  LPLD_SPI_Init(spi_init);
}
/*
 * TFT_WriteIndex
 * TFT写指令
 * 
 * 参数： 
 *    index--指令地址
 *
 * 输出:
 *    无
 */
static void TFT_WriteIndex(uint8 index)
{
  TFT_A0_CLR();       //写指令
  LPLD_SPI_Master_Write(TFT_SPIX, index, SPI_PCS0, SPI_PCS_ASSERTED); 
}
/*
 * TFT_WriteData
 * TFT写数据
 * 
 * 参数： 
 *    data--数据
 *
 * 输出:
 *    无
 */
static void TFT_WriteData(uint8 data)
{
  TFT_A0_SET();      //写数据
  LPLD_SPI_Master_Write(TFT_SPIX, data, SPI_PCS0, SPI_PCS_ASSERTED); 
}
/*
 * TFT_Init
 * TFT的配置初始化
 * 
 * 参数： 
 *    无
 *
 * 输出:
 *    无
 */
void TFT_Init()
{
  TFT_IO_Init();

  TFT_RESET_CLR();
  Delayms(20);
  TFT_RESET_SET();
  
  Delayms(10);
  TFT_WriteIndex(0x01);    //Sofeware setting
  Delayms(20);
  TFT_WriteIndex(0x11);   //SLEEP OUT
  
  TFT_WriteIndex(0x3A);   //16 bit  RGB color  format  select
  TFT_WriteData(0x05);    
  
  TFT_WriteIndex(0xB1);   // Frame rate setting   95HZ
  TFT_WriteData(0x00);
  TFT_WriteData(0x06);
  TFT_WriteData(0x03);
  
  TFT_WriteIndex(0x36);   //MV,MX,MY,RGB
  TFT_WriteData(0x60);    //RGB    a0
  
  TFT_WriteIndex(0xB6);   //Display control non-overlap
  TFT_WriteData(0x15);
  TFT_WriteData(0x02);
  
  TFT_WriteIndex(0xB4);   //line inverion
  TFT_WriteData(0x00);
  
  TFT_WriteIndex(0xC0);  //POWER CONTROL 1 GVDD&VCI1 VCI1=2.65V
  TFT_WriteData(0x02);
  TFT_WriteData(0x70);
  
  TFT_WriteIndex(0xC1);  //VGH,VGL
  TFT_WriteData(0x05);
  
  TFT_WriteIndex(0xC2);  //DC/DC SET
  TFT_WriteData(0x01); 
  TFT_WriteData(0x02);
  
  TFT_WriteIndex(0xC5);  //VCOMH=3.925V;VCOML=0.875V
  TFT_WriteData(0x3C);
  TFT_WriteData(0x38);
  
  TFT_WriteIndex(0xFC);
  TFT_WriteData(0x11);
  TFT_WriteData(0x15);
  /***********************GAMMA*************************/
  TFT_WriteIndex(0xE0);  //Positive
  TFT_WriteData(0x09);
  TFT_WriteData(0x16);
  TFT_WriteData(0x09);
  TFT_WriteData(0x20);
  TFT_WriteData(0x21);
  TFT_WriteData(0x1B);
  TFT_WriteData(0x13);
  TFT_WriteData(0x19);
  TFT_WriteData(0x17);
  TFT_WriteData(0x15);
  TFT_WriteData(0x1E);
  TFT_WriteData(0x2B);
  TFT_WriteData(0x04);
  TFT_WriteData(0x05);
  TFT_WriteData(0x02);
  TFT_WriteData(0x1E);
  
  TFT_WriteIndex(0xE1);  //Negative
  TFT_WriteData(0x0B);
  TFT_WriteData(0x14);
  TFT_WriteData(0x08);
  TFT_WriteData(0x1E);
  TFT_WriteData(0x22);
  TFT_WriteData(0x1D);
  TFT_WriteData(0x18);
  TFT_WriteData(0x1E);
  TFT_WriteData(0x1B);
  TFT_WriteData(0x1A);
  TFT_WriteData(0x24);
  TFT_WriteData(0x24);
  TFT_WriteData(0x2B);
  TFT_WriteData(0x06);
  TFT_WriteData(0x02);
  TFT_WriteData(0x1F);
  
  TFT_WriteIndex(0x2A);
  TFT_WriteData(0x00);
  TFT_WriteData(0x00);
  TFT_WriteData(0x00);
  TFT_WriteData(0x9F);
  
  TFT_WriteIndex(0x2B);
  TFT_WriteData(0x00);
  TFT_WriteData(0x00);
  TFT_WriteData(0x00);
  TFT_WriteData(0x7F);
  
  TFT_WriteIndex(0x13);
  
  TFT_WriteIndex(0x2C);
  
  TFT_WriteIndex(0x29);
}
/*
 * TFT_SetCursor
 * 指定坐标点
 * 
 * 参数： 
 *    x--行左边 y--列坐标
 *
 * 输出:
 *    无
 */
void TFT_SetCursor( uint8 x, uint8 y )
{
  TFT_WriteIndex(0x2A);    
  TFT_WriteData(0x00);
  TFT_WriteData(x);
  
  TFT_WriteIndex(0x2B);    
  TFT_WriteData(0x00);
  TFT_WriteData(y);
}
/*
 * TFT_SetWindows
 * 设置一个绘制窗口
 * 
 * 参数： 
 *    x0--行起始点 y0--列起始点 x1--行结束点 y1--列结束点
 *
 * 输出:
 *    无
 */
void TFT_SetWindows( uint8 x0, uint8 y0, uint8 x1, uint8 y1 )
{
  TFT_WriteIndex(0x2A);
  TFT_WriteData(0x00);
  TFT_WriteData(x0);
  TFT_WriteData(0x00);
  TFT_WriteData(x1);
  
  TFT_WriteIndex(0x2B);
  TFT_WriteData(0x00);
  TFT_WriteData(y0);
  TFT_WriteData(0x00);
  TFT_WriteData(y1);
}
/*
 * TFT_SetPoint
 * 绘制一点
 * 
 * 参数： 
 *    x0--行起始点 y0--列起始点 color--颜色
 *
 * 输出:
 *    无
 */
void TFT_SetPoint( uint8 x, uint8 y, uint16 color)
{ 
  TFT_SetCursor(x,y);
  TFT_WriteIndex(0x2C); 
  TFT_WriteData(color>>8);
  TFT_WriteData(color); 
}
/*
 * TFT_U8GrayToRGB
 * U8灰度颜色转为RGB颜色
 * 
 * 参数:
 *    color--8位灰度颜色值
 *
 * 输出:
 *    16位RGB颜色值
 */
uint16 TFT_U8GrayToRGB(uint8 color)
{
  uint16  r, g, b, rgb;
  
  b = ( color>>3 )  & 0x1f;
  g = ( color>>2 )  & 0x3f;
  r = ( color>>3 )  & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);
  
  return( rgb );
}
/*
 * TFT_FillBackground
 * 填充一个指定区域
 * 
 * 参数： 
 *    x0--行起始点 y0--列起始点 x1--行结束点 y1--列结束点 color--填充颜色
 *
 * 输出:
 *    无
 */
void TFT_FillBackground( uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint16 color )
{
  uint8 i,j;
  TFT_SetWindows(x0, y0, x1, y1);
  
  TFT_WriteIndex(0x2C);                     //写颜色数据
  for(i=0;i<=x1-x0;i++)
    for(j=0;j<=y1-y0;j++)
    {
      TFT_WriteData(color>>8);
      TFT_WriteData(color);
    }
}
/*
 * TFT_ClearScreen
 * 清屏
 * 
 * 参数： 
 *    color--16位灰度图像数组
 *
 * 输出:
 *    无
 */
void TFT_ClearScreen(uint16 color)
{
  TFT_FillBackground(0,0,159,127,color);
}
/*
 * TFT_DrawU8Gray
 * 显示一幅指定大小的U8灰度图像
 * 
 * 参数： 
 *    *color--8位灰度图像数组
 *    w--图像宽
 *    h--图像高
 *
 * 输出:
 *    无
 */
void TFT_DrawU8Gray( uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint8 color[][H])
{
  uint8 w,h;
  uint16 temp;
  TFT_SetWindows(x0,y0,x1,y1);                       //设定窗口大小
  TFT_WriteIndex(0x2C);                             //写颜色数据
  for(w=0;w<=y1;w++)
    for(h=23;h<=x1+23;h++)    //20
    {
      if(color[w][h] == 0x11)                //标记边线
        temp = COLOR_Red;
      else if(color[w][h] == 0x12)
        temp = COLOR_Blue;
      else if(color[w][h] == 0x13)
        temp = COLOR_Yellow;
      else
        temp = TFT_U8GrayToRGB(color[w][h]);
      TFT_WriteData(temp>>8);                     
      TFT_WriteData(temp);  
    }
}
/*
 * TFT_DrawLine
 * 画一条直线
 * 
 * 参数： 
 *    x0--行起始点 y0--列起始点 x1--行结束点 y1--列结束点 color--颜色
 *
 * 输出:
 *    无
 */
void TFT_DrawLine( uint8 x0, uint8 y0, uint8 x1, uint8 y1 , uint16 color )
{
  short dx,dy;      /* 定义X Y轴上增加的变量值 */
  short temp;       /* 起点 终点大小比较 交换数据时的中间变量 */
  
  if( x0 > x1 )     /* X轴上起点大于终点 交换数据 */
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )     /* Y轴上起点大于终点 交换数据 */
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }
  
  dx = x1-x0;       /* X轴方向上的增量 */
  dy = y1-y0;       /* Y轴方向上的增量 */
  
  if( dx == 0 )     /* X轴上没有增量 画垂直线 */ 
  {
    do
    { 
      TFT_SetPoint(x0, y0, color);   /* 逐点显示 描垂直线 */
      y0++;
    }
    while( y1 >= y0 ); 
      return; 
  }
  if( dy == 0 )     /* Y轴上没有增量 画水平直线 */ 
  {
    do
    {
      TFT_SetPoint(x0, y0, color);   /* 逐点显示 描水平线 */
      x0++;
    }
    while( x1 >= x0 ); 
      return;
  }
  /* 布兰森汉姆(Bresenham)算法画线 */
  if( dx > dy )                         /* 靠近X轴 */
  {
    temp = 2 * dy - dx;               /* 计算下个点的位置 */         
    while( x0 != x1 )
    {
      TFT_SetPoint(x0,y0,color);    /* 画起点 */ 
      x0++;                         /* X轴上加1 */
      if( temp > 0 )                /* 判断下下个点的位置 */
      {
        y0++;                     /* 为右上相邻点，即（x0+1,y0+1） */ 
        temp += 2 * dy - 2 * dx; 
      }
      else         
      {
        temp += 2 * dy;           /* 判断下下个点的位置 */  
      }       
    }
    TFT_SetPoint(x0,y0,color);
    }  
    else
    {
      temp = 2 * dx - dy;                      /* 靠近Y轴 */       
      while( y0 != y1 )
      {
        TFT_SetPoint(x0,y0,color);     
        y0++;                 
        if( temp > 0 )           
        {
          x0++;               
          temp+=2*dy-2*dx; 
        }
        else
        {
          temp += 2 * dy;
        }
     } 
  TFT_SetPoint(x0,y0,color);
  }
}
/*
 * TFT_ShowChar
 * 在指定坐标显示字符
 * 
 * 参数： 
 *    x0--X轴坐标
 *    y0--Y轴坐标
 *    ascii--ASCI码
 *    charColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void TFT_ShowChar( uint8 x0, uint8 y0, uint8 ascii, uint16 charColor, uint16 bkColor )
{
  uint16 i, j;
  uint8 tmp_char;
  for( i=0; i<10; i++ )
  {
    tmp_char = ascii_5x10[ascii-32][i];
    for( j=0; j<5; j++ )
    {
      if( (tmp_char >> j) & 0x01 == 0x01 )
      {
        TFT_SetPoint( x0 + j, y0 + i, charColor );  /* 字符颜色 */
      }
      else
      {
        TFT_SetPoint( x0 + j, y0 + i, bkColor );  /* 背景颜色 */
      }
    }
  }
}
/*
 * TFT_ShowNum
 * 在指定坐标显示数字(支持负数)
 * 
 * 参数： 
 *    x0--X轴坐标
 *    y0--Y轴坐标
 *    num--数字
 *    charColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void TFT_ShowNum( uint8 x0, uint8 y0, int32 num, uint16 charColor, uint16 bkColor )
{
  uint32 temp;
  uint8 flag=0,t=0;
  if(num == 0)
  {
    TFT_ShowChar(x0,y0,'0',charColor,bkColor);
    return;
  }
  else if(num < 0)
  {
    flag = 1;
    num = -num;
  }
  temp = num;
  while(temp)     //计算位数
  {
    temp /= 10;
    t++;
  }
  while(num)
  { 
    if(flag == 1)
    {
      TFT_ShowChar((x0 + 5*(t--)+5),y0,(num%10) + '0',charColor,bkColor);       //先显示低位，从坐到右显示
    }
    else
    {
      TFT_ShowChar((x0 + 5*(t--)),y0,(num%10) + '0',charColor,bkColor);       //先显示低位，从坐到右显示
    }
    num /= 10;
  } 
  if(flag == 1)
  {
    TFT_ShowChar(x0,y0,'-',charColor,bkColor);
  }
}
/*
 * TFT_ShowString
 * 在指定坐标显示字符串
 * 
 * 参数： 
 *    x0--X轴坐标
 *    y0--Y轴坐标
 *    *str--字符串
 *    charColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void TFT_ShowString( uint8 x0, uint8 y0, uint8 *str, uint16 charColor, uint16 bkColor )
{
  while(*str != '\0')
  {
    TFT_ShowChar(x0, y0, *str, charColor,bkColor);
    x0 += 5;
    str ++;
  }
}

