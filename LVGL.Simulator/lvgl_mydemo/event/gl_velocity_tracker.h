/************************************************************************
 * FilePath     : gl_velocity_tracker.h
 * Author       : GX.Duan
 * Date         : 2022-09-10 17:44:10
 * LastEditTime : 2022-09-10 17:44:41
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#ifndef __GL_VELOCITY_TRACKER_H__
#define __GL_VELOCITY_TRACKER_H__
/*
*********************************************************************************************************
*                                          INCLUDE HEADER FILES
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#ifdef DEF_GL_VELOCITY_TRACKER_MODULE
#define DEF_GL_VELOCITY_TRACKER_EXT
#else
#define DEF_GL_VELOCITY_TRACKER_EXT extern
#endif
/*
*********************************************************************************************************
*                                          DEFAULT CONFIGURATION
*********************************************************************************************************
*/
#define DEF_GL_VELOCITY_TRACKER_POINTS_CNT 20
#define DEF_GL_VELOCITY_TRACKER_HORIZON_MS 100
typedef struct gl_velocity_tracker_point_t {
    uint32_t Timestamp;
    int32_t  X;
    int32_t  Y;
} GL_VELOCITY_TRACKER_POINT_T;

typedef struct gl_velocity_tracker_handle_t {
    GL_VELOCITY_TRACKER_POINT_T Points[DEF_GL_VELOCITY_TRACKER_POINTS_CNT];
    uint8_t                     Rp;
    uint8_t                     Wp;
    float                       Vx;          // px/s
    float                       Vy;          // px/s
} GL_VELOCITY_TRACKER_HANDLE_T;
DEF_GL_VELOCITY_TRACKER_EXT void  Gl_VelocityTrackerInit(GL_VELOCITY_TRACKER_HANDLE_T *handle);
DEF_GL_VELOCITY_TRACKER_EXT void  Gl_VelocityTrackerReset(GL_VELOCITY_TRACKER_HANDLE_T *handle);
DEF_GL_VELOCITY_TRACKER_EXT void  Gl_VelocityTrackerAddPoint(GL_VELOCITY_TRACKER_HANDLE_T *handle, uint32_t timestamp, int32_t x, int32_t y);
DEF_GL_VELOCITY_TRACKER_EXT bool  Gl_VelocityTrackerCalc(GL_VELOCITY_TRACKER_HANDLE_T *handle, uint32_t timestamp);
DEF_GL_VELOCITY_TRACKER_EXT float Gl_VelocityTrackerGetVx(GL_VELOCITY_TRACKER_HANDLE_T *handle);
DEF_GL_VELOCITY_TRACKER_EXT float Gl_VelocityTrackerGetVy(GL_VELOCITY_TRACKER_HANDLE_T *handle);
#endif /* __GL_VELOCITY_TRACKER_H__ */