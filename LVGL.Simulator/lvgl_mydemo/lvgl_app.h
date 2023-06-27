/************************************************************************
 * FilePath     : lvgl_app.h
 * Author       : GX.Duan
 * Date         : 2022-08-07 15:20:41
 * LastEditTime : 2022-09-21 22:03:59
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#ifndef LV_DEMO_INIT_H
#define LV_DEMO_INIT_H

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "../lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/lib_sys_log.h"
#include "data/dataModel.h"
#include "data/gui_scr_mgr.h"
#include "data/dataArray.h"
#include "widgets/widgets.h"
#include "assert/image/image.h"

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#ifdef __LVGL_APP_C__
#    define DEF_APP_EXTERN
#else
#    define DEF_APP_EXTERN extern
#endif

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/
#define DEF_LCD_WIDTH_MAX     240
#define DEF_LCD_HEIGHT_MAX    320
#define DEF_LVAL_APP_BG_COLOR 0x000000

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
enum {
    LV_SET   = 1,
    LV_RESET = !LV_SET,
};

enum {
    GUI_MIAN_SCR_ID = 0,
    GUI_TEST1_SCR_ID,
};

typedef enum data_model_id_e {
    DATA_MODEL_ID_0 = 0,
    DATA_USB_INSERT_ST,
    DATA_BLUETOOTH_ST,
    DATA_WIFI_ST,
    DATA_BATTERY_VAL,

    DATA_BT_SWITCH_ST,
    DATA_WIFI_SWITCH_ST,
    DATA_MODEL_ID_MAX,
} DATA_MODEL_ID_E;
/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
DEF_APP_EXTERN void lvgl_app_init(void);
DEF_APP_EXTERN void lvgl_app_remove_all_event(lv_obj_t *obj, lv_event_cb_t event_cb);

DEF_APP_EXTERN void lvgl_app_shutdown(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_DEMO_INIT_H */
