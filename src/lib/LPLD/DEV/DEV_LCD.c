/**
 * @file DEV_LCD.c
 * @version 0.2[By LPLD]
 * @date 2013-10-21
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
 *
 * 0.2-2013-10-21 优化初始化函数、字符串数字显示函数
 */
#include "DEV_LCD.h"
#include "Font_ASC.h"

static void LPLD_LCD_Reset(void);
static void LPLD_LCD_FLEXBUS_Init(uint16 lcd_id);
static void LPLD_LCD_IO_Cfg(void);
static void LPLD_LCD_WriteIndex(uint16 index);
static void LPLD_LCD_WriteData(uint16 data);
static uint16 LPLD_LCD_ReadData(void);
static void LPLD_LCD_WriteReg(uint16 LCD_Reg,uint16 LCD_RegValue);
static uint16 LPLD_LCD_ReadReg(uint16 LCD_Reg);
static void LPLD_LCD_SetCursor( uint16 Xpos, uint16 Ypos );
static void LPLD_LCD_Delayms(uint16 ms);

LPLD_LCD_Info_t LPLD_LCD_INFO;
uint16 PreX=0, PreY=0;
uint16 *LCD_GRAM;
uint16 CurX=0, CurY=0;

/*
 * LPLD_LCD_Delayms
 * LCD内部延时函数，内部调用
 */
static void LPLD_LCD_Delayms(uint16 n)
{
  uint16 i;
  while(n--)
  {
    for(i=0; i<400; i++)
    {
      asm("nop");
    }
  }
}

/*
 * LPLD_LCD_FLEXBUS_Init
 * LCD FlexBus初始化函数，内部调用
 * 
 * 说明:
 *    本初始化函数用来配置FlexBus与LCD的连接
 *    配置为16位端口大小
 *    地址与数据线复用
 *    数据及地址线为FB_AD[15:0]
 *    RS:FB_AD[16]
 *    CS:FB_CS0
 *    RW:FB_FB_RW
 *    RD:FB_OE 
 */
static void LPLD_LCD_FLEXBUS_Init(uint16 lcd_id)
{
  FB_InitTypeDef fb_init;
  fb_init.FB_Fbx = FB0;
  fb_init.FB_ChipSelAddress = LCD_CMD_ADDRESS;
  fb_init.FB_AutoAckEnable = TRUE;
  fb_init.FB_IsRightJustied = TRUE;
  fb_init.FB_WateStates = 0x02;
  fb_init.FB_PortSize = FB_SIZE_16BIT;
  fb_init.FB_AddressSpace = FB_SPACE_KB(128);   //128K
  fb_init.FB_ReadAddrHold = 0;
  fb_init.FB_WriteAddrHold = 0;
  LPLD_FlexBus_Init(fb_init); 
}

/*
 * LPLD_LCD_IO_Cfg
 * LCD IO引脚初始化，内部调用
 *
 * 说明：
 * 此处初始化复位引脚，可根据实际硬件更改
 */
static void LPLD_LCD_IO_Cfg(void)
{
  GPIO_InitTypeDef ptc_init;
  //RST 
  ptc_init.GPIO_PTx = PTC;
  ptc_init.GPIO_Pins = GPIO_Pin19;
  ptc_init.GPIO_Dir = DIR_OUTPUT;
  ptc_init.GPIO_Output = OUTPUT_H;
  ptc_init.GPIO_PinControl = IRQC_DIS; 
  LPLD_GPIO_Init(ptc_init);    
}

/*
 * LPLD_LCD_WriteIndex
 * LCD写控制命令，内部调用
 */
static void LPLD_LCD_WriteIndex(uint16 index)
{
  *(vuint16*)LCD_CMD_ADDRESS = index;  //write
}

/*
 * LPLD_LCD_WriteData
 * LCD写数据，内部调用
 */
void LPLD_LCD_WriteData(uint16 data)
{
  *(vuint16*)LCD_DATA_ADDRESS = data;  //write
}

/*
 * LPLD_LCD_ReadData
 * LCD读数据，内部调用
 */
static uint16 LPLD_LCD_ReadData(void)
{ 
  uint16 value;
  value = *(vuint16*)LCD_DATA_ADDRESS;  //read
  return value;
}

/*
 * LPLD_LCD_WriteReg
 * LCD写寄存器，内部调用
 */
static void LPLD_LCD_WriteReg(uint16 LCD_Reg,uint16 LCD_RegValue)
{ 
  /* Write 16-bit Index, then Write Reg */  
  LPLD_LCD_WriteIndex(LCD_Reg);         
  /* Write 16-bit Reg */
  LPLD_LCD_WriteData(LCD_RegValue);  
}

/*
 * LPLD_LCD_ReadReg
 * LCD读寄存器，内部调用
 */
static uint16 LPLD_LCD_ReadReg(uint16 LCD_Reg)
{
  uint16 LCD_RAM;
  /* Write 16-bit Index (then Read Reg) */
  LPLD_LCD_WriteIndex(LCD_Reg);
  /* Read 16-bit Reg */
  LCD_RAM = LPLD_LCD_ReadData();      	
  return LCD_RAM;
}

/*
 * LPLD_LCD_SetCursor
 * 设置LCD内部GRAM光标位置，内部调用
 */
static void LPLD_LCD_SetCursor( uint16 Xpos, uint16 Ypos )
{
#if(HORIZONTAL)
    
  uint16 temp = Xpos;
  Xpos = Ypos;
  Ypos = temp;  

#else
            
#endif
  LPLD_LCD_WriteReg(LPLD_LCD_INFO.X_GRAM, Xpos );      
  LPLD_LCD_WriteReg(LPLD_LCD_INFO.Y_GRAM, Ypos );          
}


/*
 * LPLD_LCD_Reset
 * 复位LCD控制器，内部调用
 */
static void LPLD_LCD_Reset(void)
{
  LCD_CLR_RST;
  LPLD_LCD_Delayms(10); //延时
  LCD_SET_RST;
}

/*
 * LPLD_LCD_Init
 * LCD初始化
 * 
 * 参数:
 *    无
 *
 * 输出:
 *    LCD控制型号
 *    0x9320--9320
 *    0x8989--8989
 */
uint16 LPLD_LCD_Init(void)
{
  uint16 dID;
   
  LPLD_LCD_FLEXBUS_Init(0);
  LPLD_LCD_IO_Cfg();
  LPLD_LCD_Delayms(500);
  
  LPLD_LCD_Reset();                 //硬件复位
  
  dID = LPLD_LCD_ReadReg(0x0000);   //获取LCD控制器设备ID
    
  LPLD_LCD_INFO.Lcd_Id = dID;
  
  if(dID == 0x9320)
  {
    LPLD_LCD_INFO.Write_GRAM = 0x0022;
    LPLD_LCD_INFO.X_GRAM = 0x0020;
    LPLD_LCD_INFO.Y_GRAM = 0x0021;
    
    LPLD_LCD_WriteReg(0x00,0x0001);   //开启LCD内部晶振，并等待10ms稳定
    LPLD_LCD_Delayms(10);
    
    LPLD_LCD_WriteReg(0x01,0x0000);   //Driver Output Control
    LPLD_LCD_WriteReg(0x02,0x0700);   //LCD Driving Wave Control
    LPLD_LCD_WriteReg(0x03,0x1008);   //Entry Mode
    LPLD_LCD_WriteReg(0x04,0x0000);   //Resizing Control
    LPLD_LCD_WriteReg(0x08,0x0202);   //Display Control 2
    LPLD_LCD_WriteReg(0x09,0x0000);   //Display Contral 3.(0x0000)
    LPLD_LCD_WriteReg(0x0a,0x0000);//Frame Cycle Contal.(0x0000)
    LPLD_LCD_WriteReg(0x0c,0x0001);//Extern Display Interface Contral 1.(0x0000)
    LPLD_LCD_WriteReg(0x0d,0x0000);//Frame Maker Position.
    LPLD_LCD_WriteReg(0x0f,0x0000);//Extern Display Interface Contral 2.
    
    LPLD_LCD_WriteReg(0x10,0x0000);//Power Control 1.(0x16b0)
    LPLD_LCD_WriteReg(0x11,0x0000);//Power Control 2.(0x0001)	
    LPLD_LCD_WriteReg(0x12,0x0000);//Power Control 3.(0x0138)
    LPLD_LCD_WriteReg(0x13,0x0000);
    //LPLD_LCD_Delayms(200);	
    
    LPLD_LCD_WriteReg(0x10,0x17b0);//Power Control 1.(0x16b0)
    LPLD_LCD_WriteReg(0x11,0x0137);//Power Control 2.(0x0001)
    LPLD_LCD_Delayms(50);
    LPLD_LCD_WriteReg(0x12,0x0139);//Power Control 3.(0x0138)
    LPLD_LCD_Delayms(50);	
    LPLD_LCD_WriteReg(0x13,0x1700);	 
   
    LPLD_LCD_WriteReg(0x29,0x0000);
    LPLD_LCD_WriteReg(0x2B,0x0000);
    LPLD_LCD_Delayms(50);						
    
    LPLD_LCD_WriteReg(0x30,0x0000);					
    LPLD_LCD_WriteReg(0x31,0x0507);
    LPLD_LCD_WriteReg(0x32,0x0104);	
    LPLD_LCD_WriteReg(0x35,0x0105);	
    LPLD_LCD_WriteReg(0x36,0x0404);
    LPLD_LCD_WriteReg(0x37,0x0603);	
    LPLD_LCD_WriteReg(0x38,0x0004);	
    LPLD_LCD_WriteReg(0x39,0x0007);
    LPLD_LCD_WriteReg(0x3c,0x0501);	
    LPLD_LCD_WriteReg(0x3d,0x0404);
    
    LPLD_LCD_WriteReg(0x0050,0x0000);
    LPLD_LCD_WriteReg(0x0051,239);
    LPLD_LCD_WriteReg(0x0052,0x0000);
    LPLD_LCD_WriteReg(0x0053,319);
  
    LPLD_LCD_WriteReg(0x60,0x2700);//Driver Output Control.
    LPLD_LCD_WriteReg(0x61,0x0001);//Driver Output Control.
    LPLD_LCD_WriteReg(0x6a,0x0000);//Vertical Srcoll Control.
    
    LPLD_LCD_WriteReg(0x80,0x0000);//Display Position? Partial Display 1.
    LPLD_LCD_WriteReg(0x81,0x0000);//RAM Address Start? Partial Display 1.
    LPLD_LCD_WriteReg(0x82,0x0000);//RAM Address End-Partial Display 1.
    LPLD_LCD_WriteReg(0x83,0x0000);//Displsy Position? Partial Display 2.
    LPLD_LCD_WriteReg(0x84,0x0000);//RAM Address Start? Partial Display 2.
    LPLD_LCD_WriteReg(0x85,0x0000);//RAM Address End? Partial Display 2.
    
    LPLD_LCD_WriteReg(0x90,0x0010);//Frame Cycle Contral.(0x0013)
    LPLD_LCD_WriteReg(0x92,0x0000);//Panel Interface Contral 2.(0x0000)
    LPLD_LCD_WriteReg(0x93,0x0003);//Panel Interface Contral 3.
    LPLD_LCD_WriteReg(0x95,0x0110);//Frame Cycle Contral.(0x0110)
    LPLD_LCD_WriteReg(0x97,0x0000);//
    LPLD_LCD_WriteReg(0x98,0x0000);//Frame Cycle Contral.

    LPLD_LCD_WriteReg(0x07,0x0133);
    LPLD_LCD_Delayms(300);
  }  
  else if(dID == 0x8989)
  {
    LPLD_LCD_INFO.Write_GRAM = 0x0022;
    LPLD_LCD_INFO.X_GRAM = 0x004e;
    LPLD_LCD_INFO.Y_GRAM = 0x004f;
    
    //SSD1289;
    /* 打开晶振 */
    LPLD_LCD_WriteReg(0x0000,0x0001);    LPLD_LCD_Delayms(LCD_DELAY_TIME); 
    //启动频率设置为最大osc/16 消耗功率设置在large档位，提高显示画质
    //LPLD_LCD_WriteReg(0x0003,0xF8F8);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0003,0xA8A4);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   //0210
    LPLD_LCD_WriteReg(0x000C,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    LPLD_LCD_WriteReg(0x000D,0x080C);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    LPLD_LCD_WriteReg(0x000E,0x2B00);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    LPLD_LCD_WriteReg(0x001E,0x00B0);    LPLD_LCD_Delayms(LCD_DELAY_TIME); 
    //驱动输出控制320*240 0x2B3F 
    LPLD_LCD_WriteReg(0x0001,0x2B3F);    LPLD_LCD_Delayms(LCD_DELAY_TIME); 
    //驱动输出控制320*240 0x233F 将原来的第11位BGR清零这样输出是RGB就不会红色与蓝色颠倒 
    //LPLD_LCD_WriteReg(0x0001,0x233F);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    LPLD_LCD_WriteReg(0x0002,0x0600);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0010,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    /* 定义数据格式 16位色 横屏 0x6070 */
    // TY1（bit7） TY0 ID1 ID0 AM LG2 LG1 LG0
    //   0          0   1   1   1  0   0   0
    // type A  0 0
    // type B  0 1
    // type C  1 1
    // AM   指针自动增加
    // 65K颜色
    // 寄存器 4f/4e 定义窗口大小
    LPLD_LCD_WriteReg(0x0011,0x6028);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    //LPLD_LCD_WriteReg(0x0011,0x6070);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    //
    LPLD_LCD_WriteReg(0x0005,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0006,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0016,0xEF1C);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0017,0x0003);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    //PT1 PT0 VLE2 VLE1 SPT 0  0 GON DTE CM 0 D1 D0
    //  0   0        1   0  0  0  1   1  0  0  1  1
    LPLD_LCD_WriteReg(0x0007,0x0233);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    //LPLD_LCD_WriteReg(0x0007,0x0133);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    //NO1 NO0 SDT1 SDT0 0 EQ2 EQ1 EQ0 DIV1 DIV0 SDIV SRTN RTN3 RTN2 RTN1 RTN0
    //                                 1     1    0    0    0    0   0   0
    //将DIV设置成2分频
    //
    //LPLD_LCD_WriteReg(0x000B,0x0020);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x000B,0x0020);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    //配置扫描模式
    LPLD_LCD_WriteReg(0x000F,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);   
    /* 扫描开始地址 */
    //垂直控制
    LPLD_LCD_WriteReg(0x0041,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0042,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    //水平RAMposition
    LPLD_LCD_WriteReg(0x0048,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0049,0x013F);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    
    LPLD_LCD_WriteReg(0x004A,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x004B,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME); 
    
    LPLD_LCD_WriteReg(0x0030,0x0707);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0031,0x0204);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0032,0x0204);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0033,0x0502);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0034,0x0507);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0035,0x0204);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0036,0x0204);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0037,0x0502);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x003A,0x0302);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x003B,0x0302);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0023,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0024,0x0000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    /*0000 390K  50 
      0010 430K  55 
      0101 470K  60 
      1000 510K  65 
      1010 548K  70 
      1100 587K  75 
      1110 626K  80 */
    //LPLD_LCD_WriteReg(0x0025,0x8000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    LPLD_LCD_WriteReg(0x0025,0xe000);    LPLD_LCD_Delayms(LCD_DELAY_TIME);
    //设定显示窗口       
    //LPLD_LCD_UpdateData();	//更新写入的数据使之生效*/
    LPLD_LCD_Delayms(20);         //等待LCD启动
  }
  
  return dID;
}

/*
 * LPLD_LCD_FillBackground
 * 填充LCD背景色
 * 
 * 参数:
 *    Color--16位RGB颜色值
 *
 * 输出:
 *    无
 */
void LPLD_LCD_FillBackground(uint16 Color)
{
  uint32_t index=0;
  LPLD_LCD_SetCursor(0,0); 
  LPLD_LCD_WriteIndex(LPLD_LCD_INFO.Write_GRAM);
  for( index = 0; index < MAX_X * MAX_Y; index++ )
  {
    LPLD_LCD_WriteData(Color);
  }
}

/*
 * LPLD_LCD_FillBackgroundGRAM
 * 填充LCD背景色到LCD_GRAM
 * 
 * 参数:
 *    *Color--16位RGB颜色值数组，长度为LCD_GRAM数组的长度
 *
 * 输出:
 *    无
 */
void LPLD_LCD_FillBackgroundGRAM(uint16 *Color)
{
  uint32_t index=0;
  LPLD_LCD_SetCursor(0,0); 
  LPLD_LCD_WriteIndex(LPLD_LCD_INFO.Write_GRAM);
  for( index = 0; index < MAX_X * MAX_Y; index++ )
  {
    LPLD_LCD_WriteData(Color[index]);
  }
}

/*
 * LPLD_LCD_BGR2RGB
 * BGR颜色转RGB颜色
 * 
 * 参数:
 *    Color--16位BGR颜色值
 *
 * 输出:
 *    16位RGB颜色值
 */
uint16 LPLD_LCD_BGR2RGB(uint16 Color)
{
  uint16  r, g, b, rgb;
  
  b = ( Color>>0 )  & 0x1f;
  g = ( Color>>5 )  & 0x3f;
  r = ( Color>>11 ) & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);
  
  return( rgb );
}

/*
 * LPLD_LCD_U8GRAY2RGB
 * U8灰度颜色转为RGB颜色
 * 
 * 参数:
 *    Color--8位灰度颜色值
 *
 * 输出:
 *    16位RGB颜色值
 */
uint16 LPLD_LCD_U8GRAY2RGB(uint8 Color)
{
  uint16  r, g, b, rgb;
  
  b = ( Color>>3 )  & 0x1f;
  g = ( Color>>2 )  & 0x3f;
  r = ( Color>>3 )  & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);
  
  return( rgb );
}

/*
 * LPLD_LCD_GetPoint
 * 取得LCD某点的16位RGB值
 * 
 * 参数:
 *    Xpos--X轴坐标
 *    Ypos--Y轴坐标
 *
 * 输出:
 *    16位RGB颜色值
 */
uint16 LPLD_LCD_GetPoint(uint16 Xpos,uint16 Ypos)
{
  uint16 dummy;
  
  LPLD_LCD_SetCursor(Xpos,Ypos);
  LPLD_LCD_WriteIndex(LPLD_LCD_INFO.Write_GRAM);  
 
  dummy = LPLD_LCD_ReadData();   /* 空读一次 */
  dummy = LPLD_LCD_ReadData(); 
  return  dummy;	          
}

/*
 * LPLD_LCD_SetPoint
 * 设置LCD某点的16位RGB值
 * 
 * 参数:
 *    Xpos--X轴坐标
 *    Ypos--Y轴坐标
 *    Color--16位RGB颜色值
 *
 * 输出:
 *    无
 */
void LPLD_LCD_SetPoint(uint16 Xpos,uint16 Ypos,uint16 Color)
{
  if( Xpos >= MAX_X || Ypos >= MAX_Y )
  {
    return;
  } 
  LPLD_LCD_SetCursor(Xpos,Ypos);
  LPLD_LCD_WriteReg(LPLD_LCD_INFO.Write_GRAM,Color);
}

/*
 * LPLD_LCD_DrawLine
 * 在指定坐标显示直线
 * 
 * 参数:
 *    x0--X轴起始坐标
 *    y0--Y轴起始坐标
 *    x1--X轴结束坐标
 *    y1--Y轴结束坐标
 *    Color--16位RGB颜色值
 *
 * 输出:
 *    无
 */	 
void LPLD_LCD_DrawLine( uint16 x0, uint16 y0, uint16 x1, uint16 y1 , uint16 color )
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
      LPLD_LCD_SetPoint(x0, y0, color);   /* 逐点显示 描垂直线 */
      y0++;
    }
    while( y1 >= y0 ); 
      return; 
  }
  if( dy == 0 )     /* Y轴上没有增量 画水平直线 */ 
  {
    do
    {
      LPLD_LCD_SetPoint(x0, y0, color);   /* 逐点显示 描水平线 */
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
      LPLD_LCD_SetPoint(x0,y0,color);    /* 画起点 */ 
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
    LPLD_LCD_SetPoint(x0,y0,color);
    }  
    else
    {
      temp = 2 * dx - dy;                      /* 靠近Y轴 */       
      while( y0 != y1 )
      {
        LPLD_LCD_SetPoint(x0,y0,color);     
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
  LPLD_LCD_SetPoint(x0,y0,color);
  }
} 


/*
 * LPLD_LCD_DrawSquare
 * 在指定坐标显示正方形
 * 
 * 参数： 
 *    x0--正方形的起始横坐标 
 *    y0--正方形的起始纵坐标
 *    side_length--正方形的边长
 *    Line_color--正方形边的颜色
 *    Fill_color--正方形整体填充的颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_DrawSquare(uint16 x0,uint16 y0,uint16 side_length,uint16 Line_color,uint16 Fill_color)
{
  uint16 i,j;
  if(side_length > MAX_X || side_length > MAX_Y)
    return;
  
  if(x0+side_length > MAX_X)
    return;
  else
    LPLD_LCD_DrawLine(x0,y0,x0+side_length,y0,Line_color);
  
  if(y0+side_length > MAX_Y)
    return;
  else
    LPLD_LCD_DrawLine(x0,y0,x0,y0+side_length,Line_color);
  
  LPLD_LCD_DrawLine(x0+side_length,y0,x0+side_length,y0+side_length,Line_color);
  LPLD_LCD_DrawLine(x0,y0+side_length,x0+side_length,y0+side_length,Line_color);
  
  for(i=0;i<side_length-1;i++)
  {
    for(j=0;j<side_length-1;j++)
    {
      LPLD_LCD_SetPoint(x0+1+i,y0+1+j,Fill_color);
    }
  }
}

/*
 * LPLD_LCD_DrawRectangle
 * 在指定坐标显示矩形
 * 
 * 参数： 
 *    x0--长方形的起始横坐标 
 *    y0--长方形的起始纵坐标
 *    side_xlength--长
 *    side_ylength--宽
 *    Line_color--长方形边的颜色
 *    Fill_color--长方形整体填充的颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_DrawRectangle(uint16 x0,uint16 y0,uint16 side_xlength,uint16 side_ylength,uint16 Line_color,uint16 Fill_color)
{
  uint16 i,j;
  if(side_xlength > MAX_X || side_ylength > MAX_Y)
    return;
  
  if(x0+side_xlength > MAX_X)
    return;
  else
    LPLD_LCD_DrawLine(x0,y0,x0+side_xlength,y0,Line_color);
  
  if(y0+side_ylength > MAX_Y)
    return;
  else
    LPLD_LCD_DrawLine(x0,y0,x0,y0+side_ylength,Line_color);
  
  LPLD_LCD_DrawLine(x0+side_xlength,y0,x0+side_xlength,y0+side_ylength,Line_color);
  LPLD_LCD_DrawLine(x0,y0+side_ylength,x0+side_xlength,y0+side_ylength,Line_color);
  
  for(i=0;i<side_xlength-1;i++)
  {
    for(j=0;j<side_ylength-1;j++)
    {
      LPLD_LCD_SetPoint(x0+1+i,y0+1+j,Fill_color);
    }
  }
}

/*
 * LPLD_LCD_GRAMFillRec
 * 填充指定颜色的矩形到LCD_GRAM
 * 
 * 参数： 
 *    x0--X轴起始坐标
 *    y0--Y轴起始坐标
 *    x1--X轴结束坐标
 *    y1--Y轴结束坐标
 *    Color--16位RGB颜色值
 *
 * 输出:
 *    无
 */
void LPLD_LCD_GRAMFillRec(uint16 x0,uint16 y0,uint16 x1,uint16 y1,uint16 Color)
{          
  uint16 i,j;   
  for(i=y0;i<=y1;i++)
  {
    for(j=x0; j<x1; j++)
    {
      LCD_GRAM[MAX_X*MAX_Y - (j-1)*MAX_Y - (i)] = Color;
    }    
  }
} 

/*
 * LPLD_LCD_GRAMDrawHLine
 * 填充指定颜色的横线到LCD_GRAM
 * 
 * 参数： 
 *    x0--X轴起始坐标
 *    y0--Y轴起始坐标
 *    len--横线长度
 *    Color--16位RGB颜色值
 *
 * 输出:
 *    无
 */
void LPLD_LCD_GRAMDrawHLine(uint16 x0,uint16 y0,uint16 len,uint16 Color)
{
  uint16 i;
  for(i=0; i<len; i++)
  {
    LCD_GRAM[MAX_X*MAX_Y - (x0+i-1)*MAX_Y - (y0)] = Color;
  }
}

/*
 * LPLD_LCD_GRAMDrawHLine
 * 显示一幅指定大小的U8灰度图像
 * 
 * 参数： 
 *    *Color--8位灰度图像数组
 *    w--图像宽
 *    h--图像高
 *
 * 输出:
 *    无
 */
void LPLD_LCD_DrawU8Gray(uint8 *Color, uint16 w, uint16 h)
{
  uint16 i=0, j=0;
 
  for(i=0; i<w; i++)
  {  
    LPLD_LCD_SetCursor(w-i-1, 0); 
    LPLD_LCD_WriteIndex(LPLD_LCD_INFO.Write_GRAM);
    for(j=0; j<h; j++)
    {
      LPLD_LCD_WriteData(LPLD_LCD_U8GRAY2RGB(Color[j*w+i]));
    }   
  }
}

/*
 * LPLD_LCD_ShowChar
 * 在指定坐标显示字符
 * 
 * 参数： 
 *    Xpos--X轴坐标
 *    Ypos--Y轴坐标
 *    ASCI--ASCI码
 *    charColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_ShowChar( uint16 Xpos, uint16 Ypos, uint8 ASCI, uint16 charColor, uint16 bkColor )
{
  uint16 i, j;
  uint8 tmp_char;
  for( i=0; i<16; i++ )
  {
    tmp_char = asc2_1608[ASCI-32][i];
    for( j=0; j<8; j++ )
    {
      if( (tmp_char >> j) & 0x01 == 0x01 )
      {
        LPLD_LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* 字符颜色 */
      }
      else
      {
        LPLD_LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* 背景颜色 */
      }
    }
  }
}

uint32 mypow(uint8 m,uint8 n)
{
	uint32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*
 * LPLD_LCD_ShowNum
 * 在指定坐标显示数字
 * 
 * 参数： 
 *    Xpos--X轴坐标
 *    Ypos--Y轴坐标
 *    num--数字
 *    len--数字长度
 *    charColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_ShowNum(uint16 Xpos, uint16 Ypos, uint16 num, uint8 len, uint16 charColor, uint16 bkColor )
{         	
  uint8 t,temp;						   
  for(t=0;t<len;t++)
  {
    temp=(num/mypow(10,len-t-1))%10;
    LPLD_LCD_ShowChar(Xpos+(8)*t,Ypos,temp+'0',charColor,bkColor); 
    CurX = CurX + 8;  
  }
} 

/*
 * LPLD_LCD_ShowString
 * 在指定坐标显示字符串
 * 
 * 参数： 
 *    Xpos--X轴坐标
 *    Ypos--Y轴坐标
 *    *str--字符串
 *    strColor--字符串字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_ShowString(uint16 Xpos, uint16 Ypos, uint8 *str,uint16 strColor, uint16 bkColor)
{
  uint8 TempChar;
  do
  {
    TempChar = *str++;  
    LPLD_LCD_ShowChar( Xpos, Ypos, TempChar, strColor, bkColor );    
    if( Xpos <= MAX_X - 8 )
    {
      Xpos += 8;
    } 
    else if ( Ypos <= MAX_Y - 16 )
    {
      Xpos = 0;
      Ypos += 16;
    }   
    else
    {
      Xpos = 0;
      Ypos = 0;
    } 
    CurX = CurX + 8;  
  }
  while ( *str != 0 );
}

/*
 * LPLD_LCD_PrintStringLine
 * 另起一行显示字符串
 * 
 * 参数： 
 *    *str--字符串
 *    strColor--字符串字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_PrintStringLine(uint8 *str,uint16 strColor, uint16 bkColor)
{
  CurX = 0;
  LPLD_LCD_ShowString(0, CurY*18, str, strColor, bkColor);
  CurY++;
  if((CurY+1)*18>MAX_Y)
  {
    CurY=0;
  }
}

/*
 * LPLD_LCD_PrintString
 * 接着当前行显示字符串
 * 
 * 参数： 
 *    *str--字符串
 *    strColor--字符串字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_PrintString(uint8 *str,uint16 Color, uint16 bkColor)
{
  LPLD_LCD_ShowString(CurX, (CurY-1)*18, str, Color, bkColor);
}

/*
 * LPLD_LCD_PrintNumLine
 * 另起一行显示数字
 * 
 * 参数： 
 *    num--数字
 *    len--数字长度
 *    numColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_PrintNumLine(uint16 num,uint8 len,uint16 numColor, uint16 bkColor)
{
  CurX = 0;
  LPLD_LCD_ShowNum(0, CurY*18, num, len, numColor, bkColor);
  CurY++;
  if((CurY+1)*18>MAX_Y)
  {
    CurY=0;
  }
}

/*
 * LPLD_LCD_PrintNum
 * 接着当前行显示数字
 * 
 * 参数： 
 *    num--数字
 *    len--数字长度
 *    numColor--字符字体颜色
 *    bkColor--字符背景颜色
 *
 * 输出:
 *    无
 */
void LPLD_LCD_PrintNum(uint16 num,uint8 len,uint16 Color, uint16 bkColor)
{
  LPLD_LCD_ShowNum(CurX, (CurY-1)*18, num,len, Color, bkColor);
}


