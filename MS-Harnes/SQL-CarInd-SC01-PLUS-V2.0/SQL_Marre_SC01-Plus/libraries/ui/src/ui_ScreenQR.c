// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQL-CarInd-SC01-PLUS-V2.0

#include "ui.h"

void ui_ScreenQR_screen_init(void)
{
    ui_ScreenQR = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenQR, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label23 = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_Label23, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label23, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label23, 260);
    lv_obj_set_y(ui_Label23, -17);
    lv_obj_set_align(ui_Label23, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_Label23, "와이파이 네트워크:");
    lv_obj_set_style_text_font(ui_Label23, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelSSID = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_LabelSSID, 220);
    lv_obj_set_height(ui_LabelSSID, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSSID, 120);
    lv_obj_set_y(ui_LabelSSID, 13);
    lv_obj_set_align(ui_LabelSSID, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSSID, "카인드 V2.0-SD-034");
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_set_style_text_align(ui_LabelSSID, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelSSID, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LabelSSID, lv_color_hex(0x00B53B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LabelSSID, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LabelSSID, lv_color_hex(0x00B53B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_LabelSSID, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_LabelSSID, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelPSWD = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_LabelPSWD, 220);
    lv_obj_set_height(ui_LabelPSWD, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelPSWD, 120);
    lv_obj_set_y(ui_LabelPSWD, 85);
    lv_obj_set_align(ui_LabelPSWD, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPSWD, "18250131034");
    lv_obj_set_style_text_align(ui_LabelPSWD, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPSWD, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_LabelPSWD, lv_color_hex(0x00B53B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_LabelPSWD, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_LabelPSWD, lv_color_hex(0x00B53B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_LabelPSWD, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_LabelPSWD, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label24 = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_Label24, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label24, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label24, 260);
    lv_obj_set_y(ui_Label24, 55);
    lv_obj_set_align(ui_Label24, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_Label24, "와이파이 비밀번호:");
    lv_obj_set_style_text_font(ui_Label24, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageQR = lv_img_create(ui_ScreenQR);
    lv_img_set_src(ui_ImageQR, &ui_img_1776640024);
    lv_obj_set_width(ui_ImageQR, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageQR, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageQR, -111);
    lv_obj_set_y(ui_ImageQR, 1);
    lv_obj_set_align(ui_ImageQR, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags

    ui_Slider2 = lv_slider_create(ui_ScreenQR);
    lv_slider_set_range(ui_Slider2, 10, 255);
    lv_slider_set_value(ui_Slider2, 127, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider2) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider2, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider2, 215);
    lv_obj_set_height(ui_Slider2, 10);
    lv_obj_set_x(ui_Slider2, 121);
    lv_obj_set_y(ui_Slider2, 120);
    lv_obj_set_align(ui_Slider2, LV_ALIGN_CENTER);


    ui_Container2 = lv_obj_create(ui_ScreenQR);
    lv_obj_remove_style_all(ui_Container2);
    lv_obj_set_width(ui_Container2, 240);
    lv_obj_set_height(ui_Container2, 310);
    lv_obj_set_x(ui_Container2, -116);
    lv_obj_set_y(ui_Container2, -2);
    lv_obj_set_align(ui_Container2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container2, LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_Label37 = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_Label37, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label37, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label37, 412);
    lv_obj_set_y(ui_Label37, -110);
    lv_obj_set_align(ui_Label37, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_Label37, "마스타");
    lv_obj_set_style_text_align(ui_Label37, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label37, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label39 = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_Label39, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label39, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label39, -152);
    lv_obj_set_y(ui_Label39, -110);
    lv_obj_set_align(ui_Label39, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_Label39, "슬레이브");
    lv_obj_set_style_text_font(ui_Label39, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch1 = lv_switch_create(ui_ScreenQR);
    lv_obj_set_width(ui_Switch1, 50);
    lv_obj_set_height(ui_Switch1, 25);
    lv_obj_set_x(ui_Switch1, 127);
    lv_obj_set_y(ui_Switch1, -110);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);
    lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);       /// States


    ui_LabelBtnInit = lv_label_create(ui_ScreenQR);
    lv_obj_set_width(ui_LabelBtnInit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBtnInit, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBtnInit, 128);
    lv_obj_set_y(ui_LabelBtnInit, -59);
    lv_obj_set_align(ui_LabelBtnInit, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnInit, "버튼 명칭이  초기화 되었습니다.");
    lv_obj_add_flag(ui_LabelBtnInit, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_set_style_text_color(ui_LabelBtnInit, lv_color_hex(0xF2FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBtnInit, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBtnInit, &ui_font_SOYOMapleBold14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_LabelSSID, ui_event_LabelSSID, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Slider2, ui_event_Slider2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Container2, ui_event_Container2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Switch1, ui_event_Switch1, LV_EVENT_ALL, NULL);

}
