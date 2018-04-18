/**
 * @file HW_DAC.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief DAC底层模块相关函数
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
#ifndef __HW_DAC_H__
#define __HW_DAC_H__
/********************************************************************/

//DAC模块中断回调函数类型
typedef void (*DAC_ISR_CALLBACK)(void);

//缓冲区工作模式定义
#define BUFFER_MODE_NORMAL      0x00 //缓冲区正常模式
#define BUFFER_MODE_SWING       0x01 //缓冲区SWING模式
#define BUFFER_MODE_ONETIMESCAN 0x02 //缓冲区ONETIMESCAN模式
//缓冲区水印选择定义
#define WATERMARK_1WORD         0x00 //1个字
#define WATERMARK_2WORDS        0x01 //2个字
#define WATERMARK_3WORDS        0x02 //3个字
#define WATERMARK_4WORDS        0x03 //4个字

//DAC模块初始化结构体，用于配置DAC各项参数
typedef struct 
{   
  /*
    描述：
      选择DACx
    取值：
      DAC0、DAC1
    初始化：
      必须初始化
  */
  DAC_Type *DAC_Dacx;
    
  /*
    描述：
      使能缓冲区工作
    取值：
      TRUE-使能缓冲区工作
      FALSE-禁用缓冲区
    初始化：
      不必须初始化，默认FALSE
  */  
  boolean DAC_BufferEnable;
    
  /*
    描述：
      缓冲区工作模式选择，只在DAC_BufferEnable为TRUE时有效
    取值：
      BUFFER_MODE_NORMAL-普通模式，指针到达上限后清0
      BUFFER_MODE_SWING-摆动模式，指针到达上限后再递减回到0
      BUFFER_MODE_ONETIMESCAN-单次扫描模式，指针到达上限后停止
    初始化：
      不必须初始化，默认BUFFER_MODE_NORMAL
  */
  uint8 DAC_BufferWorkMode;
    
  /*
    描述：
      缓冲区水印选择，只在DAC_BufferEnable为TRUE时有效，
      在DAC_BufferWatermarkIntEnable为TRUE时产生中断请求
    取值：
      WATERMARK_1WORD-水印距离缓冲区上限1个字
      WATERMARK_2WORDS-水印距离缓冲区上限2个字
      WATERMARK_3WORDS-水印距离缓冲区上限3个字
      WATERMARK_4WORDS-水印距离缓冲区上限4个字
    初始化：
      不必须初始化，默认WATERMARK_1WORD
  */
  uint8 DAC_BufferWatermarkSel;
    
  /*
    描述：
      使能DMA
    取值：
      TRUE-使能DMA
      FALSE-禁用DMA
    初始化：
      不必须初始化，默认FALSE
  */ 
  boolean DAC_DmaEnable;
    
  /*
    描述：
      缓冲区上限数值，只在DAC_BufferEnable为TRUE时有效
    取值：
      1~16
    初始化：
      不必须初始化，默认1
  */
  uint8 DAC_BufferUpperLimit;
    
  /*
    描述：
      使能软件触发，只在DAC_BufferEnable为TRUE时有效
    取值：
      TRUE-使能软件触发
      FALSE-禁用软件触发
    初始化：
      不必须初始化，默认FALSE
  */ 
  boolean DAC_SoftTrgEnable;
    
  /*
    描述：
      使能读指针底部中断使能,使能后指针到达上限后产生中断请求
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DAC_ReadPointerBottomIntEnable;
    
  /*
    描述：
      使能读指针顶部中断使能,使能后指针等于0后产生中断请求
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DAC_ReadPointerTopIntEnable;
    
  /*
    描述：
      使能缓冲区水印中断使能,使能后指针到达水印位置后产生中断请求
    取值：
      TRUE-使能
      FALSE-禁用
    初始化：
      不必须初始化，默认FALSE
  */
  boolean DAC_BufferWatermarkIntEnable;
    
  /*
    描述：
      底部中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  DAC_ISR_CALLBACK DAC_ReadPointerBottomIsr; 
    
  /*
    描述：
      顶部中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  DAC_ISR_CALLBACK DAC_ReadPointerTopIsr; 
    
  /*
    描述：
      水印中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  DAC_ISR_CALLBACK DAC_BufferWatermarkIsr; 
  
}DAC_InitTypeDef;

//DAC初始化函数，使能DAC各项功能
uint8 LPLD_DAC_Init(DAC_InitTypeDef);
//DAC反初始化函数，禁用DAC模块
uint8 LPLD_DAC_Deinit(DAC_InitTypeDef);
//使能DACx中断
uint8 LPLD_DAC_EnableIrq(DAC_InitTypeDef);
//禁用DACx中断
uint8 LPLD_DAC_DisableIrq(DAC_InitTypeDef);
//设置DAC缓存区单个数据n
void LPLD_DAC_SetBufferDataN(DAC_Type *, uint16, uint8);
//设置DAC缓存区所有数据
void LPLD_DAC_SetBufferData(DAC_Type *, uint16 *, uint8);
//软件触发DAC缓冲区工作
void LPLD_DAC_SoftwareTrigger(DAC_Type *);

#endif /* __HW_DAC_H__ */