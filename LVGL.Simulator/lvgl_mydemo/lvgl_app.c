/************************************************************************
 * FilePath     : lvgl_app.c
 * Author       : GX.Duan
 * Date         : 2022-08-07
 * LastEditTime : 2022-08-19
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/

/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "lvgl_app.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/

/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/

void DataModel(DATA_MODEL_ID_E id) {
    if (id == DATA_MODEL_ID_0) {
        uint8_t val;
        DataModelGetU8(id, &val);
        printf("val = %d\n", val);
    }
}

void lv_btn1_event_halder(lv_event_t *e) {
    static uint8_t  val  = 0;
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
        DataModelSetU8(DATA_MODEL_ID_0, ++val);
    } else if (code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }
}

void lvgl_app_init(void) {
    DataModelInit();
    DataModelRegister(DataModel);

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn1, 100, 50);
    lv_obj_set_align(btn1, LV_ALIGN_CENTER);

    lv_obj_t *label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Buttom");
    lv_obj_set_align(label1, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(btn1, lv_btn1_event_halder, LV_EVENT_ALL, NULL);
}
