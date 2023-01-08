#ifndef __STATIC_MALLOC_H__
#define __STATIC_MALLOC_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// define
#define DEF_BYTE_ALIGN_8       0
#define DEF_BYTE_ALIGN_4       1
// config
#define DEF_LIST_ALIGN_MODE    DEF_BYTE_ALIGN_8
#define DEF_STACK_MALLOC(size) malloc(size)
#define DEF_STACK_FREE(p)      free(p)
#define DEF_TEST_MALLOC_STACK  0

//
typedef uint8_t stack_node_p;

typedef struct _stack_node_ {
    stack_node_p *data;
    stack_node_p *next;
} stack_node;

typedef struct _stack_ {
    uint32_t    size;
    stack_node *head;
    stack_node *tail;
} stack_t;

void  StackMallocInit(stack_t *s, uint32_t size);
void *StackMallocPush(stack_t *s);
void  StackMallocPop(stack_t *s, void *data);

#endif