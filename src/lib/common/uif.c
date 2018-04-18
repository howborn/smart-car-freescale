/**
 * @file uif.c
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

#include "common.h"
#include "uif.h"

/********************************************************************/
/*
 * Global messages -- constant strings
 */
const int8 HELPMSG[] =
    "Enter 'help' for help.\n";

const int8 INVARG[] =
    "Error: Invalid argument: %s\n";

const int8 INVALUE[] = 
    "Error: Invalid value: %s\n";

/*
 * Strings used by this file only
 */
static const int8 INVCMD[] =
    "Error: No such command: %s\n";

static const int8 HELPFORMAT[] = 
    "%8s  %-25s %s %s\n";

static const int8 SYNTAX[] = 
    "Error: Invalid syntax for: %s\n";

static const int8 INVOPT[] = 
    "Error:  Invalid set/show option: %s\n";

static const int8 OPTFMT[] = 
    "%12s: ";

static int8 cmdline1 [UIF_MAX_LINE];
static int8 cmdline2 [UIF_MAX_LINE];

/********************************************************************/
int8 *get_line (int8 *line)
{
    int32 pos;
    int32 ch;

    pos = 0;
    ch = (int32)in_char();
    while ( (ch != 0x0D /* CR */) &&
            (ch != 0x0A /* LF/NL */) &&
            (pos < UIF_MAX_LINE))
    {
        switch (ch)
        {
            case 0x08:      /* Backspace */
            case 0x7F:      /* Delete */
                if (pos > 0)
                {
                    pos -= 1;
                    out_char(0x08);    /* backspace */
                    out_char(' ');
                    out_char(0x08);    /* backspace */
                }
                break;
            default:
                if ((pos+1) < UIF_MAX_LINE)
                {
                    if ((ch > 0x1f) && (ch < 0x80))
                    {
                        line[pos++] = (int8)ch;
                        out_char((int8)ch);
                    }
                }
                break;
        }
        ch = (int32)in_char();
    }
    line[pos] = '\0';
    out_char(0x0D);    /* CR */
    out_char(0x0A);    /* LF */

    return line;
}

/********************************************************************/
int32 make_argv (int8 *cmdline, int8 *argv[])
{
    int32 argc, i, in_text;

    /* 
     * Break cmdline into strings and argv
     * It is permissible for argv to be NULL, in which case
     * the purpose of this routine becomes to count args
     */
    argc = 0;
    i = 0;
    in_text = FALSE;
    while (cmdline[i] != '\0')  /* getline() must place 0x00 on end */
    {
        if (((cmdline[i] == ' ')   ||
             (cmdline[i] == '\t')) )
        {
            if (in_text)
            {
                /* end of command line argument */
                cmdline[i] = '\0';
                in_text = FALSE;
            }
            else
            {
                /* still looking for next argument */
                
            }
        }
        else
        {
            /* got non-whitespace character */
            if (in_text)
            {
            }
            else
            {
                /* start of an argument */
                in_text = TRUE;
                if (argc < UIF_MAX_ARGS)
                {
                    if (argv != NULL)
                        argv[argc] = &cmdline[i];
                    argc++;
                }
                else
                    /*return argc;*/
                    break;
            }

        }
        i++;    /* proceed to next character */
    }
    if (argv != NULL)
        argv[argc] = NULL;
    return argc;
}

/********************************************************************/
void run_cmd (void)
{
    /*
     * Global array of pointers to emulate C argc,argv interface
     */
    int32 argc;
    int8 *argv[UIF_MAX_ARGS + 1];   /* one extra for null terminator */

    get_line(cmdline1);

    if (!(argc = make_argv(cmdline1,argv)))
    {
        /* no command entered, just a blank line */
        strcpy(cmdline1,cmdline2);
        argc = make_argv(cmdline1,argv);
    }
    cmdline2[0] = '\0';

    if (argc)
    {
        int32 i;
        for (i = 0; i < UIF_NUM_CMD; i++)
        {
            if (strcasecmp(UIF_CMDTAB[i].cmd,argv[0]) == 0)
            {
                if (((argc-1) >= UIF_CMDTAB[i].min_args) &&
                    ((argc-1) <= UIF_CMDTAB[i].max_args))
                {
                    if (UIF_CMDTAB[i].flags & UIF_CMD_FLAG_REPEAT)
                    {
                        strcpy(cmdline2,argv[0]);
                    }
                    UIF_CMDTAB[i].func(argc,argv);
                    return;
                }
                else
                {
                    printf(SYNTAX,argv[0]);
                    return;
                }
            }
        }
        printf(INVCMD,argv[0]);
        printf(HELPMSG);
    }
}
/********************************************************************/
uint32 get_value (int8 *s, int32 *success, int32 base)
{
    uint32 value;
    int8 *p;

    value = strtoul(s,&p,base);
    if ((value == 0) && (p == s))
    {
        *success = FALSE;
        return 0;
    }
    else
    {
        *success = TRUE;
        return value;
    }
}
/********************************************************************/
void uif_cmd_help (int32 argc, int8 **argv)
{
    int32 index;
    
    (void)argc;
    (void)argv;
    
    printf("\n");
    for (index = 0; index < UIF_NUM_CMD; index++)
    {
        printf(HELPFORMAT,
            UIF_CMDTAB[index].cmd,
            UIF_CMDTAB[index].description,
            UIF_CMDTAB[index].cmd,
            UIF_CMDTAB[index].syntax);
    }
    printf("\n");
}
/********************************************************************/
void uif_cmd_set (int32 argc, int8 **argv)
{
    int32 index;

    printf("\n");
    if (argc == 1)
    {
        printf("Valid 'set' options:\n");
        for (index = 0; index < UIF_NUM_SETCMD; ++index)
        {
            printf(OPTFMT,UIF_SETCMDTAB[index].option);
            printf("%s\n",UIF_SETCMDTAB[index].syntax);
        }
        printf("\n");
        return;
    }

    if (argc != 3)
    {
        printf("Error: Invalid argument list\n");
        return;
    }

    for (index = 0; index < UIF_NUM_SETCMD; index++)
    {
        if (strcasecmp(UIF_SETCMDTAB[index].option,argv[1]) == 0)
        {
            if (((argc-1-1) >= UIF_SETCMDTAB[index].min_args) &&
                ((argc-1-1) <= UIF_SETCMDTAB[index].max_args))
            {
                UIF_SETCMDTAB[index].func(argc,argv);
                return;
            }
            else
            {
                printf(INVARG,argv[1]);
                return;
            }
        }
    }
    printf(INVOPT,argv[1]);
}

/********************************************************************/
void uif_cmd_show (int32 argc, int8 **argv)
{
    int32 index;

    printf("\n");
    if (argc == 1)
    {
        /*
         * Show all Option settings
         */
        argc = 2;
        argv[2] = NULL;
        for (index = 0; index < UIF_NUM_SETCMD; index++)
        {
            printf(OPTFMT,UIF_SETCMDTAB[index].option);
            UIF_SETCMDTAB[index].func(argc,argv);
            printf("\n");
        }
        printf("\n");
        return;
    }

    for (index = 0; index < UIF_NUM_SETCMD; index++)
    {
        if (strcasecmp(UIF_SETCMDTAB[index].option,argv[1]) == 0)
        {
            if (((argc-1-1) >= UIF_SETCMDTAB[index].min_args) &&
                ((argc-1-1) <= UIF_SETCMDTAB[index].max_args))
            {
                printf(OPTFMT,UIF_SETCMDTAB[index].option);
                UIF_SETCMDTAB[index].func(argc,argv);
                printf("\n\n");
                return;
            }
            else
            {
                printf(INVARG,argv[1]);
                return;
            }
        }
    }
    printf(INVOPT,argv[1]);
}

/********************************************************************/
