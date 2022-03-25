/**
 * @file uif.h
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief 提供简单的交互接口
 *
 * 更改建议:不建议修改
 *
 * The commands, set/show parameters, and prompt are configured 
 * at the project level
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


#ifndef _UIF_H_
#define _UIF_H_

/********************************************************************/

/*
 * Function prototypes
 */
int8 *get_line (int8 *);

uint32 get_value (int8 *, int32 *, int32);

void run_cmd (void);

int32 make_argv (int8 *, int8 **);

void uif_cmd_help (int32, int8 **);

void uif_cmd_set (int32, int8 **);

void uif_cmd_show (int32, int8 **);

/*
 * Maximum command line arguments
 */
#define UIF_MAX_ARGS    10

/*
 * Maximum length of the command line
 */
#define UIF_MAX_LINE    80

/*
 * The command table entry data structure
 */
typedef const struct
{
    int8 *  cmd;                    /* command name user types, ie. GO  */
    int32     min_args;               /* min num of args command accepts  */
    int32     max_args;               /* max num of args command accepts  */
    int32     flags;                  /* command flags (e.g. repeat)      */
    void    (*func)(int32, int8 **);  /* actual function to call          */
    int8 *  description;            /* brief description of command     */
    int8 *  syntax;                 /* syntax of command                */
} UIF_CMD;

/*
 * Prototype and macro for size of the command table
 */
extern UIF_CMD UIF_CMDTAB[];
extern const int32 UIF_NUM_CMD;
#define UIF_CMDTAB_SIZE             (sizeof(UIF_CMDTAB)/sizeof(UIF_CMD))

#define UIF_CMD_FLAG_REPEAT         0x1

/*
 * Macros for User InterFace command table entries
 */
#ifndef UIF_CMD_HELP
#define UIF_CMD_HELP    \
    {"help",0,1,0,uif_cmd_help,"Help","<cmd>"},
#endif

#ifndef UIF_CMD_SET
#define UIF_CMD_SET \
    {"set",0,2,0,uif_cmd_set,"Set Config","<option value>"},
#endif

#ifndef UIF_CMD_SHOW
#define UIF_CMD_SHOW    \
    {"show",0,1,0,uif_cmd_show,"Show Config","<option>"},
#endif

/*
 * Macro to include all standard user interface commands
 */
#define UIF_CMDS_ALL    \
    UIF_CMD_HELP        \
    UIF_CMD_SET         \
    UIF_CMD_SHOW

/*
 * The set/show table entry data structure
 */
typedef const struct
{
    int8 *  option;
    int32     min_args;
    int32     max_args;
    void    (*func)(int32, int8 **);
    int8 *  syntax;
} UIF_SETCMD;

/*
 * Prototype and macro for size of the table
 */
extern UIF_SETCMD UIF_SETCMDTAB[];
extern const int32 UIF_NUM_SETCMD;
#define UIF_SETCMDTAB_SIZE      (sizeof(UIF_SETCMDTAB)/sizeof(UIF_SETCMD))

/*
 * Strings defined in uif.c that may be useful to external functions
 */
extern const int8 HELPMSG[];
extern const int8 INVARG[];
extern const int8 INVALUE[];

/********************************************************************/

#endif /* _UIF_H_ */
