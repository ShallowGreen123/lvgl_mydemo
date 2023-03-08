/************************************************************************
 * FilePath     : gui_test1_scr.c
 * Author       : GX.Duan
 * Date         : 2022-09-10 17:10:59
 * LastEditTime : 2022-09-21 22:46:10
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#define __TEST1_SCR_C_

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "gui_test1_scr.h"


/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
static lv_obj_t *Test1ScrRoot = NULL;
static lv_obj_t *Test1Label   = NULL;

static lv_obj_t *test1_btn    = NULL;
static lv_obj_t *BtnLabel     = NULL;

/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/
void test1_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    if (obj == test1_btn) {
        TRACE_I("Screen 2 test1_btn is pressed!");
        ScrMgrPopScr(true);
    }
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/
static lv_obj_t *Gui_Test1ScrCreate(lv_obj_t *parent)
{
    Test1ScrRoot = lv_obj_create(parent);
    lv_obj_set_size(Test1ScrRoot, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(Test1ScrRoot, lv_color_black(), LV_PART_MAIN);

    Test1Label = lv_label_create(Test1ScrRoot);
    lv_obj_set_style_text_color(Test1Label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(Test1Label, "Screen 2");

    test1_btn = lv_btn_create(Test1ScrRoot);
    lv_obj_set_size(test1_btn, 100, 40);
    BtnLabel = lv_label_create(test1_btn);
    lv_label_set_text(BtnLabel, "switch");

    return Test1ScrRoot;
}

static void Gui_Test1ScrLayout(void)
{
    lv_obj_align_to(Test1Label, Test1ScrRoot, LV_ALIGN_CENTER, 0, -30);

    lv_obj_center(BtnLabel);
    lv_obj_align_to(test1_btn, Test1ScrRoot, LV_ALIGN_CENTER, 0, 30);
}

static void Gui_Test1ScrEnter(void)
{
    Gui_Test1ScrLayout();

    lv_obj_add_event_cb(test1_btn, test1_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

static void Gui_Test1ScrExit(void)
{
    lv_obj_remove_event_cb(test1_btn, test1_btn_event_cb);
}

static void Gui_Test1ScrDestory(void)
{
}

const SCR_MGR_SCR_HANDLE_T Gui_Test1ScrHandle = {
    .ScrId      = GUI_TEST1_SCR_ID,
    .ScrCreate  = Gui_Test1ScrCreate,
    .ScrEnter   = Gui_Test1ScrEnter,
    .ScrExit    = Gui_Test1ScrExit,
    .ScrDestroy = Gui_Test1ScrDestory,
};

SCR_MGR_REG(Gui_Test1ScrHandle);
