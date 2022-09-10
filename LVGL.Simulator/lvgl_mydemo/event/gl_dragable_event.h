/************************************************************************
 * FilePath     : gl_dragable_event.h
 * Author       : GX.Duan
 * Date         : 2022-09-10 17:40:03
 * LastEditTime : 2022-09-10 17:40:52
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
/**
 * @file gl_dragable_event.h
 *
 */

#ifndef GL_GRAGABLE_EVENT_H
#define GL_GRAGABLE_EVENT_H

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void gl_set_dragable(lv_obj_t *obj, lv_anim_ready_cb_t rdy_cb);
bool gl_has_drag_flag(void);
void gl_clear_drag_flag(void);

#endif
