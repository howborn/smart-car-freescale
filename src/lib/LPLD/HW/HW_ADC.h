/**
 * @file HW_ADC.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief ADC底层模块相关函数
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
#ifndef __HW_ADC_H__
#define __HW_ADC_H__
/********************************************************************/

//ADC通道枚举定义
typedef enum AdcChnEnum
{
  DAD0       =0,
  DAD1       =1,
  DAD2       =2,
  DAD3       =3,
  AD4        =4,
  AD5        =5,
  AD6        =6,
  AD7        =7,
  AD8        =8,
  AD9        =9,
  AD10       =10,
  AD11       =11,
  AD12       =12,
  AD13       =13,
  AD14       =14,
  AD15       =15,
  AD16       =16,
  AD17       =17,
  AD18       =18,
  AD19       =19,
  AD20       =20,
  AD21       =21,
  AD22       =22,
  AD23       =23,
  AD24       =24,
  AD25       =25,
  AD26       =26,
  AD27       =27,
  AD28       =28,
  AD29       =29,
  AD30       =30,
  AD31       =31
} AdcChnEnum_Type;

//单端和差分模式宏定义
#define ADC_SE            0x00          //单端采集
#define ADC_DIFF          ADC_SC1_DIFF_MASK       //差分采集

//采集时间配置
#define SAMTIME_SHORT     0x00          //短采样时间
#define SAMTIME_LONG      ADC_CFG1_ADLSMP_MASK    //长采样时间

//长采样时间宏定义
#define LSAMTIME_20EX     0x00          //长采样时间增加20个额外AD时钟
#define LSAMTIME_12EX     0x01          //长采样时间增加12个额外AD时钟
#define LSAMTIME_6EX      0x02          //长采样时间增加6个额外AD时钟
#define LSAMTIME_2EX      0x03          //长采样时间增加2个额外AD时钟        

//转换精度宏定义
#define SE_8BIT           0x00          //单端8位精度
#define SE_10BIT          0x02          //单端10位精度
#define SE_12BIT          0x01          //单端12位精度
#define SE_16BIT          0x03          //单端16位精度
#define DIFF_9BIT         SE_8BIT       //差分9位精度
#define DIFF_11BIT        SE_10BIT      //差分11位精度
#define DIFF_13BIT        SE_12BIT      //差分13位精度
#define DIFF_16BIT        SE_16BIT      //差分16位精度

//采样硬件平均宏定义
#define HW_DISAVG         0x00                         //禁用硬件平均
#define HW_4AVG           (0x00|ADC_SC3_AVGE_MASK)     //4次硬件平均
#define HW_8AVG           (0x01|ADC_SC3_AVGE_MASK)     //8次硬件平均
#define HW_16AVG          (0x02|ADC_SC3_AVGE_MASK)     //16次硬件平均
#define HW_32AVG          (0x03|ADC_SC3_AVGE_MASK)     //32次硬件平均

//ADC复用引脚选择
#define MUX_ADXXA         0x00          //选择ADxxa通道               
#define MUX_ADXXB         ADC_CFG2_MUXSEL_MASK          //选择ADxxb通道  

//硬件触发配置
#define HW_TRGDSABLE      0  //禁用硬件触发
#define HW_TRGA           (ADC_SC2_ADTRG_MASK)  //SC1A硬件触发
#define HW_TRGB           (ADC_SC2_ADTRG_MASK|0x01)  //SC1B硬件触发

//PGA增益宏定义
#define PGA_1GAIN         (0x00|0x80)
#define PGA_2GAIN         (0x01|0x80)
#define PGA_4GAIN         (0x02|0x80)
#define PGA_8GAIN         (0x03|0x80)
#define PGA_16GAIN        (0x04|0x80)
#define PGA_32GAIN        (0x05|0x80)
#define PGA_64GAIN        (0x06|0x80)

//ADC时钟分频宏定义
#define ADIV_1            0x00
#define ADIV_2            0x01
#define ADIV_4            0x02
#define ADIV_8            0x03

//总线分频宏定义
#define ADICLK_BUS        0x00
#define ADICLK_BUS_2      0x01
#define ADICLK_ALTCLK     0x02
#define ADICLK_ADACK      0x03

//ADC参考源宏定义
#define REFSEL_EXT        0x00
#define REFSEL_ALT        0x01
#define REFSEL_RES        0x02     
#define REFSEL_RES_EXT    0x03   

//ADC模块中断回调函数类型
typedef void (*ADC_ISR_CALLBACK)(void);

//ADC模块初始化结构体，用于配置ADC各项参数
typedef struct
{    
  /*
    描述：
      选择ADCx
    取值：
      ADC0、ADC1
      <注:只有MK60F系列含有ADC2,ADC3>
      ADC2、ADC3
    初始化：
      必须初始化
  */
  ADC_Type *ADC_Adcx;
  
  /*
    描述：
      转换模式选择
    取值：
      ADC_SE-单端采集
      ADC_DIFF-差分分级
    初始化：
      不必须初始化，默认ADC_SE
  */
  uint8 ADC_DiffMode;
    
  /*
    描述：
      转换精度选择
    取值：
      SE_8BIT-单端8位精度
      SE_10BIT-单端10位精度
      SE_12BIT-单端12位精度
      SE_16BIT-单端16位精度
      DIFF_9BIT-差分9位精度
      DIFF_11BIT-差分11位精度
      DIFF_13BIT-差分13位精度
      DIFF_16BIT-差分16位精度
    初始化：
      不必须初始化，默认SE_8BIT
  */
  uint8 ADC_BitMode;
    
  /*
    描述：
      采样时间选择
    取值：
      SAMTIME_SHORT-短采样时间
      SAMTIME_LONG-长采样时间
    初始化：
      不必须初始化，默认SAMTIME_SHORT
  */
  uint8 ADC_SampleTimeCfg;
    
  /*
    描述：
      长采样时间额外AD时钟选择
    取值：
      LSAMTIME_20EX-长采样时间增加20个额外AD时钟
      LSAMTIME_12EX-长采样时间增加12个额外AD时钟
      LSAMTIME_6EX-长采样时间增加6个额外AD时钟
      LSAMTIME_2EX-长采样时间增加2个额外AD时钟 
    初始化：
      不必须初始化，默认LSAMTIME_20EX
  */
  uint8 ADC_LongSampleTimeSel;
    
  /*
    描述：
      硬件平均次数选择
    取值：
      HW_DISAVG-禁用硬件平均
      HW_4AVG-4次硬件平均
      HW_8AVG-8次硬件平均
      HW_16AVG-16次硬件平均
      HW_32AVG-32次硬件平均
    初始化：
      不必须初始化，默认HW_DISAVG
  */
  uint8 ADC_HwAvgSel;
    
  /*
    描述：
      可编程增益选择，使用相应PGA输入通道才有效
    取值：
      PGA_1GAIN-1倍增益
      PGA_2GAIN-2倍增益
      PGA_4GAIN-4倍增益
      PGA_8GAIN-8倍增益
      PGA_16GAIN-16倍增益
      PGA_32GAIN-32倍增益
      PGA_64GAIN-64倍增益
    初始化：
      不必须初始化，默认PGA_1GAIN
  */
  uint8 ADC_PgaGain;
    
  /*
    描述：
      ADC输入通道a\b复用引脚选择
    取值：
      MUX_ADXXA-A输入通道
      MUX_ADXXB-B输入通道
    初始化：
      不必须初始化，默认MUX_ADXXA
  */
  uint8 ADC_MuxSel;
    
  /*
    描述：
      ADC初始化自动校准功能使能
    取值：
      TRUE-使能自动校准
      FALSE-禁用自动校准
    初始化：
      建议初始化为TRUE
  */
  boolean ADC_CalEnable;
    
  /*
    描述：
      ADC硬件触发配置
    取值：
      HW_TRGDSABLE-禁用硬件触发
      HW_TRGA-A转换通道硬件触发
      HW_TRGB-B转换通道硬件触发
    初始化：
      不必须初始化，默认HW_TRGDSABLE
  */
  uint8 ADC_HwTrgCfg;
    
  /*
    描述：
      使能ADC准换完成触发DMA请求
    取值：
      TRUE-使能DMA
      FALSE-禁用DMA
    初始化：
      不必须初始化，默认FALSE
  */ 
  boolean ADC_DmaEnable;

  /*
    描述：
      ADCx中断回调函数
    取值：
      函数必须为无返回值,无参数(eg. void isr(void);)
    初始化：
      不必须初始化、如未初始化则不会触发中断
  */
  ADC_ISR_CALLBACK ADC_Isr; 
  
} ADC_InitTypeDef;

//ADC通用初始化函数，选择ADCx、采集模式、精度等参数
uint8 LPLD_ADC_Init(ADC_InitTypeDef);
//ADC反初始化函数，禁用ADC模块
uint8 LPLD_ADC_Deinit(ADC_InitTypeDef);
//使能ADCx相应转换通道的AD采集功能
uint8 LPLD_ADC_Chn_Enable(ADC_Type *, AdcChnEnum_Type);
//软件触发模式获取AD转换结果
uint16 LPLD_ADC_Get(ADC_Type *, AdcChnEnum_Type);
//使能ADCx中断
uint8 LPLD_ADC_EnableIrq(ADC_InitTypeDef);
//禁用ADCx中断
uint8 LPLD_ADC_DisableIrq(ADC_InitTypeDef);
//使能ADCx转换通道，用于软件中断、硬件触发等模式
void LPLD_ADC_EnableConversion(ADC_Type *, AdcChnEnum_Type, uint8, boolean);
//获取AD转换结果
uint16 LPLD_ADC_GetResult(ADC_Type *, uint8);
//获取SC1寄存器中COCO位置1的编号
uint8 LPLD_ADC_GetSC1nCOCO(ADC_Type *);

#endif /* __HW_ADC_H__ */