// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.6
// Project name: esp32_ST7789V3

#include "ui.h"

void ui_Screen5_screen_init(void)
{
    ui_Screen5 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label23 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_Label23, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label23, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label23, 0);
    lv_obj_set_y(ui_Label23, -72);
    lv_obj_set_align(ui_Label23, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label23, "- 리프트 전류값 설정 -");
    lv_obj_set_style_text_font(ui_Label23, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel5 = lv_obj_create(ui_Screen5);
    lv_obj_set_width(ui_Panel5, 141);
    lv_obj_set_height(ui_Panel5, 35);
    lv_obj_set_x(ui_Panel5, -79);
    lv_obj_set_y(ui_Panel5, -33);
    lv_obj_set_align(ui_Panel5, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_outline_color(ui_Panel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Panel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Panel5, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel5, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label21 = lv_label_create(ui_Panel5);
    lv_obj_set_width(ui_Label21, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label21, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label21, -23);
    lv_obj_set_y(ui_Label21, 0);
    lv_obj_set_align(ui_Label21, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label21, "리프트 1 :");
    lv_obj_set_style_text_font(ui_Label21, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_refValue1 = lv_label_create(ui_Panel5);
    lv_obj_set_width(ui_refValue1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_refValue1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_refValue1, -2);
    lv_obj_set_y(ui_refValue1, 0);
    lv_obj_set_align(ui_refValue1, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_refValue1, "0.0");
    lv_obj_set_style_text_color(ui_refValue1, lv_color_hex(0xE8FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_refValue1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_refValue1, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_refValue1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_refValue1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel6 = lv_obj_create(ui_Screen5);
    lv_obj_set_width(ui_Panel6, 141);
    lv_obj_set_height(ui_Panel6, 35);
    lv_obj_set_x(ui_Panel6, -79);
    lv_obj_set_y(ui_Panel6, 13);
    lv_obj_set_align(ui_Panel6, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_outline_color(ui_Panel6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Panel6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label24 = lv_label_create(ui_Panel6);
    lv_obj_set_width(ui_Label24, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label24, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label24, -23);
    lv_obj_set_y(ui_Label24, 0);
    lv_obj_set_align(ui_Label24, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label24, "리프트 2 :");
    lv_obj_set_style_text_font(ui_Label24, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_refValue2 = lv_label_create(ui_Panel6);
    lv_obj_set_width(ui_refValue2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_refValue2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_refValue2, -2);
    lv_obj_set_y(ui_refValue2, 0);
    lv_obj_set_align(ui_refValue2, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_refValue2, "0.0");
    lv_obj_set_style_text_color(ui_refValue2, lv_color_hex(0xE8FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_refValue2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_refValue2, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_refValue2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_refValue2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel7 = lv_obj_create(ui_Screen5);
    lv_obj_set_width(ui_Panel7, 141);
    lv_obj_set_height(ui_Panel7, 35);
    lv_obj_set_x(ui_Panel7, 78);
    lv_obj_set_y(ui_Panel7, -33);
    lv_obj_set_align(ui_Panel7, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_outline_color(ui_Panel7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Panel7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label26 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label26, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label26, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label26, -23);
    lv_obj_set_y(ui_Label26, 0);
    lv_obj_set_align(ui_Label26, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label26, "리프트 3 :");
    lv_obj_set_style_text_font(ui_Label26, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_refValue3 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_refValue3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_refValue3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_refValue3, -2);
    lv_obj_set_y(ui_refValue3, 0);
    lv_obj_set_align(ui_refValue3, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_refValue3, "0.0");
    lv_obj_set_style_text_color(ui_refValue3, lv_color_hex(0xE8FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_refValue3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_refValue3, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_refValue3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_refValue3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel8 = lv_obj_create(ui_Screen5);
    lv_obj_set_width(ui_Panel8, 141);
    lv_obj_set_height(ui_Panel8, 35);
    lv_obj_set_x(ui_Panel8, 78);
    lv_obj_set_y(ui_Panel8, 13);
    lv_obj_set_align(ui_Panel8, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_outline_color(ui_Panel8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Panel8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label28 = lv_label_create(ui_Panel8);
    lv_obj_set_width(ui_Label28, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label28, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label28, -23);
    lv_obj_set_y(ui_Label28, 0);
    lv_obj_set_align(ui_Label28, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label28, "리프트 4 :");
    lv_obj_set_style_text_font(ui_Label28, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_refValue4 = lv_label_create(ui_Panel8);
    lv_obj_set_width(ui_refValue4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_refValue4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_refValue4, -2);
    lv_obj_set_y(ui_refValue4, 0);
    lv_obj_set_align(ui_refValue4, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_refValue4, "0.0");
    lv_obj_set_style_text_color(ui_refValue4, lv_color_hex(0xE8FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_refValue4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_refValue4, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_refValue4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_refValue4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button3 = lv_btn_create(ui_Screen5);
    lv_obj_set_width(ui_Button3, 65);
    lv_obj_set_height(ui_Button3, 35);
    lv_obj_set_x(ui_Button3, 28);
    lv_obj_set_y(ui_Button3, 60);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_outline_color(ui_Button3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label30 = lv_label_create(ui_Button3);
    lv_obj_set_width(ui_Label30, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label30, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label30, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label30, "확인");
    lv_obj_set_style_text_font(ui_Label30, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button4 = lv_btn_create(ui_Screen5);
    lv_obj_set_width(ui_Button4, 65);
    lv_obj_set_height(ui_Button4, 35);
    lv_obj_set_x(ui_Button4, 114);
    lv_obj_set_y(ui_Button4, 60);
    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button4, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_outline_color(ui_Button4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label31 = lv_label_create(ui_Button4);
    lv_obj_set_width(ui_Label31, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label31, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label31, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label31, "취소");
    lv_obj_set_style_text_font(ui_Label31, &ui_font_SOYOMapleBold18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label32 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_Label32, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label32, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label32, -85);
    lv_obj_set_y(ui_Label32, 60);
    lv_obj_set_align(ui_Label32, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label32, "사용자가 임의로 변경 \n하지 말아 주십시요.");
    lv_obj_set_style_text_color(ui_Label32, lv_color_hex(0xFFFC00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label32, &ui_font_SOYOMapleBold14, LV_PART_MAIN | LV_STATE_DEFAULT);

}
