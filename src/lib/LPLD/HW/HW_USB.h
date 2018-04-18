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
#ifndef __HW_USB_H__
#define __HW_USB_H__

/*根据USB2.0标准*/
/*定义USB设备的类型*/
#define USB_DEVICE_CLASS_AUDIO        1
#define USB_DEVICE_CLASS_CDC          2
#define USB_DEVICE_CLASS_HID          3
#define USB_DEVICE_CLASS_PHY          4
#define USB_DEVICE_CLASS_IMAGE        5
#define USB_DEVICE_CLASS_MASS_STORAGE 6
#define USB_DEVICE_CLASS_HUB          7
#define USB_DEVICE_CLASS_CDC_DATA     8
#define USB_DEVICE_CLASS_SMARTCARD    9
//.......
#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_HID)
typedef enum 
{
  MOUSE_BUTTON_CLICK,
  MOUSE_X_MOVEMENT,
  MOUSE_Y_MOVEMENT
}CAN_HID_Mouse_EnumType;

#define  MOUSE_LEFT_CLICK        (0x01) /* Left click */
#define  MOUSE_RIGHT_CLICK       (0x02) /* Right click */
#define  MOUSE_LEFT_MOVE         (0xF8) /* Left movement */
#define  MOUSE_RIGHT_MOVE        (0x08) /* Right movement */
#define  MOUSE_UP_MOVE           (0xF8) /* up movement */
#define  MOUSE_DOWN_MOVE         (0x08) /* down movement */
#endif
//USB模块中断回调函数类型
typedef void (*USB_ISR_CALLBACK)(void);
typedef void (*USB_REV_ISR_CALLBACK)(void);
//USB初始化函数
void LPLD_USB_Init(void);
//USB反初始化函数
void LPLD_USB_DeInit(void);

#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_CDC)
//USB设置 虚拟串口 接收中断服务函数
void LPLD_USB_SetRevIsr(USB_REV_ISR_CALLBACK);
//USB 虚拟串口 接收函数
uint8 LPLD_USB_VirtualCom_Rx(uint8 *rx_buf);
//USB 虚拟串口 发送函数
uint8 LPLD_USB_VirtualCom_Tx(uint8 *tx_buf,uint8_t len);
//USB恢复接收到的数据到用户内存函数
void LPLD_USB_QueueData(void);
#endif

#if(USB_DEVICE_CLASS == USB_DEVICE_CLASS_HID)
//HID鼠标控制函数
void LPLD_USB_HID_MouseControl(uint8 * buf);
//在main loop中循环调度鼠标检测
void LPLD_USB_HID_LoopTask(uint8 * buf);
#endif

#endif