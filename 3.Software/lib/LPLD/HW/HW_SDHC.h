/**
 * @file HW_SDHC.h
 * @version 3.01[By LPLD]
 * @date 2013-10-21
 * @brief SDHC底层模块相关函数
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
 *
 * 版权说明:
 *  SDHC模块驱动程序摘取自飞思卡尔MQX底层驱动，部分功能由拉普兰德修改。
 *  HW_SDHC.h及HW_SDHC.c内的代码版权归飞思卡尔公司享有。
 */
#ifndef __HW_SDHC_H__
#define __HW_SDHC_H__

#define SDHC_CLOCK      g_core_clock

//SD卡初始数据传输需工作在低于400 kb／s的频率
#define     SDHC_INIT_BANDRATE      300000                  //启动时的波特率

#define     SDHC_BANDRATE           (30*1000*1000)          //正常工作时的波特率

#define     SDHC_BUS_WIDTH          ESDHC_BUS_WIDTH_4BIT    //带宽，目前代码仅支持 1bit 和 4bit


#define     SDCARD_BLOCK_SIZE       512                     //SD卡块大小

#define     SEND_OCR_TIME           100                     //发送 CMD55 + ACMD41 命令的次数


/* Error code returned by I/O functions */
#define IO_ERROR        (-1)

typedef enum
{
    ESDHC_IOCTL_INIT,
    ESDHC_IOCTL_SEND_CMD,
    ESDHC_IOCTL_GET_CARD,
    ESDHC_IOCTL_GET_BUS_WIDTH,
    ESDHC_IOCTL_SET_BUS_WIDTH,

} ESDHC_IOCTL_CMD_e;


/* 命令码用于 disk_ioctrl 函数 */
/* ESDHC 错误码 */
typedef enum
{
    ESDHC_IOCTL_OK,             //OK
    ESDHC_INIT_ERR,             //初始化失败
    ESDHC_CMD_ERR,              //发送命令出错
    ESDHC_CMD_TIMEOUT,          //发送命令超时
    ESDHC_CMD_INVALID,          //无效命令

    ESDHC_PARAM_INVALID,        //参数无效
    ESDHC_BUS_WIDTH_INVALID,    //总线宽度无效

    ESDHC_BUSY,                 //设备忙
} ESDHC_IOCTL_ERR_e;


/* ESDHC 总线宽度 */
typedef enum
{
    //值是根据寄存器 来配置的
    ESDHC_BUS_WIDTH_1BIT    = 0,
    ESDHC_BUS_WIDTH_4BIT    = 1,
    ESDHC_BUS_WIDTH_8BIT    = 2,
} ESDHC_BUS_WIDTH_e;

/* ESDHC 卡类型 */
typedef enum
{
    ESDHC_CARD_NONE                      ,//没有卡
    ESDHC_CARD_UNKNOWN                   ,//不能识别卡
    ESDHC_CARD_SD                        ,//SD
    ESDHC_CARD_SDHC                      ,//SDHC
    ESDHC_CARD_SDIO                      ,//SDIO
    ESDHC_CARD_SDCOMBO                   ,
    ESDHC_CARD_SDHCCOMBO                 ,
    ESDHC_CARD_MMC                       ,//MMC
    ESDHC_CARD_CEATA                     ,
} ESDHC_CARD_e;

/* ESDHC 命令类型 */
typedef enum
{
    ESDHC_TYPE_NORMAL                    ,
    ESDHC_TYPE_SUSPEND                   ,
    ESDHC_TYPE_RESUME                    ,
    ESDHC_TYPE_ABORT                     ,
} ESDHC_TYPE_e;

/* ESDHC 命令 */
#define ESDHC_CMD0                           (0)
#define ESDHC_CMD1                           (1)
#define ESDHC_CMD2                           (2)
#define ESDHC_CMD3                           (3)
#define ESDHC_CMD4                           (4)
#define ESDHC_CMD5                           (5)
#define ESDHC_CMD6                           (6)
#define ESDHC_CMD7                           (7)
#define ESDHC_CMD8                           (8)
#define ESDHC_CMD9                           (9)
#define ESDHC_CMD10                          (10)
#define ESDHC_CMD11                          (11)
#define ESDHC_CMD12                          (12)
#define ESDHC_CMD13                          (13)
#define ESDHC_CMD15                          (15)
#define ESDHC_CMD16                          (16)
#define ESDHC_CMD17                          (17)
#define ESDHC_CMD18                          (18)
#define ESDHC_CMD20                          (20)
#define ESDHC_CMD24                          (24)
#define ESDHC_CMD25                          (25)
#define ESDHC_CMD26                          (26)
#define ESDHC_CMD27                          (27)
#define ESDHC_CMD28                          (28)
#define ESDHC_CMD29                          (29)
#define ESDHC_CMD30                          (30)
#define ESDHC_CMD32                          (32)
#define ESDHC_CMD33                          (33)
#define ESDHC_CMD34                          (34)
#define ESDHC_CMD35                          (35)
#define ESDHC_CMD36                          (36)
#define ESDHC_CMD37                          (37)
#define ESDHC_CMD38                          (38)
#define ESDHC_CMD39                          (39)
#define ESDHC_CMD40                          (40)
#define ESDHC_CMD42                          (42)
#define ESDHC_CMD52                          (52)
#define ESDHC_CMD53                          (53)
#define ESDHC_CMD55                          (55)
#define ESDHC_CMD56                          (56)
#define ESDHC_CMD60                          (60)
#define ESDHC_CMD61                          (61)

#define ESDHC_ACMD6                          (6)
#define ESDHC_ACMD13                         (13)
#define ESDHC_ACMD22                         (22)
#define ESDHC_ACMD23                         (23)
#define ESDHC_ACMD41                         (41)
#define ESDHC_ACMD42                         (42)
#define ESDHC_ACMD51                         (51)




#define ESDHC_XFERTYP_RSPTYP_NO              (0x00)
#define ESDHC_XFERTYP_RSPTYP_136             (0x01)
#define ESDHC_XFERTYP_RSPTYP_48              (0x02)
#define ESDHC_XFERTYP_RSPTYP_48BUSY          (0x03)

#define ESDHC_XFERTYP_CMDTYP_ABORT           (0x03)

#define ESDHC_PROCTL_EMODE_INVARIANT         (0x02)

#define ESDHC_PROCTL_DTW_1BIT                (0x00)
#define ESDHC_PROCTL_DTW_4BIT                (0x01)
#define ESDHC_PROCTL_DTW_8BIT                (0x10)


typedef struct
{
    uint8  COMMAND;             //命令
    uint8  TYPE;                //命令类型 SDHC_XFERTYP_CMDTYP
    uint8  READ;
    uint8  RES;                 //保留
    uint32 ARGUMENT;            //命令的参数寄存器 (SDHC_CMDARG)
    uint32 BLOCKS;
    uint32 RESPONSE[4];

} ESDHC_CMD_t, *pESDHC_CMD_t;


/*!
 *  @brief      SD Card 结构信息
 */
typedef struct
{
    /* 实际卡状态                                   */
    uint32                CARD;

    /* 底层的响应超时 >= 250 ms                     */
    uint32               SD_TIMEOUT;

    /* 该设备的块数量                               */
    uint32                NUM_BLOCKS;

    /* 卡地址                                       */
    uint32                ADDRESS;

    /* 高容量 = 块寻址 (SD是字节寻址，SDHC是块寻址) */
    uint8                 SDHC;

    /* 规范2或更高版本的卡 = 不同的CSD寄存器        */
    uint8                 VERSION2;

    uint8                 RES[2];

} SDCARD_t, *pSDCARD_t;



extern SDCARD_t        SDHC_card;


extern void                 SDHC_init();                                        // SDHC 初始化
extern ESDHC_IOCTL_ERR_e    SDHC_ioctl(ESDHC_IOCTL_CMD_e  cmd, void *param_ptr); // SDHC 控制命令
extern void                 SDHC_set_baudrate(uint32 baudrate);                 // SDHC波特率配置
extern uint32               SDHC_cmd (pESDHC_CMD_t command);                    // SDHC发送cmd命令


#define SDHC_is_running()     (0 != (SDHC_PRSSTAT & (SDHC_PRSSTAT_RTA_MASK | SDHC_PRSSTAT_WTA_MASK | SDHC_PRSSTAT_DLA_MASK | SDHC_PRSSTAT_CDIHB_MASK | SDHC_PRSSTAT_CIHB_MASK)))

#define SDHC_STATUS_WAIT(MASK)      while (0 ==  (SDHC_IRQSTAT & (MASK)));
#define SDHC_STATUS_GET(MASK)       (SDHC_IRQSTAT & (MASK))

#endif /*__HW_SDHC_H__*/
