/************************************************************************
 * FilePath     : log.h
 * Author       : GX.Duan
 * Date         : 2022-09-18 17:49:09
 * LastEditTime : 2022-09-21 21:55:55
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#ifndef __LIB_TRACE_H__
#define __LIB_TRACE_H__

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

#ifdef DEF_LIB_TRACE_MODULE
#    define DEF_LIB_TRACE_EXT
#else
#    define DEF_LIB_TRACE_EXT extern
#endif

#if defined(_WIN32)
#    ifndef __packed
#        define __packed
#    endif
#    ifndef __aligned
#        define __aligned(x) __declspec(align(x))
#    endif
#else /* _WIN32 */
#    include <sys/cdefs.h>
#endif /* _WIN32 */
/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/
#define TRACE_LV_NONE                    0
#define TRACE_LV_ERR                     1
#define TRACE_LV_WARN                    2
#define TRACE_LV_INFO                    3
#define TRACE_LV_DBG                     4
#define TRACE_LV_V                       5

#define DEF_DBG_LV                       TRACE_LV_DBG

#define DEF_LIB_TRACE_FORCE_PRINTF       1

#define DEF_LOG_COLOR_PRINT_EN           1

#define DEF_LIB_TRACE_MAX_MSG_SIZE       256

#define DEF_LIB_TRACE_HEX_ARRAY_LINE_LEN 32
#define DEF_LIB_TRACE_HEX_ARRAY_ITEM_LEN 4

//////////////////////////
#if (DEF_DBG_LV >= TRACE_LV_V)
#    define TRACE_V(FMT, ...) Lib_LogPrintf(TRACE_LV_V, FMT, ##__VA_ARGS__)
#else
#    define TRACE_V(FMT, ...)
#endif

#if (DEF_DBG_LV >= TRACE_LV_DBG)
#    define TRACE_D(FMT, ...) Lib_LogPrintf(TRACE_LV_DBG, FMT, ##__VA_ARGS__)
#else
#    define TRACE_D(FMT, ...)
#endif

#if (DEF_DBG_LV >= TRACE_LV_INFO)
#    define TRACE_I(FMT, ...) Lib_LogPrintf(TRACE_LV_INFO, FMT, ##__VA_ARGS__)
#else
#    define TRACE_I(FMT, ...)
#endif

#if (DEF_DBG_LV >= TRACE_LV_WARN)
#    define TRACE_W(FMT, ...) Lib_LogPrintf(TRACE_LV_WARN, FMT, ##__VA_ARGS__)
#else
#    define TRACE_W(FMT, ...)
#endif

#if (DEF_DBG_LV >= TRACE_LV_ERR)
#    define TRACE_E(FMT, ...) Lib_LogPrintf(TRACE_LV_ERR, FMT, ##__VA_ARGS__)
#else
#    define TRACE_E(FMT, ...)
#endif

#if (DEF_DBG_LV >= TRACE_LV_INFO)
#    define TRACE_HEX_ARRAY(data, data_len) Lib_LogHexArray((uint8_t *)(data), (uint32_t)(data_len))
#endif

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
typedef struct lib_log_item_handle_t {
    uint8_t  Lv;
    uint16_t Len;
    uint32_t Ts;
    uint32_t SeqNbr;
    uint8_t  Buf[0];
} __packed LIB_TRACE_ITEM_HANDLE_T;

typedef bool (*Lib_LogWr_Fn)(LIB_TRACE_ITEM_HANDLE_T *item);

typedef void *LIB_TRACE_WRITER_HDLR_T;

/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
DEF_LIB_TRACE_EXT void                    Lib_LogInit(void);

DEF_LIB_TRACE_EXT LIB_TRACE_WRITER_HDLR_T Lib_LogRegistWriter(Lib_LogWr_Fn wr_fnct);

DEF_LIB_TRACE_EXT bool                    Lib_LogUnregistWriter(LIB_TRACE_WRITER_HDLR_T writer_hdl);

DEF_LIB_TRACE_EXT void                    Lib_LogWrDone(LIB_TRACE_WRITER_HDLR_T writer_hdl);

DEF_LIB_TRACE_EXT void                    Lib_LogPrintf(uint8_t lv, const char *fmt, ...);

DEF_LIB_TRACE_EXT void                    Lib_LogHexArray(uint8_t *data, uint32_t len);

DEF_LIB_TRACE_EXT void                    Lib_LogProc(void);

#endif /* __LIB_TRACE_H__ */