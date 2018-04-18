/**
 * @file HW_I2C.h
 * @version 3.02[By LPLD]
 * @date 2013-11-22
 * @brief I2C底层模块相关函数
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
#ifndef __HW_I2C_H__
#define __HW_I2C_H__
/********************************************************************/
//I2C模块中断回调函数类型
typedef void (*I2C_ISR_CALLBACK)(void);  
//定义ACK 
#define I2C_ACK_OFF                     (0)
#define I2C_ACK_ON                      (1)
//定义主机读写模式 
#define I2C_MWSR                        (0x00)  
#define I2C_MRSW                        (0x01)  

typedef struct 
{
  /*
    描述：
      选择I2Cx
    取值：
      I2C0 -- I2C通道0
      I2C1 -- I2C通道1
    初始化：
      必须初始化
  */
  I2C_Type *I2C_I2Cx;
  
  /*
    描述：
      查询I2C时序配置表中的ICR的取值，进而配置SCL总线频率I2C_SCL_BusSpeed、SDA Hold Time、
      SCL hold start time、SCL hold stop time.
      I2C baud rate = bus speed (Hz)/(mul × SCL divider)
      SDA hold time = bus period (s) × mul × SDA hold value
      SCL start hold time = bus period (s) × mul × SCL start hold value
      SCL stop hold time = bus period (s) × mul × SCL stop hold value
      注：
      1、其中mul = 1，SCL divider、SDA hold value、SCL start hold value、SCL stop hold value
      可以通过查找I2C时序配置表获得,I2C时序设置表在本文件结尾处.
      2、bus speed（Hz）是单片机的外设总线频率，可以通过K60_card.h中的BUS_CLK_MHZ宏定义进行设定.
      3、bus period （s）= 1/bus speed（Hz）.
      4、SCL总线频率最大400Khz.
    取值：
      在I2C时序配置表中获得ICR的值
    初始化：
      必须初始化
  */
  uint8   I2C_ICR;
  
  /*
    描述：
      使能I2C中断
    取值：
      TRUE -使能
      FALSE -禁用
    初始化：
      不必须初始化，默认值FALSE
  */
  boolean I2C_IntEnable;
  
  /*
    描述：
      SCL引脚，每个IICx对应不同的Pin
    取值：
      I2C0 -*PTB2、PTB0、PTD8
      I2C1 -*PTC10、PTE1
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type I2C_SclPin;
  
  /*
    描述：
      SDA引脚，每个IICx对应不同的Pin
    取值：
      I2C0 -*PTB3、PTB1、PTD9
      I2C1 -*PTC11、PTE0
    初始化：
      不必须初始化、默认值为带*的引脚
  */
  PortPinsEnum_Type I2C_SdaPin;
  
  /*
    描述：
      SCL、SDA引脚是否使能开漏模式
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化、默认值为FALSE
  */
  boolean I2C_OpenDrainEnable;
  
  /*
    描述：
      I2C中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  I2C_ISR_CALLBACK I2C_Isr;

}I2C_InitTypeDef;

//初始化I2Cx通道、SCL总线频率，SDA、SCL引脚，配置中断
uint8 LPLD_I2C_Init(I2C_InitTypeDef);
//I2Cx反初始化函数，关闭I2Cx中断，禁用I2Cx
uint8 LPLD_I2C_Deinit(I2C_InitTypeDef);
//I2Cx中断使能函数
void LPLD_I2C_EnableIrq(I2C_InitTypeDef);
//I2Cx中断禁止函数
void LPLD_I2C_DisableIrq(I2C_InitTypeDef);
//I2Cx产生开始信号函数
void LPLD_I2C_Start(I2C_Type *);
//I2Cx产生停止信号函数
void LPLD_I2C_Stop(I2C_Type *);
//I2Cx等待ACK信号函数
void LPLD_I2C_WaitAck(I2C_Type *, uint8);
//I2Cx再次产生开始信号函数
void LPLD_I2C_ReStart(I2C_Type *);
//I2Cx发送一个字节函数
void LPLD_I2C_WriteByte(I2C_Type *, uint8);
//I2Cx读取一个字节函数
uint8 LPLD_I2C_ReadByte(I2C_Type *);
//I2Cx主机读写模式配置函数
void LPLD_I2C_SetMasterWR(I2C_Type *, uint8);
//I2C开始传输函数，需要设置外围设备地址和读写模式
void LPLD_I2C_StartTrans(I2C_Type *, uint8, uint8);

/*
I2C时序配置表，选取ICR的值即可设置SCl divider，SDA Hold value，SCL Hold(Start、stop)Value  
By LPLD
__________________________________________________________________________________________
ICR  | SCL    |SDA Hold|SCL Hold |SCL Hold   ||  ICR  | SCL     |SDA Hold |SCL Hold |SCL Hold 
(hex)| Divider|Value   |(Start)  | (Stop)    ||  (hex)| Divider |Value    |(Start)  | (Stop)
     |        |        | Value   |  Value    ||       |         |         | Value   |  Value
_____|________|________|_________|___________||_______|_________|_________|_________|________
00   |   20   |   7    |      6  |      11   ||  20   |  160    |  17     |   78    |    81
01   |   22   |   7    |      7  |      12   ||  21   |  192    |  17     |   94    |    97
02   |   24   |   8    |      8  |      13   ||  22   |  224    |  33     |  110    |   113
03   |   26   |   8    |      9  |      14   ||  23   |  256    |  33     |  126    |   129
04   |   28   |   9    |     10  |      15   ||  24   |  288    |  49     |  142    |   145
05   |   30   |   9    |     11  |      16   ||  25   |  320    |  49     |  158    |   161
06   |   34   |  10    |     13  |      18   ||  26   |  384    |  65     |  190    |   193
07   |   40   |  10    |     16  |      21   ||  27   |  480    |  65     |  238    |   241
08   |   28   |   7    |     10  |      15   ||  28   |  320    |  33     |  158    |   161
09   |   32   |   7    |     12  |      17   ||  29   |  384    |  33     |  190    |   193
0A   |   36   |   9    |     14  |      19   ||  2A   |  448    |  65     |  222    |   225
0B   |   40   |   9    |     16  |      21   ||  2B   |  512    |  65     |  254    |   257
0C   |   44   |  11    |     18  |      23   ||  2C   |  576    |  97     |  286    |   289
0D   |   48   |  11    |     20  |      25   ||  2D   |  640    |  97     |  318    |   321
0E   |   56   |  13    |     24  |      29   ||  2E   |  768    | 129     |  382    |   385
0F   |   68   |  13    |     30  |      35   ||  2F   |  960    | 129     |  478    |   481
10   |   48   |   9    |     18  |      25   ||  30   |  640    |  65     |  318    |   321
11   |   56   |   9    |     22  |      29   ||  31   |  768    |  65     |  382    |   385
12   |   64   |  13    |     26  |      33   ||  32   |  896    | 129     |  446    |   449
13   |   72   |  13    |     30  |      37   ||  33   |  1024   | 129     |  510    |   513
14   |   80   |  17    |     34  |      41   ||  34   |  1152   | 193     |  574    |   577
15   |   88   |  17    |     38  |      45   ||  35   |  1280   | 193     |  638    |   641
16   |   104  |  21    |     46  |      53   ||  36   |  1536   | 257     |  766    |   769
17   |   128  |  21    |     58  |      65   ||  37   |  1920   | 257     |  958    |   961
18   |   80   |   9    |     38  |      41   ||  38   |  1280   | 129     |  638    |   641  
19   |   96   |   9    |     46  |      49   ||  39   |  1536   | 129     |  766    |   769
1A   |  112   |  17    |     54  |      57   ||  3A   |  1792   | 257     |  894    |   897
1B   |  128   |  17    |     62  |      65   ||  3B   |  2048   | 257     | 1022    |  1025
1C   |  144   |  25    |     70  |      73   ||  3C   |  2304   | 385     | 1150    |  1153
1D   |  160   |  25    |     78  |      81   ||  3D   |  2560   | 385     | 1278    |  1281
1E   |  192   |  33    |     94  |      97   ||  3E   |  3072   | 513     | 1534    |  1537
1F   |  240   |  33    |    118  |     121   ||  3F   |  3840   | 513     | 1918    |  1921
____________________________________________________________________________________________  

*/
#endif /* __HW_I2C_H__ */
