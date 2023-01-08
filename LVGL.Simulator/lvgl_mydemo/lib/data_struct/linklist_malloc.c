
#include "linklist_malloc.h"

#if DEF_TEST_MALLOC_LINKLIST
#include <string.h>
#endif

// define
#define DEF_NODE_P_SIZE          (sizeof(_node_p_ *) + sizeof(_node_p_ *))
#define DEF_PREV_NODE_OFFS(ll_p) (ll_p->size)
#define DEF_NEXT_NODE_OFFS(ll_p) (ll_p->size + sizeof(_node_p_ *))

static void list_node_p_set_prev(linklist2_t *p, _node_p_ *act, _node_p_ *prev);
static void list_node_p_set_next(linklist2_t *p, _node_p_ *act, _node_p_ *next);

// doubel linklist  code

void list_init_2(linklist2_t *p, uint32_t size) {
#ifdef DEF_BYTE_ALIGN_8
    size = (size + 7) & (~0x7);
#else
    size = (size + 3) & (~0x3);
#endif
    p->head = NULL;
    p->tail = NULL;
    p->size = size;
}

void *list_ins_head_2(linklist2_t *list) {
    _node_p_ *new_node = DEF_MALLOC(list->size + DEF_NODE_P_SIZE);

    if (new_node == NULL) return NULL;

    list_node_p_set_prev(list, new_node, NULL);
    list_node_p_set_next(list, new_node, list->head);

    if (list->head != NULL) {
        list_node_p_set_prev(list, list->head, new_node);
    }

    list->head = new_node;
    if (list->tail == NULL) {
        list->tail = new_node;
    }
    return new_node;
}

void *list_ins_prev_2(linklist2_t *list, void *act) {
    _node_p_ *new_node;

    if (list == NULL || act == NULL) return NULL;

    if (list_get_head_2(list) == act) {
        new_node = list_ins_head_2(list);
        if (new_node == NULL) return NULL;
    } else {
        new_node = DEF_MALLOC(list->size + DEF_NODE_P_SIZE);
        if (new_node == NULL) return NULL;

        _node_p_ *prev;
        prev = list_get_prev_2(list, act);
        list_node_p_set_next(list, prev, new_node);
        list_node_p_set_prev(list, new_node, prev);
        list_node_p_set_prev(list, act, new_node);
        list_node_p_set_next(list, new_node, act);
    }

    return new_node;
}

void *list_ins_tail_2(linklist2_t *list) {
    _node_p_ *new_node;

    new_node = DEF_MALLOC(list->size + DEF_NODE_P_SIZE);

    if (new_node == NULL) return NULL;

    list_node_p_set_next(list, new_node, NULL);
    list_node_p_set_prev(list, new_node, list->tail);

    if (list->tail != NULL) {
        list_node_p_set_next(list, list->tail, new_node);
    }

    list->tail = new_node;
    if (list->head == NULL) {
        list->head = new_node;
    }

    return new_node;
}

void list_remove_node_2(linklist2_t *list, void *node) {
    if (node == NULL) return;

    if (list_get_head_2(list) == node) {
        list->head = list_get_next_2(list, node);
        if (list->head == NULL) {
            list->tail = NULL;
        } else {
            list_node_p_set_prev(list, list->head, NULL);
        }
    } else if (list_get_tail_2(list) == node) {
        list->tail = list_get_prev_2(list, node);
        if (list->tail == NULL) {
            list->head = NULL;
        } else {
            list_node_p_set_next(list, list->tail, NULL);
        }
    } else {
        _node_p_ *prev = list_get_prev_2(list, node);
        _node_p_ *next = list_get_next_2(list, node);

        list_node_p_set_next(list, prev, next);
        list_node_p_set_prev(list, next, prev);
    }
}

void list_clear_2(linklist2_t *list) {
    void *i;
    void *i_next;

    i      = list_get_head_2(list);
    i_next = NULL;

    while (i != NULL) {
        i_next = list_get_next_2(list, i);

        list_remove_node_2(list, i);
        DEF_FREE(i);

        i = i_next;
    }
}

void list_chg_list_2(linklist2_t *ll_ori_p, linklist2_t *ll_new_p, void *node, bool head) {
    list_remove_node_2(ll_ori_p, node);

    if (head) {
        list_node_p_set_prev(ll_new_p, node, NULL);
        list_node_p_set_next(ll_new_p, node, ll_new_p->head);

        if (ll_new_p != NULL) {
            list_node_p_set_prev(ll_new_p, ll_new_p->head, node);
        }

        ll_new_p->head = node;
        if (ll_new_p->tail == NULL) {
            ll_new_p->tail = node;
        }
    } else {
        // set node as tail
        list_node_p_set_next(ll_new_p, node, NULL);
        list_node_p_set_prev(ll_new_p, node, ll_new_p->tail);

        if (ll_new_p->tail != NULL) {
            list_node_p_set_next(ll_new_p, ll_new_p->tail, node);
        }

        ll_new_p->tail = node;
        if (ll_new_p->head == NULL) {
            ll_new_p->head = node;
        }
    }
}

void *list_get_head_2(const linklist2_t *list) {
    if (list == NULL) return NULL;
    return list->head;
}

void *list_get_tail_2(const linklist2_t *list) {
    if (list == NULL) return NULL;
    return list->tail;
}

void *list_get_next_2(const linklist2_t *list, const void *n_act) {
    if (list == NULL || n_act == NULL) return NULL;
    const _node_p_ *next_n = n_act;
    next_n += DEF_NEXT_NODE_OFFS(list);
    return *((_node_p_ **)next_n);
}

void *list_get_prev_2(const linklist2_t *list, const void *n_act) {
    if (list == NULL || n_act == NULL) return NULL;
    const _node_p_ *prev_n = n_act;
    prev_n += DEF_PREV_NODE_OFFS(list);
    return *((_node_p_ **)prev_n);
}

uint32_t list_get_len_2(const linklist2_t *list) {
    uint32_t len = 0;
    void    *node;

    for (node = list_get_head_2; node != NULL; node = list_get_next_2(list, node)) {
        len++;
    }
    return len;
}

bool list_is_empty_2(linklist2_t *list) {
    if (list == NULL) return true;

    if (list->head == NULL && list->tail == NULL) return true;

    return false;
}

/* staic */
static void list_node_p_set_prev(linklist2_t *p, _node_p_ *act, _node_p_ *prev) {
    if (act == NULL) return;

    uint8_t *act8 = (uint8_t *)act;

    act8 += DEF_PREV_NODE_OFFS(p);

    _node_p_ **act_node_p_p  = (_node_p_ **)act8;
    _node_p_ **prev_node_p_p = (_node_p_ **)&prev;

    *act_node_p_p            = *prev_node_p_p;
}

static void list_node_p_set_next(linklist2_t *p, _node_p_ *act, _node_p_ *next) {
    if (act == NULL) return;

    uint8_t *act8 = (uint8_t *)act;

    act8 += DEF_NEXT_NODE_OFFS(p);

    _node_p_ **act_node_p_p  = (_node_p_ **)act8;
    _node_p_ **next_node_p_p = (_node_p_ **)&next;
    *act_node_p_p            = *next_node_p_p;
}

/***************************** test ******************************************/
#if DEF_TEST_MALLOC_LINKLIST

// test
typedef struct info {
    int   id;
    int   age;
    char  name[16];
    float soc;
} INFO;

typedef struct image {
    int id;
    int w;
    int h;
    struct pos {
        int x;
        int y;
    };
} IMAGE;

linklist2_t list_info;
linklist2_t list_image;

//
int main() {
    list_init_2(&list_info, sizeof(struct info));
    list_init_2(&list_image, sizeof(struct image));

    // info
    INFO *info1 = list_ins_head_2(&list_info);
    INFO *info2 = list_ins_head_2(&list_info);
    if (info1 != NULL) {
        info1->id  = 0;
        info1->age = 23;
        strcpy(info1->name, "ZhangSan");
        info1->soc = 19.3;
    }
    if (info2 != NULL) {
        info2->id  = 1;
        info2->age = 25;
        strcpy(info2->name, "LiSi");
        info2->soc = 56.2;
    }

    INFO *i;
    _LV_LL_READ(&list_info, i) {
        printf("id=%d, age=%d, name=%s, soc=%.2f\n", i->id, i->age, i->name, i->soc);
    }

    // image
    IMAGE *img1 = list_ins_head_2(&list_image);
    IMAGE *img2 = list_ins_head_2(&list_image);
    if (img1 != NULL) {
        img1->id = 0;
        img1->w  = 100;
        img1->h  = 101;
        img1->x  = 50;
        img1->y  = 51;
    }
    if (img2 != NULL) {
        img2->id = 1;
        img2->w  = 200;
        img2->h  = 201;
        img2->x  = 250;
        img2->y  = 251;
    }
    IMAGE *m;
    _LV_LL_READ_BACK(&list_image, m) {
        printf("id=%d, w=%d, h=%d, x=%d, y=%d\n", m->id, m->w, m->h, m->x, m->y);
    }

    return 0;
}

#endif
