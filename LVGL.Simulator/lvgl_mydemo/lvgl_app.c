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

void log_create_event_cb(lv_timer_t *t)
{
    LV_UNUSED(t);
    uint32_t i;
    uint32_t rand     = lv_rand(3, 100);
    uint8_t  buf[102] = {0};

    for (i = 0; i < rand; i++) {
        buf[i] = '*';
    }
    buf[i + 1] = '\0';

    TRACE_I("%s\n", buf);          // 随机添加长度为(3-100)的日志
}

static bool log_write_to_file(LIB_LOG_ITEM_T *item)          // 日志写回调函数
{
    // erroe rate = 134 / 80244 : 0.0016699067843079
    /**
     * 测试结果，向日志系统中写入了80244条日志，
     * 无效的日志为134条，日志无效概率为：
     *              0.167%
     */
    static uint32_t t   = 0;
    static uint32_t err = 0;          //记录日志写回调函数中无效日志的个数
    uint32_t        len = strlen(item->Buf);
    if (len == 0) {
        err++;
    }
    printf("File[%d] e=%d %s\n", t++, err, item->Buf);
    return true;
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/

void lvgl_app_init(void)
{
    Lib_LogInit();

    Lib_LogRegistWriter(log_write_to_file);          // 模拟写日志到文件

    DataModelInit();

    ScrMgrInit();
    ScrMgrSwitchScr(GUI_MIAN_SCR_ID, true);

    TRACE_W("Hello wrold!");

    lv_timer_create(log_write_timer_cb, 50, NULL);
    lv_timer_create(log_create_event_cb, 100, NULL);
}
