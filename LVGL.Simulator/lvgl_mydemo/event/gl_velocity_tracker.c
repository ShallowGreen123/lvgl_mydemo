/************************************************************************
 * FilePath     : gl_velocity_tracker.c
 * Author       : GX.Duan
 * Date         : 2022-09-10 17:44:04
 * LastEditTime : 2022-09-10 17:46:29
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/

#define DEF_GL_VELOCITY_TRACKER_MODULE
#include "gl_velocity_tracker.h"
#include <string.h>
#include "lvgl.h"

static GL_VELOCITY_TRACKER_POINT_T Gl_VelocityTrackerPoints[DEF_GL_VELOCITY_TRACKER_POINTS_CNT];

//
static bool Gl_VelocityTrackerSolveX(GL_VELOCITY_TRACKER_POINT_T *points, uint32_t points_cnt, float *x_coeff) {
    size_t count = points_cnt;

    // Solving y = a*x^2 + b*x + c
    float  sxi = 0, sxiyi = 0, syi = 0, sxi2 = 0, sxi3 = 0, sxi2yi = 0, sxi4 = 0;
    float  numerator;
    float  a, b, c;
    float  denominator;
    float  xi, yi, xi2, xi3, xi4, xiyi, xi2yi;
    float  Sxx, Sxy, Sxx2, Sx2y, Sx2x2;
    size_t i;

    for (i = 0; i < count; i++) {
        xi    = (float)(points[i].Timestamp) * -0.001f;
        yi    = (float)(points[i].X);
        xi2   = xi * xi;
        xi3   = xi2 * xi;
        xi4   = xi3 * xi;
        xiyi  = xi * yi;
        xi2yi = xi2 * yi;

        sxi += xi;
        sxi2 += xi2;
        sxiyi += xiyi;
        sxi2yi += xi2yi;
        syi += yi;
        sxi3 += xi3;
        sxi4 += xi4;
    }
    Sxx         = sxi2 - sxi * sxi / count;
    Sxy         = sxiyi - sxi * syi / count;
    Sxx2        = sxi3 - sxi * sxi2 / count;
    Sx2y        = sxi2yi - sxi2 * syi / count;
    Sx2x2       = sxi4 - sxi2 * sxi2 / count;

    denominator = Sxx * Sx2x2 - Sxx2 * Sxx2;
    if (denominator == 0) {
        return false;
    }
    // Compute a, b, c
    numerator = Sx2y * Sxx - Sxy * Sxx2;
    a         = numerator / denominator;
    numerator = Sxy * Sx2x2 - Sx2y * Sxx2;
    b         = numerator / denominator;
    c         = syi / count - b * sxi / count - a * sxi2 / count;

    *x_coeff  = b;
    return true;
}

static bool Gl_VelocityTrackerSolveY(GL_VELOCITY_TRACKER_POINT_T *points, uint32_t points_cnt, float *y_coeff) {
    size_t count = points_cnt;

    // Solving y = a*x^2 + b*x + c
    float  sxi = 0, sxiyi = 0, syi = 0, sxi2 = 0, sxi3 = 0, sxi2yi = 0, sxi4 = 0;
    float  numerator;
    float  a, b, c;
    float  denominator;
    float  xi, yi, xi2, xi3, xi4, xiyi, xi2yi;
    float  Sxx, Sxy, Sxx2, Sx2y, Sx2x2;
    size_t i;

    for (i = 0; i < count; i++) {
        xi    = (float)(points[i].Timestamp) * -0.001f;
        yi    = (float)(points[i].Y);
        xi2   = xi * xi;
        xi3   = xi2 * xi;
        xi4   = xi3 * xi;
        xiyi  = xi * yi;
        xi2yi = xi2 * yi;

        sxi += xi;
        sxi2 += xi2;
        sxiyi += xiyi;
        sxi2yi += xi2yi;
        syi += yi;
        sxi3 += xi3;
        sxi4 += xi4;
    }
    Sxx         = sxi2 - sxi * sxi / count;
    Sxy         = sxiyi - sxi * syi / count;
    Sxx2        = sxi3 - sxi * sxi2 / count;
    Sx2y        = sxi2yi - sxi2 * syi / count;
    Sx2x2       = sxi4 - sxi2 * sxi2 / count;

    denominator = Sxx * Sx2x2 - Sxx2 * Sxx2;
    if (denominator == 0) {
        LV_LOG_WARN("[VelocityTracker] Denominator zero\n");
        return false;
    }
    // Compute a, b, c
    numerator = Sx2y * Sxx - Sxy * Sxx2;
    a         = numerator / denominator;
    numerator = Sxy * Sx2x2 - Sx2y * Sxx2;
    b         = numerator / denominator;
    c         = syi / count - b * sxi / count - a * sxi2 / count;

    *y_coeff  = b;
    return true;
}

bool Gl_VelocityTrackerSolve(GL_VELOCITY_TRACKER_HANDLE_T *handle, uint32_t timestamp, float *x_coeff, float *y_coeff) {
    GL_VELOCITY_TRACKER_POINT_T *point;
    uint32_t                     cnt;
    uint32_t                     valid_cnt;
    uint32_t                     time_diff;
    uint32_t                     i;
    int32_t                      last_x;
    int32_t                      last_y;

    memset(Gl_VelocityTrackerPoints, 0, sizeof(Gl_VelocityTrackerPoints));

    if (handle->Wp >= handle->Rp) {
        cnt = handle->Wp - handle->Rp;
    } else {
        cnt = handle->Wp - handle->Rp + DEF_GL_VELOCITY_TRACKER_POINTS_CNT;
    }
    if (cnt < 4) {
        LV_LOG_WARN("[VelocityTracker] Points not enough %u\n", cnt);
        return false;
    }

    valid_cnt = 0;
    for (i = 0; i < cnt; i++) {
        point     = &handle->Points[(handle->Rp + i) % DEF_GL_VELOCITY_TRACKER_POINTS_CNT];
        time_diff = timestamp - point->Timestamp;
        // LV_LOG_TRACE("[VelocityTracker] Point[%u] = (%u, %d, %d)\n", i, point->Timestamp, point->X, point->Y);
        if (time_diff > DEF_GL_VELOCITY_TRACKER_HORIZON_MS) {
            last_x = point->X;
            last_y = point->Y;
            continue;
        }
        if (i > 0) {
            Gl_VelocityTrackerPoints[valid_cnt].Timestamp = time_diff;
            Gl_VelocityTrackerPoints.X                    = point->X - last_x;
            Gl_VelocityTrackerPoints[valid_cnt].Y         = point->Y - last_y;
            valid_cnt++;
        }
        last_x = point->X;
        last_y = point->Y;
    }

    if (valid_cnt < 3) {
        LV_LOG_WARN("[VelocityTracker] Valid points not enough %u\n", valid_cnt);
        return false;
    }
    if (Gl_VelocityTrackerSolveX(Gl_VelocityTrackerPoints, valid_cnt, x_coeff)
        && Gl_VelocityTrackerSolveY(Gl_VelocityTrackerPoints, valid_cnt, y_coeff)) {
        return true;
    }

    return false;
}

void Gl_VelocityTrackerInit(GL_VELOCITY_TRACKER_HANDLE_T *handle) {
    Gl_VelocityTrackerReset(handle);
}

void Gl_VelocityTrackerReset(GL_VELOCITY_TRACKER_HANDLE_T *handle) {
    memset(handle, 0, sizeof(GL_VELOCITY_TRACKER_HANDLE_T));
}

void Gl_VelocityTrackerAddPoint(GL_VELOCITY_TRACKER_HANDLE_T *handle, uint32_t timestamp, int32_t x, int32_t y) {
    GL_VELOCITY_TRACKER_POINT_T *point = &handle->Points[handle->Wp];

    point->Timestamp                   = timestamp;
    point->X                           = x;
    point->Y                           = y;
    handle->Wp                         = (handle->Wp + 1) % DEF_GL_VELOCITY_TRACKER_POINTS_CNT;
    if (handle->Wp == handle->Rp) {
        handle->Rp = (handle->Rp + 1) % DEF_GL_VELOCITY_TRACKER_POINTS_CNT;
    }
}
bool Gl_VelocityTrackerCalc(GL_VELOCITY_TRACKER_HANDLE_T *handle, uint32_t timestamp) {
    float x_coeff = 0.0f;
    float y_coeff = 0.0f;
    bool  ret     = Gl_VelocityTrackerSolve(handle, timestamp, &x_coeff, &y_coeff);
    if (ret) {
        handle->Vx = x_coeff;
        handle->Vy = y_coeff;
    }
    return ret;
}

float Gl_VelocityTrackerGetVx(GL_VELOCITY_TRACKER_HANDLE_T *handle) {
    return handle->Vx;
}

float Gl_VelocityTrackerGetVy(GL_VELOCITY_TRACKER_HANDLE_T *handle) {
    return handle->Vy;
}
