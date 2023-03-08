

#ifndef WIDGETS_H
#define WIDGETS_H

#include "../lvgl_app.h"

enum {
    TASKBAR_BATTERY = 1,
    TASKBAR_WIFI,
    TASKBAR_USB,
    TASKBAR_BLUETOOTH,
    TASKBAR_ITEM_MAX,
};

typedef struct taskbar_item {
    uint32_t  id;
    lv_obj_t *Obj;
} TASKBAR_ITEM_T;

typedef struct taskbar {
    lv_obj_t *Obj;
    uint16_t  Cnt;
    lv_ll_t   ItemList;
} TASKBAR_T;

TASKBAR_T *TaskbarCreate(lv_obj_t *parent);
void       TaskbarChgImg(TASKBAR_T *tb, uint32_t id, const void *img, bool is_add);
bool       TaskbarDelItem(TASKBAR_T *tb, uint32_t id);

//
void ConfirPopWin(lv_obj_t *parent, const char *filename);
//
lv_obj_t *SettingListCreate(lv_obj_t *parent);
#endif
