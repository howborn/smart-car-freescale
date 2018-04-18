/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file MT9V034.c
 * @version 0.0
 * @date 2015-2-7
 * @brief 关于该工程的描述
 *
 * 版权所有:北华航天工业学院 第十届飞思卡尔  摄像头2队 
 * 硬件平台:  MK60FX512
 * 
 */
#include "MT9V034.h"
#include "Delay.h"

//SCCB通信的引脚
/* SCL */
#define SCCB_SCL_PTX        PTB  
#define SCCB_SCL_IOX        GPIO_Pin8
#define SCCB_SCL_SET()      PTB8_O = 1 
#define SCCB_SCL_CLR()      PTB8_O = 0
/* SDA*/
#define SCCB_SDA_PTX        PTB  
#define SCCB_SDA_IOX        GPIO_Pin10
#define SCCB_SDA_O          PTB10_O 
#define SCCB_SDA_I          PTB10_I 
//定义SDA输入输出
#define SCCB_SDA_OUT()      DDRB10 = 1
#define SCCB_SDA_IN()       DDRB10 = 0

/*
 * LPLD_SCCB_Init
 * 初始化SCCB所需引脚
 */
void LPLD_SCCB_Init(void)
{
  GPIO_InitTypeDef sccb;
  //SCL
  sccb.GPIO_PTx = SCCB_SCL_PTX;
  sccb.GPIO_Pins = SCCB_SCL_IOX;
  sccb.GPIO_Dir = DIR_OUTPUT;
  sccb.GPIO_Output = OUTPUT_H;
  sccb.GPIO_PinControl = NULL;
  LPLD_GPIO_Init(sccb);
  //SDA上拉
  sccb.GPIO_PTx = SCCB_SDA_PTX;
  sccb.GPIO_Pins = SCCB_SDA_IOX;
  sccb.GPIO_Dir = DIR_OUTPUT;
  sccb.GPIO_Output = OUTPUT_H;
  sccb.GPIO_PinControl = INPUT_PULL_UP;
  LPLD_GPIO_Init(sccb);
}

/*
 * LPLD_SCCB_Start
 * SCCB起始信号，内部调用
 */
static uint8 LPLD_SCCB_Start(void)
{
  SCCB_SDA_O=1;
  SCCB_SCL_SET();
  Delayus(5000);
  
  SCCB_SDA_IN();
  if(!SCCB_SDA_I)
  {
    SCCB_SDA_OUT();	
    return 0;
  }
  SCCB_SDA_OUT();	
  SCCB_SDA_O=0;
  Delayus(5000);
  
  SCCB_SDA_IN();
  if(SCCB_SDA_I) 
  {
    SCCB_SDA_OUT();
    return 0;
  }
  SCCB_SDA_OUT();
  SCCB_SDA_O=0;

  return 1;
}

/*
 * LPLD_SCCB_Stop
 * SCCB停止信号，内部调用
 */
static void LPLD_SCCB_Stop(void)
{
  SCCB_SCL_CLR();
  SCCB_SDA_O=0;
  Delayus(5000);
  
  SCCB_SCL_SET();
  Delayus(5000);
  SCCB_SDA_O=1;
  Delayus(5000);
}

/*
 * LPLD_SCCB_Stop
 * SCCB应答信号，内部调用
 */
static void LPLD_SCCB_Ack(void)
{	
  SCCB_SCL_CLR();
  Delayus(5000);
  
  SCCB_SDA_O=0;
  Delayus(5000);
  
  SCCB_SCL_SET();
  Delayus(5000);
  
  SCCB_SCL_CLR();
  Delayus(5000);
}

/*
 * LPLD_SCCB_NoAck
 * SCCB无应答信号，内部调用
 */
static void LPLD_SCCB_NoAck(void)
{	
  SCCB_SCL_CLR();
  Delayus(5000);
  SCCB_SDA_O=1;
  Delayus(5000);
  SCCB_SCL_SET();
  Delayus(5000);
  SCCB_SCL_CLR();
  Delayus(5000);
}

/*
 * LPLD_SCCB_WaitAck
 * SCCB等待应答信号，内部调用
 */
static uint8 LPLD_SCCB_WaitAck(void) 	
{
  SCCB_SCL_CLR();
  Delayus(5000);
  SCCB_SDA_O=1;	
  Delayus(5000);
  
  SCCB_SCL_SET();
  
  SCCB_SDA_IN();
  Delayus(5000);
  
  if(SCCB_SDA_I)
  {
    SCCB_SDA_OUT();
    SCCB_SCL_CLR();
    return 0;
  }
  SCCB_SDA_OUT();
  SCCB_SCL_CLR();
  return 1;
}

/*
 * LPLD_SCCB_SendByte
 * SCCB发送数据，内部调用
 */
static void LPLD_SCCB_SendByte(uint8 data) 
{
  uint8 i=8;
  while(i--)
  {
    SCCB_SCL_CLR();
    Delayus(5000);
    if(data&0x80)
    {
      SCCB_SDA_O=1; 
    }
    else 
    {
      SCCB_SDA_O=0;   
    }
    data<<=1;
    Delayus(5000);
    SCCB_SCL_SET();
    Delayus(5000);
  }
  SCCB_SCL_CLR();
}

/*
 * LPLD_SCCB_SendByte
 * SCCB接收数据，内部调用
 */
static uint8 LPLD_SCCB_ReceiveByte(void)  
{ 
  uint8 i=8;
  uint8 ReceiveByte=0;
  
  SCCB_SDA_O=1;	
  Delayus(5000);
  
  SCCB_SDA_IN();	
  
  while(i--)
  {
    ReceiveByte<<=1;      
    SCCB_SCL_CLR();
    Delayus(5000);
    
    SCCB_SCL_SET();
    Delayus(5000);	
    
    if(SCCB_SDA_I)
    {
      ReceiveByte|=0x01;
    }
    
  }
  SCCB_SDA_OUT();
  SCCB_SCL_CLR();
  
  return ReceiveByte;
}
/*
 * LPLD_SCCB_WriteReg
 * 写SCCB设备寄存器
 * 
 * 参数:
 *    reg_addr--寄存器地址,8位
 *    data--待写数据,16位 
 *
 * 输出:
 *    1-成功
 *    0-失败
 */
uint8 LPLD_SCCB_WriteReg(uint8 reg_addr, uint16 data)
{		
  if(!LPLD_SCCB_Start())
  {
    return 0;
  }
  LPLD_SCCB_SendByte(SCCB_DEV_ADR);         
  if(!LPLD_SCCB_WaitAck())
  {
    LPLD_SCCB_Stop(); 
    return 0;
  }
  LPLD_SCCB_SendByte(reg_addr);   
  LPLD_SCCB_WaitAck();	
  LPLD_SCCB_SendByte((uint8)(data>>8));                        //高八位数据
  LPLD_SCCB_WaitAck();   
  LPLD_SCCB_SendByte((uint8)(data&0x00ff));           //低八位数据
  LPLD_SCCB_WaitAck();  
  LPLD_SCCB_Stop(); 
  return 1;
}									 

/*
 * LPLD_SCCB_ReadReg
 * 读SCCB设备寄存器
 * 
 * 参数:
 *    reg_addr--寄存器地址
 *    *data--待存读出数据地址
 *
 * 输出:
 *    1-成功
 *    0-失败
 */          
uint8 LPLD_SCCB_ReadReg(uint8 reg_addr, uint16* data)
{
  uint8 data_l;
  if(!LPLD_SCCB_Start())
  {
    return 0;
  }
  LPLD_SCCB_SendByte(SCCB_DEV_ADR); 
  if( !LPLD_SCCB_WaitAck() )
  {
    LPLD_SCCB_Stop(); 
    return 0;
  }
  LPLD_SCCB_SendByte(reg_addr); 
  LPLD_SCCB_WaitAck();	
  LPLD_SCCB_Stop(); 
  
  if(!LPLD_SCCB_Start())
  {
    return 0;
  }
  LPLD_SCCB_SendByte(SCCB_DEV_ADR+1); 
  if(!LPLD_SCCB_WaitAck())
  {
    LPLD_SCCB_Stop(); 
    return 0;
  }
  *data = LPLD_SCCB_ReceiveByte();                //高八位数据
  LPLD_SCCB_Ack(); 
  data_l = LPLD_SCCB_ReceiveByte();               //低八位数据
  *data = (*data<<8)|data_l;                      //接受到的16位数据
  LPLD_SCCB_NoAck();
  
  LPLD_SCCB_Stop();
  return 1;
}
/*-----------------------------------------------------------------------
* MT9V034_Init: 摄像头初始化
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void MT9V034_Init()
{
  LPLD_SCCB_Init();
  Delayms(20);
  LPLD_SCCB_WriteReg(MTV_READ_MODE_REG_A,0x033a);       //修改为4分频，输出方式为倒像
  LPLD_SCCB_WriteReg(MTV_VREF_ADC_REG,0x0007);          //修改ADC转化的参考电压
//LPLD_SCCB_WriteReg(MTV_MAX_GAIN_REG,0x0032);
}