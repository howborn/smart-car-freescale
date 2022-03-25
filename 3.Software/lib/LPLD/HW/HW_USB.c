/**
 * @file HW_USB.c
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
#include "common.h"     /* Common definitions */
#include "HW_USB.h"
#include "usb_driver.h"

//用户自定义中断服务函数数组
USB_ISR_CALLBACK USB_ISR[1];

/*
* LPLD_USB_Init
* K60 USB外设初始化函数
* 在该函数中,初始化USB外设时钟,设置USB外设的中断服务函数,
* 并且调用USB设备初始化函数
*/

void LPLD_USB_Init(void)
{ 
  MPU->CESR=0; //MPU 配置
#if (defined(CPU_MK60DZ10))  
  SIM->SOPT2 |= SIM_SOPT2_USBSRC_MASK |   //选择PLL/FLL作为时钟源
               SIM_SOPT2_PLLFLLSEL_MASK; //用PLL作为USB的时钟源，此时的PLL的频率为96Mhz 
#if(CORE_CLK_MHZ == PLL_96)
  SIM->CLKDIV2 = 0x02;                    //设置分频系数USB时钟 = 96Mhz/2 =48Mhz
#endif 
  SIM->SCGC4 |= SIM_SCGC4_USBOTG_MASK;  //使能USB外设时钟
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
  SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1)      // 选择 PLL0 作为参考   
            |  SIM_SOPT2_USBFSRC(0)         // 选择 MCGPLLCLK 作为 CLKC 参考源 
            |  SIM_SOPT2_USBF_CLKSEL_MASK;  // 选择 USB fractional divider 座位 USB reference clock 参考源 
#if(CORE_CLK_MHZ == PLL_120)
// MCGPLLCLK =  120MHz 
// USB Clock = PLL0 x (FRAC +1) / (DIV+1)    
// USB Clock = 120MHz x (1+1) / (4+1) = 48 MHz   
  SIM->CLKDIV2 = SIM_CLKDIV2_USBFSFRAC_MASK | SIM_CLKDIV2_USBFSDIV(4);
#endif
  SIM->SCGC4 |= SIM_SCGC4_USBFS_MASK;   //使能USB外设时钟 
#endif 
  USB0->USBTRC0 = 0x40;                   //按照文档所述，此位必须设置为1
  SIM->SOPT1 |= SIM_SOPT1_USBREGEN_MASK;  //配置USB设备稳压源
  
  /* NVIC模块配置 */
  enable_irq(USB0_IRQn);                 //使能NVIC中的USB OTG中断
  USB_ISR[0] = USB_DEVICE_ISR;           //将USB协议处理函数添加到中断处理函数数组中
  
#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_CDC)
  USB_CDC_Init();                        //初始化CDC类
#elif(USB_DEVICE_CLASS == USB_DEVICE_CLASS_HID)
  USB_HID_Mouse_Init();                  //初始化HID类
#endif
  
  
}

/*
* LPLD_USB_DeInit
* K60 USB模块注销函数
* 注销所选择的USB设备
*/

void LPLD_USB_DeInit(void)
{ 
#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_CDC)
  USB_Class_CDC_DeInit(CONTROLLER_ID);                        //注销CDC类
#elif(USB_DEVICE_CLASS == USB_DEVICE_CLASS_HID)
  USB_Class_HID_DeInit(CONTROLLER_ID);                        //注销HID类
#endif
}

#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_CDC)
/*
*  LPLD_USB_VirtualCom_Rx
*  将USB CDC类模式虚拟成串口模式，该函数是串口接收函数
*  参数：
*      *rx_buf--指向用户数据存储区，用于储存接收到的数据
*  输出：接收数据的长度，以字节为单位
*/
uint8 LPLD_USB_VirtualCom_Rx(uint8 *rx_buf)
{
  memcpy(rx_buf,tUSB_Rev_Data.buffer,tUSB_Rev_Data.len);
  return tUSB_Rev_Data.len;
}

/*
*  LPLD_USB_VirtualCom_Tx
*  将USB CDC类模式虚拟成串口模式，该函数是串口发送函数
*  参数：
*      *tx_buf -- 指向用户数据存储区，用于储存要发送的数据
*      len -- 要发送的数据长度
*  输出：0x00   发送成功
*        非0x00 发送失败
*/
uint8 LPLD_USB_VirtualCom_Tx(uint8 *tx_buf,uint8 len)
{
  uint8 status = 0;
  status = USB_Class_CDC_Interface_DIC_Send_Data(CONTROLLER_ID,tx_buf,len);
  return status;
}

/*
*  LPLD_USB_SetRevIsr
*  添加USB 虚拟串口接收数据中断服务函数
*  无
*  无
*/
void LPLD_USB_SetRevIsr(USB_REV_ISR_CALLBACK isr)
{
  USB_Rev_SetIsr(isr);
}

/*
*  LPLD_USB_QueueData
*  从USB的内存中恢复接收到的数据到用户内存中
*  接收成功后调用此函数
*  无
*  无
*/
void LPLD_USB_QueueData(void)
{
  (void)USB_Class_CDC_Interface_DIC_Recv_Data(CONTROLLER_ID, NULL, 0);
}
#endif

#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_HID)
/*
*  LPLD_USB_HID_MouseControl
*  HID鼠标控制函数
*  参数
*    uint8 *buf -- 数组包含4个字节控制命令
*      第一个字节控制鼠标左右键
*        取值：MOUSE_LEFT_CLICK  左键
*              MOUSE_RIGHT_CLICK 右键
*      第二个字节控制X轴移动
*        取值：MOUSE_LEFT_MOVE   左移
*              MOUSE_RIGHT_MOVE  右移
*      第三个字节控制Y轴移动
*        取值：MOUSE_UP_MOVE    上移
*              MOUSE_DOWN_MOVE  下移
*  输出：
*    无
*/
void LPLD_USB_HID_MouseControl(uint8 *buf)
{
  USB_HID_MouseControl(buf);
}

/*
*  LPLD_USB_HID_LoopTask
*  在main loop中循环调度鼠标检测
*  参数
*    uint8 *buf -- 数组包含4个字节控制命令
*      第一个字节控制鼠标左右键
*        取值：MOUSE_LEFT_CLICK  左键
*              MOUSE_RIGHT_CLICK 右键
*      第二个字节控制X轴移动
*        取值：MOUSE_LEFT_MOVE   左移
*              MOUSE_RIGHT_MOVE  右移
*      第三个字节控制Y轴移动
*        取值：MOUSE_UP_MOVE    上移
*              MOUSE_DOWN_MOVE  下移
*  输出：
*    无
*/
void LPLD_USB_HID_LoopTask(uint8 *buf)
{
  if(USB_HID_Mouse_Task())
  {
    LPLD_USB_HID_MouseControl(buf);
  }
}
#endif
/*
*  LPLD_USB_Isr
*  USB中断处理函数
*/
void USB_OTG_IRQHandler(void)
{
  //调用用户自定义中断服务
  USB_ISR[0]();  
}