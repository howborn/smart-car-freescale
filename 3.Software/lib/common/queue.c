/**
 * @file queue.c
 * @version 3.0[By LPLD]
 * @date 2013-06-18
 * @brief 实现先进先出链表
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
#include "queue.h"

/********************************************************************/
/* 
 * Initialize the specified queue to an empty state
 * 
 * Parameters:
 *  q   Pointer to queue structure
 */
void queue_init(QUEUE *q)
{
    q->head = NULL;
}
/********************************************************************/
/* 
 * Check for an empty queue
 *
 * Parameters:
 *  q       Pointer to queue structure
 * 
 * Return Value:
 *  1 if Queue is empty
 *  0 otherwise
 */
int32 queue_isempty(QUEUE *q)
{
    return (q->head == NULL);
}
/********************************************************************/
/* 
 * Add an item to the end of the queue 
 *
 * Parameters:
 *  q       Pointer to queue structure
 *  node    New node to add to the queue
 */
void queue_add(QUEUE *q, QNODE *node)
{
    if (queue_isempty(q))
    {
        q->head = q->tail = node;
    }
    else
    {
        q->tail->next = node;
        q->tail = node;
    }
    
    node->next = NULL;
}

/********************************************************************/
/* 
 * Remove and return first (oldest) entry from the specified queue 
 *
 * Parameters:
 *  q       Pointer to queue structure
 *
 * Return Value:
 *  Node at head of queue - NULL if queue is empty
 */
QNODE* queue_remove(QUEUE *q)
{
    QNODE *oldest;
    
    if (queue_isempty(q))
        return NULL;
    
    oldest = q->head;
    q->head = oldest->next;
    return oldest;
}
/********************************************************************/
/* 
 * Peek into the queue and return pointer to first (oldest) entry.
 * The queue is not modified
 *
 * Parameters:
 *  q       Pointer to queue structure
 *
 * Return Value:
 *  Node at head of queue - NULL if queue is empty
 */
QNODE* queue_peek(QUEUE *q)
{
    return q->head;
}
/********************************************************************/
/* 
 * Move entire contents of one queue to the other
 *
 * Parameters:
 *  src     Pointer to source queue
 *  dst     Pointer to destination queue
 */
void queue_move(QUEUE *dst, QUEUE *src)
{
    if (queue_isempty(src))
        return;
    
    if (queue_isempty(dst))
        dst->head = src->head;
    else
        dst->tail->next = src->head;

    dst->tail = src->tail;
    src->head = NULL;
    return;
}
/********************************************************************/
