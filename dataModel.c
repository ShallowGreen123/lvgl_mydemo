
#define __DATA_MODEL_C__
#include "dataModel.h"
#include <string.h>
#include <stdio.h>

static C_GENERIC_TYPE_S  DataModelDataMap[DATA_MODEL_ID_MAX];
static DataModelListener DataModelListenerList[DATA_MODEL_ID_MAX];

/*** init ***/
void DataModelInit(void) {
    memset(DataModelDataMap, 0, sizeof(DataModelDataMap));
    memset(DataModelListenerList, 0, sizeof(DataModelListenerList));
}

int DataModelRegister(DataModelListener listener) {
    int i;
    int idx   = -1;
    int exist = 0;

    if (!listener) {
        return DATA_MODEL_ERR_NULL_POINT;
    }
    for (i = 0; i < DATA_MODEL_ID_MAX; i++) {
        if (DataModelListenerList[i] == listener) {
            exist = 1;
            break;
        }
        if ((idx == -1) && (DataModelListenerList[i] == NULL)) {
            idx = i;
        }
    }
    if (exist) {
        return DATA_MODEL_ERR_EXIT;
    }
    if (idx >= 0) {
        DataModelListenerList[idx] = listener;
        return DATA_MODEL_OK;
    }
    return DATA_MODEL_ERR_FULL_LIST;
}

int DataModelRemove(DataModelListener listener) {
    int i;
    for (i = 0; i < DATA_MODEL_ID_MAX; i++) {
        if (DataModelListenerList[i] == listener) {
            DataModelListenerList[i] = NULL;
            return DATA_MODEL_OK;
        }
    }
    return DATA_MODEL_ERR_EXIT;
}

static int setValue(DATA_MODEL_ID_E id, void *data, u32 extra1, u32 extra2) {
    int               i;
    C_GENERIC_TYPE_S *c_data;

    if (id > DATA_MODEL_ID_MAX) {
        return DATA_MODEL_ERR_OUT_RANGE;
    }
    c_data         = &DataModelDataMap[id];
    c_data->data   = data;
    c_data->extra1 = extra1;
    c_data->extra2 = extra2;

    for (i = 0; i < DATA_MODEL_ID_MAX; i++) {
        if (DataModelListenerList[i] != NULL) {
            DataModelListenerList[i](id);
        }
    }
    return DATA_MODEL_OK;
}

static int getValue(DATA_MODEL_ID_E id, C_GENERIC_TYPE_S **c_data) {
    if (id > DATA_MODEL_ID_MAX) {
        return DATA_MODEL_ERR_OUT_RANGE;
    }
    *c_data = &DataModelDataMap[id];
    return DATA_MODEL_OK;
}
/*** 8 ***/
int DataModelSetU8(DATA_MODEL_ID_E id, u8 val) {
    return setValue(id, NULL, (u32)val, 0);
}

int DataModelGetU8(DATA_MODEL_ID_E id, u8 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (u8)c_data->extra1;
    return DATA_MODEL_OK;
}

int DataModelSetS8(DATA_MODEL_ID_E id, s8 val) {
    return setValue(id, NULL, (u32)val, 0);
}

int DataModelGetS8(DATA_MODEL_ID_E id, s8 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (s8)c_data->extra1;
    return DATA_MODEL_OK;
}
/*** 16 ***/
int DataModelSetU16(DATA_MODEL_ID_E id, u16 val) {
    return setValue(id, NULL, (u32)val, 0);
}

int DataModelGetU16(DATA_MODEL_ID_E id, u16 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (u16)c_data->extra1;
    return DATA_MODEL_OK;
}

int DataModelSetS16(DATA_MODEL_ID_E id, s16 val) {
    return setValue(id, NULL, (u32)val, 0);
}

int DataModelGetS16(DATA_MODEL_ID_E id, s16 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (s16)c_data->extra1;
    return DATA_MODEL_OK;
}
/*** 32 ***/
int DataModelSetU32(DATA_MODEL_ID_E id, u32 val) {
    printf("setU32 = %d\n", val);
    return setValue(id, NULL, val, 0);
}

int DataModelGetU32(DATA_MODEL_ID_E id, u32 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = c_data->extra1;
    return DATA_MODEL_OK;
}

int DataModelSetS32(DATA_MODEL_ID_E id, s32 val) {
    return setValue(id, NULL, (u32)val, 0);
}

int DataModelGetS32(DATA_MODEL_ID_E id, s32 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (s32)c_data->extra1;
    return DATA_MODEL_OK;
}
/*** 64 ***/
int DataModelSetU64(DATA_MODEL_ID_E id, u64 val) {
    return setValue(id, NULL, (u32)(val >> 32), (u32)val);
}

int DataModelGetU64(DATA_MODEL_ID_E id, u64 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (u64)(0xFFFFFFFFULL & (c_data->extra1 << 32) | c_data->extra2);
    return DATA_MODEL_OK;
}

int DataModelSetS64(DATA_MODEL_ID_E id, s64 val) {
    return setValue(id, NULL, (u32)(val >> 32), (u32)val);
}

int DataModelGetS64(DATA_MODEL_ID_E id, s64 *val) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *val = (s64)(0xFFFFFFFFULL & (c_data->extra1 << 32) | c_data->extra2);
    return DATA_MODEL_OK;
}
/*** Array ***/
int DataModelSetArray(DATA_MODEL_ID_E id, void *arr, u32 len) {
    return setValue(id, arr, len, 0);
}

int DataModelGetArray(DATA_MODEL_ID_E id, void **arr, u32 *len) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *arr = (void *)c_data->data;
    *len = (u32)c_data->extra1;
    return DATA_MODEL_OK;
}
/*** string ***/
int DataModelSetString(DATA_MODEL_ID_E id, void *str) {
    return setValue(id, str, 0, 0);
}

int DataModelGetString(DATA_MODEL_ID_E id, void **str) {
    C_GENERIC_TYPE_S *c_data;
    getValue(id, &c_data);
    *str = (void *)c_data->data;
    return DATA_MODEL_OK;
}

/*** test function ***/
void TestCb(DATA_MODEL_ID_E id) {
    int i;
    u8  vu8;
    u8 *arr;
    u32 len;
    u8 *str;

    if (id == DATA_MODEL_ID_0) {
        DataModelGetU8(DATA_MODEL_ID_0, &vu8);
        printf("vu8 = %d\n", vu8);
    }
    if (id == DATA_MODEL_ID_1) {
        DataModelGetArray(DATA_MODEL_ID_1, (void *)&arr, &len);
        for (i = 0; i < len; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    if (id == DATA_MODEL_ID_2) {
        DataModelGetString(DATA_MODEL_ID_2, (void *)&str);
        printf("%s\n", str);
    }
}

int main() {
    u8  vu8       = ~0;
    u8  array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    u8 *str       = "hello world!";

    DataModelInit();
    DataModelRegister(TestCb);

    DataModelSetU8(DATA_MODEL_ID_0, vu8);
    DataModelSetArray(DATA_MODEL_ID_1, (void *)array, sizeof(array));
    DataModelSetString(DATA_MODEL_ID_2, (void *)str);
    return 0;
}
