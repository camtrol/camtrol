// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.6
// Project name: SQL_AuToHomes_ST7789-V3

#include "ui.h"

void ui_Screen3_screen_init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Switch1 = lv_switch_create(ui_Screen3);
    lv_obj_set_width(ui_Switch1, 50);
    lv_obj_set_height(ui_Switch1, 25);
    lv_obj_set_x(ui_Switch1, 0);
    lv_obj_set_y(ui_Switch1, 90);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);


    ui_Label8 = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_Label8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label8, -76);
    lv_obj_set_y(ui_Label8, 90);
    lv_obj_set_align(ui_Label8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label8, "와이파이 ");
    lv_obj_set_style_text_font(ui_Label8, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label10 = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_Label10, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label10, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label10, 71);
    lv_obj_set_y(ui_Label10, 90);
    lv_obj_set_align(ui_Label10, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label10, "핫스팟");
    lv_obj_set_style_text_font(ui_Label10, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelNetwork = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_LabelNetwork, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelNetwork, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelNetwork, 0);
    lv_obj_set_y(ui_LabelNetwork, -102);
    lv_obj_set_align(ui_LabelNetwork, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelNetwork, "-  네트워크 선택  -");
    lv_obj_set_style_text_align(ui_LabelNetwork, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelNetwork, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label11 = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label11, -105);
    lv_obj_set_y(ui_Label11, -60);
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "SSID :");
    lv_obj_set_style_text_align(ui_Label11, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label11, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label12 = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_Label12, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label12, -100);
    lv_obj_set_y(ui_Label12, 10);
    lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label12, "PSWD :");
    lv_obj_set_style_text_align(ui_Label12, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label12, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelSSID = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_LabelSSID, 235);
    lv_obj_set_height(ui_LabelSSID, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSSID, 6);
    lv_obj_set_y(ui_LabelSSID, -30);
    lv_obj_set_align(ui_LabelSSID, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSSID, "");
    lv_obj_set_style_text_color(ui_LabelSSID, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelSSID, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelSSID, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelSSID, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LabelSSID, lv_color_hex(0x03FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LabelSSID, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LabelSSID, lv_color_hex(0x03FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_LabelSSID, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_LabelSSID, 3, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelPSWD = lv_label_create(ui_Screen3);
    lv_obj_set_width(ui_LabelPSWD, 235);
    lv_obj_set_height(ui_LabelPSWD, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelPSWD, 6);
    lv_obj_set_y(ui_LabelPSWD, 40);
    lv_obj_set_align(ui_LabelPSWD, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPSWD, "");
    lv_obj_set_style_text_color(ui_LabelPSWD, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelPSWD, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelPSWD, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPSWD, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LabelPSWD, lv_color_hex(0x03FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LabelPSWD, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LabelPSWD, lv_color_hex(0x03FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_LabelPSWD, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_LabelPSWD, 3, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Switch1, ui_event_Switch1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Screen3, ui_event_Screen3, LV_EVENT_ALL, NULL);

}
