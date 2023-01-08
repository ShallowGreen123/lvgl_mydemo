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
#include "lib/lib_sys_log.h"
#include "data/dataModel.h"
#include "data/gui_scr_mgr.h"

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#ifdef __LVGL_APP_C__
#    define DEF_EXTERN
#else
#    define DEF_EXTERN extern
#endif

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
enum {
    GUI_MIAN_SCR_ID = 0,
    GUI_TEST1_SCR_ID,
};

/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
DEF_EXTERN void lvgl_app_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_DEMO_INIT_H */
