/************************************************************************
 * FilePath     : dataModel.h
 * Author       : GX.Duan
 * Date         : 2022-08-18
 * LastEditTime : 2022-08-19
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#ifndef __DATA_MODEL__
#define __DATA_MODEL__

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include <stdint.h>
#include "../lvgl_app.h"
/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/
#ifdef __DATA_MODEL_C__
#    define DEF_DATA_EXTERN
#else
#    define DEF_DATA_EXTERN extern
#endif

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;

typedef enum data_model_err_e {
    DATA_MODEL_OK = 0,
    DATA_MODEL_ERR_EXIT,
    DATA_MODEL_ERR_NULL_POINT,
    DATA_MODEL_ERR_OUT_RANGE,
    DATA_MODEL_ERR_FULL_LIST,
} DATA_MODEL_ERR_E;

typedef void (*DataModelListener)(uint32_t id);

typedef struct c_generic_type_s {
    void *data;
    u32   extra1;
    u32   extra2;
} C_GENERIC_TYPE_S;

/*********************************************************************************
 *                              GLOBAL PROTOTYPES
 * *******************************************************************************/
/*** DataModelInit ***/
DEF_DATA_EXTERN void DataModelInit();
DEF_DATA_EXTERN int  DataModelRegister(DataModelListener listener);
DEF_DATA_EXTERN int  DataModelRemove(DataModelListener listener);
/*** 8 ***/
DEF_DATA_EXTERN int DataModelSetU8(uint32_t id, u8 val);
DEF_DATA_EXTERN int DataModelGetU8(uint32_t id, u8 *val);
DEF_DATA_EXTERN int DataModelSetS8(uint32_t id, s8 val);
DEF_DATA_EXTERN int DataModelGetS8(uint32_t id, s8 *val);
/*** 16 ***/
DEF_DATA_EXTERN int DataModelSetU16(uint32_t id, u16 val);
DEF_DATA_EXTERN int DataModelGetU16(uint32_t id, u16 *val);
DEF_DATA_EXTERN int DataModelSetS16(uint32_t id, s16 val);
DEF_DATA_EXTERN int DataModelGetS16(uint32_t id, s16 *val);
/*** 32 ***/
DEF_DATA_EXTERN int DataModelSetU32(uint32_t id, u32 val);
DEF_DATA_EXTERN int DataModelGetU32(uint32_t id, u32 *val);
DEF_DATA_EXTERN int DataModelSetS32(uint32_t id, s32 val);
DEF_DATA_EXTERN int DataModelGetS32(uint32_t id, s32 *val);
/*** 64 ***/
DEF_DATA_EXTERN int DataModelSetU64(uint32_t id, u64 val);
DEF_DATA_EXTERN int DataModelGetU64(uint32_t id, u64 *val);
DEF_DATA_EXTERN int DataModelSetS64(uint32_t id, s64 val);
DEF_DATA_EXTERN int DataModelGetS64(uint32_t id, s64 *val);
/*** Array ***/
DEF_DATA_EXTERN int DataModelSetArray(uint32_t id, void *arr, u32 len);
DEF_DATA_EXTERN int DataModelGetArray(uint32_t id, void **arr, u32 *len);
/*** string ***/
DEF_DATA_EXTERN int DataModelSetString(uint32_t id, void *str);
DEF_DATA_EXTERN int DataModelGetString(uint32_t id, void **str);

#endif
