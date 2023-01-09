
#include "lib_sys_log.h"
#include "data_struct/queue_malloc.h"

typedef struct lib_log_writer_handle_t {
    void        *LastLogElement;
    uint32_t     LastSeqNbr;
    Lib_LogWr_Fn Wr;
    bool         PerifRdy;
} LIB_LOG_WRITER_T;

typedef struct lib_log_handle_t {
    bool             InitDone;
    uint32_t         RdSeqNbr;
    uint32_t         WrSeqNbr;
    queue_t          Queue;
    uint8_t          WorkBuf[DEF_LIB_LOG_MAX_MSG_SIZE];
    LIB_LOG_WRITER_T WriterArray[DEF_LIB_LOG_MAX_WRITER_NBR];
} LIB_LOG_HANDLE_T;

static LIB_LOG_HANDLE_T Lib_LogHdl;
static uint8_t          Lib_LogBuff[DEF_LIB_LOG_MSG_Q_SIZE];
static uint32_t         Lib_LogSeqNbr  = 0;
static volatile bool    Lib_LogProcReq = false;

/********** static ************/
static inline void Lib_LogLock(void)
{
    // LockFun();
}

static inline void Lib_LogUnlock(void)
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
//
void Lib_LogInit(void)
{
    QueueMallocInit(&Lib_LogHdl.Queue, Lib_LogBuff, sizeof(Lib_LogBuff));
    Lib_LogHdl.InitDone = true;
}

void Lib_LogPrintf(uint8_t level, const char *fmt, ...)
{
    LIB_LOG_ITEM_T *log_item;
    queue_t        *que = &Lib_LogHdl.Queue;
    char           *prefix;
    va_list         args;
    uint32_t        ts;
    int32_t         ret;
    uint32_t        log_item_size;

    if (!Lib_LogHdl.InitDone) {
        return;
    }
    /* clang-format off */
    switch (level) {
        case DEF_LIB_LOG_NONE: prefix = "[N]"; break;
        case DEF_LIB_LOG_ERR : prefix = "[E]"; break;
        case DEF_LIB_LOG_WARN: prefix = "[W]"; break;
        case DEF_LIB_LOG_INFO: prefix = "[I]"; break;
        case DEF_LIB_LOG_DBG : prefix = "[D]"; break;
        default: prefix = ""; break;
    }
    /* clang-format on */

    Lib_LogLock();
    log_item = (LIB_LOG_ITEM_T *)Lib_LogHdl.WorkBuf;
    va_start(args, fmt);
    ts  = Lib_LogGetTs();
    ret = snprintf((char *)log_item->Buf, DEF_LIB_LOG_MAX_MSG_SIZE, "(%u) %s", ts, prefix);
    ret += vsnprintf((char *)log_item->Buf + ret, DEF_LIB_LOG_MAX_MSG_SIZE - ret, fmt, args);
    ret++;
    va_end(args);

    if (ret > 0) {
        log_item->Lv     = level;
        log_item->Len    = ret;
        log_item->Ts     = ts;
        log_item->SeqNbr = Lib_LogSeqNbr++;
        log_item_size    = ret + sizeof(LIB_LOG_ITEM_T);
        while (!QueueMallocSpaceChk(que, log_item_size)) {
            QueueMallocOutput(que, NULL);
            Lib_LogHdl.RdSeqNbr++;
        }
        QueueMallocInput(que, log_item, log_item_size);
        Lib_LogHdl.WrSeqNbr = log_item->SeqNbr;
        Lib_LogNeedWr();
    }
    Lib_LogUnlock();

#if DEF_LIB_LOG_PRINTF
    // static uint32_t t   = 0;
    // static uint32_t err = 0;
    // uint32_t        len = strlen(log_item->Buf);
    // if (len == 0) {
    //     err++;
    // }
    // printf("File[%d] e=%d %s\n", t++, err, log_item->Buf);
    printf("%s\n", log_item->Buf);
#endif
}

void Lib_LogHexArray(uint8_t *data, uint32_t len)
{
}

LIB_LOG_WR_T Lib_LogRegistWriter(Lib_LogWr_Fn wr_fnct)
{
    uint32_t i;

    Lib_LogLock();
    for (i = 0; i < DEF_LIB_LOG_MAX_WRITER_NBR; i++) {
        if (Lib_LogHdl.WriterArray[i].Wr == NULL) {
            Lib_LogHdl.WriterArray[i].Wr         = wr_fnct;
            Lib_LogHdl.WriterArray[i].PerifRdy   = true;
            Lib_LogHdl.WriterArray[i].LastSeqNbr = Lib_LogHdl.RdSeqNbr;
            Lib_LogUnlock();
            return (LIB_LOG_WR_T *)(&Lib_LogHdl.WriterArray[i]);
        }
    }
    Lib_LogUnlock();
    return NULL;
}

bool Lib_LogUnregistWriter(LIB_LOG_WR_T writer_hdl)
{
    uint32_t i;

    Lib_LogLock();
    for (i = 0; i < DEF_LIB_LOG_MAX_WRITER_NBR; i++) {
        if (Lib_LogHdl.WriterArray[i].Wr == writer_hdl) {
            Lib_LogHdl.WriterArray[i].Wr         = NULL;
            Lib_LogHdl.WriterArray[i].PerifRdy   = false;
            Lib_LogHdl.WriterArray[i].LastSeqNbr = 0;
            Lib_LogUnlock();
            return true;
        }
    }
    Lib_LogUnlock();
    return false;
}

void Lib_LogNeedWr(void)
{
    uint32_t i;

    Lib_LogLock();
    for (i = 0; i < DEF_LIB_LOG_MAX_WRITER_NBR; i++) {
        if (Lib_LogHdl.WriterArray[i].Wr != NULL) {
            Lib_LogHdl.WriterArray[i].PerifRdy = true;
        }
    }
    Lib_LogUnlock();
    Lib_LogStartProcReq();
}

void Lib_LogLoop(void)
{
    uint32_t          i;
    LIB_LOG_WRITER_T *write;
    LIB_LOG_ITEM_T   *log_item;

    if (!Lib_LogProcReq) {
        return;
    }
    Lib_LogProcReq = false;

    for (i = 0; i < DEF_LIB_LOG_MAX_WRITER_NBR; i++) {
        Lib_LogLock();
        write = &Lib_LogHdl.WriterArray[i];
        if (write->Wr == NULL || write->PerifRdy == false) {
            Lib_LogUnlock();
            continue;
        }
        if (write->LastLogElement == NULL || write->LastSeqNbr < Lib_LogHdl.RdSeqNbr) {          // 第一次写
            write->LastSeqNbr = Lib_LogHdl.RdSeqNbr;
            log_item          = QueueMallocPeek(&Lib_LogHdl.Queue, NULL);
            Lib_LogUnlock();
            if (log_item) {
                write->PerifRdy = false;
                if (write->Wr(log_item)) {
                    write->LastLogElement = log_item;
                } else {
                    write->PerifRdy = true;
                }
                Lib_LogProcReq = true;
            }
        } else {
            log_item = QueueMallocPeekAfter(&Lib_LogHdl.Queue, write->LastLogElement, NULL);
            Lib_LogUnlock();
            if (log_item) {
                write->PerifRdy = false;
                if (write->Wr(log_item)) {
                    write->LastLogElement = log_item;
                    write->LastSeqNbr++;
                } else {
                    write->PerifRdy = true;
                }
                Lib_LogProcReq = true;
            }
        }
    }
}