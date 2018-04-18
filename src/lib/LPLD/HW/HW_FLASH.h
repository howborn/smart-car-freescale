/**
 * @file HW_FLASH.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief FTFL底层模块相关函数
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
#ifndef _HW_FLASH_H_
#define _HW_FLASH_H_

/* 错误代码定义 */
#define FLASH_OK                    0x00
#define FLASH_FACCERR               0x01
#define FLASH_FPVIOL                0x02
#define FLASH_MGSTAT0               0x04
#define FLASH_RDCOLERR              0x08
#define FLASH_NOT_ERASED            0x10
#define FLASH_CONTENTERR            0x11

/* flash命令 */
#define FlashCmd_ProgramPhrase      0x07
#define FlashCmd_SectorErase        0x09
#define FlashCmd_ProgramSection     0x0B
#define BYTE_DIV    (8)

void LPLD_Flash_Init(void);
uint8 LPLD_Flash_SectorErase(uint32);
uint8 LPLD_Flash_ByteProgram(uint32, uint32*, uint32);

#endif /*_HW_FLASH_H_*/

