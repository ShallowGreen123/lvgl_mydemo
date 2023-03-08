/************************************************************************
 * FilePath     : drv_log.h
 * Author       : GX.Duan
 * Date         : 2022-09-21 22:22:43
 * LastEditTime : 2022-10-01 22:23:17
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#ifndef DRV_LOG_H
#define DRV_LOG_H

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#ifdef __DRV_LOG_C__
#    define DEF_LOG_EXTERN
#else
#    define DEF_LOG_EXTERN extern
#endif

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/
#define RTT_CTRL_RESET               "\x1B[0m"          // Reset to default colors
#define RTT_CTRL_CLEAR               "\x1B[2J"          // Clear screen, reposition cursor to top left

#define RTT_CTRL_TEXT_BLACK          "\x1B[2;30m"
#define RTT_CTRL_TEXT_RED            "\x1B[2;31m"
#define RTT_CTRL_TEXT_GREEN          "\x1B[2;32m"
#define RTT_CTRL_TEXT_YELLOW         "\x1B[2;33m"
#define RTT_CTRL_TEXT_BLUE           "\x1B[2;34m"
#define RTT_CTRL_TEXT_MAGENTA        "\x1B[2;35m"
#define RTT_CTRL_TEXT_CYAN           "\x1B[2;36m"
#define RTT_CTRL_TEXT_WHITE          "\x1B[2;37m"

#define RTT_CTRL_TEXT_BRIGHT_BLACK   "\x1B[1;30m"
#define RTT_CTRL_TEXT_BRIGHT_RED     "\x1B[1;31m"
#define RTT_CTRL_TEXT_BRIGHT_GREEN   "\x1B[1;32m"
#define RTT_CTRL_TEXT_BRIGHT_YELLOW  "\x1B[1;33m"
#define RTT_CTRL_TEXT_BRIGHT_BLUE    "\x1B[1;34m"
#define RTT_CTRL_TEXT_BRIGHT_MAGENTA "\x1B[1;35m"
#define RTT_CTRL_TEXT_BRIGHT_CYAN    "\x1B[1;36m"
#define RTT_CTRL_TEXT_BRIGHT_WHITE   "\x1B[1;37m"

#define RTT_CTRL_BG_BLACK            "\x1B[24;40m"
#define RTT_CTRL_BG_RED              "\x1B[24;41m"
#define RTT_CTRL_BG_GREEN            "\x1B[24;42m"
#define RTT_CTRL_BG_YELLOW           "\x1B[24;43m"
#define RTT_CTRL_BG_BLUE             "\x1B[24;44m"
#define RTT_CTRL_BG_MAGENTA          "\x1B[24;45m"
#define RTT_CTRL_BG_CYAN             "\x1B[24;46m"
#define RTT_CTRL_BG_WHITE            "\x1B[24;47m"

#define RTT_CTRL_BG_BRIGHT_BLACK     "\x1B[4;40m"
#define RTT_CTRL_BG_BRIGHT_RED       "\x1B[4;41m"
#define RTT_CTRL_BG_BRIGHT_GREEN     "\x1B[4;42m"
#define RTT_CTRL_BG_BRIGHT_YELLOW    "\x1B[4;43m"
#define RTT_CTRL_BG_BRIGHT_BLUE      "\x1B[4;44m"
#define RTT_CTRL_BG_BRIGHT_MAGENTA   "\x1B[4;45m"
#define RTT_CTRL_BG_BRIGHT_CYAN      "\x1B[4;46m"
#define RTT_CTRL_BG_BRIGHT_WHITE     "\x1B[4;47m"

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/

/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
DEF_LOG_EXTERN void Drv_LogPrintf(void *str);
DEF_LOG_EXTERN void Drv_LogColorPrintf(void *str, void *prefix);

#endif /* DRV_LOG_H */