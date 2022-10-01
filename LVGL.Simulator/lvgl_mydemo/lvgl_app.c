/************************************************************************
 * FilePath     : lvgl_app.c
 * Author       : GX.Duan
 * Date         : 2022-08-07
 * LastEditTime : 2022-08-19
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "lvgl_app.h"
#include "data/gui_scr_mgr.h"

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
static void log_write_timer_cb(lv_timer_t *t)
{
    LV_UNUSED(t);

    TRACE_V("Hello wrold!");
    TRACE_D("Hello wrold!");
    TRACE_I("Hello wrold!");
    TRACE_E("Hello wrold!");
    TRACE_W("Hello wrold!");

    Lib_LogProc();
}

static bool log_write_to_file(LIB_TRACE_ITEM_HANDLE_T *item)
{
    TRACE_D("item info = %s", item->Buf);
    return true;
}
/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/

void lvgl_app_init(void)
{
    Lib_LogInit();
    Lib_LogRegistWriter(log_write_to_file);

    DataModelInit();

    ScrMgrInit();
    ScrMgrSwitchScr(GUI_MIAN_SCR_ID, true);

    lv_timer_create(log_write_timer_cb, 3000, NULL);
}
