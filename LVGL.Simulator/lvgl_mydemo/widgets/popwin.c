

#include "widgets.h"

lv_obj_t *ui_Screen1;
lv_obj_t *ui_Button2;
lv_obj_t *ui_Label1;
lv_obj_t *ui_Label2;
lv_obj_t *ui_Button1;
lv_obj_t *ui_Label3;

typedef void (*popwin_cb)(void);

popwin_cb ui_popwin_cb = NULL;
//
void ConfirPopWin1(lv_obj_t *parent, const char *filename);

void event_cb1(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    if (obj == ui_Button2) {
        // TODO 确认删除文件
        printf("confir del\n");
        lv_obj_del(ui_Screen1);
    } else if (obj == ui_Button1) {
        lv_obj_del(ui_Screen1);
    }
}

void event_cb(lv_event_t *e)
{
    lv_obj_t *obj       = lv_event_get_target(e);
    lv_obj_t *user_data = lv_event_get_user_data(e);

    if (obj == ui_Button2) {
        // TODO 调用运行脚本文件函数
        printf("runing\n");
        lv_obj_del(ui_Screen1);
    } else if (obj == ui_Button1) {
        lv_obj_del(ui_Screen1);
        ConfirPopWin1(NULL, (const char *)user_data);
    }
}

void ConfirPopWinCreateUi(const char *filename)
{
    ui_Screen1 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui_Screen1, lv_pct(100), lv_pct(100));
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);          /// Flags
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x060404), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Screen1, 0, LV_PART_MAIN);

    ui_Button2 = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_Button2, lv_pct(50));
    lv_obj_set_height(ui_Button2, lv_pct(14));
    lv_obj_set_x(ui_Button2, lv_pct(-1));
    lv_obj_set_y(ui_Button2, lv_pct(22));
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);          /// Flags
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);             /// Flags
    lv_obj_set_style_radius(ui_Button2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label1 = lv_label_create(ui_Button2);
    lv_obj_set_width(ui_Label1, lv_pct(110));
    lv_obj_set_height(ui_Label1, lv_pct(58));
    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, 1);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "run\n");
    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label2, lv_pct(91));
    lv_obj_set_height(ui_Label2, lv_pct(15));
    lv_obj_set_x(ui_Label2, lv_pct(-1));
    lv_obj_set_y(ui_Label2, lv_pct(6));
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, filename);
    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_decor(ui_Label2, LV_TEXT_DECOR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Label2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button1 = lv_btn_create(ui_Screen1);
    lv_obj_set_width(ui_Button1, lv_pct(50));
    lv_obj_set_height(ui_Button1, lv_pct(14));
    lv_obj_set_x(ui_Button1, lv_pct(-1));
    lv_obj_set_y(ui_Button1, lv_pct(38));
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);          /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);             /// Flags
    lv_obj_set_style_radius(ui_Button1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label3 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label3, lv_pct(110));
    lv_obj_set_height(ui_Label3, lv_pct(58));
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "del");
    lv_obj_set_style_text_color(ui_Label3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ConfirPopWin1(lv_obj_t *parent, const char *filename)
{
    ConfirPopWinCreateUi(filename);
    lv_label_set_text(ui_Label1, "comfir\n");
    lv_label_set_text(ui_Label3, "cancel");
    lv_obj_add_event_cb(ui_Button2, event_cb1, LV_EVENT_CLICKED, (void *)filename);
    lv_obj_add_event_cb(ui_Button1, event_cb1, LV_EVENT_CLICKED, (void *)filename);
}

void ConfirPopWin(lv_obj_t *parent, const char *filename)
{
    ConfirPopWinCreateUi(filename);
    lv_obj_add_event_cb(ui_Button2, event_cb, LV_EVENT_CLICKED, (void *)filename);
    lv_obj_add_event_cb(ui_Button1, event_cb, LV_EVENT_CLICKED, (void *)filename);
}
