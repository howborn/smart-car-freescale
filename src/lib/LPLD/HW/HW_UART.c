/**
 * @file HW_UART.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief UART底层模块相关函数
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
#include "HW_UART.h"

//用户接收中断函数入口地址数组
UART_ISR_CALLBACK UART_R_ISR[6];
//用户发送中断函数入口地址数组
UART_ISR_CALLBACK UART_T_ISR[6];  


/*
 * LPLD_UART_Init
 * 初始化UART通道、波特率、发送接收引脚
 * 
 * 参数:
 *    uart_init_structure--UART初始化结构体，
 *                        具体定义见UART_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_Init(UART_InitTypeDef uart_init_structure)
{
  register uint16 sbr, brfa;
  uint32 sysclk;
  uint8 temp, x;
  UART_Type *uartx = uart_init_structure.UART_Uartx;
  uint32 baud = uart_init_structure.UART_BaudRate;
  PortPinsEnum_Type tx_pin = uart_init_structure.UART_TxPin;
  PortPinsEnum_Type rx_pin = uart_init_structure.UART_RxPin;
  UART_ISR_CALLBACK rx_isr = uart_init_structure.UART_RxIsr;
  UART_ISR_CALLBACK tx_isr = uart_init_structure.UART_TxIsr;
  
  if(baud == NULL)
  {
    baud = 9600;
  }
  
  //使能选中的UART串口通道时钟，相应GPIO的UART复用功能   
  if(uartx == UART0)
  {
    x = 0;
    sysclk = g_core_clock;
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    if(tx_pin == PTA2)
      PORTA->PCR[2] = PORT_PCR_MUX(2); 
    else if(tx_pin == PTA14)
      PORTA->PCR[14] = PORT_PCR_MUX(3); 
    else
      PORTB->PCR[17] = PORT_PCR_MUX(3); 
    
    if(rx_pin == PTA1)
      PORTA->PCR[1] = PORT_PCR_MUX(2); 
    else if(rx_pin == PTA15)
      PORTA->PCR[15] = PORT_PCR_MUX(3); 
    else
      PORTB->PCR[16] = PORT_PCR_MUX(3); 
  }
  else
  {
    if (uartx == UART1)
    {
      x = 1;
      sysclk = g_core_clock;
      SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
      
      if(tx_pin == PTE0)   
        PORTE->PCR[0] = PORT_PCR_MUX(3); 
      else
        PORTC->PCR[4] = PORT_PCR_MUX(3); 
      
      if(rx_pin == PTE1)
        PORTE->PCR[1] = PORT_PCR_MUX(3); 
      else
        PORTC->PCR[3] = PORT_PCR_MUX(3); 
    }
    else
    {
      sysclk = g_bus_clock;
      if (uartx == UART2)
      {
        x = 2;
        SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
        
        PORTD->PCR[3] = PORT_PCR_MUX(3); 
        PORTD->PCR[2] = PORT_PCR_MUX(3);
      }
      else
      {
        if(uartx == UART3)
        {
          x = 3;
          SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
          
          if(tx_pin == PTE4)
            PORTE->PCR[4] = PORT_PCR_MUX(3); 
          else if(tx_pin == PTB11)
            PORTB->PCR[11] = PORT_PCR_MUX(3); 
          else 
            PORTC->PCR[17] = PORT_PCR_MUX(3); 
          
          if(rx_pin == PTE5)
            PORTE->PCR[5] = PORT_PCR_MUX(3); 
          else if(rx_pin == PTB10)
            PORTB->PCR[10] = PORT_PCR_MUX(3); 
          else
            PORTC->PCR[16] = PORT_PCR_MUX(3);
        }
        else
        {
          if(uartx == UART4)
          {
            x = 4;
            SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
            
            if(tx_pin == PTE24)
              PORTE->PCR[24] = PORT_PCR_MUX(3); 
            else
              PORTC->PCR[15] = PORT_PCR_MUX(3); 
            
            if(rx_pin == PTE25)
              PORTE->PCR[25] = PORT_PCR_MUX(3); 
            else
              PORTC->PCR[14] = PORT_PCR_MUX(3);
          }
          else
          {
            x = 5;
            uartx = UART5;
            SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
            
            if(tx_pin == PTD9)
              PORTD->PCR[9] = PORT_PCR_MUX(3); 
            else
              PORTE->PCR[8] = PORT_PCR_MUX(3); 
            
            if(rx_pin == PTD8)
              PORTD->PCR[8] = PORT_PCR_MUX(3); 
            else
              PORTE->PCR[9] = PORT_PCR_MUX(3); 
          }
        }
      }
    }
  }
  
  //在配置好其他寄存器前，先关闭发送器和接收器
  uartx->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
  
  //配置UART为 8位, 无奇偶校验 */
  uartx->C1 = 0;	
  
  //计算波特率
  sbr = (uint16)((sysclk)/(baud * 16));
  
  //保存UARTx_BDH寄存器中除了SBR的值
  temp = uartx->BDH & ~(UART_BDH_SBR(0x1F));
  
  uartx->BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
  uartx->BDL = (uint8)(sbr & UART_BDL_SBR_MASK);
  
  //配置波特率的微调分数
  brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
  
  //保存UARTx_C4寄存器中除了BRFA的值
  temp = uartx->C4 & ~(UART_C4_BRFA(0x1F));
  
  uartx->C4 = temp |  UART_C4_BRFA(brfa);    
  
  //配置发送接收中断
  if(uart_init_structure.UART_RxIntEnable == TRUE && rx_isr != NULL)
  {
    uartx->C2 |= UART_C2_RIE_MASK; 
    UART_R_ISR[x] = rx_isr;
  } 
  else
  {
    uartx->C2 &= ~(UART_C2_RIE_MASK); 
  }
  if(uart_init_structure.UART_TxIntEnable == TRUE && tx_isr != NULL)
  {
    uartx->C2 |= UART_C2_TIE_MASK; 
    UART_T_ISR[x] = tx_isr;
  } 
  else
  {
    uartx->C2 &= ~(UART_C2_TIE_MASK); 
  }
  
  //使能发送器和接收器
  uartx->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );    
}

/*
 * LPLD_UART_GetChar
 * 串口查询方式读取一个字节
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *
 * 输出:
 *    串口接收的1个字节
 *
 */
int8 LPLD_UART_GetChar(UART_Type *uartx)
{
  //等待数据接收
  while (!(uartx->S1 & UART_S1_RDRF_MASK));
  
  //返回接收的1个字节数据
  return uartx->D;
}

/********************************************************************/
/*
 * LPLD_UART_GetChar_Present
 * 检查是否接收到字符
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *
 * 输出:
 *  0       没有接收到字符
 *  1       已经接收到字符
 */
int32 LPLD_UART_GetChar_Present(UART_Type *uartx)
{
    return (uartx->S1 & UART_S1_RDRF_MASK);
}

/*
 * LPLD_UART_PutChar
 * 串口查询方式发送一个字节
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *    ch--待发送的1个字节
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_PutChar(UART_Type *uartx, int8 ch)
{
  //等待FIFO准备就绪
  while(!(uartx->S1 & UART_S1_TDRE_MASK));
  
  //将要发送的1个字节发给UART数据寄存器
  uartx->D = (uint8)ch;
}


/*
 * LPLD_UART_PutCharArr
 * 串口查询方式发送字节型数组
 * 
 * 参数:
 *    uartx--UART模块号
 *      |__UART0          --UART0
 *      |__UART1          --UART1
 *      |__UART2          --UART2
 *      |__UART3          --UART3
 *      |__UART4          --UART4
 *      |__UART5          --UART5
 *    *ch--待发送的字节数组头地址
 *    len--字节数组长度
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_PutCharArr(UART_Type *uartx, int8 *ch, int32 len)
{
  while(len--)
  {
    LPLD_UART_PutChar(uartx, *(ch++));
  }
}

/*
 * LPLD_UART_EnableIrq
 * 使能串口数据接收中断
 * 
 * 参数:
 *    uart_init_structure--UART初始化结构体，
 *                        具体定义见UART_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_EnableIrq(UART_InitTypeDef uart_init_structure)
{
  if(uart_init_structure.UART_Uartx == UART0)
  {
    enable_irq(UART0_RX_TX_IRQn);
  }
  else if(uart_init_structure.UART_Uartx == UART1)
  {
    enable_irq(UART1_RX_TX_IRQn);
  }
  else if(uart_init_structure.UART_Uartx == UART2)
  {
    enable_irq(UART2_RX_TX_IRQn);
  }
  else if(uart_init_structure.UART_Uartx == UART3)
  {
    enable_irq(UART3_RX_TX_IRQn);
  }
  else if(uart_init_structure.UART_Uartx == UART4)
  {
    enable_irq(UART4_RX_TX_IRQn);
  }
  else if(uart_init_structure.UART_Uartx == UART5)
  {
    enable_irq(UART5_RX_TX_IRQn);
  }
}

/*
 * LPLD_UART_DisableIrq
 * 禁用串口数据接收中断
 * 
 * 参数:
 *    uart_init_structure--UART初始化结构体，
 *                        具体定义见UART_InitTypeDef
 *
 * 输出:
 *    无
 *
 */
void LPLD_UART_DisableIrq(UART_InitTypeDef uart_init_structure)
{
  //根据中断请求号使能相应中断
  if(uart_init_structure.UART_Uartx == UART0)
    disable_irq(UART0_RX_TX_IRQn);
  else if(uart_init_structure.UART_Uartx == UART1)
    disable_irq(UART1_RX_TX_IRQn);
  else if(uart_init_structure.UART_Uartx == UART2)
    disable_irq(UART2_RX_TX_IRQn);
  else if(uart_init_structure.UART_Uartx == UART3)
    disable_irq(UART3_RX_TX_IRQn);
  else if(uart_init_structure.UART_Uartx == UART4)
    disable_irq(UART4_RX_TX_IRQn);
  else if(uart_init_structure.UART_Uartx == UART5)
    disable_irq(UART5_RX_TX_IRQn);
}

//HW层中断函数，用户无需调用
void UART0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //进入接收中断函数
  if((UART0->S1 & UART_S1_RDRF_MASK) && (UART0->C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[0]();
  }
  //进入发送中断函数
  if((UART0->S1 & UART_S1_TDRE_MASK) && (UART0->C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[0]();
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

//HW层中断函数，用户无需调用
void UART1_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //进入接收中断函数
  if((UART1->S1 & UART_S1_RDRF_MASK) && (UART1->C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[1]();
  }
  //进入发送中断函数
  if((UART1->S1 & UART_S1_TDRE_MASK) && (UART1->C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[1]();
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

//HW层中断函数，用户无需调用
void UART2_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //进入接收中断函数
  if((UART2->S1 & UART_S1_RDRF_MASK) && (UART2->C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[2]();
  }
  //进入发送中断函数
  if((UART2->S1 & UART_S1_TDRE_MASK) && (UART2->C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[2]();
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

//HW层中断函数，用户无需调用
void UART3_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //进入接收中断函数
  if((UART3->S1 & UART_S1_RDRF_MASK) && (UART3->C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[3]();
  }
  //进入发送中断函数
  if((UART3->S1 & UART_S1_TDRE_MASK) && (UART3->C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[3]();
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

//HW层中断函数，用户无需调用
void UART4_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //进入接收中断函数
  if((UART4->S1 & UART_S1_RDRF_MASK) && (UART4->C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[4]();
  }
  //进入发送中断函数
  if((UART4->S1 & UART_S1_TDRE_MASK) && (UART4->C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[4]();
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}

//HW层中断函数，用户无需调用
void UART5_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //进入接收中断函数
  if((UART5->S1 & UART_S1_RDRF_MASK) && (UART5->C2 & UART_C2_RIE_MASK))
  {
    UART_R_ISR[5]();
  }
  //进入发送中断函数
  if((UART5->S1 & UART_S1_TDRE_MASK) && (UART5->C2 & UART_C2_TIE_MASK))
  {
    UART_T_ISR[5]();
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
