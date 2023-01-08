

#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// define
#define DEF_BYTE_ALIGN_8         0
#define DEF_BYTE_ALIGN_4         1
// config
#define DEF_LIST_ALIGN_MODE      DEF_BYTE_ALIGN_8
#define DEF_MALLOC(s)            malloc(s)
#define DEF_FREE(s)              free(s)
#define DEF_TEST_MALLOC_LINKLIST 0

//
typedef uint8_t _node_p_;

typedef struct _linklist2 {
    uint32_t  size;
    _node_p_ *head;
    _node_p_ *tail;
} linklist2_t;

void     list_init_2(linklist2_t *p, uint32_t size);
void    *list_ins_head_2(linklist2_t *list);
void    *list_ins_prev_2(linklist2_t *list, void *act);
void    *list_ins_tail_2(linklist2_t *list);
void     list_remove_node_2(linklist2_t *list, void *node);
void     list_clear_2(linklist2_t *list);
void     list_chg_list_2(linklist2_t *ll_ori_p, linklist2_t *ll_new_p, void *node, bool head);
void    *list_get_head_2(const linklist2_t *list);
void    *list_get_tail_2(const linklist2_t *list);
void    *list_get_next_2(const linklist2_t *list, const void *n_act);
void    *list_get_prev_2(const linklist2_t *list, const void *n_act);
uint32_t list_get_len_2(const linklist2_t *list);
bool     list_is_empty_2(linklist2_t *list);

#define _LV_LL_READ(list, i)      for (i = list_get_head_2(list); i != NULL; i = list_get_next_2(list, i))
#define _LV_LL_READ_BACK(list, i) for (i = list_get_tail_2(list); i != NULL; i = list_get_prev_2(list, i))

#endif