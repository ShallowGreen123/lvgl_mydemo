

#ifndef __DATA_MODEL__
#define __DATA_MODEL__

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;

#ifdef __DATA_MODEL_C__
#define DEF_EXTERN
#else
#define DEF_EXTERN extern
#endif

typedef enum data_model_id_e {
    DATA_MODEL_ID_0 = 0,
    DATA_MODEL_ID_1,
    DATA_MODEL_ID_2,
    DATA_MODEL_ID_3,
    DATA_MODEL_ID_4,
    DATA_MODEL_ID_5,
    DATA_MODEL_ID_6,
    DATA_MODEL_ID_7,
    DATA_MODEL_ID_8,
    DATA_MODEL_ID_MAX,
} DATA_MODEL_ID_E;

typedef enum data_model_err_e {
    DATA_MODEL_OK = 0,
    DATA_MODEL_ERR_EXIT,
    DATA_MODEL_ERR_NULL_POINT,
    DATA_MODEL_ERR_OUT_RANGE,
    DATA_MODEL_ERR_FULL_LIST,
} DATA_MODEL_ERR_E;

typedef void (*DataModelListener)(DATA_MODEL_ID_E id);

typedef struct c_generic_type_s {
    void *data;
    u32   extra1;
    u32   extra2;
} C_GENERIC_TYPE_S;

/*** DataModelInit ***/
DEF_EXTERN void DataModelInit();
DEF_EXTERN int  DataModelRegister(DataModelListener listener);
DEF_EXTERN int  DataModelRemove(DataModelListener listener);
/*** 8 ***/
DEF_EXTERN int DataModelSetU8(DATA_MODEL_ID_E id, u8 val);
DEF_EXTERN int DataModelGetU8(DATA_MODEL_ID_E id, u8 *val);
DEF_EXTERN int DataModelSetS8(DATA_MODEL_ID_E id, s8 val);
DEF_EXTERN int DataModelGetS8(DATA_MODEL_ID_E id, s8 *val);
/*** 16 ***/
DEF_EXTERN int DataModelSetU16(DATA_MODEL_ID_E id, u16 val);
DEF_EXTERN int DataModelGetU16(DATA_MODEL_ID_E id, u16 *val);
DEF_EXTERN int DataModelSetS16(DATA_MODEL_ID_E id, s16 val);
DEF_EXTERN int DataModelGetS16(DATA_MODEL_ID_E id, s16 *val);
/*** 32 ***/
DEF_EXTERN int DataModelSetU32(DATA_MODEL_ID_E id, u32 val);
DEF_EXTERN int DataModelGetU32(DATA_MODEL_ID_E id, u32 *val);
DEF_EXTERN int DataModelSetS32(DATA_MODEL_ID_E id, s32 val);
DEF_EXTERN int DataModelGetS32(DATA_MODEL_ID_E id, s32 *val);
/*** 64 ***/
DEF_EXTERN int DataModelSetU64(DATA_MODEL_ID_E id, u64 val);
DEF_EXTERN int DataModelGetU64(DATA_MODEL_ID_E id, u64 *val);
DEF_EXTERN int DataModelSetS64(DATA_MODEL_ID_E id, s64 val);
DEF_EXTERN int DataModelGetS64(DATA_MODEL_ID_E id, s64 *val);
/*** Array ***/
DEF_EXTERN int DataModelSetArray(DATA_MODEL_ID_E id, void *arr, u32 len);
DEF_EXTERN int DataModelGetArray(DATA_MODEL_ID_E id, void **arr, u32 *len);
/*** string ***/
DEF_EXTERN int DataModelSetString(DATA_MODEL_ID_E id, void *str);
DEF_EXTERN int DataModelGetString(DATA_MODEL_ID_E id, void **str);

#endif
