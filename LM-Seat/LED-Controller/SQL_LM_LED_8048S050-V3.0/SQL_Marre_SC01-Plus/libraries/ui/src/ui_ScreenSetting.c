// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.6
// Project name: SQL_LM_LED_8048S050-V3.0

#include "ui.h"

void ui_ScreenSetting_screen_init(void)
{
    ui_ScreenSetting = lv_obj_create(NULL);
    lv_obj_add_flag(ui_ScreenSetting, LV_OBJ_FLAG_CHECKABLE);     /// Flags
    lv_obj_clear_flag(ui_ScreenSetting, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenSetting, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenSetting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button9 = lv_btn_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Button9, 100);
    lv_obj_set_height(ui_Button9, 60);
    lv_obj_set_x(ui_Button9, 300);
    lv_obj_set_y(ui_Button9, 77);
    lv_obj_set_align(ui_Button9, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);       /// States
    lv_obj_add_flag(ui_Button9, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label15 = lv_label_create(ui_Button9);
    lv_obj_set_width(ui_Label15, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label15, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label15, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label15, "저장");
    lv_obj_set_style_text_font(ui_Label15, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button12 = lv_btn_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Button12, 100);
    lv_obj_set_height(ui_Button12, 60);
    lv_obj_set_x(ui_Button12, 300);
    lv_obj_set_y(ui_Button12, 174);
    lv_obj_set_align(ui_Button12, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button12, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button12, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label16 = lv_label_create(ui_Button12);
    lv_obj_set_width(ui_Label16, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label16, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label16, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label16, "취소");
    lv_obj_set_style_text_font(ui_Label16, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label17 = lv_label_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Label17, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label17, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label17, -260);
    lv_obj_set_y(ui_Label17, -180);
    lv_obj_set_align(ui_Label17, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label17, "무드등 LED");
    lv_obj_set_style_text_font(ui_Label17, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SwitchMood = lv_switch_create(ui_ScreenSetting);
    lv_obj_set_width(ui_SwitchMood, 60);
    lv_obj_set_height(ui_SwitchMood, 30);
    lv_obj_set_x(ui_SwitchMood, -260);
    lv_obj_set_y(ui_SwitchMood, -130);
    lv_obj_set_align(ui_SwitchMood, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_SwitchMood, LV_STATE_CHECKED);       /// States

    ui_Label33 = lv_label_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Label33, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label33, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label33, -324);
    lv_obj_set_y(ui_Label33, -130);
    lv_obj_set_align(ui_Label33, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label33, "일반");
    lv_obj_set_style_text_font(ui_Label33, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label34 = lv_label_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Label34, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label34, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label34, -193);
    lv_obj_set_y(ui_Label34, -130);
    lv_obj_set_align(ui_Label34, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label34, "NEO");
    lv_obj_set_style_text_font(ui_Label34, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label45 = lv_label_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Label45, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label45, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label45, 40);
    lv_obj_set_y(ui_Label45, -180);
    lv_obj_set_align(ui_Label45, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label45, "사이드 LED");
    lv_obj_set_style_text_font(ui_Label45, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SwitchSide = lv_switch_create(ui_ScreenSetting);
    lv_obj_set_width(ui_SwitchSide, 60);
    lv_obj_set_height(ui_SwitchSide, 30);
    lv_obj_set_x(ui_SwitchSide, 40);
    lv_obj_set_y(ui_SwitchSide, -130);
    lv_obj_set_align(ui_SwitchSide, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_SwitchSide, LV_STATE_CHECKED);       /// States

    ui_Label46 = lv_label_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Label46, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label46, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label46, -24);
    lv_obj_set_y(ui_Label46, -130);
    lv_obj_set_align(ui_Label46, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label46, "일반");
    lv_obj_set_style_text_font(ui_Label46, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label47 = lv_label_create(ui_ScreenSetting);
    lv_obj_set_width(ui_Label47, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label47, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label47, 109);
    lv_obj_set_y(ui_Label47, -130);
    lv_obj_set_align(ui_Label47, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label47, "NEO");
    lv_obj_set_style_text_font(ui_Label47, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button9, ui_event_Button9, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button12, ui_event_Button12, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SwitchMood, ui_event_SwitchMood, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SwitchSide, ui_event_SwitchSide, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenSetting, ui_event_ScreenSetting, LV_EVENT_ALL, NULL);

}
