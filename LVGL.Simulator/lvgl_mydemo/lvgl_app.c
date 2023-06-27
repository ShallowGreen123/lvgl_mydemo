/************************************************************************
 * FilePath     : lvgl_app.c
 * Author       : GX.Duan
 * Date         : 2022-08-07 15:20:41
 * LastEditTime : 2022-10-01 23:02:06
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/

#define __LVGL_APP_C__

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "lvgl_app.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/

/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/

static void lv_app_test_timer_cb(lv_timer_t *t)
{
    LV_UNUSED(t);
    static uint8_t bat     = 0;
    static uint8_t bt_st   = 0;
    static uint8_t usb_st  = 0;
    static uint8_t wifi_st = 0;

    DataModelSetU8(DATA_BATTERY_VAL, bat);

    DataModelSetU8(DATA_BLUETOOTH_ST, bt_st);

    DataModelSetU8(DATA_USB_INSERT_ST, usb_st);

    DataModelSetU8(DATA_WIFI_ST, wifi_st);

    bat += 5;
    bt_st   = !bt_st;
    usb_st  = !usb_st;
    wifi_st = !wifi_st;

    if (bat > 100) {
        bat = 0;
    }
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/

void lvgl_app_init(void)
{
    // Lib_LogInit();
    // Lib_LogRegistWriter(log_write_to_file);          // 模拟写日志到文件

    DataModelInit();

    ScrMgrInit();
    ScrMgrSwitchScr(GUI_MIAN_SCR_ID, true);

    // TODO 上电加载data model map的数据，掉电保存data model map数据到flash

    TRACE_W("Hello wrold!");

    lv_timer_create(lv_app_test_timer_cb, 1000, NULL);
}

void lvgl_app_remove_all_event(lv_obj_t *obj, lv_event_cb_t event_cb)
{
    while (lv_obj_remove_event_cb(obj, event_cb)) {}
}

void lvgl_app_shutdown(void)
{
    printf("shutdown\n");
}