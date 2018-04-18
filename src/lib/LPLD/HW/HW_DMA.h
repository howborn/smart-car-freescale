/**
* @file HW_DMA.h
* @version 3.0[By LPLD]
* @date 2013-06-18
* @brief DMA底层模块相关函数
*
* 更改建议:不建议修改
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
#ifndef __HW_DMA_H__
#define __HW_DMA_H__

//声明eDMA中断回掉函数
typedef void (*DMA_ISR_CALLBACK)(void);

#if defined(CPU_MK60DZ10)
//=====================================
//  DMA request sources Number--MUX0
//      外设的DMA请求号
//=====================================
#define UART0_REV_DMAREQ  2
#define UART0_TRAN_DMAREQ 3
#define UART1_REV_DMAREQ  4
#define UART1_TRAN_DMAREQ 5
#define UART2_REV_DMAREQ  6
#define UART2_TRAN_DMAREQ 7
#define UART3_REV_DMAREQ  8
#define UART3_TRAN_DMAREQ 9
#define UART4_REV_DMAREQ  10
#define UART4_TRAN_DMAREQ 11
#define UART5_REV_DMAREQ  12
#define UART5_TRAN_DMAREQ 13
#define I2S0_REV_DMAREQ   14
#define I2S0_TRAN_DMAREQ  15
#define SPI0_REV_DMAREQ   16
#define SPI0_TRAN_DMAREQ  17
#define SPI1_REV_DMAREQ   18
#define SPI1_TRAN_DMAREQ  19
#define I2C0_DMAREQ       22
#define I2C1_DMAREQ       23
#define FTM0_CH0_DMAREQ   24
#define FTM0_CH1_DMAREQ   25
#define FTM0_CH2_DMAREQ   26
#define FTM0_CH3_DMAREQ   27
#define FTM0_CH4_DMAREQ   28
#define FTM0_CH5_DMAREQ   29
#define FTM0_CH6_DMAREQ   30
#define FTM0_CH7_DMAREQ   31
#define FTM1_CH0_DMAREQ   32
#define FTM1_CH1_DMAREQ   33
#define FTM2_CH0_DMAREQ   34
#define FTM2_CH1_DMAREQ   35
#define FTM3_CH0_DMAREQ   36
#define FTM3_CH1_DMAREQ   37
#define FTM3_CH2_DMAREQ   38
#define FTM1_CH3_DMAREQ   39  
#define ADC0_DMAREQ       40
#define ADC1_DMAREQ       41
#define CMP0_DMAREQ       42
#define CMP1_DMAREQ       43
#define CMP2_DMAREQ       44
#define DAC0_DMAREQ       45
#define DAC1_DMAREQ       46
#define CMT_DMAREQ        47
#define PDB_DMAREQ        48
#define PORTA_DMAREQ      49
#define PORTB_DMAREQ      50
#define PORTC_DMAREQ      51
#define PORTD_DMAREQ      52
#define PORTE_DMAREQ      53
#define FTM3_CH4_DMAREQ   54
#define FTM3_CH5_DMAREQ   55
#define FTM3_CH6_DMAREQ   56
#define FTM3_CH7_DMAREQ   57
#define DMA_MUX_58        58
#define DMA_MUX_59        59
#define DMA_MUX_60        60
#define DMA_MUX_61        61
#define DMA_MUX_62        62
#define DMA_MUX_63        63

#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
//==========================================================
//  DMA request sources Number--MUX0 channel00 -- channel15
//      外设的DMA请求号
//===========================================================
#define UART0_REV_DMAREQ  2
#define UART0_TRAN_DMAREQ 3
#define UART1_REV_DMAREQ  4
#define UART1_TRAN_DMAREQ 5
#define UART2_REV_DMAREQ  6
#define UART2_TRAN_DMAREQ 7
#define UART3_REV_DMAREQ  8
#define UART3_TRAN_DMAREQ 9
#define UART4_REV_DMAREQ  10
#define UART4_TRAN_DMAREQ 11
#define UART5_REV_DMAREQ  12
#define UART5_TRAN_DMAREQ 13
#define I2S0_REV_DMAREQ   14
#define I2S0_TRAN_DMAREQ  15
#define SPI0_REV_DMAREQ   16
#define SPI0_TRAN_DMAREQ  17
#define SPI1_REV_DMAREQ   18
#define SPI1_TRAN_DMAREQ  19
#define SPI2_REV_DMAREQ   20
#define SPI2_TRAN_DMAREQ  21
#define I2C0_DMAREQ       22
#define I2C1_I2C2_DMAREQ  23
#define FTM0_CH0_DMAREQ   24
#define FTM0_CH1_DMAREQ   25
#define FTM0_CH2_DMAREQ   26
#define FTM0_CH3_DMAREQ   27
#define FTM0_CH4_DMAREQ   28
#define FTM0_CH5_DMAREQ   29
#define FTM0_CH6_DMAREQ   30
#define FTM0_CH7_DMAREQ   31
#define FTM1_CH0_DMAREQ   32
#define FTM1_CH1_DMAREQ   33
#define FTM2_CH0_DMAREQ   34
#define FTM2_CH1_DMAREQ   35
#define IEEE_1588_Timers0 36
#define IEEE_1588_Timers1 37
#define IEEE_1588_Timers2 38
#define IEEE_1588_Timers3 39
#define ADC0_DMAREQ       40
#define ADC1_DMAREQ       41
#define CMP0_DMAREQ       42
#define CMP1_DMAREQ       43
#define CMP2_DMAREQ       44
#define DAC0_DMAREQ       45
#define DAC1_DMAREQ       46
#define CMT_DMAREQ        47
#define PDB_DMAREQ        48
#define PORTA_DMAREQ      49
#define PORTB_DMAREQ      50
#define PORTC_DMAREQ      51
#define PORTD_DMAREQ      52
#define PORTE_DMAREQ      53
#define DMA_MUX_54        54
#define DMA_MUX_55        55
#define DMA_MUX_56        56
#define DMA_MUX_57        57
#define DMA_MUX_58        58
#define DMA_MUX_59        59
#define DMA_MUX_60        60
#define DMA_MUX_61        61
#define DMA_MUX_62        62
#define DMA_MUX_63        63
//==========================================================
//  DMA request sources Number--MUX1 channel16 -- channel31
//      外设的DMA请求号
//==========================================================
#define UART0_REV_DMAREQ  2
#define UART0_TRAN_DMAREQ 3
#define UART1_REV_DMAREQ  4
#define UART1_TRAN_DMAREQ 5
#define UART2_REV_DMAREQ  6
#define UART2_TRAN_DMAREQ 7
#define UART3_REV_DMAREQ  8
#define UART3_TRAN_DMAREQ 9
#define UART4_REV_DMAREQ  10
#define UART4_TRAN_DMAREQ 11
#define UART5_REV_DMAREQ  12
#define UART5_TRAN_DMAREQ 13
#define I2S0_REV_DMAREQ   14
#define I2S0_TRAN_DMAREQ  15
#define SPI0_REV_DMAREQ   16
#define SPI0_TRAN_DMAREQ  17
#define SPI1_REV_DMAREQ   18
#define SPI1_TRAN_DMAREQ  19
#define SPI2_REV_DMAREQ   20
#define SPI2_TRAN_DMAREQ  21
#define FTM3_CH0_DMAREQ   24
#define FTM3_CH1_DMAREQ   25
#define FTM3_CH2_DMAREQ   26
#define FTM3_CH3_DMAREQ   27
#define FTM3_CH4_DMAREQ   28
#define FTM3_CH5_DMAREQ   29
#define FTM3_CH6_DMAREQ   30
#define FTM3_CH7_DMAREQ   31
#define IEEE_1588_Timers0 36
#define IEEE_1588_Timers1 37
#define IEEE_1588_Timers2 38
#define IEEE_1588_Timers3 39
#define ADC0_DMAREQ       40
#define ADC1_DMAREQ       41
#define ADC2_DMAREQ       42
#define ADC3_DMAREQ       43
#define DAC0_DMAREQ       45
#define DAC1_DMAREQ       46
#define CMP0_REQ          47
#define CMP1_REQ          48
#define CMP2_REQ          49
#define CMP3_REQ          50
#define PORTF_DMAREQ      53
#define DMA_MUX_54        54
#define DMA_MUX_55        55
#define DMA_MUX_56        56
#define DMA_MUX_57        57
#define DMA_MUX_58        58
#define DMA_MUX_59        59
#define DMA_MUX_60        60
#define DMA_MUX_61        61
#define DMA_MUX_62        62
#define DMA_MUX_63        63
#endif

//==========================================
//      定义DMA通道号
//==========================================
#define  DMA_CH0 0
#define  DMA_CH1 1
#define  DMA_CH2 2
#define  DMA_CH3 3
#define  DMA_CH4 4
#define  DMA_CH5 5
#define  DMA_CH6 6
#define  DMA_CH7 7
#define  DMA_CH8 8
#define  DMA_CH9 9
#define  DMA_CH10 10
#define  DMA_CH11 11
#define  DMA_CH12 12
#define  DMA_CH13 13
#define  DMA_CH14 14
#define  DMA_CH15 15
#if defined(CPU_MK60F12) || defined(CPU_MK60F15)
#define  DMA_CH16 16
#define  DMA_CH17 17
#define  DMA_CH18 18
#define  DMA_CH19 19
#define  DMA_CH20 20
#define  DMA_CH21 21
#define  DMA_CH22 22
#define  DMA_CH23 23
#define  DMA_CH24 24
#define  DMA_CH25 25
#define  DMA_CH26 26
#define  DMA_CH27 27
#define  DMA_CH28 28
#define  DMA_CH29 29
#define  DMA_CH30 30
#define  DMA_CH31 31
#endif
//===========================================
//      DMA 传输的数据长度
//      源地址数据长度分为：  8bit、16bit、32bit
//      目的地址数据长度分为：8bit、16bit、32bit
//===========================================
#define DMA_SRC_8BIT    0
#define DMA_SRC_16BIT   1
#define DMA_SRC_32BIT   2
#define DMA_SRC_16BYTE  4

#define DMA_DST_8BIT    0
#define DMA_DST_16BIT   1
#define DMA_DST_32BIT   2
#define DMA_DST_16BYTE  4


typedef struct 
{
  /*
    描述：
      选择DMA通道
    取值：
      DMA_CH0~DMA_CH15
    <注:只有MK60F系列含有DMA_CH16~DMA_CH31>
    初始化：
      必须初始化
  */
  uint8  DMA_CHx;  
  
  /*
    描述：
      选择DMA通道对应的请求源
    取值：
      见本页"DMA request sources Number"宏定义
    初始化：
      必须初始化
  */
  uint8  DMA_Req; 
  
  /*
    描述：
      是否使能DMA的周期触发功能
    取值：
      TRUE-使能周期触发
      FALSE-禁用周期触发
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DMA_PeriodicTriggerEnable;
  
  /*
    描述：
      配置主循环计数
    取值：
      15位无符号整型（0~0x7FFF）
    初始化：
      必须初始化
  */
  uint16 DMA_MajorLoopCnt;
  
  /*
    描述：
      配置次循环传输字节数
    取值：
      0~0xFFFFFFFF-32位无符号整型
    初始化：
      必须初始化
  */
  uint32 DMA_MinorByteCnt;   
    
  /*
    描述：
      配置源数据地址
    取值：
      32位地址（0~0xFFFFFFFF）
    初始化：
      必须初始化
  */  
  uint32 DMA_SourceAddr;
    
  /*
    描述：
      配置源数据大小
    取值：
      DMA_SRC_8BIT-8位数据
      DMA_SRC_16BIT-16位数据
      DMA_SRC_32BIT-32位数据
      DMA_SRC_16BYTE-16字节数据
    初始化：
      不必须初始化，默认DMA_SRC_8BIT
  */
  uint8 DMA_SourceDataSize;
  
  /*
    描述：
      配置源数据地址偏移，即在上次读取后源地址的偏移量
    取值：
      16位有符号整型（-32768~32767）
    初始化：
      不必须初始化，默认0
  */ 
  int16 DMA_SourceAddrOffset;
  
  /*
    描述：
      配置源数据地址在主循环完成后的调整量
    取值：
      32位有符号整型
    初始化：
      不必须初始化，默认0
  */ 
  int32 DMA_LastSourceAddrAdj;  
    
  /*
    描述：
      配置目的数据地址
    取值：
      32位地址（0~0xFFFFFFFF）
    初始化：
      必须初始化
  */  
  uint32 DMA_DestAddr;
    
  /*
    描述：
      配置目的数据大小
    取值：
      DMA_DST_8BIT-8位数据
      DMA_DST_16BIT-16位数据
      DMA_DST_32BIT-32位数据
      DMA_DST_16BYTE-16字节数据
    初始化：
      不必须初始化，默认DMA_DST_8BIT
  */
  uint8 DMA_DestDataSize;
  
  /*
    描述：
      配置目的数据地址偏移，即在上次写入后目的地址的偏移量
    取值：
      16位有符号整型（-32768~32767）
    初始化：
      不必须初始化，默认0
  */ 
  int16 DMA_DestAddrOffset;
  
  /*
    描述：
      配置目的数据地址在主循环完成后的调整量
    取值：
      32位有符号整型
    初始化：
      不必须初始化，默认0
  */ 
  int32 DMA_LastDestAddrAdj;
  
  /*
    描述：
      使能自动禁用请求，使能后通道请求将在主循环结束后禁用
    取值：
      TRUE-使能自动禁用请求
      FALSE-禁用自动禁用请求
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DMA_AutoDisableReq;
  
  /*
    描述：
      使能DMA主循环完成中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DMA_MajorCompleteIntEnable;
  
  /*
    描述：
      使能DMA主循环完成到一半中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DMA_MajorHalfCompleteIntEnable;
    
  /*
    描述：
      DMA中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  DMA_ISR_CALLBACK DMA_Isr; 
   
}DMA_InitTypeDef;

//使能通道x的DMA请求
#define LPLD_DMA_EnableReq(CHX)  (DMA0->ERQ|=(1<<CHX))
//禁用通道x的DMA请求
#define LPLD_DMA_DisableReq(CHX) (DMA0->ERQ&=~(1<<CHX))
//加载源地址
#define LPLD_DMA_LoadSrcAddr(CHX, ADDR)   (DMA0->TCD[CHX].SADDR=DMA_SADDR_SADDR(ADDR))
//加载目的地址
#define LPLD_DMA_LoadDstAddr(CHX, ADDR)   (DMA0->TCD[CHX].DADDR=DMA_DADDR_DADDR(ADDR))

//初始化eDMA模块
uint8 LPLD_DMA_Init(DMA_InitTypeDef);
//使能eDMA中断
uint8 LPLD_DMA_EnableIrq(DMA_InitTypeDef);
//禁用eDMA中断
uint8 LPLD_DMA_DisableIrq(DMA_InitTypeDef);
//DMA服务请求软件开始
void LPLD_DMA_SoftwareStartService(DMA_InitTypeDef);

#endif /* __HW_DMA_H__ */