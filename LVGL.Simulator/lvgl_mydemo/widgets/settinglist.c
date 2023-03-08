

#include "widgets.h"

#define SETTING_ITEM_NUM   3
#define SETTING_OBJ_WIDTH  lv_pct(100)
#define SETTING_ITEM_WIDTH (SETTING_OBJ_WIDTH / SETTING_ITEM_NUM)

lv_obj_t *bt_set;
lv_obj_t *wifi_set;
lv_obj_t *shutdown;
lv_obj_t *bt_sw;
lv_obj_t *wifi_sw;

lv_obj_t *shutdown_win;
lv_obj_t *ui_Button1;
lv_obj_t *ui_Switch1;
lv_obj_t *ui_Label1;
// fun

void shutdown_delay_cb(lv_timer_t *t)
{
    lv_anim_del(ui_Switch1, NULL);
    lv_obj_del(shutdown_win);
    lvgl_app_shutdown();          // TODO调用关机函数
    lv_timer_del(t);
}

void shutdown_event_cb(lv_event_t *e)
{
    lv_obj_t *tgt = lv_event_get_target(e);
    if (tgt == ui_Button1) {
        // cancel
        lv_obj_del(shutdown_win);
    } else if (tgt == ui_Switch1) {
        lv_obj_del(ui_Label1);
        lv_timer_create(shutdown_delay_cb, 500, NULL);
    } else {
    }
}

void SettingShutdownWin(void)
{
    shutdown_win = lv_obj_create(lv_scr_act());
    lv_obj_set_size(shutdown_win, lv_pct(100), lv_pct(100));
    lv_obj_clear_flag(shutdown_win, LV_OBJ_FLAG_SCROLLABLE);          /// Flags
    lv_obj_set_style_bg_color(shutdown_win, lv_color_hex(0x060404), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(shutdown_win, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(shutdown_win, 0, LV_PART_MAIN);

    ui_Switch1 = lv_switch_create(shutdown_win);
    lv_obj_set_width(ui_Switch1, lv_pct(76));
    lv_obj_set_height(ui_Switch1, lv_pct(20));
    lv_obj_set_x(ui_Switch1, 0);
    lv_obj_set_y(ui_Switch1, -19);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0xFF5858), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0x070707), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(ui_Switch1, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Switch1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_Switch1, &ui_img_power_off_png, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_Label1 = lv_label_create(ui_Switch1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);           /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);          /// 1
    lv_obj_set_x(ui_Label1, 24);
    lv_obj_set_y(ui_Label1, -1);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "shutdown");
    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button1 = lv_btn_create(shutdown_win);
    lv_obj_set_width(ui_Button1, lv_pct(49));
    lv_obj_set_height(ui_Button1, lv_pct(15));
    lv_obj_set_x(ui_Button1, 2);
    lv_obj_set_y(ui_Button1, 97);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);          /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);             /// Flags
    lv_obj_set_style_radius(ui_Button1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_Label2 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);           /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);          /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "cancel");
    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0x060606), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Switch1, shutdown_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Button1, shutdown_event_cb, LV_EVENT_CLICKED, NULL);
}

void setting_list_event_cb(lv_event_t *e)
{
    lv_obj_t *tgt   = lv_event_get_target(e);
    bool      sw_st = false;

    if (tgt == bt_sw) {
        sw_st = lv_obj_has_state(bt_sw, LV_STATE_CHECKED);
        DataModelSetU8(DATA_BT_SWITCH_ST, sw_st);
    } else if (tgt == wifi_sw) {
        sw_st = lv_obj_has_state(wifi_sw, LV_STATE_CHECKED);
        DataModelSetU8(DATA_WIFI_SWITCH_ST, sw_st);
    } else if (tgt == shutdown) {
        SettingShutdownWin();
    } else {
    }
}

lv_obj_t *SettingListCreate(lv_obj_t *parent)
{
    uint8_t           val;
    static lv_point_t top_line[] = {
        {                    10, 0},
        {DEF_LCD_WIDTH_MAX - 10, 0},
    };

    static lv_point_t bot_line[] = {
        {                    10, 56},
        {DEF_LCD_WIDTH_MAX - 10, 56},
    };

    lv_color_t bg_color = lv_obj_get_style_bg_color(parent, LV_PART_MAIN);

    lv_obj_t  *obj      = lv_obj_create(parent);
    lv_obj_set_size(obj, lv_pct(100), lv_pct(60));
    lv_obj_set_style_bg_color(obj, bg_color, LV_PART_MAIN);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN);

    bt_set = lv_obj_create(obj);
    lv_obj_set_size(bt_set, lv_pct(100), lv_pct(30));
    lv_obj_set_style_bg_color(bt_set, bg_color, LV_PART_MAIN);
    lv_obj_set_style_pad_all(bt_set, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(bt_set, 0, LV_PART_MAIN);

    lv_obj_t *line = lv_line_create(bt_set);
    lv_obj_set_style_line_color(line, lv_color_make(48, 48, 48), LV_PART_MAIN);
    lv_line_set_points(line, top_line, 2);

    line = lv_line_create(bt_set);
    lv_obj_set_style_line_color(line, lv_color_make(48, 48, 48), LV_PART_MAIN);
    lv_line_set_points(line, bot_line, 2);

    wifi_set = lv_obj_create(obj);
    lv_obj_set_size(wifi_set, lv_pct(100), lv_pct(30));
    lv_obj_set_style_bg_color(wifi_set, bg_color, LV_PART_MAIN);
    lv_obj_set_style_pad_all(wifi_set, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(wifi_set, 0, LV_PART_MAIN);

    line = lv_line_create(wifi_set);
    lv_obj_set_style_line_color(line, lv_color_make(48, 48, 48), LV_PART_MAIN);
    lv_line_set_points(line, bot_line, 2);

    shutdown = lv_obj_create(obj);
    lv_obj_set_size(shutdown, lv_pct(100), lv_pct(30));
    lv_obj_set_style_bg_color(shutdown, bg_color, LV_PART_MAIN);
    lv_obj_set_style_pad_all(shutdown, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(shutdown, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(shutdown, lv_color_make(48, 48, 48), LV_STATE_PRESSED);
    lv_obj_add_flag(shutdown, LV_OBJ_FLAG_CLICKABLE);

    line = lv_line_create(shutdown);
    lv_obj_set_style_line_color(line, lv_color_make(48, 48, 48), LV_PART_MAIN);
    lv_line_set_points(line, bot_line, 2);

    bt_sw = lv_switch_create(bt_set);
    lv_obj_set_width(bt_sw, 50);
    lv_obj_set_height(bt_sw, 25);
    lv_obj_set_x(bt_sw, 85);
    lv_obj_set_y(bt_sw, 126);
    lv_obj_set_style_bg_color(bt_sw, lv_color_hex(0x499B5F), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(bt_sw, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(bt_sw, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(bt_sw, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    DataModelGetU8(DATA_USB_INSERT_ST, &val);
    if (val) {
        lv_obj_add_state(bt_sw, LV_STATE_CHECKED);          /// States
    }

    wifi_sw = lv_switch_create(wifi_set);
    lv_obj_set_width(wifi_sw, 50);
    lv_obj_set_height(wifi_sw, 25);
    lv_obj_set_x(wifi_sw, 85);
    lv_obj_set_y(wifi_sw, 126);
    lv_obj_set_style_bg_color(wifi_sw, lv_color_hex(0x499B5F), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(wifi_sw, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(wifi_sw, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wifi_sw, 255, LV_PART_KNOB | LV_STATE_DEFAULT);

    DataModelGetU8(DATA_USB_INSERT_ST, &val);
    if (val) {
        lv_obj_add_state(wifi_sw, LV_STATE_CHECKED);          /// States
    }

    lv_obj_t *label = lv_label_create(bt_set);
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);
    lv_label_set_text(label, "buletooth");

    label = lv_label_create(wifi_set);
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);
    lv_label_set_text(label, "Wi-Fi");

    label = lv_label_create(shutdown);
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);
    lv_label_set_text(label, "shoudown");

    lv_obj_align_to(bt_sw, bt_set, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_align_to(wifi_sw, wifi_set, LV_ALIGN_RIGHT_MID, -10, 0);
    // item align
    lv_obj_align_to(bt_set, obj, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align_to(wifi_set, bt_set, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_align_to(shutdown, wifi_set, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_add_event_cb(bt_sw, setting_list_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(wifi_sw, setting_list_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(shutdown, setting_list_event_cb, LV_EVENT_CLICKED, NULL);

    return obj;
}
