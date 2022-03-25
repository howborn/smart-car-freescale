/**
 * @file TimeStamp.c
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
#include "common.h"
#include "TimeStamp.h"
//定义每月天数常量
const uint8 month_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//定义星期补偿常量
const uint8 week_table[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 

const uint8 month_list[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

/*
 * LPLD_DateTimeToTimeStamp
 * 将时间结构体转换为秒
 * 以IBM时间标准为基础
 * 
 * 参数:
 *    TimeStamp_FormatTypeDef* realtime_init_struct
 *                        具体定义见TimeStamp_FormatTypeDef
 *
 * 输出:
 *    返回值为秒，可以做为RTC秒计数器的值
 *    返回零表示错误
 *
 */
uint32 LPLD_DateTimeToTimeStamp(TimeStamp_FormatTypeDef *realtime_init_struct)
{
  uint16 i; 
  uint32 current_second = 0;
  uint16 current_year = realtime_init_struct->year;
  uint8 current_month = realtime_init_struct->month;
  uint8 current_day = realtime_init_struct->day;
  uint8 current_hour = realtime_init_struct->hour;
  uint8 current_minute = realtime_init_struct->minute;
  //判断时间起点终端是否越界
  if(current_year < IBM_YEAR_START || current_year > IBM_YEAR_END)
  {
    return 0;
  }
  //将年转化为秒
  for (i = IBM_YEAR_START; i < current_year; i++)
  {
    if(IS_LEAPYEAR(i))
    {
        current_second += LEAPYEAR_CONVERTTO_SECONEDS;
    }
    else
    {
        current_second += AVERAGEYEAR_CONVERTTO_SECONEDS;
    }
  }
  //将月转化为秒
  for ( i = 0; i < (current_month - 1); i++)
  {
    current_second += (uint32)month_table[i] * DAY_CONVERTTO_SECONEDS;
    if(IS_LEAPYEAR(current_year) && i == FEBRUARY)
    {
        current_second += DAY_CONVERTTO_SECONEDS;
    }
  }
  //将天，小时，分转化为秒
  current_second += (uint32)(current_day - 1) * DAY_CONVERTTO_SECONEDS;
  current_second += (uint32)current_hour * HOUR_CONVERTTO_SECONEDS;
  current_second += (uint32)current_minute * MINUTE_CONVERTTO_SECONEDS;
  current_second += realtime_init_struct->second;   
  return current_second;   
}
/*
 * LPLD_TimeStampToDateTime
 * 将秒转换为时间结构体
 * 以IBM时间标准为基础
 * 
 * 参数:
 *    uint32 -- rtc_second  RTC秒计数器的值
 *    TimeStamp_FormatTypeDef* realtime_init_struct
 *                        具体定义见TimeStamp_FormatTypeDef
 *
 * 输出:
 *    返回值为秒，可以做为RTC秒计数器的值
 *    返回1表示正确
 *
 */
uint8 LPLD_TimeStampToDateTime(uint32 rtc_second, TimeStamp_FormatTypeDef *realtime_init_struct)
{
  static uint32 day_counter = 0;
  uint32 days = 0;
  uint32 i = 0;
  days = rtc_second / DAY_CONVERTTO_SECONEDS;//second to days
  if(day_counter != days)
  {
    day_counter = days;
    i = IBM_YEAR_START;
    //days to years
    while(days >= AVERAGEYEAR_DAYS)
    {
      if(IS_LEAPYEAR(i)) 
      {
        if(days >= LEAPYEAR_DAYS)
        {
            days-= LEAPYEAR_DAYS;
        }   
        else 
        {
            i++;
            break;
        }   
      }
      else 
      {
         days -= AVERAGEYEAR_DAYS; 
      }
      i++;  
    }
    realtime_init_struct->year = i;
    i = 0;
    //days to month
    while(days >= AVERAGEYEAR_FEBRUARY_DAYS )
    {
      if(IS_LEAPYEAR(realtime_init_struct->year) && i == FEBRUARY )
      {
        if(days >= LEAPYEAR_FEBRUARY_DAYS) 
        {
          days -= LEAPYEAR_FEBRUARY_DAYS;
        }
        else break; 
      }
      else 
      {
        if(days >= month_table[i]) 
        {
          days -= month_table[i];
        }
        else break;
      }
      i++;  
    }
    realtime_init_struct->month = i+1;
    realtime_init_struct->day = days+1;
  }
  i = rtc_second % DAY_CONVERTTO_SECONEDS;     
  realtime_init_struct->week = LPLD_GetWeek(realtime_init_struct);         
  realtime_init_struct->hour = i / HOUR_CONVERTTO_SECONEDS;        //hours
  realtime_init_struct->minute = (i % HOUR_CONVERTTO_SECONEDS) / MINUTE_CONVERTTO_SECONEDS; //minutes  
  realtime_init_struct->second = (i % HOUR_CONVERTTO_SECONEDS) % MINUTE_CONVERTTO_SECONEDS; //seconds
  return 1;
}
/*
 * LPLD_GetWeek
 * 从时间结构体获得周数
 * 
 * 参数:
 *    TimeStamp_FormatTypeDef* realtime_init_struct
 *                        具体定义见TimeStamp_FormatTypeDef
 *
 * 输出:
 *    返回值为周数
 *
 */
uint8 LPLD_GetWeek(TimeStamp_FormatTypeDef *realtime_init_struct)
{   
  uint16 i;
  uint8 yh,yl,day,month;
  i = realtime_init_struct->year;
  day = realtime_init_struct->day;
  month = realtime_init_struct->month;
  yh = i / 100;
  yl = i % 100;
  if(yh >= 20 && yh <21)
  {
    yl +=100;
  }
  i = (yl + yl/4)%7 + day + week_table[month - 1];
  if(!(yl % 4) && month < 3)
  {
    i--;
  }
  return (i % 7);
} 

/*
 * LPLD_AutoInitTimeStamp
 * 根据编译器时钟自动初始化TimeStamp_FormatTypeDef结构体
 * 
 * 参数:
 *    TimeStamp_FormatTypeDef* realtime_init_struct
 *                        具体定义见TimeStamp_FormatTypeDef
 *
 * 输出:
 */
void LPLD_AutoInitTimeStamp(TimeStamp_FormatTypeDef *realtime_init_struct)
{
  int i;
  for(i = 0;i < 12;i++)
  {
    if(strncmp((char*)month_list[i],__DATE__,3)==0)
    {
      realtime_init_struct->month = i + 1;
      break;
    }
  }
  realtime_init_struct->year = GET_COMPILE_YEAR;
  realtime_init_struct->day = GET_COMPILE_DAY;
  realtime_init_struct->week = 0;
  realtime_init_struct->hour = GET_COMPILE_HOUR;
  realtime_init_struct->minute = GET_COMPILE_MINUTE;
  realtime_init_struct->second = GET_COMPILE_SECOND;
}

