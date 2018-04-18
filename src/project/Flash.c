/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Global.h
 * @version 0.0
 * @date 2015-2-7
 * @brief 关于该工程的描述
 *
 * 版权所有:北华航天工业学院 第十届飞思卡尔  摄像头2队 
 * 硬件平台:  MK60FX512
 * 
 */
#include "common.h"
#include "Flash.h"

/*
 * 数据存储Flash函数
   *要写入的ADDR扇区号
   *X要存储的数据变量
   *需要类似宏定义
    //Flash宏定义
     #define FLASH_TEST_SECTOR   (127)
     #define FLASH_TEST_ADDR     (FLASH_TEST_SECTOR*2048)// FLASH_TEST_SECTOR*(2048)127*2048=260096
     #define DATA_SIZE           (2)现在代码仅支持uint16型但是基本也够用了
     DN为
     数据最大为五位数0,4,8,12,16,20等4的倍数
     FX为
     数据最大为五位数0,8,16,24,32,40等8的倍数
 */
void Write_flash(uint32 ADDR,uint8 offset,uint16 date)
{
  //Flash变量定义 
  uint8 write_buffer1[2]; //定义写缓冲区
  uint32 addr;
  addr=ADDR+offset;
  
  write_buffer1[0]=date/256;
  write_buffer1[1]=date%256;
  LPLD_Flash_ByteProgram((uint32)addr, (uint32*)write_buffer1, 2);
}


/*
 * 数据读取Flash函数
  *m要读取的扇区号
  *出口参数为读取存储数据的值
 */

uint16 Read_flash(uint32 ADDR,uint8 offset)
{
  //Flash变量定义 
  uint8 read_buffer1[2];  //定义读缓冲区
  uint8 *ptr;
  int8 len;
  uint16 utter;//出口
  
  ptr = (uint8*)(ADDR+offset);
  for( len = 0 ; len < 2 ; len++)
  {
    read_buffer1[len] = *(ptr + len);
  }
  
  utter=read_buffer1[0]*256+read_buffer1[1];
  return utter;
}
