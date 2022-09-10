/************************************************************************
 * FilePath     : gl_dragable_event.c
 * Author       : GX.Duan
 * Date         : 2022-09-10 17:40:10
 * LastEditTime : 2022-09-10 17:41:17
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#include "gl_dragable_event.h"

static bool drag_flag = false;          // if you has setten dragable and clicked event togather, use this flag to make sure not mix up them;
static int  innerX    = -1;
static int  innerY    = -1;
static void gl_obj_add_anim(lv_obj_t *obj, lv_anim_exec_xcb_t exec_cb, lv_anim_ready_cb_t rdy_cb,
                            uint16_t time, lv_coord_t start, lv_coord_t end, lv_anim_path_cb_t path_cb) {
    lv_anim_t a;

    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_exec_cb(&a, exec_cb);
    if (rdy_cb != NULL) {
        lv_anim_set_ready_cb(&a, rdy_cb);
    }
    lv_anim_set_time(&a, time);

    a.start_value   = start;
    a.end_value     = end;
    a.path_cb       = path_cb;
    a.playback_time = 0;

    lv_anim_start(&a);
}

static void gl_drag_event_cb(lv_event_t *e) {
    lv_obj_t       *lvObj = lv_event_get_target(e);
    lv_event_code_t code  = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED) {
        drag_flag         = false;
        lv_indev_t *indev = lv_indev_get_act();

        if (lv_indev_get_type(indev) != LV_INDEV_TYPE_POINTER) return;

        lv_point_t p;
        lv_indev_get_point(indev, &p);
        innerX = p.x - lvObj->coords.x1;
        innerY = p.y - lvObj->coords.y1;
    } else if (code == LV_EVENT_PRESSING && innerY != -1 && innerX != -1) {
        lv_point_t  p;
        lv_indev_t *indev = lv_indev_get_act();
        lv_coord_t  tx, ty;
        lv_coord_t  w, h;

        if (lv_indev_get_type(indev) != LV_INDEV_TYPE_POINTER) {
            return;
        }

        lv_indev_get_point(indev, &p);
        if (abs(p.x - lvObj->coords.x1 - innerX) + abs(p.y - lvObj->coords.y1 - innerY) > 3) {
            drag_flag = true;
        }
        if (drag_flag) {
            w  = lv_obj_get_width(lvObj->parent) - lv_obj_get_width(lvObj);
            h  = lv_obj_get_height(lvObj->parent) - lv_obj_get_height(lvObj);
            tx = p.x - innerX;
            ty = p.y - innerY;
            if (tx < 0) {
                tx = 0;
            } else if (tx >= w) {
                tx = w - 1;
            }
            if (ty < 0) {
                ty = 0;
            } else if (ty >= h) {
                ty = h - 1;
            }
            lv_obj_set_pos(lvObj, tx, ty);
        }
    } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
        lv_coord_t cx        = lvObj->coords.x1 + (lvObj->coords.x2 - lvObj->coords.x1) / 2;
        lv_coord_t parent_cx = lvObj->parent->coords.x1 + (lvObj->parent->coords.x2 - lvObj->parent->coords.x1) / 2;

        if (cx < parent_cx) {
            gl_obj_add_anim(lvObj, (lv_anim_exec_xcb_t)lv_obj_set_x, (lv_anim_ready_cb_t)e->user_data, 200,
                            lvObj->coords.x1, lvObj->parent->coords.x1, lv_anim_path_linear);
        } else {
            gl_obj_add_anim(lvObj, (lv_anim_exec_xcb_t)lv_obj_set_x, (lv_anim_ready_cb_t)e->user_data, 200,
                            lvObj->coords.x1, lvObj->parent->coords.x2 - lvObj->coords.x2 + lvObj->coords.x1, lv_anim_path_linear);
        }

        innerX = -1;
        innerY = -1;
    }
}
bool gl_has_drag_flag(void) {
    return drag_flag;
}

void gl_clear_drag_flag(void) {
    drag_flag = false;
}
