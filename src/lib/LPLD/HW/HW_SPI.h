/**
 * @file HW_SPI.h
 * @version 3.03[By LPLD]
 * @date 2014-2-10
 * @brief SPI底层模块相关函数
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
#ifndef __HW_SPI_H__
#define __HW_SPI_H__

//SPI模块中断回调函数类型
typedef void (*SPI_ISR_CALLBACK)(void);
//定义SPI工作模式
#define SPI_MODE_MASTER      (0)
#define SPI_MODE_SLAVE       (1)
//定义SPI FIFO 事件方式
#define SPI_FIFO_DMAREQUEST  (TRUE)
#define SPI_FIFO_INTREQUEST  (FALSE)
//定义SPI 分频系数
#define SPI_SCK_DIV_2        (0)
#define SPI_SCK_DIV_4        (1)
#define SPI_SCK_DIV_6        (2)
#define SPI_SCK_DIV_8        (3)
#define SPI_SCK_DIV_16       (4)
#define SPI_SCK_DIV_32       (5)
#define SPI_SCK_DIV_64       (6)
#define SPI_SCK_DIV_128      (7)
#define SPI_SCK_DIV_256      (8)
#define SPI_SCK_DIV_512      (9)
#define SPI_SCK_DIV_1024     (10)
#define SPI_SCK_DIV_2048     (11)
#define SPI_SCK_DIV_4096     (12)
#define SPI_SCK_DIV_8192     (13)
#define SPI_SCK_DIV_16384    (14
#define SPI_SCK_DIV_32768    (15)
//定义SPI PCS取值
#define SPI_PCS0             (1) //0x000001
#define SPI_PCS1             (2) //0x000010
#define SPI_PCS2             (4) //0x000100
#define SPI_PCS3             (8) //0x001000
#define SPI_PCS4             (16)//0x010000
#define SPI_PCS5             (32)//0x100000
//定义发送完毕后SPI PCS的状态
#define SPI_PCS_ASSERTED     (0x01)
#define SPI_PCS_INACTIVE     (0x00)

//SPI中断序号
#define SPI_TxComplete_Int              0
#define SPI_QueueEndReq_Int             1
#define SPI_TxFIFO_UnderflowInt         2
#define SPI_RxFIFO_OverflowInt          3
#define SPI_TxFIFO_FillInt              4
#define SPI_RxFIFO_DrainInt             5

typedef struct
{
  /*
    描述：
      选择SPIx
    取值：
      SPI0 - SPI通道0
      SPI1 - SPI通道1
      SPI2 - SPI通道2
    初始化：
      必须初始化
  */
  SPI_Type *SPI_SPIx;
  
  /*
    描述：
      选择SPIx 工作模式
    取值：
      SPI_MODE_MASTER - SPI主机工作模式
      SPI_MODE_SLAVE - SPI从机工作模式
    初始化：
      不必须初始化，默认主机工作模式
  */
  uint8 SPI_ModeSelect;
  
  /*
    描述：
      选择SPIx SCK总线分频系数，SPI外设的时钟基准是g_bus_clock
      SCK总线频率 = g_bus_clock/ SCK_DIV_XX
    取值：
      SPI_SCK_DIV_2     - 2分频
      SPI_SCK_DIV_4     - 4分频
      SPI_SCK_DIV_6     - 6分频
      SPI_SCK_DIV_8     - 8分频
      SPI_SCK_DIV_16    - 16分频
      SPI_SCK_DIV_32    - 32分频
      SPI_SCK_DIV_64    - 64分频
      SPI_SCK_DIV_128   - 128分频
      SPI_SCK_DIV_256   - 256分频
      SPI_SCK_DIV_512   - 512分频
      SPI_SCK_DIV_1024  - 1024分频
      SPI_SCK_DIV_2048  - 2048分频
      SPI_SCK_DIV_4096  - 4096分频
      SPI_SCK_DIV_8192  - 8192分频
      SPI_SCK_DIV_16384 - 16384分频
      SPI_SCK_DIV_32768 - 32768分频
    初始化：
      必须初始化
  */ 
  uint8 SPI_SckDivider;
  
  /*
    描述：
      使能Tx FIFO方式
    取值：
      TRUE - 使能Tx FIFO方式
      FALSE - 禁止Tx FIFO方式，采用传统SPI方式
    初始化：
      不必须初始化，默认禁止Tx FIFO方式，采用传统SPI方式
  */
  boolean SPI_EnableTxFIFO;
  
  /*
    描述：
      使能Rx FIFO方式
    取值：
      TRUE - 使能Rx FIFO方式
      FALSE - 禁止Rx FIFO方式，采用传统SPI方式
    初始化：
      不必须初始化，默认禁止Rx FIFO方式，采用传统SPI方式
  */
  boolean SPI_EnableRxFIFO;
  
  /*
    描述：
      使能发送完成中断
    取值：
      TRUE - 使能发送完成中断
      FALSE - 禁止发送完成中断
    初始化：
      不必须初始化，默认禁止发送完成中断
  */
  boolean SPI_TxCompleteIntEnable;
  
  /*
    描述：
      选择使能发送队列结束中断
    取值：
      TRUE - 使能发送队列结束中断
      FALSE - 禁止发送队列结束中断
    初始化：
      不必须初始化，默认禁止发送队列结束中断
  */
  boolean SPI_QueueEndIntEnable; 
  
  /*
    描述：
      选择使能txFIFO为空中断
    取值：
      TRUE - 使能txFIFO为空中断
      FALSE - 禁止txFIFO为空中断
    初始化：
      不必须初始化，默认禁止txFIFO为空中断
  */
  boolean SPI_TxFIFO_UnderflowIntEnable;
  
  /*
    描述：
      选择使能rxFIFO溢出中断
    取值：
      TRUE - 使能rxFIFO溢出中断
      FALSE - 禁止rxFIFO溢出中断
    初始化：
      不必须初始化，默认禁止rxFIFO溢出中断
  */
  boolean SPI_RxFIFO_OverflowIntEnable;
  
  /*
    描述：
      选择使能txFIFO填充中断
    取值：
      TRUE - 使能txFIFO填充中断
      FALSE - 禁止txFIFO填充中断
    初始化：
      不必须初始化，默认禁止txFIFO填充中断
  */
  boolean SPI_TxFIFO_FillIntEnable;
  
  /*
    描述：
      选择使能rxFIFO提取中断
    取值：
      TRUE - 使能rxFIFO提取中断
      FALSE - 禁止rxFIFO提取中断
    初始化：
      不必须初始化，默认禁止rxFIFO提取中断
  */
  boolean SPI_RxFIFO_DrainIntEnable;
  
  /*
    描述：
      TxFIFO请求方式选择
    取值：
      SPI_FIFO_INTREQUEST - 产生中断请求
      SPI_FIFO_DMAREQUEST - 产生DMA请求
    初始化：
      不必须初始化，默认为SPI_FIFO_INTREQUEST
  */ 
  boolean SPI_TxFIFO_RequestSelect;
  
  /*
    描述：
      RxFIFO请求方式选择
    取值：
      SPI_FIFO_INTREQUEST - 产生中断请求
      SPI_FIFO_DMAREQUEST - 产生DMA请求
    初始化：
      不必须初始化，默认为SPI_FIFO_INTREQUEST
  */
  boolean SPI_RxFIFO_RequestSelect;
  
  /*
    描述：
      Pcs0引脚，每个SPIx对应不同的Pcs0Pin
    取值：
      SPI0-PTA14、*PTC4
      SPI1-PTB10、*PTE4
      SPI2-PTD11、*PTB20
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_Pcs0Pin;
  
  /*
    描述：
      Pcs1引脚，每个SPIx对应不同的Pcs1Pin
    取值：
      SPI0-PTC3、*PTD4
      SPI1-PTB9、*PTE0
      SPI2-*PTD15
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_Pcs1Pin;
  
  /*
    描述：
      Pcs2引脚，每个SPIx对应不同的Pcs2Pin
    取值：
      SPI0-PTC2、*PTD5
      SPI1-*PTE5
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_Pcs2Pin;
  
  /*
    描述：
      Pcs3引脚，每个SPIx对应不同的Pcs3Pin
    取值：
      SPI0-PTC1、*PTD6
      SPI1-*PTE6
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_Pcs3Pin;
  
  /*
    描述：
      Pcs4引脚，每个SPIx对应不同的Pcs4Pin
    取值：
      SPI0-*PTC0
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_Pcs4Pin;
  
  /*
    描述：
      Pcs5引脚，每个SPIx对应不同的Pcs2Pin
    取值：
      SPI0-*PTB23
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_Pcs5Pin;
  
  /*
    描述：
      Sck引脚，每个SPIx对应不同的SckPin
    取值：
      SPI0-PTA15、*PTC5
      SPI1-PTB11、*PTE2
      SPI2-PTD12、*PTB21
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_SckPin;
  
  /*
    描述：
      MOSI引脚，每个SPIx对应不同的MosiPin
    取值：
      SPI0-PTA16、*PTC6
      SPI1-PTB16、*PTE1
      SPI2-PTD13、*PTB22
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_MosiPin;
  
  /*
    描述：
      MISO引脚，每个SPIx对应不同的MisoPin
    取值：
      SPI0-PTA17、*PTC7
      SPI1-PTB17、*PTE3
      SPI2-PTD14、*PTB23
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type SPI_MisoPin;
  
  /*
    描述：
      发送完成中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SPI_ISR_CALLBACK SPI_TxCompleteIntIsr;  
  
  /*
    描述：
      发送队列结束中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SPI_ISR_CALLBACK SPI_QueueEndIntIsr;
  
  /*
    描述：
      txFIFO为空中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SPI_ISR_CALLBACK SPI_TxFIFO_UnderflowIntIsr;
  
  /*
    描述：
      rxFIFO溢出中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SPI_ISR_CALLBACK SPI_RxFIFO_OverflowIntIsr;
  
  /*
    描述：
      txFIFO填充中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SPI_ISR_CALLBACK SPI_TxFIFO_FillIntIsr;
  
  /*
    描述：
      rxFIFO提取中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  SPI_ISR_CALLBACK SPI_RxFIFO_DrainIntIsr;
  
}SPI_InitTypeDef;

//SPI初始化函数
uint8 LPLD_SPI_Init(SPI_InitTypeDef );
//SPI反初始化函数
uint8 LPLD_SPI_Deinit(SPI_InitTypeDef );
//SPI使能外设中断
uint8 LPLD_SPI_EnableIrq(SPI_InitTypeDef );
//SPI禁止外设中断
uint8 LPLD_SPI_DisableIrq(SPI_InitTypeDef );
//SPI主机读写函数
uint8 LPLD_SPI_Master_WriteRead(SPI_Type * ,uint8 ,uint8 ,uint8);
//SPI主机读函数
uint8 LPLD_SPI_Master_Read(SPI_Type *);
//SPI主机写函数
void LPLD_SPI_Master_Write(SPI_Type * ,uint8 ,uint8 ,uint8);

/********************************************************************/

#endif /* __HW_SPI_H__ */
