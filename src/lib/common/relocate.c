/**
 * @file relocate.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief Kinetis通用数据代码迁移函数
 *
 * 更改建议:不建议修改
 *
 * 实现数据、代码由ROM向RAM迁移的过程
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

#if defined(__IAR_SYSTEMS_ICC__)
  #pragma section = ".data"
  #pragma section = ".data_init"
  #pragma section = ".bss"
  #pragma section = "CodeRelocate"
  #pragma section = "CodeRelocateRam"
#endif
   
/********************************************************************/
void common_relocate(void)
{
    #ifndef __IAR_SYSTEMS_ICC__
      #warning 非IAR编译器需确定此处地址
      extern char __START_BSS[];
      extern char __END_BSS[];
      extern uint32 __DATA_ROM[];
      extern uint32 __DATA_RAM[];
      extern char __DATA_END[];
    #endif
  
  /* 声明一个计数器在拷贝循环中使用 */
  uint32 n;
  
  /* 为不同的数据段定义指针。
  * 这些变量将由链接文件中获取的值初始化
  */
  uint8 * data_ram, * data_rom, * data_rom_end;
  uint8 * bss_start, * bss_end;
  
  
  /* 引进链接文件中的VECTOR_TABLE和VECTOR_RAM的地址 */
  extern uint32 __VECTOR_TABLE[];
  extern uint32 __VECTOR_RAM[];
  
  /* 将中断向量表复制到RAM中 */
  if (__VECTOR_RAM != __VECTOR_TABLE)
  {
    for (n = 0; n < 0x410; n++)
      __VECTOR_RAM[n] = __VECTOR_TABLE[n];
  }
  /* 将新的中断向量表指针赋给VTOR寄存器 */
  write_vtor((uint32)__VECTOR_RAM);
  
  /* 获得.data段的地址(已初始化的数据段) */
  #if defined(__IAR_SYSTEMS_ICC__)
    data_ram = __section_begin(".data");
    data_rom = __section_begin(".data_init");
    data_rom_end = __section_end(".data_init");
    n = data_rom_end - data_rom;	
  #else
    #warning 非IAR编译器需确定此处地址
    data_ram = (uint8 *)__DATA_RAM;
    data_rom = (uint8 *)__DATA_ROM;
    data_rom_end  = (uint8 *)__DATA_END; /* 该段在CodeWarrior编译器中为RAM地址 */
    n = data_rom_end - data_rom;
  #endif
  
  /* 从ROM复制已初始化的数据到RAM */
  while (n--)
    *data_ram++ = *data_rom++;
  
  
  /* 获得.bss段的地址 (初始化为0的数据) */
  #if defined(__IAR_SYSTEMS_ICC__)
  bss_start = __section_begin(".bss");
  bss_end = __section_end(".bss");  
  #else
  #warning 非IAR编译器需确定此处地址
  bss_start = (uint8 *)__START_BSS;
  bss_end = (uint8 *)__END_BSS;
  #endif
  
  /* 清零初始化为0的数据段 */
  n = bss_end - bss_start;
  while(n--)
    *bss_start++ = 0;
  
  /* 取得所有应该从ROM复制到RAM的代码段的地址。
  * IAR有一个预定义的关键字可以标记独立的函数为从RAM执行。
  * 在函数的返回类型前添加"__ramfunc"关键字可以将函数标记为从RAM中执行。
  * 例如:__ramfunc void foo(void);
  */
  #if defined(__IAR_SYSTEMS_ICC__)
  uint8* code_relocate_ram = __section_begin("CodeRelocateRam");
  uint8* code_relocate = __section_begin("CodeRelocate");
  uint8* code_relocate_end = __section_end("CodeRelocate");
  
  /* 将函数从ROM复制到RAM */
  n = code_relocate_end - code_relocate;
  while (n--)
    *code_relocate_ram++ = *code_relocate++;
  #endif
}
/********************************************************************/
