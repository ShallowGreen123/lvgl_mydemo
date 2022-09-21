/************************************************************************
 * FilePath     : gui_main_scr.c
 * Author       : GX.Duan
 * Date         : 2022-08-19 00:25:00
 * LastEditTime : 2022-09-21 22:11:15
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
/************************************************************************
 * FilePath     : gui_main_scr.c
 * Author       : GX.Duan
 * Date         : 2022-08-19 00:25:00
 * LastEditTime : 2022-09-10 16:01:12
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#define __MAIN_SCR_C_

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "gui_main_scr.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
static lv_obj_t *MainScrRoot = NULL;
static lv_obj_t *label       = NULL;

static lv_obj_t *switch_btn  = NULL;
static lv_obj_t *label2      = NULL;

/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/
void swithc_btn_event_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_target(e);

    if (obj == switch_btn) {
        TRACE_I("Screen 1 switch_btn is pressed!");
        ScrMgrPushScr(GUI_TEST1_SCR_ID, true);
    }
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/
static lv_obj_t *Gui_MainScrCreate(lv_obj_t *parent) {
    MainScrRoot = lv_obj_create(parent);
    lv_obj_set_size(MainScrRoot, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(MainScrRoot, lv_color_black(), LV_PART_MAIN);

    label = lv_label_create(MainScrRoot);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(label, "Screen 1");

    switch_btn = lv_btn_create(MainScrRoot);
    lv_obj_set_size(switch_btn, 100, 40);
    label2 = lv_label_create(switch_btn);
    lv_label_set_text(label2, "switch");

    return MainScrRoot;
}

static void Gui_MainScrLayout(void) {
    lv_obj_align_to(label, MainScrRoot, LV_ALIGN_CENTER, 0, -30);

    lv_obj_center(label2);
    lv_obj_align_to(switch_btn, MainScrRoot, LV_ALIGN_CENTER, 0, 30);
}

static void Gui_MainScrEnter(void) {
    Gui_MainScrLayout();

    lv_obj_add_event_cb(switch_btn, swithc_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

static void Gui_MainScrExit(void) {
    lv_obj_remove_event_cb(switch_btn, swithc_btn_event_cb);
}

static void Gui_MainScrDestory(void) {
}

const SCR_MGR_SCR_HANDLE_T Gui_MainScrHandle = {
    .ScrId      = GUI_MIAN_SCR_ID,
    .ScrCreate  = Gui_MainScrCreate,
    .ScrEnter   = Gui_MainScrEnter,
    .ScrExit    = Gui_MainScrExit,
    .ScrDestroy = Gui_MainScrDestory,
};

SCR_MGR_REG(Gui_MainScrHandle);
