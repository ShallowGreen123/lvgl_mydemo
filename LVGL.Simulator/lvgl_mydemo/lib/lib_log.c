/************************************************************************
 * FilePath     : log.c
 * Author       : GX.Duan
 * Date         : 2022-09-18 20:31:59
 * LastEditTime : 2022-09-18 20:33:48
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#define DEF_LIB_TRACE_MODULE

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "lib_log.h"
#include "drv_log.h"
#include "../../lvgl/lvgl.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#define DEF_LIB_TRACE_CIRCULAR_Q_REM_SIZE(q)          (q->QBufSize - q->UsedSize)

#define DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, pos) ((0xFF & q->QBuf[pos]) | ((0xFF & q->QBuf[pos + 1]) << 8))

#define DEF_LIB_TRACE_MSG_Q_SIZE                      4096
#define DEF_LIB_TRACE_MAX_WRITER_NBR                  5

// #define DEF_LIB_TRACE_TIME_HANDLE                     &hlptim5
// #define DEF_LIB_TRACE_TIME_PERIOD                     65535U
// #define DEF_LIB_TRACE_TIME_MS(x)                      (((x)*25000) / 128)
// #define DEF_LIB_TRACE_MAX_TIMEOUT_MS                  335
// #define DEF_LIB_TRACE_USED_OS                         false

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
typedef struct lib_log_circular_q_handle_t {
    uint8_t *QBuf;
    uint32_t QBufSize;
    uint32_t FirstElement;
    uint32_t LastElement;
    uint32_t ElementCnt;
    uint32_t UsedSize;
} LIB_TRACE_CIRCULAR_Q_HANDLE_T;

typedef struct lib_log_writer_handle_t {
    uint8_t     *LastLogElement;
    uint32_t     LastSeqNbr;
    Lib_LogWr_Fn Wr;
    bool         PerifRdy;
} LIB_TRACE_WRITER_HANDLE_T;

typedef struct lib_log_handle_t {
    bool                          InitDone;
    uint32_t                      RdSeqNbr;
    uint32_t                      WrSeqNbr;
    LIB_TRACE_CIRCULAR_Q_HANDLE_T Q;
    uint8_t                       WorkBuf[DEF_LIB_TRACE_MAX_MSG_SIZE];
    LIB_TRACE_WRITER_HANDLE_T     WriterArray[DEF_LIB_TRACE_MAX_WRITER_NBR];
} LIB_TRACE_HANDLE_T;

//
static uint8_t            Lib_LogQBuf[DEF_LIB_TRACE_MSG_Q_SIZE];
static uint32_t           Lib_LogSeqNbr = 0;
static LIB_TRACE_HANDLE_T Lib_LogHdl;
static volatile bool      Lib_LogProcReq = false;

/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/
static int32_t  Lib_LogCircularQInit(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint8_t *q_buf, uint32_t q_buf_size);

static bool     Lib_LogCircularQChkSpace(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint16_t need_size);

static uint8_t *Lib_LogCircularQPush(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint8_t *element, uint16_t element_size);

static uint8_t *Lib_LogCircularQPop(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint16_t *element_size);

static uint8_t *Lib_LogCircularQPeek(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint16_t *element_size);

static uint8_t *Lib_LogCircularQPeekAfterElement(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint8_t *element, uint16_t *element_size);

static int32_t  Lib_LogCircularQClear(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q);

static uint32_t Lib_LogCircularQGetElementNbr(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q);

static int32_t  Lib_LogCircularQInit(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint8_t *q_buf, uint32_t q_buf_size)
{
    q->QBuf         = q_buf;
    q->QBufSize     = q_buf_size;
    q->FirstElement = 0;
    q->LastElement  = 0;
    q->ElementCnt   = 0;
    q->UsedSize     = 0;
    return 0;
}

static bool Lib_LogCircularQChkSpace(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint16_t need_size)
{
    uint16_t sto_size              = need_size + sizeof(uint16_t);
    uint32_t last_element_sto_size = 0;
    uint32_t buf_pos               = 0;

    if (DEF_LIB_TRACE_CIRCULAR_Q_REM_SIZE(q) < sto_size) {
        return false;
    }

    if (q->ElementCnt > 0) {
        last_element_sto_size = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, q->LastElement) + sizeof(uint16_t);
        buf_pos               = q->LastElement + last_element_sto_size;
        if (q->LastElement >= q->FirstElement) {
            if ((q->QBufSize - buf_pos < sto_size) && (q->FirstElement < sto_size)) {
                return false;
            }
        } else if (q->FirstElement - buf_pos < sto_size) {
            return false;
        }
    }
    return true;
}

static uint8_t *Lib_LogCircularQPush(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint8_t *element, uint16_t element_size)
{
    uint16_t sto_size              = element_size + sizeof(uint16_t);
    uint32_t last_element_sto_size = 0;
    uint32_t buf_pos               = 0;

    if (element == NULL || element_size == 0) {
        return NULL;
    }

    if (DEF_LIB_TRACE_CIRCULAR_Q_REM_SIZE(q) < sto_size) {
        // No enough space to store this element
        return NULL;
    }

    if (q->ElementCnt > 0) {
        last_element_sto_size = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, q->LastElement) + sizeof(uint16_t);
        buf_pos               = q->LastElement + last_element_sto_size;
        if (q->LastElement >= q->FirstElement) {
            if ((q->QBufSize - buf_pos < sto_size) && (q->FirstElement < sto_size)) {
                // No enough space to store this element
                return NULL;
            }
            if (q->QBufSize - buf_pos < sto_size) {
                // Mark eof as unused
                q->QBuf[buf_pos]     = 0xFF;
                q->QBuf[buf_pos + 1] = 0xFF;
                buf_pos              = 0;
            }
        } else if (q->FirstElement - buf_pos < sto_size) {
            // No enough space to store this element
            return NULL;
        }
    }

    q->LastElement = buf_pos;

    // Store size
    q->QBuf[buf_pos++] = 0xFF & element_size;
    q->QBuf[buf_pos++] = (0xFF00 & element_size) >> 8;

    // Store data
    memcpy((void *)&q->QBuf[buf_pos], element, element_size);

    q->UsedSize += sto_size;
    q->ElementCnt++;

    return element;
}

static uint8_t *Lib_LogCircularQPop(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint16_t *element_size)
{
    uint8_t *data;
    uint16_t sto_size;

    if (q->ElementCnt == 0) {
        return NULL;
    }

    *element_size = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, q->FirstElement);
    sto_size      = *element_size + sizeof(uint16_t);
    data          = &q->QBuf[q->FirstElement + sizeof(uint16_t)];
    q->FirstElement += sto_size;
    if (DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, q->FirstElement) == 0xFFFF) {
        // Eof mark
        q->FirstElement = 0;
    }
    q->UsedSize -= sto_size;
    q->ElementCnt--;

    return data;
}

static uint8_t *Lib_LogCircularQPeek(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint16_t *element_size)
{
    if (q->ElementCnt == 0) {
        return NULL;
    }

    *element_size = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, q->FirstElement);
    return &q->QBuf[q->FirstElement + sizeof(uint16_t)];
}

static uint8_t *Lib_LogCircularQPeekAfterElement(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q, uint8_t *element, uint16_t *element_size)
{
    uint16_t size;
    uint32_t offset = element - q->QBuf - 2;

    if ((q->ElementCnt == 0) || (q->LastElement == offset)) {
        return NULL;
    }
    size = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, offset);
    offset += (size + sizeof(uint16_t));
    size = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, offset);
    if (size == 0xFFFF) {
        offset = 0;
        size   = DEF_LIB_TRACE_CIRCULAR_Q_ELEMENT_SIZE(q, offset);
    }
    *element_size = size;
    return &q->QBuf[offset + sizeof(uint16_t)];
}

static int32_t Lib_LogCircularQClear(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q)
{
    q->FirstElement = 0;
    q->LastElement  = 0;
    q->ElementCnt   = 0;
    q->UsedSize     = 0;
    return 0;
}

static uint32_t Lib_LogCircularQGetElementNbr(LIB_TRACE_CIRCULAR_Q_HANDLE_T *q)
{
    return q->ElementCnt;
}

static inline void Lib_LogIntLock(void)
{
    // IntLockFun();
}

static inline void Lib_LogIntUnlock(void)
{
    // UnlockFun();
}

static inline uint32_t Lib_LogGetTs(void)
{
    // return GetTick();
    return lv_tick_get();
}

static void Lib_LogStartProcReq(void)
{
    Lib_LogProcReq = true;
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/
void Lib_LogInit(void)
{
    memset((void *)&Lib_LogHdl, 0, sizeof(Lib_LogHdl));
    Lib_LogCircularQInit(&Lib_LogHdl.Q, Lib_LogQBuf, sizeof(Lib_LogQBuf));
    Lib_LogHdl.InitDone = true;

#if !(DEF_LIB_TRACE_FORCE_PRINTF)
    Drv_LogSetupPrintf();
#endif
}

LIB_TRACE_WRITER_HDLR_T Lib_LogRegistWriter(Lib_LogWr_Fn wr_fnct)
{
    uint32_t i;

    Lib_LogIntLock();
    for (i = 0; i < DEF_LIB_TRACE_MAX_WRITER_NBR; i++) {
        if (Lib_LogHdl.WriterArray[i].Wr == NULL) {
            Lib_LogHdl.WriterArray[i].Wr         = wr_fnct;
            Lib_LogHdl.WriterArray[i].LastSeqNbr = Lib_LogHdl.RdSeqNbr;
            Lib_LogHdl.WriterArray[i].PerifRdy   = true;
            Lib_LogIntUnlock();
            return (LIB_TRACE_WRITER_HDLR_T)(&Lib_LogHdl.WriterArray[i]);
        }
    }
    Lib_LogIntUnlock();
    return NULL;
}

bool Lib_LogUnregistWriter(LIB_TRACE_WRITER_HDLR_T writer_hdl)
{
    uint32_t i;

    Lib_LogIntLock();
    for (i = 0; i < DEF_LIB_TRACE_MAX_WRITER_NBR; i++) {
        if (&Lib_LogHdl.WriterArray[i] == writer_hdl) {
            Lib_LogHdl.WriterArray[i].Wr         = NULL;
            Lib_LogHdl.WriterArray[i].LastSeqNbr = 0;
            Lib_LogHdl.WriterArray[i].PerifRdy   = false;
            Lib_LogIntUnlock();
            return true;
        }
    }
    Lib_LogIntUnlock();
    return false;
}

void Lib_LogWrDone(LIB_TRACE_WRITER_HDLR_T writer_hdl)
{
    if (writer_hdl == NULL) {
        return;
    }
    Lib_LogIntLock();
    ((LIB_TRACE_WRITER_HANDLE_T *)writer_hdl)->PerifRdy = true;
    Lib_LogIntUnlock();
    Lib_LogStartProcReq();
}

void Lib_LogPrintf(uint8_t lv, const char *fmt, ...)
{
    LIB_TRACE_ITEM_HANDLE_T       *log_element;
    LIB_TRACE_CIRCULAR_Q_HANDLE_T *queue = &Lib_LogHdl.Q;
    char                          *prefix;
    va_list                        args;
    int32_t                        ret;
    uint32_t                       ts;
    uint16_t                       element_size;
    uint16_t                       tmp_size;

    if (!Lib_LogHdl.InitDone) {
        return;
    }

    switch (lv) {
        case TRACE_LV_V:
            prefix = "[V]";
            break;
        case TRACE_LV_DBG:
            prefix = "[D]";
            break;
        case TRACE_LV_INFO:
            prefix = "[I]";
            break;
        case TRACE_LV_WARN:
            prefix = "[W]";
            break;
        case TRACE_LV_ERR:
            prefix = "[E]";
            break;
        default:
            prefix = "[X]";
            break;
    }

    Lib_LogIntLock();
    log_element = (LIB_TRACE_ITEM_HANDLE_T *)Lib_LogHdl.WorkBuf;
    va_start(args, fmt);
    ts  = Lib_LogGetTs();
    ret = snprintf((char *)log_element->Buf, DEF_LIB_TRACE_MAX_MSG_SIZE, "(%u) %s ", ts, prefix);
    ret += vsnprintf((char *)log_element->Buf + ret, DEF_LIB_TRACE_MAX_MSG_SIZE - ret, fmt, args);
    ret++; /* for terminating null character */
    va_end(args);

    if (ret > 0) {
        log_element->Lv     = lv;
        log_element->Len    = ret;
        log_element->Ts     = ts;
        log_element->SeqNbr = Lib_LogSeqNbr++;
        element_size        = ret + sizeof(LIB_TRACE_ITEM_HANDLE_T);
        while (!Lib_LogCircularQChkSpace(queue, element_size)) {
            Lib_LogCircularQPop(queue, &tmp_size);
            Lib_LogHdl.RdSeqNbr++;
        }
        Lib_LogCircularQPush(queue, (uint8_t *)log_element, element_size);
        Lib_LogHdl.WrSeqNbr = log_element->SeqNbr;
        Lib_LogStartProcReq();
    }
    Lib_LogIntUnlock();

#if DEF_LIB_TRACE_FORCE_PRINTF
#endif

#if DEF_LOG_COLOR_PRINT_EN
    Drv_LogColorPrintf(log_element->Buf, prefix);
#else
    Drv_LogPrintf(log_element->Buf);
#endif
}

void Lib_LogHexArray(uint8_t *data, uint32_t len)
{
    uint8_t  buf[DEF_LIB_TRACE_HEX_ARRAY_LINE_LEN * DEF_LIB_TRACE_HEX_ARRAY_ITEM_LEN + 1];
    uint8_t  remain = len;
    uint8_t *p      = data;
    uint8_t  dump_len;
    uint8_t  i;

    while (remain > 0) {
        if (remain > DEF_LIB_TRACE_HEX_ARRAY_LINE_LEN) {
            dump_len = DEF_LIB_TRACE_HEX_ARRAY_LINE_LEN;
        } else {
            dump_len = remain;
        }
        for (i = 0; i < dump_len; i++) {
            snprintf((char *)buf + i * DEF_LIB_TRACE_HEX_ARRAY_ITEM_LEN, sizeof(buf) - i * DEF_LIB_TRACE_HEX_ARRAY_ITEM_LEN, "%02X, ", *p++);
        }
        TRACE_I("%s\n", buf);
        remain -= dump_len;
    }
}

void Lib_LogProc(void)
{
    LIB_TRACE_WRITER_HANDLE_T *writer;
    uint8_t                   *element;
    uint16_t                   element_size;
    uint32_t                   i;

    if (!Lib_LogProcReq) {
        return;
    }
    Lib_LogProcReq = false;
    for (i = 0; i < DEF_LIB_TRACE_MAX_WRITER_NBR; i++) {
        Lib_LogIntLock();
        writer = &Lib_LogHdl.WriterArray[i];
        if ((writer->Wr == NULL) || (!writer->PerifRdy)) {
            Lib_LogIntUnlock();
            continue;
        }
        if ((!writer->LastLogElement) || (writer->LastSeqNbr < Lib_LogHdl.RdSeqNbr)) {
            writer->LastSeqNbr = Lib_LogHdl.RdSeqNbr;
            element            = Lib_LogCircularQPeek(&Lib_LogHdl.Q, &element_size);
            Lib_LogIntUnlock();
            if (element) {
                writer->PerifRdy = false;
                if (writer->Wr((LIB_TRACE_ITEM_HANDLE_T *)element)) {
                    writer->LastLogElement = element;
                } else {
                    writer->PerifRdy = true;
                }
                Lib_LogProcReq = true;
            }
        } else {
            element = Lib_LogCircularQPeekAfterElement(&Lib_LogHdl.Q, writer->LastLogElement, &element_size);
            Lib_LogIntUnlock();
            if (element) {
                writer->PerifRdy = false;
                if (writer->Wr((LIB_TRACE_ITEM_HANDLE_T *)element)) {
                    writer->LastSeqNbr++;
                    writer->LastLogElement = element;
                } else {
                    writer->PerifRdy = true;
                }
                Lib_LogProcReq = true;
            }
        }
    }
}
