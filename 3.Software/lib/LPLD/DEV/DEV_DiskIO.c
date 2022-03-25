/**
 * @file DEV_DiskIO.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief 磁盘设备层相关接口函数，需调用SDHC底层驱动
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
#include "DEV_DiskIO.h"

//SD卡信息全局变量
extern SDCARD_STRUCT_PTR sdcard_ptr;

/*
 * LPLD_Disk_Initialize
 * 磁盘初始化
 * 
 * 参数:
 *    drv--物理磁盘号，只能为0
 *
 * 输出:
 *    RES_OK--成功
 *    RES_ERROR--读/写错误
 *    RES_WRPRT--写保护
 *    RES_NOTRDY--未准备好
 *    RES_PARERR--参数错误
 *    RES_NONRSPNS--未响应 
 */

DSTATUS LPLD_Disk_Initialize(uint8 drv)
{
  if(drv)
    return RES_PARERR;
 
  return LPLD_SDHC_InitCard();
}

/*
 * LPLD_Disk_Status
 * 返回磁盘状态
 * 
 * 参数:
 *    drv--物理磁盘号，只能为0
 *
 * 输出:
 *    RES_OK--成功
 *    RES_ERROR--读/写错误
 *    RES_WRPRT--写保护
 *    RES_NOTRDY--未准备好
 *    RES_PARERR--参数错误
 *    RES_NONRSPNS--未响应 
 */
DSTATUS LPLD_Disk_Status(uint8 drv)
{
  return sdcard_ptr->STATUS;
}

/*
 * LPLD_Disk_Read
 * 读磁盘的一个或多个扇区
 * 
 * 参数:
 *    drv--物理磁盘号，只能为0
 *    buff--数据读出所存的地址指针
 *    sector--扇区起始号
 *    count--所读的扇区数
 *
 * 输出:
 *    RES_OK--成功
 *    RES_ERROR--读/写错误
 *    RES_WRPRT--写保护
 *    RES_NOTRDY--未准备好
 *    RES_PARERR--参数错误
 *    RES_NONRSPNS--未响应 
 */
DRESULT LPLD_Disk_Read(uint8 drv, uint8* buff, uint32 sector, uint8 count)
{
  if(drv || (count == 0))
    return RES_PARERR;

  return LPLD_SDHC_ReadBlocks(buff, sector, count);
}

//如果磁盘系统为只读，则不编译以下函数
#if	_READONLY == 0
/*
 * LPLD_Disk_Write
 * 写磁盘的一个或多个扇区
 * 
 * 参数:
 *    drv--物理磁盘号，只能为0
 *    buff--数据读出所存的地址指针
 *    sector--扇区起始号
 *    count--所读的扇区数
 *
 * 输出:
 *    RES_OK--成功
 *    RES_ERROR--读/写错误
 *    RES_WRPRT--写保护
 *    RES_NOTRDY--未准备好
 *    RES_PARERR--参数错误
 *    RES_NONRSPNS--未响应 
 */

DRESULT LPLD_Disk_Write(uint8 drv, const uint8* buff, uint32 sector, uint8 count)
{
  if(drv || (count == 0))
    return RES_PARERR;
  
  return LPLD_SDHC_WriteBlocks((uint8*)buff, sector, count);
}
#endif

/*
 * LPLD_Disk_IOC
 * 磁盘功能控制函数
 * 
 * 参数:
 *    drv--物理磁盘号，只能为0
 *    ctrl--控制命令
 *    buff--IO操作所需数据的指针
 *
 * 输出:
 *    RES_OK--成功
 *    RES_ERROR--读/写错误
 *    RES_WRPRT--写保护
 *    RES_NOTRDY--未准备好
 *    RES_PARERR--参数错误
 *    RES_NONRSPNS--未响应 
 */
DRESULT LPLD_Disk_IOC(uint8 drv, uint8 ctrl, void* buff)
{
  DRESULT result = RES_OK;
  
  if(drv)
    return RES_PARERR;
  
  switch(ctrl)
  {
    case CTRL_SYNC:
      /*
      确定磁盘驱动已经完成写操作挂起的处理. 
      当磁盘IO模块有一个会写缓存，会立即擦除扇区. 该命令不能再只读模式使用.
      */
      // 在POLLING模式中，所有写操作已完成。
      break;
    case GET_SECTOR_SIZE:
      /*
      以WORD型指针变量的形式返回扇区大小.
      此命令不能用在可变扇区大小的配置, 
      _MAX_SS 为 512.
      */
      if(buff == NULL)
        result = RES_PARERR;
      else
        *(uint32*)buff = IO_SDCARD_BLOCK_SIZE;
      
      break;
    case GET_SECTOR_COUNT:
      /*
      以UINT32型指针变量的形式返回磁盘的可用扇区数. 
      该命令仅被f_mkfs函数调用以决定可创建多大的卷. 
      */
      if(buff == NULL)
        result = RES_PARERR;
      else
        *(uint32*)buff = sdcard_ptr->NUM_BLOCKS;
      break;
    case GET_BLOCK_SIZE:
      /*
      以UINT32类型的指针变量返回返回flash内存中擦除的扇区数.
      合法的数值为2的1至32768次方.
      返回1代表擦除大小或磁盘设备未知.
      该命令仅被f_mkfs函数调用并试图将擦除的扇区边界进行数据对齐.
      */
      result = RES_PARERR;
      break;
    case CTRL_ERASE_SECTOR:
      /*
      擦除由UINT32类型指针数组指定的flash内存,{<start sector>, <end sector>}.
      如果介质为非flash内存,则该命令无效.
      FatFs系统不会检查结果,如果擦除失败也不会影响文件函数.
      当_USE_ERASE为1时移动一个簇链会调用此命令.
      */
      result = RES_PARERR;
      break;
    default:
      return RES_PARERR;
    
  }
  return result;
}

