

#ifndef __QUEUE_MALLOC_H__
#define __QUEUE_MALLOC_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// config
#define DEF_TEST_MALLOC_QUEUE 0

//
typedef uint16_t val_size;          // 两个字节记录数据长度

typedef struct _queue_ {
    bool     init;
    uint8_t *buf;
    uint32_t bufSize;
    uint32_t front;
    uint32_t tail;
    uint32_t elemCnt;
    uint32_t usedSize;
} queue_t;

void     QueueMallocInit(queue_t *q, uint8_t *buf, uint32_t buf_size);
void    *QueueMallocInput(queue_t *q, void *val, val_size valSize);
void    *QueueMallocOutput(queue_t *q, val_size *size);
bool     QueueMallocSpaceChk(queue_t *q, val_size size);
void    *QueueMallocPeek(queue_t *q, val_size *size);
void    *QueueMallocPeekAfter(queue_t *q, void *cur_item, val_size *size);
void     QueueMallocClear(queue_t *q);
uint32_t QueueMallocGetCnt(queue_t *q);

#endif
