/*
 * @file HW_I2C.c
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

#include "common.h"
#include "HW_I2C.h"

//用户自定义中断服务函数数组
I2C_ISR_CALLBACK I2C_ISR[2];

/*
 * LPLD_I2C_Init
 * I2C通用初始化函数，在该函数中选择I2C通道，选择I2C SCK总线频率，
 * 选择I2C SDA 和 I2C SCL的引脚，配置I2C的中断回调函数
 * 
 * 参数:
 *    I2C_InitTypeDef--i2c_init_structure
 *                     具体定义见I2C_InitTypeDef
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */
uint8 LPLD_I2C_Init(I2C_InitTypeDef i2c_init_structure)
{
  I2C_Type *i2cx = i2c_init_structure.I2C_I2Cx;
  uint8 bus_speed = i2c_init_structure.I2C_ICR;
  PortPinsEnum_Type scl_pin = i2c_init_structure.I2C_SclPin;
  PortPinsEnum_Type sda_pin = i2c_init_structure.I2C_SdaPin;
  I2C_ISR_CALLBACK isr_func = i2c_init_structure.I2C_Isr;
  boolean ode = i2c_init_structure.I2C_OpenDrainEnable;
  uint8 ode_mask = 0;

  //参数检查，判断SCL频率
  ASSERT( bus_speed <= 0x3F);
  
  if(ode == TRUE)
  {
    ode_mask = PORT_PCR_ODE_MASK;
  }

  if(i2cx == I2C0)
  {
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; //开启I2C0时钟
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC4 |= SIM_SCGC4_IIC0_MASK; //开启I2C0时钟
#endif 
    if(scl_pin == PTD8)
    {
      PORTD->PCR[8] = PORT_PCR_MUX(2) | ode_mask;         
    }
    else if(scl_pin == PTB0) 
    {
      PORTB->PCR[0] = PORT_PCR_MUX(2) | ode_mask;
    }
    else //scl_pin = PTB2
    {
      PORTB->PCR[2] = PORT_PCR_MUX(2) | ode_mask;
    }

    if(sda_pin == PTD9)
    {
      PORTD->PCR[9] = PORT_PCR_MUX(2) | ode_mask;
    }
    else if(sda_pin == PTB1) 
    {
      PORTB->PCR[1] = PORT_PCR_MUX(2) | ode_mask;
    }
    else //sda_pin = PTB3
    {
      PORTB->PCR[3] = PORT_PCR_MUX(2) | ode_mask; 
    }
  }
  else if(i2cx == I2C1)
  { 
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK; //开启I2C0时钟
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC4 |= SIM_SCGC4_IIC1_MASK; //开启I2C0时钟
#endif

    if(scl_pin == PTE1)
    {
      PORTE->PCR[1] = PORT_PCR_MUX(6) | ode_mask;         
    }
    else //scl_pin = PTC10
    {
      PORTC->PCR[10] = PORT_PCR_MUX(2) | ode_mask;  
    }

    if(sda_pin == PTE0)
    {
      PORTE->PCR[0] = PORT_PCR_MUX(6) | ode_mask;
    }
    else //sda_pin = PTC11
    {
      PORTC->PCR[11] = PORT_PCR_MUX(2) | ode_mask; 
    }
  }
  else
    return 0;
  
  if(i2c_init_structure.I2C_IntEnable == TRUE && isr_func != NULL)
  {
    //产生I2C中断的中断源：
    //1,完成1个字节传输时，IICIF置位产生中断;
    //2,当Calling Address匹配成功时产生中断，参考K60文档1456页I2Cx_S寄存器IAAS位;
    //3,从机模式下当总线仲裁丢失时，IICIF置位产生中断;
    //  需要同时写1清除II2Cx_S的ARBL标志位和 I2Cx_S的 IICIF的标志位;
    //4,如果SMB寄存器的SHTF2 interrupt使能，当SHTF2 timeout时IICIF置位产生中断;
    //  需要同时写1清除I2Cx_SMB的SLTF标志位和 I2Cx_S的 IICIF的标志位;
    //5,当SLT寄存器不为0时，SMBus的SCL low timer计数等于SLT的值时IICIF置位产生中断;
    //  需要同时写1清除I2Cx_SMB的SHTF2标志位和 I2Cx_S的 IICIF的标志位;
    //6,当Wakeup 使能，I2C在停止模式下接收到Wakeup信号，将产生中断.

    i2cx->C1 |= I2C_C1_IICIE_MASK;

    if(i2cx == I2C0)
    {
      I2C_ISR[0] = isr_func;
    }
    else if(i2cx == I2C0)
    {
      I2C_ISR[1] = isr_func;
    }
    else 
      return 0;
  }

  //i2cx->C2 |= I2C_C2_HDRS_MASK;      //提高I2C驱动能力
  i2cx->F  = I2C_F_ICR(bus_speed)|I2C_F_MULT(0);   //配置I2Cx SCL BusSpeed
  i2cx->C1 |= I2C_C1_IICEN_MASK;      //使能I2Cx
  
  return 1;
}

/*
 * LPLD_I2C_Deinit
 * I2C模块反初始化函数，在该函数中关闭I2Cx的外设总线时钟，关闭I2C模块的
 * 时钟，禁止外设中断。
 *
 * 参数:
 *    I2C_InitTypeDef--i2c_init_structure
 *                     具体定义见I2C_InitTypeDef
 *
 * 输出:
 *    无
 *
 */  
uint8 LPLD_I2C_Deinit(I2C_InitTypeDef i2c_init_structure)
{
  I2C_Type *i2cx = i2c_init_structure.I2C_I2Cx;

  i2cx->C1 &= ~I2C_C1_IICEN_MASK;      //I2Cx
  if(i2cx == I2C0)
  {
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; //开启I2C0时钟
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC4 |= SIM_SCGC4_IIC0_MASK; //开启I2C0时钟
#endif
    disable_irq((IRQn_Type)I2C0_IRQn);
  }
  else if (i2cx == I2C1)
  {
#if (defined(CPU_MK60DZ10))  
    SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK; //开启I2C0时钟
#elif defined(CPU_MK60F12) || defined(CPU_MK60F15)
    SIM->SCGC4 |= SIM_SCGC4_IIC1_MASK; //开启I2C0时钟
#endif
    disable_irq((IRQn_Type)I2C1_IRQn);
  }
  else
  {
    return 0;
  }
  return 1;
}

/*
 * LPLD_I2C_EnableIrq
 * I2C外设中断使能
 *
 * 参数:
 *    I2C_InitTypeDef--i2c_init_structure
 *                     具体定义见I2C_InitTypeDef
 *
 * 输出:
 *    无
 *
 */  
void LPLD_I2C_EnableIrq(I2C_InitTypeDef i2c_init_structure)
{
  I2C_Type *i2cx = i2c_init_structure.I2C_I2Cx; 

  if(i2cx == I2C0)
  {
    enable_irq((IRQn_Type)I2C0_IRQn);
  }
  else if (i2cx == I2C1)
  {
    enable_irq((IRQn_Type)I2C1_IRQn);
  }
  else
  {
    return;
  }
}

/*
 * LPLD_I2C_DisableIrq
 * 禁止I2C外设中断
 *
 * 参数:
 *    I2C_InitTypeDef--i2c_init_structure
 *                     具体定义见I2C_InitTypeDef
 *
 * 输出:
 *    无
 *
 */  
void LPLD_I2C_DisableIrq(I2C_InitTypeDef i2c_init_structure)
{
  I2C_Type *i2cx = i2c_init_structure.I2C_I2Cx;
  i2cx->C1 &= ~I2C_C1_IICIE_MASK;

  if(i2cx == I2C0)
  {
    disable_irq((IRQn_Type)I2C0_IRQn);
  }
  else if (i2cx == I2C1)
  {
    disable_irq((IRQn_Type)I2C1_IRQn);
  }
  else
  {
    return;
  }
}

/*
 * LPLD_I2C_Start
 * 产生I2C开始信号
 * 
 * 参数:
 *    i2cx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 * 输出:
 *    无
 */
void LPLD_I2C_Start(I2C_Type *i2cx)
{
  i2cx->C1 |= I2C_C1_TX_MASK ;
  i2cx->C1 |= I2C_C1_MST_MASK ;
}

/*
 * LPLD_ReStart
 * I2C再次产生开始信号
 * 
 * 参数:
 *    i2cx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 * 输出:
 *    无
*/
void LPLD_I2C_ReStart(I2C_Type *i2cx)
{
  i2cx->C1 |= I2C_C1_RSTA_MASK ;
}

/*
 * LPLD_I2C_Stop
 * 产生I2C停止信号
 * 
 * 参数:
 *    i2cx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 * 输出:
 *    无
 */
void LPLD_I2C_Stop(I2C_Type *i2cx)
{
  i2cx->C1 &=(~I2C_C1_MST_MASK);
  i2cx->C1 &=(~I2C_C1_TX_MASK); 
}

/*
 * LPLD_I2C_WaitAck
 * I2C设置等待应答信号，开启则等待，关闭则不等待
 * 
 * 参数:
 *    i2cx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 *    is_wait--选择是否等待应答
 *      |__I2C_ACK_OFF    --关闭等待Ack
 *      |__I2C_ACK_ON     --开启等待Ack，并等待ACK信号
 * 输出:
 *    无
 */
void LPLD_I2C_WaitAck(I2C_Type *i2cx, uint8 is_wait)
{
  uint16 time_out;
  if(is_wait == I2C_ACK_ON)
  {
    while(!(i2cx->S & I2C_S_IICIF_MASK))
    {
      if(time_out>60000) //如果等待超时，强行退出
        break;
      else time_out++;
    }
    i2cx->S |= I2C_S_IICIF_MASK;
  }
  else
  {
    //关闭I2C的ACK
    i2cx->C1 |= I2C_C1_TXAK_MASK; 
  }
}

/*
 * LPLD_I2C_Write
 * I2C发送一个字节给目的地址设备
 * 
 * 参数:
 *    i2cx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 *    data8--要发送的字节数据
 * 输出:
 *    无
 *
 */
void LPLD_I2C_WriteByte(I2C_Type *i2cx, uint8 data8)
{
  i2cx->D = data8; 
}

/*
 * LPLD_I2C_Read
 * I2C从外部设备读一个字节
 * 
 * 参数:
 *    i2cx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 * 输出:
 *    I2C读取的字节 
 */

uint8 LPLD_I2C_ReadByte(I2C_Type *i2cx)
{
  uint8 temp;
  temp = i2cx->D; 
  return temp;
}

/*
 * LPLD_I2C_SetMasterWR
 * I2C主机读写模式配置
 * 
 * 参数:
 *    IICx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 *    mode--读写模式选择
 *      |__I2C_MWSR         --主机写
 *      |__I2C_MRSW         --主机读
 * 输出:
 *    无
 */
void LPLD_I2C_SetMasterWR(I2C_Type *i2cx, uint8 mode)
{
  if(mode==I2C_MRSW) 
    i2cx->C1 &= (~I2C_C1_TX_MASK);
  else
    i2cx->C1 |= ( I2C_C1_TX_MASK);
}

/*
 * LPLD_I2C_StartTrans
 * I2C开始传输函数，需要设置外围设备地址和读写模式
 * 
 * 参数:
 *    IICx--选择I2C模块的通道
 *      |__I2C0           --I2C通道0
 *      |__I2C1           --I2C通道1
 *    addr--外围设备地址     
 *    mode--读写模式选择
 *      |__I2C_MWSR         --主机写
 *      |__I2C_MRSW         --主机读
 * 输出:
 *    无
 */
void LPLD_I2C_StartTrans(I2C_Type *i2cx, uint8 addr, uint8 mode)
{
  //I2C产生start信号
  LPLD_I2C_Start(i2cx);
  //将从机地址和主机读写位合成一个字节写入
  LPLD_I2C_WriteByte(i2cx, (addr<<1)|mode );
}

//HW层中断函数，用户无需调用
void I2C0_IRQHandler(void)
{
#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif
  if(I2C0->S & I2C_S_IICIF_MASK)
  {
    I2C_ISR[0]();
    if(I2C0->SMB & I2C_SMB_SLTF_MASK)
    {
      I2C0->SMB |= I2C_SMB_SLTF_MASK;
    }
    if(I2C0->SMB & I2C_SMB_SHTF2_MASK)
    {
      I2C0->SMB |= I2C_SMB_SHTF2_MASK;
    }
    if(I2C0->S & I2C_S_ARBL_MASK)
    {
      I2C0->S |= I2C_S_ARBL_MASK;
    }
    I2C0->S |= I2C_S_IICIF_MASK;
  }
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}
//HW层中断函数，用户无需调用
void I2C1_IRQHandler(void)
{

#if (UCOS_II > 0u)
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL(); //告知系统此时已经进入了中断服务子函数
  OSIntEnter();
  OS_EXIT_CRITICAL();
#endif

  if(I2C1->S & I2C_S_IICIF_MASK)
  {
    I2C_ISR[1]();
    if(I2C1->SMB & I2C_SMB_SLTF_MASK)
    {
      I2C1->SMB |= I2C_SMB_SLTF_MASK;
    }
    if(I2C1->SMB & I2C_SMB_SHTF2_MASK)
    {
      I2C1->SMB |= I2C_SMB_SHTF2_MASK;
    }
    if(I2C1->S & I2C_S_ARBL_MASK)
    {
      I2C1->S |= I2C_S_ARBL_MASK;
    }
    I2C1->S |= I2C_S_IICIF_MASK;
  }
  
#if (UCOS_II > 0u)
  OSIntExit();          //告知系统此时即将离开中断服务子函数
#endif
}



