/**
* @file HW_CAN.h
* @version 3.03[By LPLD]
* @date 2014-3-9
* @brief CAN底层模块相关函数
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
#ifndef __HW_CAN_H__
#define __HW_CAN_H__

//声明FlexCAN中断回掉函数
typedef void (*CAN_ISR_CALLBACK)(void);


#define MSG_NUM_0                (0)  //定义Msg缓冲区0
#define MSG_NUM_1                (1)  //定义Msg缓冲区1
#define MSG_NUM_2                (2)  //定义Msg缓冲区2
#define MSG_NUM_3                (3)  //定义Msg缓冲区3
#define MSG_NUM_4                (4)  //定义Msg缓冲区4
#define MSG_NUM_5                (5)  //定义Msg缓冲区5
#define MSG_NUM_6                (6)  //定义Msg缓冲区6
#define MSG_NUM_7                (7)  //定义Msg缓冲区7
#define MSG_NUM_8                (8)  //定义Msg缓冲区8
#define MSG_NUM_9                (9)  //定义Msg缓冲区9
#define MSG_NUM_10               (10) //定义Msg缓冲区10
#define MSG_NUM_11               (11) //定义Msg缓冲区11
#define MSG_NUM_12               (12) //定义Msg缓冲区12
#define MSG_NUM_13               (13) //定义Msg缓冲区13
#define MSG_NUM_14               (14) //定义Msg缓冲区14
#define MSG_NUM_15               (15) //定义Msg缓冲区15

//FlexCAN模块Bit Rate Values
#define CAN_BAUD_RATE_33KBPS    (33333ul)
#define CAN_BAUD_RATE_50KBPS    (50000ul)
#define CAN_BAUD_RATE_83KBPS    (83333ul)
#define CAN_BAUD_RATE_100KBPS   (100000ul)
#define CAN_BAUD_RATE_125KBPS   (125000ul)
#define CAN_BAUD_RATE_250KBPS   (250000ul)
#define CAN_BAUD_RATE_500KBPS   (500000ul)
#define CAN_BAUD_RATE_1MBPS     (1000000ul)

//获得FlexCAN Ctr1寄存器的Prescale段的值，用于设置FlexCAN的通信波特率
#define CAN_GET_PRESCALE( device_freq, desired_baud, time_quanta )\
     (uint8)(( device_freq/(desired_baud * time_quanta)) - 1 )

//选择初始化邮箱的数目，该数目由CAN_MCR_REG中设置Msg多少的相关位决定
#define MSG_MAX_NO    (16)
       
//FlexCAN_MB的CODE位定义
#define CAN_MB_CS_CODE_MASK         (0x0F000000u)
#define CAN_MB_CS_CODE_SHIFT        (24) 
#define CAN_MB_CS_CODE(x)           (((uint32)(((uint32)(x))<<CAN_MB_CS_CODE_SHIFT))&CAN_MB_CS_CODE_MASK)
#define CAN_MB_CS_RTR_MASK          (0x00100000u)
#define CAN_MB_CS_RTR_SHIFT         (20)
#define CAN_MB_CS_RTR(x)            (((uint32)(((uint32)(x))<<CAN_MB_CS_RTR_SHIFT))&CAN_MB_CS_RTR_MASK)
#define CAN_MB_CS_IDE_MASK          (0x00200000u)
#define CAN_MB_CS_IDE_SHIFT         (21)
#define CAN_MB_CS_IDE(x)            (((uint32)(((uint32)(x))<<CAN_MB_CS_IDE_SHIFT))&CAN_MB_CS_IDE_MASK)
#define CAN_MB_CS_SRR_MASK          (0x00400000u)
#define CAN_MB_CS_SRR_SHIFT         (22)
#define CAN_MB_CS_SRR(x)            (((uint32)(((uint32)(x))<<CAN_MB_CS_SRR_SHIFT))&CAN_MB_CS_SRR_MASK) 
#define CAN_MB_CS_DLC_MASK          (0x000F0000u)
#define CAN_MB_CS_DLC_SHIFT         (16)       
#define CAN_MB_CS_DLC(x)            (((uint32)(((uint32)(x))<<CAN_MB_CS_DLC_SHIFT))&CAN_MB_CS_DLC_MASK)
#define CAN_MB_CS_TIMESTAMP_MASK    (0x0000FFFFu)
#define CAN_MB_CS_TIMESTAMP_SHIFT   (0)     
#define CAN_MB_CS_TIMESTAMP(x)      (((uint32)(((uint32)(x))<<CAN_MB_CS_TIMESTAMP_SHIFT))&CAN_MB_CS_TIMESTAMP_MASK)
#define CAN_GET_MB_CS_CODE(x)       (((x) & CAN_MB_CS_CODE_MASK)>>CAN_MB_CS_CODE_SHIFT)
#define CAN_GET_MB_CS_LENGTH(x)     (((x) & CAN_MB_CS_DLC_MASK )>>CAN_MB_CS_DLC_SHIFT)
#define CAN_GET_MB_CS_IDE(x)        (((x) & CAN_MB_CS_IDE_MASK )>>CAN_MB_CS_IDE_SHIFT)
#define CAN_GET_MB_CS_TIMESTAMP(x) (((x) & CAN_MB_CS_TIMESTAMP_MASK )>>CAN_MB_CS_TIMESTAMP_SHIFT)    
//FLEXCAN_MB_ID的位定义和宏
#define CAN_MB_ID_PRIO_MASK         (0xE0000000u)
#define CAN_MB_ID_PRIO_SHIFT        (29)
#define CAN_MB_ID_PRIO(x)           (((uint32)(((uint32)(x))<<CAN_MB_ID_PRIO_SHIFT))&CAN_MB_ID_PRIO_MASK)  
#define CAN_MB_ID_STD_MASK          (0x1FFC0000u)
#define CAN_MB_ID_STD_SHIFT         (18)
#define CAN_MB_ID_STD(x)            (((uint32)(((uint32)(x))<<CAN_MB_ID_STD_SHIFT))&CAN_MB_ID_STD_MASK)
#define CAN_MB_ID_EXT_MASK          (0x1FFFFFFFu)
#define CAN_MB_ID_EXT_SHIFT         (0)
#define CAN_MB_ID_EXT(x)            (((uint32)(((uint32)(x))<<CAN_MB_ID_EXT_SHIFT))&CAN_MB_ID_EXT_MASK)     
#define CAN_GET_MB_ID_STD(x)        (((x) & CAN_MB_ID_STD_MASK )>>CAN_MB_ID_STD_SHIFT)
#define CAN_GET_MB_ID_EXT(x)        (((x) & CAN_MB_ID_EXT_MASK )>>CAN_MB_ID_EXT_SHIFT)
     
//枚举FlexCAN模块邮箱的CODE段的数值
typedef enum CAN_Msg_Code_Tag
{
  CAN_MSGOBJ_RX_INACTIVE      = 0x0,  //@emem RX Inactive
  CAN_MSGOBJ_RX_BUSY          = 0x1,  //@emem RX Busy
  CAN_MSGOBJ_RX_FULL          = 0x2,  //@emem RX FULL
  CAN_MSGOBJ_RX_EMPTY         = 0x4,  //@emem RX Empty
  CAN_MSGOBJ_RX_OVERRUN       = 0x6,  //@emem RX Overrun
  CAN_MSGOBJ_TX_INACTIVE      = 0x8,  //@emem TX Inactive
  CAN_MSGOBJ_TX_REMOTE        = 0xA,  //@emem TX Remote
  CAN_MSGOBJ_TX_ONCE          = 0xC,  //@emem TX Unconditional
  CAN_MSGOBJ_TX_REMOTE_MATCH  = 0xE   //@emem Tx Remote match
} CAN_Msg_CodeEnum_Type ;


//枚举FlexCAN模块中断形式
typedef enum CAN_Int_Tag
{
  CAN_MB_INT,
  CAN_BUS_OFF_INT,
  CAN_ERROR_INT,
  CAN_TRANS_WARNING_INT,
  CAN_RECV_WARNING_INT,
  CAN_WAKEUP_INT,
  CAN_IMEU_INT,
  CAN_LOST_RECV_INT
}CAN_IntEnum_Type;

//枚举FlexCAN模块数据长度
typedef enum CAN_MSGOBJ_Data_Length_Tag
{
  CAN_DATA_0_BYTES = 0,
  CAN_DATA_1_BYTES = 1,
  CAN_DATA_2_BYTES = 2,
  CAN_DATA_3_BYTES = 3,
  CAN_DATA_4_BYTES = 4,
  CAN_DATA_5_BYTES = 5,
  CAN_DATA_6_BYTES = 6,
  CAN_DATA_7_BYTES = 7,
  CAN_DATA_MAX_BYTES = 8
}CAN_MSGOBJ_Data_LengthEnum_Type;

//枚举FlexCAN模块Msg的方向
typedef enum CAN_MSGOBJ_DIR_Tag
{
  CAN_MSGOBJ_DIR_NULL,
  CAN_MSGOBJ_DIR_RX, 
  CAN_MSGOBJ_DIR_TX  
}CAN_MSGOBJ_DIR_T;

//枚举FlexCAN模块Msg的ID形式
typedef enum CAN_MSGOBJ_ID_Tag
{
  CAN_MSGOBJ_ID_STD, 
  CAN_MSGOBJ_ID_EXT 
}CAN_MSGOBJ_IDEnum_Type;

//枚举FlexCAN模块Msg的接受屏蔽方式
typedef enum CAN_MSGOBJ_RxMasking_Tag
{
  CAN_MSGOBJ_INDIVIDUAL_MASKING, 
  CAN_MSGOBJ_GLOBAL_MASKING 
}CAN_MSGOBJ_RxMaskingEnum_Type;

typedef struct 
{
  /*
    描述：
      选择CANx
    取值：
      CAN0
      CAN1
    初始化：
      必须初始化
  */
  CAN_Type *CAN_Canx;
  
  /*
    描述：
      设置FlexCAN波特率
    推荐取值：
      CAN_BAUD_RATE_50KBPS - 波特率50KBPS
      CAN_BAUD_RATE_100KBPS- 波特率100KBPS
      CAN_BAUD_RATE_250KBPS- 波特率250KBPS
      CAN_BAUD_RATE_500KBPS- 波特率500KBPS
      CAN_BAUD_RATE_1MBPS  - 波特率1MKBPS
    初始化：
      必须初始化
  */
  uint32 CAN_BaudRate;
  
  /*
    描述：
      设置FlexCAN Msg接收匹配方式
    推荐取值：
      CAN_MSGOBJ_INDIVIDUAL_MASKING-每一路Msg独立进行匹配
      CAN_MSGOBJ_GLOBAL_MASKING-采用RXMGMASK, RX14MASK and RX15MASK, RXFGMASK匹配机制
    初始化：
      不必须初始化，默认为CAN_MSGOBJ_INDIVIDUAL_MASKING
  */
  uint8 CAN_RxMaskMode;
  
  /*
    描述：
      发送引脚，每个CANTx对应不同的TxPin
    取值：
      CAN0-*PTA12，PTB18
      CAN1-*PTE24，PTC17
    初始化：
      如果是CAN0，默认TX IO为PTA12
      如果是CAN1，默认TX IO为PTE24
  */
  PortPinsEnum_Type CAN_TxPin;  
  

  /*
    描述：
      接收引脚，每个CANRx对应不同的RxPin
    取值：
      CAN0-*PTA13，PTB19
      CAN1-*PTE25，PTC16
    初始化：
      如果是CAN0，默认RX IO为PTA13
      如果是CAN1，默认RX IO为PTE25
  */
  PortPinsEnum_Type CAN_RxPin;

}CAN_InitTypeDef;

typedef struct 
{
  /*
    描述：
      选择CANx
    取值：
      CAN0
      CAN1
    初始化：
      必须初始化
  */
  CAN_Type *CAN_Canx;
  
  /*
    描述：
      设置FlexCAN外设中的MSG number
    取值：
      MSG_NUM_0-Msg-Msg缓冲区0
      ......
      MSG_NUM_15-Msg-Msg缓冲区15
    初始化：
      必须初始化
  */
  uint8 CAN_MsgNum;
  
  /*
    描述：
      设置ID长度
    取值：
      CAN_MSGOBJ_ID_STD-标准帧
      CAN_MSGOBJ_ID_EXT-扩展帧
    初始化：
      必须初始化
  */
  uint8 CAN_MsgIdLength;
  
  /*
    描述：
      设置Substitute Remote Request（SRR）
    取值：
      1-使能
      0-禁止
    初始化：
      必须初始化
  */
  uint8 CAN_MsgSRR;
  
  /*
    描述：
      设置Remote Transmission Request（RTR）
    取值：
      1-使能
      0-禁止
    初始化：
      必须初始化
  */
  uint8 CAN_MsgRTR;
  
  /*
    描述：
      设置FlexCAN外设中的MB方向
    取值：
      FLEXCAN_MSGOBJ_DIR_RX-接收消息
      FLEXCAN_MSGOBJ_DIR_TX-发送消息
    初始化：
      必须初始化
  */
  uint8 CAN_MsgDirection;
  
  /*
    描述：
      设置Msg缓冲区是否中断
    取值：
      TRUE-使能
      FALSE-禁止
    初始化：
      必须初始化
  */
  boolean CAN_MsgInterrupt;
  
  /*
    描述：
      发送中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  CAN_ISR_CALLBACK CAN_Isr;
 
}CAN_MSGOBJ_InitTypeDef;

typedef struct 
{
  /*
    描述：
      CAN帧的ID
    取值：
      如果该结构体用于发送功能，需要根据标准帧/扩展帧来设置ID
        ID的长度分为11位标准帧和29位扩展帧两种
        CAN_MSGOBJ_ID_STD 表示标准帧，如CAN_MSGOBJ_InitTypeDef中设置id长度为CAN_MSGOBJ_ID_STD
        此时CAN_MsgID最长为11位，不能超过0x07FF；
        CAN_MSGOBJ_ID_EXT 表示扩展帧，如CAN_MSGOBJ_InitTypeDef中设置id长度为CAN_MSGOBJ_ID_EXT
        此时CAN_MsgID最长为29位，不能超过0x1FFFFFFF；
      如果该结构体用于接收数据，接收函数会将接收的ID赋值给这个变量
    初始化：
      根据该结构体的用途而定，如果用于发送功能，ID必须初始化；
  */
  uint32 CAN_MsgID;
  /*
    描述：
      CAN帧的时间戳
    取值：
      如果该结构体用于发送功能，该变量没有作用
      如果该结构体用于接收数据，接收函数会将接收的Msg缓冲区的时间戳给这个变量
    初始化：
      不必初始化；
  */
  uint16 CAN_MsgTimeStamp;
  /*
    描述：
      CAN帧的数据区域
    取值：
      如果该结构体用于发送功能，需要根据向该缓冲区写入要发送的数据
      如果该结构体用于接收数据，接收函数会将接收的数据复制到这个缓冲区
    初始化：
      根据该结构体的用途而定，如果用于发送功能，缓冲区必须初始化；
  */
  uint8  CAN_MsgDataBuffer[8];
  /*
    描述：
      CAN帧的数据长度
    取值：
      如果该结构体用于发送功能，需要设定要发送的数据长度
      如果该结构体用于接收数据，接收函数会将接收的数据长度赋值给这个变量
    初始化：
      根据该结构体的用途而定，如果用于发送功能，要发送数据长度必须初始化；
  */
  uint8  CAN_MsgDataLength;
  /*
    描述：
      CAN帧的优先级
    取值：
      0--7
    初始化：
      应用本地仲裁优先级时， MCR[LPRIO_EN]必须使能
  */
  uint8  CAN_MsgPriority;
  
}CAN_MessageFormat_TypeDef;

extern CAN_MSGOBJ_InitTypeDef flexcan_msgobj_param_t[];
//FlexCAN模块初始化函数
void LPLD_CAN_Init(CAN_InitTypeDef );
//FlexCAN模块反初始化函数
void LPLD_CAN_Deinit(CAN_InitTypeDef );
//FlexCAN模块中断使能
void LPLD_CAN_EnableIrq(CAN_InitTypeDef );
//FlexCAN模块中断禁止
void LPLD_CAN_DisableIrq(CAN_InitTypeDef );
//FlexCAN Msg缓冲区初始化
void LPLD_CAN_InitMessageObject(CAN_MSGOBJ_InitTypeDef ,uint32 );
//FlexCAN 传输Msg函数
uint8 LPLD_CAN_TransmitMessage(CAN_MSGOBJ_InitTypeDef, CAN_MessageFormat_TypeDef * );
//FlexCAN 接收中断处理函数
void LPLD_CAN_ReceivedMessage(CAN_MSGOBJ_InitTypeDef , CAN_MessageFormat_TypeDef * );


#endif /* __HW_CAN_H__ */
