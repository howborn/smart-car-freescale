/**
 * @file HW_GPIO.c
 * @version 3.03[By LPLD]
 * @date 2014-2-10
 * @brief GPIO底层模块相关函数
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
#include "HW_GPIO.h"


//用户自定义中断服务函数数组
GPIO_ISR_CALLBACK GPIO_ISR[5];

/*
 * LPLD_GPIO_Init
 * GPIO通用初始化函数
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体，
 *                        具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_GPIO_Init(GPIO_InitTypeDef gpio_init_structure)
{
  uint8 i;
  PORT_Type *portx;
  uint32 pcr = PORT_PCR_MUX(1)| gpio_init_structure.GPIO_PinControl; 
  GPIO_Type *ptx = gpio_init_structure.GPIO_PTx;
  uint32 pins = gpio_init_structure.GPIO_Pins;
  uint8 dir = gpio_init_structure.GPIO_Dir;
  uint8 output = gpio_init_structure.GPIO_Output;
  GPIO_ISR_CALLBACK isr_func = gpio_init_structure.GPIO_Isr;
  
  //参数检查
  ASSERT( ptx <= PTE);                  //判断端口
  ASSERT( dir <= 1 );                   //判断方向
  ASSERT( output <= 1 );                //判断输出电平状态
  
  if(ptx == PTA)
  {
    portx = PORTA;  
    i = 0;
  }
  else if(ptx == PTB) 
  {
    portx = PORTB;
    i = 1;
  }
  else if(ptx == PTC) 
  {
    portx = PORTC;
    i = 2;
  }
  else if(ptx == PTD) 
  {
    portx = PORTD;
    i = 3; 
  }
  else if(ptx == PTE) 
  {
    portx = PORTE;
    i = 4;
  }
  else
    return 0;
  
  //输入或输出设置
  if(dir==DIR_OUTPUT)
  {
    ptx->PDDR |= pins;
    //设置初始输出
    if(output==OUTPUT_H)
    {
      ptx->PSOR = pins; 
    }
    else
    {
      ptx->PCOR = pins;
    }
  }
  else
  { 
    ptx->PDDR &= ~(pins);
  }
  
  //配置所选引脚的控制寄存器
  for(uint8 i=0; i<32; i++)
  {
    if(pins&(1ul<<i))
    {
      portx->PCR[i] = pcr;
    }
  }

  if(isr_func!=NULL)
    GPIO_ISR[i] = isr_func;
  
  return 1;
}

/*
 * LPLD_GPIO_EnableIrq
 * 使能GPIO外部中断
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体，
 *                        具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_GPIO_EnableIrq(GPIO_InitTypeDef gpio_init_structure)
{
  uint8 i;
  GPIO_Type *ptx = gpio_init_structure.GPIO_PTx;
  
  //参数检查
  ASSERT( ptx <= PTE);                  //判断端口
  
  if(ptx == PTA)
    i = 0;
  else if(ptx == PTB) 
    i = 1;
  else if(ptx == PTC) 
    i = 2;
  else if(ptx == PTD) 
    i = 3;
  else
    i = 4;

  enable_irq((IRQn_Type)(i + PORTA_IRQn));
  
  return 1;
}

/*
 * LPLD_GPIO_DisableIrq
 * 禁用GPIO外部中断
 * 
 * 参数:
 *    gpio_init_structure--GPIO初始化结构体，
 *                        具体定义见GPIO_InitTypeDef
 *
 * 输出:
 *    0--配置错误
 *    1--配置成功
 *
 */
uint8 LPLD_GPIO_DisableIrq(GPIO_InitTypeDef gpio_init_structure)
{
  uint8 i;
  GPIO_Type *ptx = gpio_init_structure.GPIO_PTx;
  
  //参数检查
  ASSERT( ptx <= PTE);                  //判断端口
  
  if(ptx == PTA)
    i = 0;
  else if(ptx == PTB) 
    i = 1;
  else if(ptx == PTC) 
    i = 2;
  else if(ptx == PTD) 
    i = 3;
  else
    i = 4;
  
  disable_irq((IRQn_Type)(i + PORTA_IRQn));
  
  return 1;
}

/*
 * LPLD_GPIO_Ouptut
 * 设置GPIO端口0~31位的输出
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    data32--输出数据
 *      |__0x00000000~0xFFFFFFFF--低到高代表GPIO口的第0~31位数据
 *
 * 输出:
 *
 */
__INLINE void LPLD_GPIO_Output(GPIO_Type *ptx, uint32 data32)
{ 
  ptx->PDOR = data32;
}

/*
 * LPLD_GPIO_Ouptut_b
 * 设置GPIO端口一位的输出
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--端口引脚位数编号
 *      |__0~31       --GPIO口的第0~31位
 *    data1--输出数据
 *      |__0          --输出低电平
 *      |__1          --输出高电平
 *
 * 输出:
 *
 */
__INLINE void LPLD_GPIO_Output_b(GPIO_Type *ptx, uint32 lsb_num, uint8 data1)
{
  ptx->PDOR = (ptx->PDOR & ~(0x01L<<lsb_num)) | (uint32)data1<<lsb_num;
}

/*
 * LPLD_GPIO_Ouptut_8b
 * 设置GPIO端口8位的输出
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--代表8个端口引脚的最低位数编号
 *      |__0~24       --GPIO口的第0~24位
 *    data8--输出数据
 *      |__0x00~0xFF--代表GPIO口输出的8位数据
 *
 * 输出:
 *
 */
__INLINE void LPLD_GPIO_Output_8b(GPIO_Type *ptx, uint8 lsb_num, uint8 data8)
{
  ptx->PDOR = (ptx->PDOR & ~(0xFFL<<lsb_num)) | (uint32)data8<<lsb_num;
}

/*
 * LPLD_GPIO_Toggle
 * 设置GPIO端口0~31的电平翻转
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    data32--翻转数据
 *      |__0x00000000~0xFFFFFFFF--低到高代表GPIO口的第0~31位的翻转，1为反转，0为保持不变。
 *
 * 输出:
 *
 */
__INLINE void LPLD_GPIO_Toggle(GPIO_Type *ptx, uint32 data32)
{  
  ptx->PTOR = data32;
}

/*
 * LPLD_GPIO_Toggle_b
 * 设置GPIO端口一位的翻转
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--端口引脚位数编号
 *      |__0~31       --GPIO口的第0~31位
 *
 * 输出:
 *
 */
__INLINE void LPLD_GPIO_Toggle_b(GPIO_Type *ptx, uint8 lsb_num)
{  
  ptx->PTOR = 0x01L<<lsb_num;
}

/*
 * LPLD_GPIO_Toggle_8b
 * 设置GPIO端口8位的翻转
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--代表8个端口引脚的最低位数编号
 *      |__0~24       --GPIO口的第0~24位
 *    data8--输出数据
 *      |__0x00~0xFF--代表GPIO口输出的8位数据
 *
 * 输出:
 *
 */
__INLINE void LPLD_GPIO_Toggle_8b(GPIO_Type *ptx, uint8 lsb_num, uint8 data8)
{
  ptx->PTOR = (uint32)data8<<lsb_num;
}

/*
 * LPLD_GPIO_Input
 * 取得GPIO口0~31位的数据
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *
 * 输出:
 *    指定GPIO口的32位输入
 *
 */
__INLINE uint32 LPLD_GPIO_Input(GPIO_Type *ptx)
{
  return ptx->PDIR;
}

/*
 * LPLD_GPIO_Input_b
 * 取得GPIO口某一位的数据
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--端口引脚位数编号
 *      |__0~31       --GPIO口的第0~31位
 *
 * 输出:
 *    指定GPIO口的指定位数的电平
 *
 */
__INLINE uint8 LPLD_GPIO_Input_b(GPIO_Type *ptx, uint8 lsb_num)
{  
  return (uint8)((ptx->PDIR>>lsb_num)&0x01L);
}

/*
 * LPLD_GPIO_Input_8b
 * 取得GPIO端口8位数据的输入
 * 
 * 参数:
 *    ptx--端口号
 *      |__PTA        --Port A
 *      |__PTB        --Port B
 *      |__PTC        --Port C
 *      |__PTD        --Port D
 *      |__PTE        --Port E
 *    lsb_num--代表8个端口引脚的最低位数编号
 *      |__0~24       --GPIO口的第0~24位
 *
 * 输出:
 *    指定GPIO口的8位数据的电平
 *
 */
__INLINE uint8 LPLD_GPIO_Input_8b(GPIO_Type *ptx, uint8 lsb_num)
{
  return (uint8)((ptx->PDIR>>lsb_num)&0xFFL);
}

/*
 * PORTA--PORTE中断处理函数
 * 与启动文件startup_K60.s中的中断向量表关联
 * 用户无需修改，程序自动进入对应通道中断函数
 */
void PORTA_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[0](); 
  PORTA->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;87:  PORT A interrupt

void PORTB_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[1](); 
  PORTB->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;88:  PORT B interrupt

void PORTC_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[2](); 
  PORTC->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;89:  PORT C interrupt

void PORTD_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[3](); 
  PORTD->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;90:  PORT D interrupt

void PORTE_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  
  //调用用户自定义中断服务
  GPIO_ISR[4](); 
  PORTE->ISFR =0xFFFFFFFF;
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}//           ;91:  PORT E interrupt