/************************************************************************
 * FilePath     : lvgl_app.c
 * Author       : GX.Duan
 * Date         : 2022-08-07 15:20:41
 * LastEditTime : 2022-10-01 23:02:06
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
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

    Lib_LogLoop();
}

static bool log_write_to_file(LIB_LOG_ITEM_T *item)
{
    printf("File %s\n", item->Buf);
    return true;
}

static bool log_write_to_flash(LIB_LOG_ITEM_T *item)
{
    printf("Flash %s\n", item->Buf);
    return true;
}

static bool log_write_to_sd(LIB_LOG_ITEM_T *item)
{
    printf("Sd %s\n", item->Buf);
    return true;
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/

void lvgl_app_init(void)
{
    Lib_LogInit();

    Lib_LogRegistWriter(log_write_to_file);           // 模拟写日志到文件
    Lib_LogRegistWriter(log_write_to_flash);          // 模拟写日志到Flash
    Lib_LogRegistWriter(log_write_to_sd);             // 模拟写日志到SD卡

    Lib_LogUnregistWriter(log_write_to_sd);

    DataModelInit();

    ScrMgrInit();
    ScrMgrSwitchScr(GUI_MIAN_SCR_ID, true);

    TRACE_W("Hello wrold!");

    lv_timer_create(log_write_timer_cb, 50, NULL);
}
