/**
 * @file DEV_DiskIO.h
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
#ifndef _DEV_DISKIO_H_
#define _DEV_DISKIO_H_

#include "common.h"

#define _READONLY	0	/* 1: 只读功能 */

/* 磁盘函数返回结果枚举类型 */
typedef enum {
  RES_OK = 0,		/* 0: 成功 */
  RES_ERROR,		/* 1: 读/写错误 */
  RES_WRPRT,		/* 2: 写保护 */
  RES_NOTRDY,		/* 3: 未准备好 */
  RES_PARERR,		/* 4: 参数错误 */
  RES_NONRSPNS          /* 5: 未响应 */
} DRESULT;


/* 磁盘函数返回状态 */
typedef int32	DSTATUS;
/* 磁盘状态(DSTATUS) */
#define STA_OK		        0x00	/* 状态正常 */
#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* LPLD_Disk_IOC函数控制命令 */
/* 通用命令(为磁盘文件系统FatFs定义) */
#define CTRL_SYNC		0	/* 刷新磁盘缓存 (针对写功能) */
#define GET_SECTOR_COUNT	1	/* 获得媒介大小 (仅针对 f_mkfs()) */
#define GET_SECTOR_SIZE		2	/* 获得扇区大小 (针对多扇区大小 (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE		3	/* 获得扇区擦除大小 (仅针对 f_mkfs()) */
#define CTRL_ERASE_SECTOR	4	/* 强制擦除一个扇区 (仅针对 _USE_ERASE) */

/* 通用命令 */
#define CTRL_POWER		5	/* 获取/设置电源状态 */
#define CTRL_LOCK		6	/* 锁定/解锁媒介移除 */
#define CTRL_EJECT		7	/* 弹出媒介 */

/* MMC/SDC特定ioctl命令 */
#define MMC_GET_TYPE		10	/* 获得卡类型 */
#define MMC_GET_CSD		11	/* 获得CSD */
#define MMC_GET_CID		12	/* 获得CID */
#define MMC_GET_OCR		13	/* 获得OCR */
#define MMC_GET_SDSTAT		14	/* 获得SD卡状态 */

/* ATA/CF特定ioctl命令 */
#define ATA_GET_REV		20	/* 获得F/W修正 */
#define ATA_GET_MODEL		21	/* 获得模型名字 */
#define ATA_GET_SN		22	/* 获得序列号 */

/* NAND特定ioctl命令 */
#define NAND_FORMAT		30	/* 创建物理格式 */


/* 磁盘控制函数声明 */
DSTATUS LPLD_Disk_Initialize(uint8);
DSTATUS LPLD_Disk_Status(uint8);
DRESULT LPLD_Disk_Read(uint8, uint8*, uint32, uint8);
#if _READONLY == 0
DRESULT LPLD_Disk_Write(uint8, const uint8*, uint32, uint8);
#endif
DRESULT LPLD_Disk_IOC(uint8, uint8, void*);


#endif /*_DEV_DISKIO_H_*/
