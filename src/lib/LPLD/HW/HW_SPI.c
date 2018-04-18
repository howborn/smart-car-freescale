/**
 * @file HW_SPI.c
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
#include "common.h"
#include "HW_SPI.h"

SPI_ISR_CALLBACK SPI0_ISR[6];
SPI_ISR_CALLBACK SPI1_ISR[6];
SPI_ISR_CALLBACK SPI2_ISR[6];

/*
 * LPLD_SPI_Init
 * SPI初始化函数,在该函数中选择SPI通道，初始化SPI SCK总线时钟
 * 选择使能Tx，Rx FIFO，选择开启SPI外设的发送完成，发送队列结束，
 * Tx FIFO队列为空，Rx FIFO队列溢出，选择FIFO的中断方式和DMA请求等
 * 
 * 参数:
 *    spi_init_structure--SPI初始化结构体，
 *                        具体定义见SPI_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_SPI_Init(SPI_InitTypeDef spi_init_structure)
{
  SPI_Type * spix = spi_init_structure.SPI_SPIx;
  uint8 spi_mode = spi_init_structure.SPI_ModeSelect;
  uint8 sck_div = spi_init_structure.SPI_SckDivider; 
  boolean txFIFO_enable = spi_init_structure.SPI_EnableTxFIFO;
  boolean rxFIFO_enable = spi_init_structure.SPI_EnableRxFIFO;
  //定义SPI外设中断变量
  boolean tx_complete_int = spi_init_structure.SPI_TxCompleteIntEnable;
  boolean QueueEnd_Request_int = spi_init_structure.SPI_QueueEndIntEnable; 
  boolean txFIFO_underflow_int = spi_init_structure.SPI_TxFIFO_UnderflowIntEnable;
  boolean rxFIFO_overflow_int = spi_init_structure.SPI_RxFIFO_OverflowIntEnable;
  boolean txFIFO_Fill_int = spi_init_structure.SPI_TxFIFO_FillIntEnable;
  boolean rxFIFO_Drain_int = spi_init_structure.SPI_RxFIFO_DrainIntEnable; 
  boolean txFIFO_req = spi_init_structure.SPI_TxFIFO_RequestSelect;
  boolean rxFIFO_req = spi_init_structure.SPI_RxFIFO_RequestSelect;
  //选择SPI引脚
  PortPinsEnum_Type miso_pin = spi_init_structure.SPI_MisoPin;//MISO 
  PortPinsEnum_Type mosi_pin = spi_init_structure.SPI_MosiPin;//MOSI
  PortPinsEnum_Type sck_pin = spi_init_structure.SPI_SckPin;  //SCK
  PortPinsEnum_Type pcs0_pin = spi_init_structure.SPI_Pcs0Pin;//PCS0
  PortPinsEnum_Type pcs1_pin = spi_init_structure.SPI_Pcs1Pin;//PCS1
  PortPinsEnum_Type pcs2_pin = spi_init_structure.SPI_Pcs2Pin;//PCS2
  PortPinsEnum_Type pcs3_pin = spi_init_structure.SPI_Pcs3Pin;//PCS3
  PortPinsEnum_Type pcs4_pin = spi_init_structure.SPI_Pcs4Pin;//PCS4
  PortPinsEnum_Type pcs5_pin = spi_init_structure.SPI_Pcs5Pin;//PCS5
  //定义中断回掉函数
  SPI_ISR_CALLBACK TxComplete_isr = spi_init_structure.SPI_TxCompleteIntIsr;
  SPI_ISR_CALLBACK QueueEndReq_isr = spi_init_structure.SPI_QueueEndIntIsr;
  SPI_ISR_CALLBACK UnderflowInt_isr = spi_init_structure.SPI_TxFIFO_UnderflowIntIsr;
  SPI_ISR_CALLBACK OverflowInt_isr = spi_init_structure.SPI_RxFIFO_OverflowIntIsr;
  SPI_ISR_CALLBACK FillInt_isr = spi_init_structure.SPI_TxFIFO_FillIntIsr;
  SPI_ISR_CALLBACK DrainInt_isr = spi_init_structure.SPI_RxFIFO_DrainIntIsr;
  //检测参数
  ASSERT(spi_mode <= SPI_MODE_MASTER);
  ASSERT(sck_div <= SPI_SCK_DIV_32768);

  if(spix == SPI0)
  {
    SIM->SCGC6 |= SIM_SCGC6_DSPI0_MASK;  

    //选择PCS0
    if(pcs0_pin == PTA14)
    {
      PORTA->PCR[14] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else//pcs0_pin == PTC4
    {
      PORTC->PCR[4] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS1
    if(pcs1_pin == PTC3)
    {
      PORTC->PCR[3]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else //pcs1_pin == PTD4
    {
      PORTD->PCR[4] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS2
    if(pcs2_pin == PTC2)
    {
      PORTC->PCR[2]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else //pcs2_pin == PTD5
    {
      PORTD->PCR[5]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS3
    if(pcs3_pin == PTC1)
    {
      PORTC->PCR[1]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else //pcs3_pin == PTD6
    {
      PORTD->PCR[6]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS4
    if(pcs4_pin == PTC0)
    {
      PORTC->PCR[0]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS5
    if(pcs5_pin == PTB23)
    {
      PORTB->PCR[23] = 0 | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
    }

    //选择SCK
    if(sck_pin == PTA15)
    {
      PORTA->PCR[15] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SCK
    }
    else //sck_pin == PTC5
    {
      PORTC->PCR[5] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SCK
    }
    //选择MOSI
    if(mosi_pin == PTA16)
    {
      PORTA->PCR[16] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SOUT
    }
    else //mosi_pin == PTC6
    {
      PORTC->PCR[6] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SOUT
    }
    //选择MISO
    if(miso_pin == PTA17)
    {
      PORTA->PCR[17] = 0 | PORT_PCR_MUX(2); //SIN
    }
    else //miso_pin == PTC7
    {
      PORTC->PCR[7] = 0 | PORT_PCR_MUX(2); //SIN
    } 
  
  }
  else if(spix == SPI1)
  {
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK; 
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC6 |= SIM_SCGC6_DSPI1_MASK; 
#endif
    //选择PCS0
    if(pcs0_pin == PTB10)
    {
      PORTB->PCR[10] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else//pcs0_pin == PTE4
    {
      PORTE->PCR[4] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS1
    if(pcs1_pin == PTB9)
    {
      PORTB->PCR[9]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else //pcs1_pin == PTE0
    {
      PORTE->PCR[0] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS2
    if(pcs2_pin == PTE5)
    {
      PORTE->PCR[5]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }

    //选择PCS3
    if(pcs3_pin == PTE6)
    {
      PORTE->PCR[6]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }

    //选择SCK
    if(sck_pin == PTB11)
    {
      PORTB->PCR[11] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else //sck_pin == PTE2
    {
      PORTE->PCR[2] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SCK
    }
    //选择MOSI
    if(mosi_pin == PTB16)
    {
      PORTB->PCR[16] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SOUT
    }
    else //mosi_pin == PTE1
    {
      PORTE->PCR[1] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SOUT
    }
    //选择MISO
    if(miso_pin == PTB17)
    {
      PORTB->PCR[17] = 0 | PORT_PCR_MUX(2); //SIN
    }
    else //miso_pin == PTE3
    {
      PORTE->PCR[3] = 0 | PORT_PCR_MUX(2); //SIN
    } 
  }
  else if(spix == SPI2)
  {
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC3 |= SIM_SCGC3_SPI2_MASK; 
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC3 |= SIM_SCGC3_DSPI2_MASK; 
#endif
    //选择PCS0
    if(pcs0_pin == PTD11)
    {
      PORTD->PCR[11] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else//pcs0_pin == PTB20
    {
      PORTB->PCR[20] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    //选择PCS1
    if(pcs1_pin == PTD15)
    {
      PORTD->PCR[15]  = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }

    //选择SCK
    if(sck_pin == PTD12)
    {
      PORTD->PCR[12] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;
    }
    else //sck_pin == PTB21
    {
      PORTB->PCR[21] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SCK
    }
    //选择MOSI
    if(mosi_pin == PTD13)
    {
      PORTD->PCR[13] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SOUT
    }
    else //mosi_pin == PTB22
    {
      PORTB->PCR[22] = 0 | PORT_PCR_MUX(2) | PORT_PCR_DSE_MASK;//SOUT
    }
    //选择MISO
    if(miso_pin == PTD14)
    {
      PORTD->PCR[14] = 0 | PORT_PCR_MUX(2); //SIN
    }
    else //miso_pin == PTB23
    {
      PORTB->PCR[23] = 0 | PORT_PCR_MUX(2); //SIN
    } 
  }
  else
    return 0;
  //配置SPI MCR寄存器
  spix->MCR = 0 & (~SPI_MCR_MDIS_MASK)        
                |SPI_MCR_HALT_MASK        
                |SPI_MCR_PCSIS_MASK       
                |SPI_MCR_CLR_TXF_MASK    
                |SPI_MCR_CLR_RXF_MASK;  
  //选择SPI 工作模式
  if(spi_mode == SPI_MODE_SLAVE)
  {
    spix->MCR &= ~SPI_MCR_MSTR_MASK;//从机模式  
  }
  else
  {
    spix->MCR |= SPI_MCR_MSTR_MASK; //主机模式 
  }
  //选择使能tx FIFO
  if(txFIFO_enable == TRUE)
  {
    spix->MCR &= ~SPI_MCR_DIS_TXF_MASK; 
  }
  else
  {
    spix->MCR |= SPI_MCR_DIS_TXF_MASK;//选择传统方式
  }
  //选择使能Rx FIFO
  if(rxFIFO_enable == TRUE)
  {
    spix->MCR &= ~SPI_MCR_DIS_RXF_MASK;
  }
  else
  {
    spix->MCR |= SPI_MCR_DIS_RXF_MASK; //选择传统方式
  }
  //选择使能发送完成中断
  if(tx_complete_int == TRUE)
  {
    spix->RSER |= SPI_RSER_TCF_RE_MASK; 
  }
  else
  { 
    spix->RSER &= ~SPI_RSER_TCF_RE_MASK;
  }
  //选择使能发送队列结束中断
  if(QueueEnd_Request_int == TRUE)
  {
    spix->RSER |=SPI_RSER_EOQF_RE_MASK;
  }
  else
    spix->RSER &= ~SPI_RSER_EOQF_RE_MASK;
  //选择使能txFIFO为空中断
  if(txFIFO_underflow_int== TRUE)
  {
    spix->RSER |=SPI_RSER_TFUF_RE_MASK;
  }
  else
    spix->RSER &= ~SPI_RSER_TFUF_RE_MASK;
  //选择使能rxFIFO溢出中断
  if(rxFIFO_overflow_int== TRUE)
  {
    spix->RSER |=SPI_RSER_RFOF_RE_MASK;
  }
  else
  {
    spix->RSER &= ~SPI_RSER_RFOF_RE_MASK;
  }
  //选择使能txFIFO有数据进入队列中断或者DMA请求
  if(txFIFO_Fill_int== TRUE)
  {
    spix->RSER |=SPI_RSER_TFFF_RE_MASK;
  }
  else
  {
    spix->RSER &= ~SPI_RSER_TFFF_RE_MASK;
  }
  //选择使能rxFIFO非空中断或者DMA请求
  if(rxFIFO_Drain_int== TRUE)
  {
    spix->RSER |=SPI_RSER_RFDF_RE_MASK;
  }
  else
  {
    spix->RSER &= ~SPI_RSER_RFDF_RE_MASK;
  }
  //选择使能txFIFO中断或者DMA请求
  if(txFIFO_req == SPI_FIFO_DMAREQUEST)
  {
    spix->RSER |=SPI_RSER_TFFF_DIRS_MASK;
  }
  else
  {
    spix->RSER &= ~SPI_RSER_TFFF_DIRS_MASK;
  }
  //选择使能rxFIFO中断或者DMA请求
  if(rxFIFO_req == SPI_FIFO_DMAREQUEST)
  {
    spix->RSER |= SPI_RSER_RFDF_DIRS_MASK;
  }
  else 
  {
    spix->RSER &= ~SPI_RSER_RFDF_DIRS_MASK;
  }
  //添加中断回调函数

  if(spix == SPI0)
  {
    if(tx_complete_int == TRUE)
    { SPI0_ISR[SPI_TxComplete_Int] = TxComplete_isr; }
    if(QueueEnd_Request_int == TRUE)
    { SPI0_ISR[SPI_QueueEndReq_Int] = QueueEndReq_isr; }
    if(txFIFO_underflow_int == TRUE)
    { SPI0_ISR[SPI_TxFIFO_UnderflowInt] = UnderflowInt_isr; }
    if(rxFIFO_overflow_int == TRUE)
    { SPI0_ISR[SPI_RxFIFO_OverflowInt] = OverflowInt_isr; }
    if(txFIFO_Fill_int == TRUE && txFIFO_req == SPI_FIFO_INTREQUEST)
    { SPI0_ISR[SPI_TxFIFO_FillInt] = FillInt_isr; }
    if(rxFIFO_Drain_int == TRUE && rxFIFO_req == SPI_FIFO_INTREQUEST)
    { SPI0_ISR[SPI_RxFIFO_DrainInt] = DrainInt_isr; }
  }
  else if (spix == SPI1)
  {
    if(tx_complete_int == TRUE)
    { SPI1_ISR[SPI_TxComplete_Int] = TxComplete_isr; }
    if(QueueEnd_Request_int == TRUE)
    { SPI1_ISR[SPI_QueueEndReq_Int] = QueueEndReq_isr; }
    if(txFIFO_underflow_int == TRUE)
    { SPI1_ISR[SPI_TxFIFO_UnderflowInt] = UnderflowInt_isr; }
    if(rxFIFO_overflow_int == TRUE)
    { SPI1_ISR[SPI_RxFIFO_OverflowInt] = OverflowInt_isr; }
    if(txFIFO_Fill_int == TRUE && txFIFO_req == SPI_FIFO_INTREQUEST)
    { SPI1_ISR[SPI_TxFIFO_FillInt] = FillInt_isr; }
    if(rxFIFO_Drain_int == TRUE && rxFIFO_req == SPI_FIFO_INTREQUEST)
    { SPI1_ISR[SPI_RxFIFO_DrainInt] = DrainInt_isr; }
  }
  else if (spix == SPI2)
  {
    if(tx_complete_int == TRUE)
    { SPI2_ISR[SPI_TxComplete_Int] = TxComplete_isr; }
    if(QueueEnd_Request_int == TRUE)
    { SPI2_ISR[SPI_QueueEndReq_Int] = QueueEndReq_isr; }
    if(txFIFO_underflow_int == TRUE)
    { SPI2_ISR[SPI_TxFIFO_UnderflowInt] = UnderflowInt_isr; }
    if(rxFIFO_overflow_int == TRUE)
    { SPI2_ISR[SPI_RxFIFO_OverflowInt] = OverflowInt_isr; }
    if(txFIFO_Fill_int == TRUE && txFIFO_req == SPI_FIFO_INTREQUEST)
    { SPI2_ISR[SPI_TxFIFO_FillInt] = FillInt_isr; }
    if(rxFIFO_Drain_int == TRUE && rxFIFO_req == SPI_FIFO_INTREQUEST)
    { SPI2_ISR[SPI_RxFIFO_DrainInt] = DrainInt_isr; }
  }
  //配置SPI CTAR寄存器，设置SPI的总线时序
  spix->CTAR[0] = 0 & (~SPI_CTAR_LSBFE_MASK)
                      |SPI_CTAR_DBR_MASK  
                      |SPI_CTAR_PBR(0)           
                      |SPI_CTAR_FMSZ(7);        
                       
  //设置SPI总线频率
  //SCK总线频率 = g_bus_clock/ SCK_DIV_x                                             
  spix->CTAR[0] |=SPI_CTAR_BR(sck_div);

  //tCSC = (1/g_bus_clock) x PCSSCK x CSSCK
  //tCSC = 1/50,000,000 x PCSSCK x CSSCK
  spix->CTAR[0] |=SPI_CTAR_PCSSCK(1); 
  spix->CTAR[0] |=SPI_CTAR_CSSCK(1);  
  
  //tDT = (1/g_bus_clock) x PDT x DT
  spix->CTAR[0] |=SPI_CTAR_DT(1);
  spix->CTAR[0] |=SPI_CTAR_PDT(1);
  
  //Config the Delay of the last edge of SCK and the negation of PCS
  //tASC = (1/g_bus_clock) x PASC x ASC
  spix->CTAR[0] |=SPI_CTAR_PASC(1);
  spix->CTAR[0] |=SPI_CTAR_ASC(1);
  //清除标志位
  spix->SR =  SPI_SR_RFDF_MASK   
              |SPI_SR_RFOF_MASK
              |SPI_SR_TFFF_MASK
              |SPI_SR_TFUF_MASK
              |SPI_SR_TCF_MASK
              |SPI_SR_EOQF_MASK;
  //使能SPIx
  spix->MCR &=~SPI_MCR_HALT_MASK; 
  
  return 1;
}

/*
 * LPLD_SPI_Deinit
 * SPI反初始化函数,在该函数中将SPI设置成为暂停模式，关闭SPI总线时钟，
 * 关闭SPI外设中断
 * 
 * 参数:
 *    spi_init_structure--SPI初始化结构体，
 *                        具体定义见SPI_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_SPI_Deinit(SPI_InitTypeDef spi_init_structure)
{
  SPI_Type * spix = spi_init_structure.SPI_SPIx;
  spix->MCR |= SPI_MCR_HALT_MASK; //
  if(spix == SPI0)
  {
    disable_irq(SPI0_IRQn);
    SIM->SCGC6 &= ~SIM_SCGC6_DSPI0_MASK;
  }
  else if(spix == SPI1)
  {
    disable_irq(SPI1_IRQn);
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC6 &= ~SIM_SCGC6_SPI1_MASK; 
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC6 &= ~SIM_SCGC6_DSPI1_MASK; 
#endif
  }
  else if(spix == SPI2)
  {
    disable_irq(SPI2_IRQn);
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC3 &= ~SIM_SCGC3_SPI2_MASK; 
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC3 &= ~SIM_SCGC3_DSPI2_MASK; 
#endif
  }
  else
    return 0;
  return 1;
}

/*
 * LPLD_SPI_EnableIrq
 * 使能SPI外设中断
 * 
 * 参数:
 *    spi_init_structure--SPI初始化结构体，
 *                        具体定义见SPI_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_SPI_EnableIrq(SPI_InitTypeDef spi_init_structure)
{
  SPI_Type * spix = spi_init_structure.SPI_SPIx;
  if(spix == SPI0)
  {
    enable_irq(SPI0_IRQn);
  }
  else if(spix == SPI1)
  {
    enable_irq(SPI1_IRQn);
  }
  else if(spix == SPI2)
  {
    enable_irq(SPI2_IRQn);
  }
  else
    return 0;

  return 1;
}

/*
 * LPLD_SPI_EnableIrq
 * 禁止SPI外设中断
 * 
 * 参数:
 *    spi_init_structure--SPI初始化结构体，
 *                        具体定义见SPI_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_SPI_DisableIrq(SPI_InitTypeDef spi_init_structure)
{
  SPI_Type * spix = spi_init_structure.SPI_SPIx;
  if(spix == SPI0)
  {
    disable_irq(SPI0_IRQn);
  }
  else if(spix == SPI1)
  {
    disable_irq(SPI1_IRQn);
  }
  else if(spix == SPI2)
  {
    disable_irq(SPI2_IRQn);
  }
  else
    return 0;
  return 1;
}

/*
 * LPLD_SPI_Master_WriteRead
 * K60主机SPI向从机写数据，并读取从机数据
 * 
 * 参数:
 *    spix--SPI选择
 *      |__SPI0 -选择SPI0模块
 *      |__SPI1 -选择SPI1模块
 *      |__SPI2 -选择SPI2模块
 *    data--要发送数据
 *      |__单位为一个字节，8位
 *    pcsx--CS片选端口号
 *      |__SPI_PCS0 -0号片选(SPI0、SPI1、SPI2含有)
 *      |__SPI_PCS1 -1号片选(SPI0、SPI1、SPI2含有)
 *      |__SPI_PCS2 -2号片选(SPI0、SPI1含有)
 *      |__SPI_PCS3 -3号片选(SPI0、SPI1含有)
 *      |__SPI_PCS4 -4号片选(SPI0含有)
 *      |__SPI_PCS5 -5号片选(SPI0含有)
 *    pcs_state--一帧数据传输完成后CS的状态
 *      |__SPI_PCS_ASSERTED -保持片选有效,PCS信号保持为低电平
 *      |__SPI_PCS_INACTIVE -片选无效,PCS信号变为高电平
 * 输出:
 *    读取从机8位的数据
 */
uint8 LPLD_SPI_Master_WriteRead(SPI_Type *spix,uint8 data,uint8 pcsx,uint8 pcs_state)
{
  uint8 temp;
  
  spix->PUSHR  = (((uint32_t)(((uint32_t)(pcs_state))<<SPI_PUSHR_CONT_SHIFT))&SPI_PUSHR_CONT_MASK)
               |SPI_PUSHR_CTAS(0)
               |SPI_PUSHR_PCS(pcsx)
               |data;                 
  
  while(!(spix->SR & SPI_SR_TCF_MASK));
  spix->SR |= SPI_SR_TCF_MASK ;               
  
  while(!(spix->SR & SPI_SR_RFDF_MASK)); 
  temp = (uint8)(spix->POPR & 0xff);           
  spix->SR |= SPI_SR_RFDF_MASK;                
  return temp;
  
}

/*
 * LPLD_SPI_Master_Read
 * K60主机读取从机数据
 * 
 * 参数:
 *    spix--SPI选择
 *      |__SPI0 -选择SPI0模块
 *      |__SPI1 -选择SPI1模块
 *      |__SPI2 -选择SPI2模块
 * 输出:
 *    读取从机8位的数据
 */
uint8 LPLD_SPI_Master_Read(SPI_Type *spix)
{
  uint8 temp;

  while(!(spix->SR & SPI_SR_RFDF_MASK)); //=1 RIFO is not empty
  temp=(uint8)spix->POPR;
  spix->SR |=SPI_SR_RFDF_MASK;

  return temp;
}
/*
 * LPLD_SPI_Master_Write
 * K60主机SPI向从机写数据
 * 
 * 参数:
 *    spix--SPI选择
 *      |__SPI0 -选择SPI0模块
 *      |__SPI1 -选择SPI1模块
 *      |__SPI2 -选择SPI2模块
 *    data--要发送数据
 *      |__单位为一个字节，8位
 *    pcsx--CS片选端口号
 *      |__SPI_PCS0  -0号片选(SPI0、SPI1、SPI2含有)
 *      |__SPI_PCS1  -1号片选(SPI0、SPI1、SPI2含有)
 *      |__SPI_PCS2  -2号片选(SPI0、SPI1含有)
 *      |__SPI_PCS3  -3号片选(SPI0、SPI1含有)
 *      |__SPI_PCS4  -4号片选(SPI0含有)
 *      |__SPI_PCS5  -5号片选(SPI0含有)
 *    pcs_state--一帧数据传输完成后CS的状态
 *      |__SPI_PCS_ASSERTED -保持片选有效,PCS信号保持为低电平
 *      |__SPI_PCS_INACTIVE -片选无效,PCS信号变为高电平
 */
void LPLD_SPI_Master_Write(SPI_Type *spix,uint8 data,uint8 pcsx,uint8 pcs_state)
{  
  
  spix->PUSHR = (((uint32_t)(((uint32_t)(pcs_state))<<SPI_PUSHR_CONT_SHIFT)) & SPI_PUSHR_CONT_MASK)
               |SPI_PUSHR_CTAS(0)
               |SPI_PUSHR_PCS(pcsx)
               |data; 

  while(!(spix->SR & SPI_SR_TCF_MASK));
  spix->SR |= SPI_SR_TCF_MASK ;            
}


void SPI0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); 
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  if( SPI0->SR & SPI_SR_TCF_MASK)
  {
    SPI0_ISR[SPI_TxComplete_Int]();
    SPI0->SR |= SPI_SR_TCF_MASK;
  }
  else if( SPI0->SR & SPI_SR_EOQF_MASK)
  {
    SPI0_ISR[SPI_QueueEndReq_Int]();
    SPI0->SR |= SPI_SR_EOQF_MASK;
  }
  else if( SPI0->SR & SPI_SR_TFUF_MASK)
  {
    SPI0_ISR[SPI_TxFIFO_UnderflowInt]();
    SPI0->SR |= SPI_SR_TFUF_MASK;
  }
  else if( SPI0->SR & SPI_SR_TFFF_MASK)
  {  
    SPI0_ISR[SPI_TxFIFO_FillInt]();    
    SPI0->SR |= SPI_SR_TFFF_MASK;
  }
  else if( SPI0->SR & SPI_SR_RFOF_MASK)
  {   
    SPI0_ISR[SPI_RxFIFO_OverflowInt]();
    SPI0->SR |= SPI_SR_RFOF_MASK;
  }
  else if( SPI0->SR & SPI_SR_RFDF_MASK)
  {
    SPI0_ISR[SPI_RxFIFO_DrainInt]();
    SPI0->SR |= SPI_SR_RFDF_MASK;
  }
#if (UCOS_II > 0u)
  OSIntExit();          
#endif
}


void SPI1_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); 
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  if( SPI1->SR & SPI_SR_TCF_MASK)
  {
    SPI1_ISR[SPI_TxComplete_Int]();
    SPI1->SR |= SPI_SR_TCF_MASK;
  }
  else if( SPI1->SR & SPI_SR_EOQF_MASK)
  {
    SPI1_ISR[SPI_QueueEndReq_Int]();
    SPI1->SR |= SPI_SR_EOQF_MASK;
  }
  else if( SPI1->SR & SPI_SR_TFUF_MASK)
  {
    SPI1_ISR[SPI_TxFIFO_UnderflowInt]();
    SPI1->SR |= SPI_SR_TFUF_MASK;
  }
  else if( SPI1->SR & SPI_SR_TFFF_MASK)
  {  
    SPI1_ISR[SPI_TxFIFO_FillInt]();    
    SPI1->SR |= SPI_SR_TFFF_MASK;
  }
  else if( SPI1->SR & SPI_SR_RFOF_MASK)
  {   
    SPI1_ISR[SPI_RxFIFO_OverflowInt]();
    SPI1->SR |= SPI_SR_RFOF_MASK;
  }
  else if( SPI1->SR & SPI_SR_RFDF_MASK)
  {
    SPI1_ISR[SPI_RxFIFO_DrainInt]();
    SPI1->SR |= SPI_SR_RFDF_MASK;
  }
#if (UCOS_II > 0u)
  OSIntExit();         
#endif
}


void SPI2_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); 
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  if( SPI2->SR & SPI_SR_TCF_MASK)
  {
    SPI2_ISR[SPI_TxComplete_Int]();
    SPI2->SR |= SPI_SR_TCF_MASK;
  }
  else if( SPI2->SR & SPI_SR_EOQF_MASK)
  {
    SPI2_ISR[SPI_QueueEndReq_Int]();
    SPI2->SR |= SPI_SR_EOQF_MASK;
  }
  else if( SPI2->SR & SPI_SR_TFUF_MASK)
  {
    SPI2_ISR[SPI_TxFIFO_UnderflowInt]();
    SPI2->SR |= SPI_SR_TFUF_MASK;
  }
  else if( SPI2->SR & SPI_SR_TFFF_MASK)
  {  
    SPI2_ISR[SPI_TxFIFO_FillInt]();    
    SPI2->SR |= SPI_SR_TFFF_MASK;
  }
  else if( SPI2->SR & SPI_SR_RFOF_MASK)
  {   
    SPI2_ISR[SPI_RxFIFO_OverflowInt]();
    SPI2->SR |= SPI_SR_RFOF_MASK;
  }
  else if( SPI2->SR & SPI_SR_RFDF_MASK)
  {
    SPI2_ISR[SPI_RxFIFO_DrainInt]();
    SPI2->SR |= SPI_SR_RFDF_MASK;
  }
#if (UCOS_II > 0u)
  OSIntExit();          
#endif
}





