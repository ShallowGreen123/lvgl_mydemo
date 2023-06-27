#include "widgets.h"

#define DEF_ITEM_SPACING 10

static TASKBAR_ITEM_T *TaskbarFindItem(TASKBAR_T *tb, uint32_t id)
{
    TASKBAR_ITEM_T *i;
    _LV_LL_READ(&tb->ItemList, i)
    {
        if (i->id == id) {
            return i;
        }
    }
    return NULL;
}
static void TaskbarLayout(TASKBAR_T *tb)
{
    TASKBAR_ITEM_T *i;
    lv_obj_t       *prev_obj = NULL;

    _LV_LL_READ(&tb->ItemList, i)
    {
        // printf("id=%d, obj=%p\n", i->id, i->Obj);
        if (prev_obj == NULL) {
            lv_obj_align_to(i->Obj, tb->Obj, LV_ALIGN_RIGHT_MID, -DEF_ITEM_SPACING, 0);
            prev_obj = i->Obj;
            continue;
        }
        lv_obj_align_to(i->Obj, prev_obj, LV_ALIGN_OUT_LEFT_MID, (-DEF_ITEM_SPACING), 0);
        prev_obj = i->Obj;
    }
}

static bool TaskbarAddItem(TASKBAR_T *tb, uint32_t id, const void *img)
{
    if (TaskbarFindItem(tb, id)) {
        // printf("this item is exist!\n");
        return false;
    }

    TASKBAR_ITEM_T *item = _lv_ll_ins_tail(&tb->ItemList);
    item->id             = id;
    item->Obj            = lv_img_create(tb->Obj);
    if (img) {
        lv_img_set_src(item->Obj, img);
    }
    tb->Cnt++;
    TaskbarLayout(tb);
    return true;
}

TASKBAR_T *TaskbarCreate(lv_obj_t *parent)
{
    TASKBAR_T *tb = lv_mem_alloc(sizeof(TASKBAR_T));
    tb->Obj       = lv_obj_create(parent);
    tb->Cnt       = 0;
    _lv_ll_init(&tb->ItemList, sizeof(TASKBAR_ITEM_T));
    lv_obj_set_width(tb->Obj, lv_pct(100));
    lv_obj_set_height(tb->Obj, lv_pct(10));
    lv_obj_set_style_border_width(tb->Obj, 0, LV_PART_MAIN);

    lv_obj_set_style_pad_all(tb->Obj, 0, LV_PART_MAIN);

    lv_color_t bg_color = lv_obj_get_style_bg_color(parent, LV_PART_MAIN);
    lv_obj_set_style_bg_color(tb->Obj, bg_color, LV_PART_MAIN);

    return tb;
}

void TaskbarChgImg(TASKBAR_T *tb, uint32_t id, const void *img, bool is_add)
{
    TASKBAR_ITEM_T *item = TaskbarFindItem(tb, id);
    if (item != NULL) {
        if (img) {
            lv_img_set_src(item->Obj, img);
        }
    } else {
        if (is_add) {
            TaskbarAddItem(tb, id, img);
        }
    }
}

bool TaskbarDelItem(TASKBAR_T *tb, uint32_t id)
{
    TASKBAR_ITEM_T *item = TaskbarFindItem(tb, id);

    if (item == NULL) {
        return false;
    }
    tb->Cnt--;
    lv_obj_del(item->Obj);
    _lv_ll_remove(&tb->ItemList, item);
    TaskbarLayout(tb);
    return true;
}
