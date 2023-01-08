
#include "stack_malloc.h"
#if DEF_TEST_MALLOC_STACK
#include <string.h>
#endif

#define DEF_STACK_NODE_NEXT_OFFS(s) (s->size)
#define DEF_STACK_NODE_P_SIZE       (sizeof(stack_node_p *))
#define DEF_STACK_NODE_SIZE(s)      (s->size + sizeof(stack_node_p *))

static void StackMallocSetNext(stack_t *s, stack_node *act, stack_node *next) {
    uint8_t *act8 = (uint8_t *)act;
    act8 += DEF_STACK_NODE_NEXT_OFFS(s);

    stack_node_p **act_node_next = (stack_node_p **)act8;
    stack_node_p **next_node_p   = (stack_node_p **)&next;

    *act_node_next               = *next_node_p;
}

void StackMallocInit(stack_t *s, uint32_t size) {
    if (s == NULL) return;

#ifdef DEF_BYTE_ALIGN_8
    size = (size + 7) & (~0x7);
#else
    size = (size + 3) & (~0x3);
#endif

    s->size = size;
    if (s != NULL) {
        s->head = DEF_STACK_MALLOC(DEF_STACK_NODE_SIZE(s));
        s->tail = s->head;
        if (s->head != NULL) {
            StackMallocSetNext(s, s->head, NULL);
        }
    }
}

void *StackMallocGetNext(stack_t *s) {
    const stack_node_p *head = (stack_node_p *)s->head;
    head += DEF_STACK_NODE_NEXT_OFFS(s);
    return *((stack_node_p **)head);
}

void *StackMallocPush(stack_t *s) {
    stack_node *new_node;

    new_node = DEF_STACK_MALLOC(s->size + DEF_STACK_NODE_P_SIZE);

    if (new_node != NULL) {
        StackMallocSetNext(s, new_node, s->head);
        s->head = new_node;
    }

    return new_node;
}

bool StackMallocEmpty(stack_t *s) {
    if (s->head == s->tail) {
        return true;
    } else {
        return false;
    }
}

void StackMallocPop(stack_t *s, void *data) {
    stack_node *next_node;
    stack_node *old_node;

    if (StackMallocEmpty(s)) {
        return;
    }

    old_node = s->head;
    s->head  = StackMallocGetNext(s);

    if (data != NULL) {
        memcpy(data, old_node, DEF_STACK_NODE_SIZE(s));
    }
    DEF_STACK_FREE(old_node);
}

/****************************** test ***************************************/
#if DEF_TEST_MALLOC_STACK

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

stack_t stack_info;
stack_t stack_image;
//
int main() {
    StackMallocInit(&stack_info, sizeof(INFO));
    StackMallocInit(&stack_image, sizeof(IMAGE));

    // info
    INFO *info1 = StackMallocPush(&stack_info);
    INFO *info2 = StackMallocPush(&stack_info);
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

    while (!StackMallocEmpty(&stack_info)) {
        INFO i;
        StackMallocPop(&stack_info, &i);
        printf("id=%d, age=%d, name=%s, soc=%.2f\n", i.id, i.age, i.name, i.soc);
    }

    // image
    IMAGE *img1 = StackMallocPush(&stack_image);
    IMAGE *img2 = StackMallocPush(&stack_image);
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

    while (!StackMallocEmpty(&stack_image)) {
        IMAGE m;
        StackMallocPop(&stack_image, &m);
        printf("id=%d, w=%d, h=%d, x=%d, y=%d\n", m.id, m.w, m.h, m.x, m.y);
    }
    return 0;
}
#endif
