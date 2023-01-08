#ifndef __DEF_LIB_LOG_H__
#define __DEF_LIB_LOG_H__

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
#    define DEF_LIB_LOG_EXT
#else
#    define DEF_LIB_LOG_EXT extern
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
#define DEF_LIB_LOG_NONE               0
#define DEF_LIB_LOG_ERR                1
#define DEF_LIB_LOG_WARN               2
#define DEF_LIB_LOG_INFO               3
#define DEF_LIB_LOG_DBG                4
#define DEF_LIB_LOG_V                  5
// config
#define DEF_LIB_LOG_LEVEL              DEF_LIB_LOG_DBG

#define DEF_LIB_LOG_PRINTF             0
#define DEF_LIB_LOG_COLOR_PRINT_EN     1

#define DEF_LIB_LOG_MSG_Q_SIZE         4096
#define DEF_LIB_LOG_MAX_MSG_SIZE       256
#define DEF_LIB_LOG_MAX_WRITER_NBR     5

#define DEF_LIB_LOG_HEX_ARRAY_LINE_LEN 32
#define DEF_LIB_LOG_HEX_ARRAY_ITEM_LEN 4

//////////////////////////
#if (DEF_LIB_LOG_LEVEL >= DEF_LIB_LOG_V)
#    define TRACE_V(FMT, ...) Lib_LogPrintf(DEF_LIB_LOG_V, FMT, ##__VA_ARGS__)
#else
#    define TRACE_V(FMT, ...)
#endif

#if (DEF_LIB_LOG_LEVEL >= DEF_LIB_LOG_DBG)
#    define TRACE_D(FMT, ...) Lib_LogPrintf(DEF_LIB_LOG_DBG, FMT, ##__VA_ARGS__)
#else
#    define TRACE_D(FMT, ...)
#endif

#if (DEF_LIB_LOG_LEVEL >= DEF_LIB_LOG_INFO)
#    define TRACE_I(FMT, ...) Lib_LogPrintf(DEF_LIB_LOG_INFO, FMT, ##__VA_ARGS__)
#else
#    define TRACE_I(FMT, ...)
#endif

#if (DEF_LIB_LOG_LEVEL >= DEF_LIB_LOG_WARN)
#    define TRACE_W(FMT, ...) Lib_LogPrintf(DEF_LIB_LOG_WARN, FMT, ##__VA_ARGS__)
#else
#    define TRACE_W(FMT, ...)
#endif

#if (DEF_LIB_LOG_LEVEL >= DEF_LIB_LOG_ERR)
#    define TRACE_E(FMT, ...) Lib_LogPrintf(DEF_LIB_LOG_ERR, FMT, ##__VA_ARGS__)
#else
#    define TRACE_E(FMT, ...)
#endif

#if (DEF_LIB_LOG_LEVEL >= DEF_LIB_LOG_INFO)
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
} __packed LIB_LOG_ITEM_T;

typedef bool (*Lib_LogWr_Fn)(LIB_LOG_ITEM_T *item);

typedef void *LIB_LOG_WR_T;

/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
DEF_LIB_LOG_EXT void         Lib_LogInit(void);

DEF_LIB_LOG_EXT void         Lib_LogPrintf(uint8_t lv, const char *fmt, ...);

DEF_LIB_LOG_EXT void         Lib_LogHexArray(uint8_t *data, uint32_t len);

DEF_LIB_LOG_EXT LIB_LOG_WR_T Lib_LogRegistWriter(Lib_LogWr_Fn wr_fnct);

DEF_LIB_LOG_EXT bool         Lib_LogUnregistWriter(LIB_LOG_WR_T writer_hdl);

DEF_LIB_LOG_EXT void         Lib_LogNeedWr(void);

DEF_LIB_LOG_EXT void         Lib_LogLoop(void);

#endif /* __DEF_LIB_LOG_H__ */