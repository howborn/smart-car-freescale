/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Smart_Car.c
 * @version 0.0
 * @date 2015-2-7
 * @brief ���ڸù��̵�����
 *
 * ��Ȩ����:�������칤ҵѧԺ ��ʮ���˼����  ����ͷ2�� 
 * Ӳ��ƽ̨:  MK60FX512
 * 
 *�߼���˵��
 *��һ�����뿪��ȷ���������ܵĹ������Ƿ��ͼƬ�洢��SD����
 *Ȼ������ڶ������뿪�ز���ȥ˵�����ݵ���Դ��SD���е����ݣ����Ծ����ε�ͼ��ʵʱ����Ĳ���
 *�����ֲ�һ���ĵط�ֻ��ͼ�����ݵ���Դ
 *ʵʱ�������ȵõ�ͼ��Ȼ�����ߣ��ҿ��Ƶ����ߣ�������ƣ��ٶȿ��ƣ��ٶȺ������ٶȱ궨��Ȼ������ʾ�������ͺ�����
 *SD���������֣�ǰ������FatFsϵͳ�Ķ���������ζ�ȡͼƬ��Ҳ����ƽ�����Բ��֣�ԭ����ǳ�����Ļ��ˢ��ʱ��Ƚ϶̣�
   ��������һ���ŷ�ͼƬȻ����ͼƬ�������ʱ���ð������Ʒ��͵��������������ڷ��������õ������ⲿ�ֻ�ܿ����Ӧ�ġ�
 *�������������
 */
#include "common.h"
#include "global.h"
#include "control.h"
#include "interrupt.h"
#include "init.h"
#include "gui.h"
#include "analyze.h"
#include "TFT.h"

//�ļ�ϵͳ�ļ�
uint32 size, sizetmp;
int16  res,synchronous=0;

//����ʾ��������
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
//����ʾ������װ���ͺ���
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

//������
void main (void)
{
  DisableInterrupts;                  //�ر�ȫ���ж�
  Init();                             //ϵͳ��ʼ��
  EnableInterrupts;                   //ʹ��ȫ���ж�
  
  FIL     fdst;   //�ļ�
  FATFS   fs;     //�ļ�ϵͳ
  memset(buff,0,BUFF_SIZE);
  f_mount(0, &fs);                                                              //�����ļ�ϵͳ
  res = f_open(&fdst, "0:/Smtcar.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);    //���ļ�������ھ��滻�����û�оʹ���������д��//FA_OPEN_ALWAYS(������ھͱ���)FA_CREATE_ALWAYS
  if( res == FR_DISK_ERR)
  {
    printf( "\nû����SD��??\n" );
    return;
  }
  else if ( res == FR_OK )
  {
    printf( "\n�ļ��򿪳ɹ� \r\n" );
  }
  else
  {
    printf("\n����ֵ�쳣");
    return;
  }

  while(1)
  {
    if(PTC1_I==1)     //����ڶ�������û�а�����ʵʱ�ɼ��������
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
           f_sync(&fdst);                                              //�ղ�д�������ݣ�ʵ�������ݲ�û�������д�룬��Ҫ���ô˺���ͬ�����߹ر��ļ����Ż�����д��
           synchronous=2;
         }
      }
      LPLD_GPIO_Toggle_b(PTE,24);
    }
    else       //����ڶ������������������SD���еĳ���
    {
      size = f_size(&fdst);                                       //��ȡ�ļ��Ĵ�С
      frame=size/(BUFF_SIZE);                                     //�����ļ���С��ÿ֡���ݴ�Сȷ�������ж���֡
      f_lseek(&fdst, (Which_pic*BUFF_SIZE));                      //�ƶ��ļ�ָ��
      f_read (&fdst,buff,(BUFF_SIZE), &sizetmp);                  //��ȡ
      
      for(uint8 jj=0;jj<V;jj++)
        for(uint8 l=0;l<H;l++)
          Image_show[jj][l]=buff[jj*H+l];
      if(printf_flag==0)
        printf("��ʼ������ %d֡ͼƬ\r\n",Which_pic);
      Find_Line(Image_show);
      printf_flag=1;
      Get_Center_Line();
      GUI_Menu(Image_show);
      LPLD_GPIO_Toggle_b(PTE,24);
    }
  }
}