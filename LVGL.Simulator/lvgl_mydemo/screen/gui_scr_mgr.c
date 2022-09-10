/************************************************************************
 * FilePath     : gui_scr_mgr.c
 * Author       : GX.Duan
 * Date         : 2022-09-10 14:17:29
 * LastEditTime : 2022-09-10 15:43:26
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#define __SCR_MGR_C__

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "gui_scr_mgr.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/

typedef struct scr_mgr_reg_scr_list_handle_t {
    uint32_t              ScrCnt;
    SCR_MGR_SCR_HANDLE_T *ScrHandles[0];
} SCR_MGR_REG_SCR_LIST_HANDLE_T;

static SCR_MGR_REG_SCR_LIST_HANDLE_T *ScrMgrRegScrList;
static SCR_MGR_SCR_STACK_HANDLE_T    *ScrMgrScrStackTop;
static SCR_MGR_SCR_STACK_HANDLE_T    *ScrMgrScrStackRoot;

static const SCR_MGR_SCR_HANDLE_T    *ScrMgrScrHandles[] = {
       &Gui_MainScrHandle,
       &Gui_Test1ScrHandle,
};

/*********************************************************************************
 *                              STATIC FUNCTION
 *********************************************************************************/

static SCR_MGR_SCR_HANDLE_T *ScrMgrFindScrById(uint32_t id) {
    uint32_t i;

    for (i = 0; i < ScrMgrRegScrList->ScrCnt; i++) {
        if (ScrMgrRegScrList->ScrHandles[i]->ScrId == id) {
            return ScrMgrRegScrList->ScrHandles[i];
        }
    }
    return NULL;
}

static void ScrMgrStackScrActive(SCR_MGR_SCR_STACK_HANDLE_T *stack_item) {
    if (stack_item->State == SCR_MGR_SCR_STATE_DESTROYED) {
        stack_item->ScrObj = stack_item->ScrHandle->ScrCreate(NULL);
        stack_item->ScrHandle->ScrEnter();
        stack_item->State = SCR_MGR_SCR_STATE_ACTIVE;
    } else if ((stack_item->State == SCR_MGR_SCR_STATE_CREATED) || (stack_item->State == SCR_MGR_SCR_STATE_INACTIVE)) {
        stack_item->ScrHandle->ScrEnter();
        stack_item->State = SCR_MGR_SCR_STATE_ACTIVE;
    }
}

static void ScrMgrStackScrInactive(SCR_MGR_SCR_STACK_HANDLE_T *stack_item) {
    if (stack_item->State > SCR_MGR_SCR_STATE_INACTIVE) {
        stack_item->ScrHandle->ScrExit();
        stack_item->State = SCR_MGR_SCR_STATE_INACTIVE;
    }
}

static void ScrMgrStackScrDestroy(SCR_MGR_SCR_STACK_HANDLE_T *stack_item) {
    if (stack_item->State > SCR_MGR_SCR_STATE_INACTIVE) {
        stack_item->ScrHandle->ScrExit();
        stack_item->ScrHandle->ScrDestroy();
        stack_item->State = SCR_MGR_SCR_STATE_DESTROYED;
    } else if (stack_item->State > SCR_MGR_SCR_STATE_DESTROYED) {
        stack_item->ScrHandle->ScrDestroy();
        stack_item->State = SCR_MGR_SCR_STATE_DESTROYED;
    }
}

static void ScrMgrStackScrRemove(SCR_MGR_SCR_STACK_HANDLE_T *stack_item) {
    if (stack_item->State > SCR_MGR_SCR_STATE_INACTIVE) {
        stack_item->ScrHandle->ScrExit();
        stack_item->ScrHandle->ScrDestroy();
        stack_item->State = SCR_MGR_SCR_STATE_IDLE;
    } else if (stack_item->State > SCR_MGR_SCR_STATE_DESTROYED) {
        stack_item->ScrHandle->ScrDestroy();
        stack_item->State = SCR_MGR_SCR_STATE_IDLE;
    }
}

/*********************************************************************************
 *                              GLOBAL FUNCTION
 *********************************************************************************/

void ScrMgrInit(void) {
    uint32_t cnt = sizeof(ScrMgrScrHandles) / sizeof(ScrMgrScrHandles[0]);
    uint32_t i;

    LV_ASSERT(cnt);
    ScrMgrScrStackRoot = NULL;
    ScrMgrScrStackTop  = NULL;
    ScrMgrRegScrList   = lv_mem_alloc(sizeof(SCR_MGR_REG_SCR_LIST_HANDLE_T) + cnt * sizeof(SCR_MGR_SCR_HANDLE_T *));
    LV_ASSERT(ScrMgrRegScrList);

    ScrMgrRegScrList->ScrCnt = cnt;
    for (i = 0; i < cnt; i++) {
        ScrMgrRegScrList->ScrHandles[i] = (SCR_MGR_SCR_HANDLE_T *)ScrMgrScrHandles[i];
    }
}

bool ScrMgrSwitchScr(uint32_t scr_id, bool anim) {
    SCR_MGR_SCR_HANDLE_T       *tgt_scr     = ScrMgrFindScrById(scr_id);
    SCR_MGR_SCR_HANDLE_T       *cur_scr     = NULL;
    lv_obj_t                   *cur_scr_obj = NULL;
    SCR_MGR_SCR_STACK_HANDLE_T *stack_item  = NULL;

    if (tgt_scr == NULL) {
        return false;
    }

    if (ScrMgrScrStackTop != NULL) {
        cur_scr     = ScrMgrScrStackTop->ScrHandle;
        cur_scr_obj = ScrMgrScrStackTop->ScrObj;
        stack_item  = ScrMgrScrStackTop->Prev;
        ScrMgrStackScrRemove(ScrMgrScrStackTop);
        lv_mem_free((void *)ScrMgrScrStackTop);
        ScrMgrScrStackTop = stack_item;
    }
    while (ScrMgrScrStackTop != NULL) {
        stack_item = ScrMgrScrStackTop->Prev;
        ScrMgrStackScrRemove(ScrMgrScrStackTop);
        lv_mem_free((void *)ScrMgrScrStackTop);
        ScrMgrScrStackTop = stack_item;
    }

    if ((cur_scr != NULL) && (cur_scr->ScrId == tgt_scr->ScrId)) {
        /* same scr, need destroy immediately, not support anim */
        anim = false;
    }
    stack_item            = lv_mem_alloc(sizeof(SCR_MGR_SCR_STACK_HANDLE_T));
    stack_item->ScrHandle = tgt_scr;
    stack_item->Next      = NULL;
    stack_item->Prev      = NULL;
    stack_item->ScrObj    = tgt_scr->ScrCreate(NULL);
    stack_item->State     = SCR_MGR_SCR_STATE_CREATED;
    ScrMgrScrStackRoot    = stack_item;
    ScrMgrScrStackTop     = stack_item;
#if DEF_SCR_MGR_LITTLE_MEM
    anim = false;
    if (cur_scr_obj) {
        lv_obj_del(cur_scr_obj);
        cur_scr_obj = NULL;
    }
#endif
    ScrMgrStackScrActive(stack_item);
    if ((SCR_MSG_SCR_SWITCH_ANIM != LV_SCR_LOAD_ANIM_NONE) && anim) {
        lv_scr_load_anim(stack_item->ScrObj, SCR_MSG_SCR_SWITCH_ANIM, SCR_MGR_SCR_TRANS_ANIM_INT, 0, true);
    } else {
        lv_scr_load(stack_item->ScrObj);
        if (cur_scr_obj) {
            lv_obj_del(cur_scr_obj);
        }
    }

    return true;
}

bool ScrMgrPushScr(uint32_t scr_id, bool anim) {
    SCR_MGR_SCR_HANDLE_T       *tgt_scr = ScrMgrFindScrById(scr_id);
    SCR_MGR_SCR_STACK_HANDLE_T *stack_item;

    if (tgt_scr == NULL) {
        return false;
    }

    if ((ScrMgrScrStackTop != NULL) && (ScrMgrScrStackTop->ScrHandle->ScrId == tgt_scr->ScrId)) {
        return false;
    }
#if DEF_SCR_MGR_LITTLE_MEM
    anim = false;
    if (ScrMgrScrStackTop != NULL) {
        ScrMgrStackScrDestroy(ScrMgrScrStackTop);
        lv_obj_del(ScrMgrScrStackTop->ScrObj);
        ScrMgrScrStackTop->ScrObj = NULL;
    }
#endif
    stack_item            = lv_mem_alloc(sizeof(SCR_MGR_SCR_STACK_HANDLE_T));
    stack_item->ScrHandle = tgt_scr;
    stack_item->ScrObj    = tgt_scr->ScrCreate(NULL);
    stack_item->State     = SCR_MGR_SCR_STATE_CREATED;
    if (ScrMgrScrStackTop == NULL) {
        ScrMgrScrStackRoot = stack_item;
        ScrMgrScrStackTop  = stack_item;
        stack_item->Prev   = NULL;
        stack_item->Next   = NULL;
    } else {
        ScrMgrStackScrInactive(ScrMgrScrStackTop);
        ScrMgrScrStackTop->Next = stack_item;
        stack_item->Prev        = ScrMgrScrStackTop;
        stack_item->Next        = NULL;
        ScrMgrScrStackTop       = stack_item;
    }
    ScrMgrStackScrActive(stack_item);
    if ((SCR_MSG_SCR_PUSH_ANIM != LV_SCR_LOAD_ANIM_NONE) && anim) {
        lv_scr_load_anim(stack_item->ScrObj, SCR_MSG_SCR_PUSH_ANIM, SCR_MGR_SCR_TRANS_ANIM_INT, 0, false);
    } else {
        lv_scr_load(stack_item->ScrObj);
    }

    return true;
}

bool ScrMgrPopScr(bool anim) {
    SCR_MGR_SCR_STACK_HANDLE_T *dst_item = NULL;
    lv_obj_t                   *cur_scr_obj;
    lv_obj_t                   *dst_scr_obj;

    if ((ScrMgrScrStackTop == NULL) || (ScrMgrScrStackTop == ScrMgrScrStackRoot)) {
        return false;
    }
    cur_scr_obj = ScrMgrScrStackTop->ScrObj;
    dst_item    = ScrMgrScrStackTop->Prev;
    ScrMgrStackScrRemove(ScrMgrScrStackTop);
    lv_mem_free((void *)ScrMgrScrStackTop);
    ScrMgrScrStackTop = dst_item;

#if DEF_SCR_MGR_LITTLE_MEM
    anim = false;
    if (cur_scr_obj) {
        lv_obj_del(cur_scr_obj);
        cur_scr_obj = NULL;
    }
#endif
    ScrMgrStackScrActive(dst_item);
    dst_scr_obj = dst_item->ScrObj;
    if ((SCR_MSG_SCR_POP_ANIM != LV_SCR_LOAD_ANIM_NONE) && anim) {
        lv_scr_load_anim(dst_scr_obj, SCR_MSG_SCR_POP_ANIM, SCR_MGR_SCR_TRANS_ANIM_INT, 0, true);
    } else {
        lv_scr_load(dst_scr_obj);
        if (cur_scr_obj) {
            lv_obj_del(cur_scr_obj);
        }
    }

    return true;
}

bool ScrMgrPopNScr(uint32_t cnt, bool anim) {
    SCR_MGR_SCR_STACK_HANDLE_T *dst_item = NULL;
    lv_obj_t                   *cur_scr_obj;
    lv_obj_t                   *dst_scr_obj;
    uint32_t                    pop_cnt = 0;

    if ((ScrMgrScrStackTop == NULL) || (ScrMgrScrStackTop == ScrMgrScrStackRoot)) {
        return false;
    }
    cur_scr_obj = ScrMgrScrStackTop->ScrObj;
    dst_item    = ScrMgrScrStackTop->Prev;
    ScrMgrStackScrRemove(ScrMgrScrStackTop);
    lv_mem_free((void *)ScrMgrScrStackTop);
    ScrMgrScrStackTop = dst_item;

    while ((++pop_cnt < cnt) && (ScrMgrScrStackTop != NULL) && (ScrMgrScrStackTop != ScrMgrScrStackRoot)) {
        dst_item = ScrMgrScrStackTop->Prev;
        ScrMgrStackScrRemove(ScrMgrScrStackTop);
        lv_obj_del(ScrMgrScrStackTop->ScrObj);
        lv_mem_free((void *)ScrMgrScrStackTop);
        ScrMgrScrStackTop = dst_item;
    }
#if DEF_SCR_MGR_LITTLE_MEM
    anim = false;
    if (cur_scr_obj) {
        lv_obj_del(cur_scr_obj);
        cur_scr_obj = NULL;
    }
#endif
    ScrMgrStackScrActive(dst_item);
    dst_scr_obj = dst_item->ScrObj;
    if ((SCR_MSG_SCR_POP_ANIM != LV_SCR_LOAD_ANIM_NONE) && anim) {
        lv_scr_load_anim(dst_scr_obj, SCR_MSG_SCR_POP_ANIM, SCR_MGR_SCR_TRANS_ANIM_INT, 0, true);
    } else {
        lv_scr_load(dst_scr_obj);
        if (cur_scr_obj) {
            lv_obj_del(cur_scr_obj);
        }
    }

    return true;
}

bool ScrMgrPopToRoot(bool anim) {
    SCR_MGR_SCR_STACK_HANDLE_T *dst_item = NULL;
    lv_obj_t                   *cur_scr_obj;

    if ((ScrMgrScrStackTop == NULL) || (ScrMgrScrStackTop == ScrMgrScrStackRoot)) {
        return false;
    }

    cur_scr_obj = ScrMgrScrStackTop->ScrObj;
    dst_item    = ScrMgrScrStackTop->Prev;
    ScrMgrStackScrRemove(ScrMgrScrStackTop);
    lv_mem_free((void *)ScrMgrScrStackTop);
    ScrMgrScrStackTop = dst_item;
    while ((ScrMgrScrStackTop != NULL) && (ScrMgrScrStackTop != ScrMgrScrStackRoot)) {
        dst_item = ScrMgrScrStackTop->Prev;
        ScrMgrStackScrRemove(ScrMgrScrStackTop);
        lv_obj_del(ScrMgrScrStackTop->ScrObj);
        lv_mem_free((void *)ScrMgrScrStackTop);
        ScrMgrScrStackTop = dst_item;
    }
    ScrMgrScrStackTop = ScrMgrScrStackRoot;
#if DEF_SCR_MGR_LITTLE_MEM
    anim = false;
    if (cur_scr_obj) {
        lv_obj_del(cur_scr_obj);
        cur_scr_obj = NULL;
    }
#endif

    ScrMgrStackScrActive(ScrMgrScrStackRoot);
    if ((SCR_MSG_SCR_POP_ANIM != LV_SCR_LOAD_ANIM_NONE) && anim) {
        lv_scr_load_anim(ScrMgrScrStackRoot->ScrObj, SCR_MSG_SCR_POP_ANIM, SCR_MGR_SCR_TRANS_ANIM_INT, 0, true);
    } else {
        lv_scr_load(ScrMgrScrStackRoot->ScrObj);
        if (cur_scr_obj) {
            lv_obj_del(cur_scr_obj);
        }
    }

    return true;
}

void ScrMgrTrimMem(void) {
    SCR_MGR_SCR_STACK_HANDLE_T *stack_item = ScrMgrScrStackRoot;

    while ((stack_item != NULL) && (stack_item != ScrMgrScrStackTop)) {
        if (stack_item->State > SCR_MGR_SCR_STATE_DESTROYED) {
            ScrMgrStackScrDestroy(stack_item);
            lv_obj_del(stack_item->ScrObj);
            stack_item->ScrObj = NULL;
        }
        stack_item = stack_item->Next;
    }
}
