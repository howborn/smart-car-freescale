/**
 * @file TIMESTAMP.h
 * @version 3.01[By LPLD]
 * @date 2013-10-29
 * @brief TIMESTAMP时间戳功能定义
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
#ifndef __HW_TIMESTAMP_H__
#define __HW_TIMESTAMP_H__

//定义时间宏定义
#define IBM_YEAR_START (1970)
#define IBM_YEAR_END   (2099)
#define LEAPYEAR_CONVERTTO_SECONEDS    (31622400)
#define AVERAGEYEAR_CONVERTTO_SECONEDS (31536000)
#define DAY_CONVERTTO_SECONEDS         (86400)
#define HOUR_CONVERTTO_SECONEDS        (3600)
#define MINUTE_CONVERTTO_SECONEDS      (60)
#define LEAPYEAR_DAYS      (366)
#define AVERAGEYEAR_DAYS   (365)
#define LEAPYEAR_FEBRUARY_DAYS      (29)
#define AVERAGEYEAR_FEBRUARY_DAYS   (28)
//判断平年和闰年宏定义
#define IS_LEAPYEAR(YEAR) (YEAR % 4 ? FALSE : YEAR % 100 ? TRUE : YEAR % 400 ? FALSE : TRUE )
//获得编译器时钟
#define GET_COMPILE_YEAR  ((__DATE__[7] - '0')*1000+\
                          (__DATE__[8] - '0')*100+\
                          (__DATE__[9] - '0')*10+\
                          (__DATE__[10] - '0'))
#define GET_COMPILE_DAY  ((__DATE__[4]==0x20?0:(__DATE__[4] - '0')*10) + (__DATE__[5] - '0'))
#define GET_COMPILE_HOUR ((__TIME__[0] - '0')*10 + (__TIME__[1] - '0'))
#define GET_COMPILE_MINUTE ((__TIME__[3] - '0')*10 + (__TIME__[4] - '0'))
#define GET_COMPILE_SECOND ((__TIME__[6] - '0')*10 + (__TIME__[7] - '0'))
//枚举月份
typedef enum 
{  
  JANUARY,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AUGUST,
  SEPTEMBER,
  OCTOBER,
  NOVOMBER,
  DECEMBER
}Month_TypeEnum;
//时间结构体
typedef struct 
{
  uint8 hour;
  uint8 minute;
  uint8 second;         
  uint8 week; 
  uint8 day;
  uint8 month; 
  uint16 year;     
}TimeStamp_FormatTypeDef;
//将时间结构体转换为秒
uint32 LPLD_DateTimeToTimeStamp(TimeStamp_FormatTypeDef*);
//将秒转换为时间结构体
uint8 LPLD_TimeStampToDateTime(uint32 ,TimeStamp_FormatTypeDef*);
//获得周数结构体
uint8 LPLD_GetWeek(TimeStamp_FormatTypeDef*);
//根据编译器时钟初始化TimeStamp_FormatTypeDef结构体
void LPLD_AutoInitTimeStamp(TimeStamp_FormatTypeDef *);
#endif