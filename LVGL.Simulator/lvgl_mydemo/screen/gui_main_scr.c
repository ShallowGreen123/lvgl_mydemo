/************************************************************************
 * FilePath     : gui_main_scr.c
 * Author       : GX.Duan
 * Date         : 2022-08-19 00:25:00
 * LastEditTime : 2022-10-03 14:42:59
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#define __MAIN_SCR_C_

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "gui_main_scr.h"
#include "../lvgl_app.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#define LV_LIST_ADD_BTN(list, symb, name, list_cb, btn)                                        \
    do {                                                                                       \
        if (list) {                                                                            \
            btn = lv_list_add_btn(list, symb, name);                                           \
            lv_obj_set_height(btn, 50);                                                        \
            lv_obj_set_style_bg_color(btn, lv_color_hex(DEF_LVAL_APP_BG_COLOR), LV_PART_MAIN); \
            lv_obj_set_style_text_color(btn, lv_color_white(), LV_PART_MAIN);                  \
            lv_obj_add_event_cb(btn, list_cb, LV_EVENT_CLICKED, NULL);                         \
        }                                                                                      \
    } while (0);

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/
#define DEF_SELECT_COLOR   lv_color_make(251, 176, 59)
#define DEF_UNSELECT_COLOR lv_color_make(220, 184, 98)

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
static lv_obj_t     *MainScrRoot        = NULL;

static lv_obj_t     *MainScrPlayList    = NULL;
static lv_obj_t     *MainScrSettingList = NULL;

static TASKBAR_T    *tb;

static lv_obj_t     *MainScrDownloadBtn = NULL;
static lv_obj_t     *MainScrSettingBnt  = NULL;
static lv_obj_t     *MainScrPrevBnt     = NULL;

volatile static bool IsEntryMainScr     = false;
/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/
static void MainScrAsyncStyle(void *data)
{
    if (MainScrPrevBnt == MainScrSettingBnt) {
        lv_img_set_src(MainScrDownloadBtn, &ui_img_download0_png);
        lv_img_set_src(MainScrSettingBnt, &ui_img_setting1_png);
        lv_obj_add_flag(MainScrPlayList, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(MainScrSettingList, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_img_set_src(MainScrDownloadBtn, &ui_img_download1_png);
        lv_img_set_src(MainScrSettingBnt, &ui_img_setting0_png);
        lv_obj_clear_flag(MainScrPlayList, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(MainScrSettingList, LV_OBJ_FLAG_HIDDEN);
    }
}

static void swithc_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    if (obj == MainScrDownloadBtn) {
        if (MainScrPrevBnt == MainScrSettingBnt) {
            MainScrPrevBnt = MainScrDownloadBtn;
            lv_async_call(MainScrAsyncStyle, NULL);
        }
    } else if (obj == MainScrSettingBnt) {
        if (MainScrPrevBnt == MainScrDownloadBtn) {
            MainScrPrevBnt = MainScrSettingBnt;
            lv_async_call(MainScrAsyncStyle, NULL);
        }
    }
}

static void MainScrDataListernCb(uint32_t id)
{
    if (IsEntryMainScr == false) {
        return;
    }
    uint8_t val;

    DataModelGetU8(id, &val);
    if (id == DATA_USB_INSERT_ST) {
        if (val == LV_SET) {
            TaskbarChgImg(tb, TASKBAR_USB, &ui_img_usb_png, true);
        } else {
            TaskbarChgImg(tb, TASKBAR_USB, &ui_img_no_usb_png, true);
        }
    } else if (id == DATA_BLUETOOTH_ST) {
        if (val == LV_SET) {
            TaskbarChgImg(tb, TASKBAR_BLUETOOTH, &ui_img_bluetooth_png, false);
        } else {
            TaskbarChgImg(tb, TASKBAR_BLUETOOTH, &ui_img_no_bluetooth_png, false);
        }
    } else if (id == DATA_WIFI_ST) {
        if (val == LV_SET) {
            TaskbarChgImg(tb, TASKBAR_WIFI, &ui_img_wifi_png, false);
        } else {
            TaskbarChgImg(tb, TASKBAR_WIFI, &ui_img_no_wifi_png, false);
        }
    } else if (id == DATA_BATTERY_VAL) {
        if (val < 25) {
            TaskbarChgImg(tb, TASKBAR_BATTERY, &ui_img_battery_0_png, false);
        } else if (val < 50) {
            TaskbarChgImg(tb, TASKBAR_BATTERY, &ui_img_battery_1_png, false);
        } else if (val < 75) {
            TaskbarChgImg(tb, TASKBAR_BATTERY, &ui_img_battery_2_png, false);
        } else {
            TaskbarChgImg(tb, TASKBAR_BATTERY, &ui_img_battery_3_png, false);
        }
        printf("battery: %d\n", val);
    } else if (id == DATA_BT_SWITCH_ST) {
        printf("usb switch state: %d\n", val);
        if (val) {
            TaskbarChgImg(tb, TASKBAR_BLUETOOTH, &ui_img_no_bluetooth_png, true);
        } else {
            TaskbarDelItem(tb, TASKBAR_BLUETOOTH);
        }

    } else if (id == DATA_WIFI_SWITCH_ST) {
        printf("wifi switch state: %d\n", val);
        if (val) {
            TaskbarChgImg(tb, TASKBAR_WIFI, &ui_img_no_wifi_png, true);
        } else {
            TaskbarDelItem(tb, TASKBAR_WIFI);
        }
    } else {
        // default
    }
}

static void download_event_handler(lv_event_t *e)
{
    lv_event_code_t  code     = lv_event_get_code(e);
    lv_obj_t        *obj      = lv_event_get_target(e);
    const char      *name     = NULL;
    static lv_obj_t *prev_obj = NULL;
    if (code == LV_EVENT_CLICKED) {
        LV_UNUSED(obj);
        name = lv_list_get_btn_text(MainScrPlayList, obj);
        if (obj != prev_obj) {
            if (prev_obj) {
                lv_obj_set_style_bg_color(prev_obj, lv_color_hex(DEF_LVAL_APP_BG_COLOR), LV_PART_MAIN);
            }
            lv_obj_set_style_bg_color(obj, lv_color_make(49, 49, 49), LV_PART_MAIN);
        } else {
            lv_obj_set_style_bg_color(obj, lv_color_hex(DEF_LVAL_APP_BG_COLOR), LV_PART_MAIN);
            ConfirPopWin(MainScrRoot, name);
        }
        prev_obj = obj;
    }
}

// scan file list
typedef struct {
    int   index;
    char *filename;
} scanfile_t;

void intchar_copy(void *_dst, const void *_src)
{
    scanfile_t *dst = (scanfile_t *)_dst, *src = (scanfile_t *)_src;
    dst->index    = src->index;
    dst->filename = src->filename ? src->filename : NULL;
}

void intchar_dtor(void *_elt)
{
    scanfile_t *elt = (scanfile_t *)_elt;
    if (elt->filename) free(elt->filename);
}

UT_icd intchar_icd = {sizeof(scanfile_t), NULL, intchar_copy, intchar_dtor};

//
int intsort(const void *a, const void *b)
{
    scanfile_t _a = *(scanfile_t *)a;
    scanfile_t _b = *(scanfile_t *)b;
    return (_a.index < _b.index) ? -1 : (_a.index > _b.index);
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/

static lv_obj_t *Gui_MainScrCreate(lv_obj_t *parent)
{
    MainScrRoot = lv_obj_create(parent);
    lv_obj_set_size(MainScrRoot, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(MainScrRoot, lv_color_hex(DEF_LVAL_APP_BG_COLOR), LV_PART_MAIN);

    tb = TaskbarCreate(MainScrRoot);
    TaskbarChgImg(tb, TASKBAR_BATTERY, &ui_img_battery_3_png, true);

    MainScrDownloadBtn = lv_img_create(MainScrRoot);
    lv_img_set_src(MainScrDownloadBtn, &ui_img_download1_png);
    lv_obj_add_flag(MainScrDownloadBtn, LV_OBJ_FLAG_CLICKABLE);

    MainScrSettingBnt = lv_img_create(MainScrRoot);
    lv_img_set_src(MainScrSettingBnt, &ui_img_setting0_png);
    lv_obj_add_flag(MainScrSettingBnt, LV_OBJ_FLAG_CLICKABLE);

    MainScrSettingList = SettingListCreate(MainScrRoot);

    UT_array  *intchars;          // TODO 完成文件扫描函数
    scanfile_t ic, *p;
    utarray_new(intchars, &intchar_icd);

    ic.index    = 1;
    ic.filename = "hello";
    utarray_push_back(intchars, &ic);
    ic.index    = 3;
    ic.filename = "world";
    utarray_push_back(intchars, &ic);
    ic.index    = 2;
    ic.filename = "kitty";
    utarray_push_back(intchars, &ic);
    ic.index    = 4;
    ic.filename = "sigma";
    utarray_push_back(intchars, &ic);

    utarray_sort(intchars, intsort);

    /*Create a list*/
    MainScrPlayList = lv_list_create(MainScrRoot);
    lv_obj_set_size(MainScrPlayList, lv_pct(100), lv_pct(60));
    lv_obj_set_style_bg_color(MainScrPlayList, lv_color_hex(DEF_LVAL_APP_BG_COLOR), LV_PART_MAIN);
    lv_obj_set_style_border_width(MainScrPlayList, 0, LV_PART_MAIN);

    /*Add buttons to the list*/
    lv_obj_t *btn = NULL;
    p             = NULL;
    char buff[16];
    while ((p = (scanfile_t *)utarray_next(intchars, p))) {
        lv_snprintf(buff, sizeof(buff), "%d. %s", p->index, p->filename);
        LV_LIST_ADD_BTN(MainScrPlayList, NULL, buff, download_event_handler, btn);
    }

    DataModelRegister(MainScrDataListernCb);

    MainScrPrevBnt = MainScrDownloadBtn;
    lv_obj_clear_flag(MainScrSettingList, LV_OBJ_FLAG_HIDDEN);

    return MainScrRoot;
}

static void Gui_MainScrLayout(void)
{
    lv_obj_set_x(MainScrDownloadBtn, -52);
    lv_obj_set_y(MainScrDownloadBtn, -82);
    lv_obj_set_align(MainScrDownloadBtn, LV_ALIGN_CENTER);

    lv_obj_set_x(MainScrSettingBnt, lv_pct(23));
    lv_obj_set_y(MainScrSettingBnt, lv_pct(-25));
    lv_obj_set_align(MainScrSettingBnt, LV_ALIGN_CENTER);

    lv_obj_set_align(MainScrSettingList, LV_ALIGN_BOTTOM_MID);

    lv_obj_set_align(MainScrPlayList, LV_ALIGN_BOTTOM_MID);
}

static void Gui_MainScrEnter(void)
{
    IsEntryMainScr = true;
    Gui_MainScrLayout();

    lv_obj_add_event_cb(MainScrDownloadBtn, swithc_btn_event_cb, LV_EVENT_CLICKED, MainScrDownloadBtn);
    lv_obj_add_event_cb(MainScrSettingBnt, swithc_btn_event_cb, LV_EVENT_CLICKED, MainScrSettingBnt);
}

static void Gui_MainScrExit(void)
{
    IsEntryMainScr = false;
    lvgl_app_remove_all_event(MainScrDownloadBtn, swithc_btn_event_cb);
    lvgl_app_remove_all_event(MainScrSettingBnt, swithc_btn_event_cb);
}

static void Gui_MainScrDestory(void)
{
    DataModelRemove(MainScrDataListernCb);
}

const SCR_MGR_SCR_HANDLE_T Gui_MainScrHandle = {
    .ScrId      = GUI_MIAN_SCR_ID,
    .ScrCreate  = Gui_MainScrCreate,
    .ScrEnter   = Gui_MainScrEnter,
    .ScrExit    = Gui_MainScrExit,
    .ScrDestroy = Gui_MainScrDestory,
};

SCR_MGR_REG(Gui_MainScrHandle);
