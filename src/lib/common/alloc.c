/**
 * @file alloc.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief 通用 malloc()、free() 函数
 *
 * 更改建议:不建议修改
 *
 * 该代码提供系统内存分配和释放函数。
 * 用户可选片内或片外内存，片外内存需FlexBus支持。
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
#include "stdlib.h"

#if defined(__IAR_SYSTEMS_ICC__)
  #pragma section = "HEAP"
#endif

/********************************************************************/

/*
 * This struct forms the minimum block size which is allocated, and
 * also forms the linked list for the memory space used with alloc()
 * and free().  It is padded so that on a 32-bit machine, all malloc'ed
 * pointers are 16-byte aligned.
 */
typedef struct ALLOC_HDR
{
    struct
    {
        struct ALLOC_HDR     *ptr;
        unsigned int size;
    } s;
    unsigned int align;
    unsigned int pad;
} ALLOC_HDR;

static ALLOC_HDR base;
static ALLOC_HDR *freep = NULL;

/********************************************************************/
void free (void *ap)
{
    ALLOC_HDR *bp, *p;

    bp = (ALLOC_HDR *)ap - 1;   /* point to block header */
    for (p = freep; !((bp > p) && (bp < p->s.ptr)) ; p = p->s.ptr)
    {
        if ((p >= p->s.ptr) && ((bp > p) || (bp < p->s.ptr)))
        {
            break; /* freed block at start or end of arena */
        }
    }

    if ((bp + bp->s.size) == p->s.ptr)
    {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
    {
        bp->s.ptr = p->s.ptr;
    }

    if ((p + p->s.size) == bp)
    {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    }
    else
    {
        p->s.ptr = bp;
    }

    freep = p;
}

/********************************************************************/
void * malloc (unsigned nbytes)
{
    /* Get addresses for the HEAP start and end */
    #if defined(__IAR_SYSTEMS_ICC__)
      char* __HEAP_START = __section_begin("HEAP");
      char* __HEAP_END = __section_end("HEAP");
    #else
      #warning 非IAR编译器需确定HEAP起始结束地址
      extern char __HEAP_START;
      extern char __HEAP_END[];
    #endif
   
    ALLOC_HDR *p, *prevp;
    unsigned nunits;

    nunits = ((nbytes+sizeof(ALLOC_HDR)-1) / sizeof(ALLOC_HDR)) + 1;

    if ((prevp = freep) == NULL)
    {
        p = (ALLOC_HDR *)__HEAP_START;
        p->s.size = ( ((uint32)__HEAP_END - (uint32)__HEAP_START)
            / sizeof(ALLOC_HDR) );
        p->s.ptr = &base;
        base.s.ptr = p;
        base.s.size = 0;
        prevp = freep = &base;
    }

    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
    {
        if (p->s.size >= nunits)
        {
            if (p->s.size == nunits)
            {
                prevp->s.ptr = p->s.ptr;
            }
            else
            {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }

        if (p == freep)
            return NULL;
    }
}

/********************************************************************/
