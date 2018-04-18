/**
 * @file HW_UART.h
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
#ifndef __HW_UART_H__
#define __HW_UART_H__


//UART模块中断回调函数类型
typedef void (*UART_ISR_CALLBACK)(void);

//UART模块初始化结构体，用于配置UART各项参数
typedef struct
{
  /*
    描述：
      选择UARTx
    取值：
      UART0~UART5
    初始化：
      不必须初始化，默认值UART5
  */
  UART_Type *UART_Uartx;        
      
  /*
    描述：
      设置UARTx波特率
    推荐取值：
      4800、9600、19200、38400、115200
    初始化：
      不必须初始化，默认值9600
  */
  uint32 UART_BaudRate;  
  
  /*
    描述：
      发送引脚，每个UARTx对应不同的TxPin
    取值：
      UART0-PTA2、PTA14、*PTB17
      UART1-PTE0、*PTC4
      UART2-*PTD3
      UART3-PTE4、PTB11、*PTC17
      UART4-PTE24、*PTC15
      UART5-PTD9、*PTE8
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type UART_TxPin;   
  
  /*
    描述：
      接收引脚，每个UARTx对应不同的RxPin
    取值：
      UART0-PTA1、PTA15、*PTB16
      UART1-PTE1、*PTC3
      UART2-*PTD2
      UART3-PTE5、PTB10、*PTC16
      UART4-PTE25、*PTC14
      UART5-PTD8、*PTE9
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type UART_RxPin; 
  
  /*
    描述：
      使能接收中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认值FALSE
  */
  boolean UART_RxIntEnable;
  
  /*
    描述：
      使能发送中断
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认值FALSE
  */
  boolean UART_TxIntEnable;
  
  /*
    描述：
      接收中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  UART_ISR_CALLBACK UART_RxIsr; 
  
  /*
    描述：
      发送中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  UART_ISR_CALLBACK UART_TxIsr; 
} UART_InitTypeDef;


//初始化UART通道、波特率、发送接收引脚
void LPLD_UART_Init(UART_InitTypeDef);
//UART获取一个字符
int8 LPLD_UART_GetChar(UART_Type *);
//检查是否接收到字符
int32 LPLD_UART_GetChar_Present(UART_Type *);
//UART发送一个字符
void LPLD_UART_PutChar(UART_Type *, int8);
//UART发送字符数组
void LPLD_UART_PutCharArr(UART_Type *, int8*, int32);
//UART中断使能
void LPLD_UART_EnableIrq(UART_InitTypeDef);
//UART中断禁用
void LPLD_UART_DisableIrq(UART_InitTypeDef);

#endif