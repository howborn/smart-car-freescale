/**
 * --------------Smart_Car工程-----------------
 * @file Smart_Car.c
 * @version 0.0
 * @date 2015-2-7
 *
 *
 * 版权所有:  北华航天工学院 第十届飞思卡尔  摄像头2队
 * 硬件平台:  MK60FX512
 *
 *逻辑简单说明
 *第一个薄码开关确定车子再跑的过程中是否把图片存储到SD卡中
 *然后如果第二个薄码开关拨下去说明数据的来源是SD卡中的数据，所以就屏蔽掉图像实时处理的部分
 *两部分不一样的地方只是图像数据的来源
 *实时处理部分先得到图像。然后找线，找控制的中线，方向控制，速度控制，速度和期望速度标定，然后虚拟示波器发送函数。
 *SD卡分析部分，前部分是FatFs系统的东西，是如何读取图片，也就是平常调试部分，原理就是车载屏幕的刷新时间比较短，
   所以我们一张张翻图片然后发现图片有问题的时候用按键控制发送到串口助手中用于分析，当拿到车子这部分会很快的适应的。
 *主函数分析完毕
 */
#include "common.h"
#include "global.h"
#include "control.h"
#include "interrupt.h"
#include "init.h"
#include "gui.h"
#include "analyze.h"
#include "tft.h"

//文件系统文件
uint32 size, sizetmp;
int16  res,synchronous=0;

//虚拟示波器检验
uint16 CRC_Check(uint8 *buf, uint8 CRC_CNT)
{
    uint16 CRC_temp;
    uint8 i,j;
    CRC_temp = 0xffff;
    for (i=0;i<CRC_CNT; i++){
        CRC_temp ^= buf[i];
        for (j=0;j<8;j++) {
            if (CRC_temp & 0x01)
                CRC_temp = (CRC_temp >>1 ) ^ 0xa001;
            else
                CRC_temp = CRC_temp >> 1;
        }
    }
    return(CRC_temp);
}
//虚拟示波器封装发送函数
void OutPut_Data(void)
{
  uint8 databuf[10] = {0};
  uint8 i;
  uint16 CRC16 = 0;
  for(i=0;i<4;i++)
  {
    databuf[i*2]   = (uint8)(Send_Data[i]%256);
    databuf[i*2+1] = (uint8)(Send_Data[i]/256);
  }
  CRC16 = CRC_Check(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  for(i=0;i<10;i++)
     LPLD_UART_PutChar(UART0,databuf[i]);
}

//主函数
void main (void)
{
  DisableInterrupts;                  //关闭全局中断
  Init();                             //系统初始化
  EnableInterrupts;                   //使能全局中断

  FIL     fdst;   //文件
  FATFS   fs;     //文件系统
  memset(buff,0,BUFF_SIZE);
  f_mount(0, &fs);                                                              //挂载文件系统
  res = f_open(&fdst, "0:/Smtcar.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);    //打开文件如果存在就替换，如果没有就创建，带读写打开//FA_OPEN_ALWAYS(如果存在就保持)FA_CREATE_ALWAYS
  if( res == FR_DISK_ERR)
  {
    printf( "\n没插入SD卡??\n" );
    return;
  }
  else if ( res == FR_OK )
  {
    printf( "\n文件打开成功 \r\n" );
  }
  else
  {
    printf("\n返回值异常");
    return;
  }

  while(1)
  {
    if(PTC1_I==1)     //如果第二个按键没有按下是实时采集处理程序
    {
      if(PTA6_I == 0)
      {
        LPLD_GPIO_ClearIntFlag(PORTC);
        Stop_Flag=0;
      }
      Picture_get();
      if(PTC0_I==0 && Stop_Flag == 0)
      {
        f_write(&fdst,&buff,BUFF_SIZE,&size);
        synchronous=1;
      }
      Find_Line(Image);
      Get_Center_Line();
      Direction_Control();
      Speed_Control();
      Send_Data[0] = Get_L_Speed;
      Send_Data[1] = Set_L_Speed;
      Send_Data[2] = Get_R_Speed;
      Send_Data[3] = Set_R_Speed;
      OutPut_Data();
      if(Stop_Flag == 1)
      {
         GUI_Menu(Image);
         if(synchronous==1)
         {
           f_sync(&fdst);                                              //刚才写入了数据，实际上数据并没真正完成写入，需要调用此函数同步或者关闭文件，才会真正写入
           synchronous=2;
         }
      }
      LPLD_GPIO_Toggle_b(PTE,24);
    }
    else       //如果第二个按键按下则处理的是SD卡中的程序
    {
      size = f_size(&fdst);                                       //获取文件的大小
      frame=size/(BUFF_SIZE);                                     //根据文件大小和每帧数据大小确定出共有多少帧
      f_lseek(&fdst, (Which_pic*BUFF_SIZE));                      //移动文件指针
      f_read (&fdst,buff,(BUFF_SIZE), &sizetmp);                  //读取

      for(uint8 jj=0;jj<V;jj++)
        for(uint8 l=0;l<H;l++)
          Image_show[jj][l]=buff[jj*H+l];
      if(printf_flag==0)
        printf("开始分析第 %d帧图片\r\n",Which_pic);
      Find_Line(Image_show);
      printf_flag=1;
      Get_Center_Line();
      GUI_Menu(Image_show);
      LPLD_GPIO_Toggle_b(PTE,24);
    }
  }
}
