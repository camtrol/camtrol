// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQL-CarInd-waveShare_5.0-V3.0

#include "ui.h"

void ui_ScreenHome_screen_init(void)
{
    ui_ScreenHome = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenHome, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_ScreenHome, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ScreenHome, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container1 = lv_obj_create(ui_ScreenHome);
    lv_obj_remove_style_all(ui_Container1);
    lv_obj_set_width(ui_Container1, 800);
    lv_obj_set_height(ui_Container1, 50);
    lv_obj_set_x(ui_Container1, 0);
    lv_obj_set_y(ui_Container1, -215);
    lv_obj_set_align(ui_Container1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Container1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container1, lv_color_hex(0x3D3D3D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image1 = lv_img_create(ui_Container1);
    lv_img_set_src(ui_Image1, &ui_img_1446094990);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image1, -358);
    lv_obj_set_y(ui_Image1, -2);
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image1, 210);

    ui_Label27 = lv_label_create(ui_Container1);
    lv_obj_set_width(ui_Label27, 108);
    lv_obj_set_height(ui_Label27, 30);
    lv_obj_set_x(ui_Label27, -257);
    lv_obj_set_y(ui_Label27, 0);
    lv_obj_set_align(ui_Label27, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label27, "CARIND");
    lv_obj_set_style_text_align(ui_Label27, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label27, &ui_font_SOYOMapleBold26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelDay = lv_label_create(ui_Container1);
    lv_obj_set_width(ui_LabelDay, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelDay, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelDay, -271);
    lv_obj_set_y(ui_LabelDay, 0);
    lv_obj_set_align(ui_LabelDay, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_LabelDay, "20년 12월 12일 토요일");
    lv_obj_add_flag(ui_LabelDay, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_set_style_text_font(ui_LabelDay, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelAmPm = lv_label_create(ui_Container1);
    lv_obj_set_width(ui_LabelAmPm, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelAmPm, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelAmPm, 158);
    lv_obj_set_y(ui_LabelAmPm, 0);
    lv_obj_set_align(ui_LabelAmPm, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelAmPm, "Am");
    lv_obj_add_flag(ui_LabelAmPm, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_set_style_text_font(ui_LabelAmPm, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelTime = lv_label_create(ui_Container1);
    lv_obj_set_width(ui_LabelTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelTime, -224);
    lv_obj_set_y(ui_LabelTime, 0);
    lv_obj_set_align(ui_LabelTime, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_LabelTime, "12");
    lv_obj_set_style_text_font(ui_LabelTime, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelSec = lv_label_create(ui_Container1);
    lv_obj_set_width(ui_LabelSec, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelSec, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelSec, 185);
    lv_obj_set_y(ui_LabelSec, 2);
    lv_obj_set_align(ui_LabelSec, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSec, ":");
    lv_obj_set_style_text_font(ui_LabelSec, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelMin = lv_label_create(ui_Container1);
    lv_obj_set_width(ui_LabelMin, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelMin, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelMin, 595);
    lv_obj_set_y(ui_LabelMin, 0);
    lv_obj_set_align(ui_LabelMin, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_LabelMin, "01");
    lv_obj_set_style_text_font(ui_LabelMin, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageNetwork = lv_img_create(ui_Container1);
    lv_img_set_src(ui_ImageNetwork, &ui_img_network_png);
    lv_obj_set_width(ui_ImageNetwork, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageNetwork, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageNetwork, 300);
    lv_obj_set_y(ui_ImageNetwork, 0);
    lv_obj_set_align(ui_ImageNetwork, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageNetwork, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageNetwork, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_ImageNetwork, 250);

    ui_ImageWiFi = lv_img_create(ui_Container1);
    lv_img_set_src(ui_ImageWiFi, &ui_img_wifi_png);
    lv_obj_set_width(ui_ImageWiFi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageWiFi, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageWiFi, 366);
    lv_obj_set_y(ui_ImageWiFi, -2);
    lv_obj_set_align(ui_ImageWiFi, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_ImageWiFi, 250);

    ui_LabelPWM = lv_label_create(ui_ScreenHome);
    lv_obj_set_width(ui_LabelPWM, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelPWM, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelPWM, -360);
    lv_obj_set_y(ui_LabelPWM, -164);
    lv_obj_set_align(ui_LabelPWM, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPWM, "0");
    lv_obj_set_style_text_color(ui_LabelPWM, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelPWM, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPWM, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider1 = lv_slider_create(ui_ScreenHome);
    lv_slider_set_range(ui_Slider1, 1, 100);
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider1) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider1, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider1, 10);
    lv_obj_set_height(ui_Slider1, 300);
    lv_obj_set_x(ui_Slider1, -360);
    lv_obj_set_y(ui_Slider1, 25);
    lv_obj_set_align(ui_Slider1, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0x818181), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0xFF0202), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Slider1, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Slider1, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Slider1, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Slider1, 10, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_LabelLED = lv_label_create(ui_ScreenHome);
    lv_obj_set_width(ui_LabelLED, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelLED, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelLED, -360);
    lv_obj_set_y(ui_LabelLED, 214);
    lv_obj_set_align(ui_LabelLED, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelLED, "무드등");
    lv_obj_set_style_text_color(ui_LabelLED, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelLED, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelLED, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelLED = lv_obj_create(ui_ScreenHome);
    lv_obj_set_width(ui_PanelLED, 620);
    lv_obj_set_height(ui_PanelLED, 410);
    lv_obj_set_x(ui_PanelLED, 0);
    lv_obj_set_y(ui_PanelLED, 27);
    lv_obj_set_align(ui_PanelLED, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_PanelLED, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_PanelLED, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PanelLED, lv_color_hex(0x2F2F2F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelLED, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_PanelLED, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_PanelLED, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_PanelLED, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_PanelLED, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_PanelLED, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button1 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button1, 110);
    lv_obj_set_height(ui_Button1, 110);
    lv_obj_set_x(ui_Button1, -200);
    lv_obj_set_y(ui_Button1, -128);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Button1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Button1, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Button1, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_scroll_dir(ui_Button1, LV_DIR_LEFT);
    lv_obj_set_style_radius(ui_Button1, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image6 = lv_img_create(ui_Button1);
    lv_img_set_src(ui_Image6, &ui_img_1965946780);
    lv_obj_set_width(ui_Image6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Image6, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image6, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image6, 250);

    ui_Label1 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "무드등");
    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label1, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button2 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button2, 110);
    lv_obj_set_height(ui_Button2, 110);
    lv_obj_set_x(ui_Button2, 0);
    lv_obj_set_y(ui_Button2, -128);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Button2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Button2, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Button2, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_scroll_dir(ui_Button2, LV_DIR_LEFT);
    lv_obj_set_style_radius(ui_Button2, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button2, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image7 = lv_img_create(ui_Button2);
    lv_img_set_src(ui_Image7, &ui_img_1965946780);
    lv_obj_set_width(ui_Image7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Image7, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image7, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image7, 250);

    ui_Label2 = lv_label_create(ui_Button2);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "실내등");
    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label2, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button3 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button3, 110);
    lv_obj_set_height(ui_Button3, 110);
    lv_obj_set_x(ui_Button3, 200);
    lv_obj_set_y(ui_Button3, -128);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Button3, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Button3, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_scrollbar_mode(ui_Button3, LV_SCROLLBAR_MODE_ACTIVE);
    lv_obj_set_scroll_dir(ui_Button3, LV_DIR_LEFT);
    lv_obj_set_style_radius(ui_Button3, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button3, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button3, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image8 = lv_img_create(ui_Button3);
    lv_img_set_src(ui_Image8, &ui_img_1965946780);
    lv_obj_set_width(ui_Image8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image8, 3);
    lv_obj_set_y(ui_Image8, -12);
    lv_obj_set_align(ui_Image8, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image8, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image8, 250);

    ui_Label3 = lv_label_create(ui_Button3);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, 0);
    lv_obj_set_y(ui_Label3, 19);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "예  비");
    lv_obj_set_style_text_color(ui_Label3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label3, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button4 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button4, 110);
    lv_obj_set_height(ui_Button4, 110);
    lv_obj_set_x(ui_Button4, -200);
    lv_obj_set_y(ui_Button4, 0);
    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Button4, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Button4, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_obj_clear_flag(ui_Button4, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_style_radius(ui_Button4, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button4, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button4, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button4, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button4, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image11 = lv_img_create(ui_Button4);
    lv_img_set_src(ui_Image11, &ui_img_1543939845);
    lv_obj_set_width(ui_Image11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image11, 0);
    lv_obj_set_y(ui_Image11, -10);
    lv_obj_set_align(ui_Image11, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image11, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image11, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image11, 230);

    ui_Label4 = lv_label_create(ui_Button4);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label4, 0);
    lv_obj_set_y(ui_Label4, 21);
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "트렁크등");
    lv_obj_set_style_text_color(ui_Label4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label4, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button5 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button5, 110);
    lv_obj_set_height(ui_Button5, 110);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_style_radius(ui_Button5, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button5, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button5, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image2 = lv_img_create(ui_Button5);
    lv_img_set_src(ui_Image2, &ui_img_213141187);
    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image2, 0);
    lv_obj_set_y(ui_Image2, -18);
    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image2, 300);

    ui_Label5 = lv_label_create(ui_Button5);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label5, 0);
    lv_obj_set_y(ui_Label5, 32);
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "트렁크");
    lv_obj_set_style_text_color(ui_Label5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label5, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image10 = lv_img_create(ui_Button5);
    lv_img_set_src(ui_Image10, &ui_img_861741736);
    lv_obj_set_width(ui_Image10, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image10, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image10, 3);
    lv_obj_set_y(ui_Image10, -12);
    lv_obj_set_align(ui_Image10, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image10, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image10, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image10, 260);

    ui_Button6 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button6, 110);
    lv_obj_set_height(ui_Button6, 110);
    lv_obj_set_x(ui_Button6, 200);
    lv_obj_set_y(ui_Button6, 0);
    lv_obj_set_align(ui_Button6, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button6, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_style_radius(ui_Button6, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button6, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button6, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button6, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button6, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image3 = lv_img_create(ui_Button6);
    lv_img_set_src(ui_Image3, &ui_img_140505863);
    lv_obj_set_width(ui_Image3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image3, 0);
    lv_obj_set_y(ui_Image3, -17);
    lv_obj_set_align(ui_Image3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image3, 250);

    ui_Label6 = lv_label_create(ui_Button6);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label6, 0);
    lv_obj_set_y(ui_Label6, 34);
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, "시거잭 1");
    lv_obj_set_style_text_color(ui_Label6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label6, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button7 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button7, 110);
    lv_obj_set_height(ui_Button7, 110);
    lv_obj_set_x(ui_Button7, -200);
    lv_obj_set_y(ui_Button7, 128);
    lv_obj_set_align(ui_Button7, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button7, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_style_radius(ui_Button7, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button7, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button7, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button7, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button7, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button7, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image4 = lv_img_create(ui_Button7);
    lv_img_set_src(ui_Image4, &ui_img_1190290745);
    lv_obj_set_width(ui_Image4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image4, 0);
    lv_obj_set_y(ui_Image4, -17);
    lv_obj_set_align(ui_Image4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image4, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image4, 250);

    ui_Image12 = lv_img_create(ui_Button7);
    lv_img_set_src(ui_Image12, &ui_img_140505863);
    lv_obj_set_width(ui_Image12, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image12, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image12, 0);
    lv_obj_set_y(ui_Image12, -17);
    lv_obj_set_align(ui_Image12, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image12, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image12, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image12, 250);

    ui_Label7 = lv_label_create(ui_Button7);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label7, 0);
    lv_obj_set_y(ui_Label7, 30);
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "시거잭 2");
    lv_obj_set_style_text_color(ui_Label7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label7, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button8 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button8, 110);
    lv_obj_set_height(ui_Button8, 110);
    lv_obj_set_x(ui_Button8, 0);
    lv_obj_set_y(ui_Button8, 128);
    lv_obj_set_align(ui_Button8, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button8, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_style_radius(ui_Button8, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button8, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button8, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button8, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button8, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image5 = lv_img_create(ui_Button8);
    lv_img_set_src(ui_Image5, &ui_img_1247291337);
    lv_obj_set_width(ui_Image5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image5, 0);
    lv_obj_set_y(ui_Image5, -19);
    lv_obj_set_align(ui_Image5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image5, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image5, 230);

    ui_Label8 = lv_label_create(ui_Button8);
    lv_obj_set_width(ui_Label8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label8, 0);
    lv_obj_set_y(ui_Label8, 31);
    lv_obj_set_align(ui_Label8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label8, "상부등");
    lv_obj_set_style_text_color(ui_Label8, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label8, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button9 = lv_btn_create(ui_PanelLED);
    lv_obj_set_width(ui_Button9, 110);
    lv_obj_set_height(ui_Button9, 110);
    lv_obj_set_x(ui_Button9, 200);
    lv_obj_set_y(ui_Button9, 128);
    lv_obj_set_align(ui_Button9, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button9, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLL_CHAIN);      /// Flags
    lv_obj_set_style_radius(ui_Button9, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button9, lv_color_hex(0xFDFDFD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Button9, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button9, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(0xFA9F10), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    ui_Image9 = lv_img_create(ui_Button9);
    lv_img_set_src(ui_Image9, &ui_img_1294863193);
    lv_obj_set_width(ui_Image9, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image9, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Image9, 0);
    lv_obj_set_y(ui_Image9, -17);
    lv_obj_set_align(ui_Image9, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image9, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image9, 250);

    ui_Label9 = lv_label_create(ui_Button9);
    lv_obj_set_width(ui_Label9, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label9, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label9, 0);
    lv_obj_set_y(ui_Label9, 31);
    lv_obj_set_align(ui_Label9, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label9, "인버터");
    lv_obj_set_style_text_color(ui_Label9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label9, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider3 = lv_slider_create(ui_ScreenHome);
    lv_slider_set_range(ui_Slider3, 1, 100);
    lv_slider_set_value(ui_Slider3, 100, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_Slider3) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(ui_Slider3, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_Slider3, 10);
    lv_obj_set_height(ui_Slider3, 300);
    lv_obj_set_x(ui_Slider3, 360);
    lv_obj_set_y(ui_Slider3, 25);
    lv_obj_set_align(ui_Slider3, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_Slider3, lv_color_hex(0x818181), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_Slider3, lv_color_hex(0xFF0202), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Slider3, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Slider3, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Slider3, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_Slider3, 5, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Slider3, 10, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_LabelBright2 = lv_label_create(ui_ScreenHome);
    lv_obj_set_width(ui_LabelBright2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBright2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBright2, 360);
    lv_obj_set_y(ui_LabelBright2, 214);
    lv_obj_set_align(ui_LabelBright2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBright2, "밝기");
    lv_obj_set_style_text_color(ui_LabelBright2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBright2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBright2, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBright = lv_label_create(ui_ScreenHome);
    lv_obj_set_width(ui_LabelBright, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBright, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBright, 360);
    lv_obj_set_y(ui_LabelBright, -164);
    lv_obj_set_align(ui_LabelBright, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBright, "100");
    lv_obj_set_style_text_color(ui_LabelBright, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBright, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBright, &ui_font_SOYOMapleBold22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ContainerSleep = lv_obj_create(ui_ScreenHome);
    lv_obj_remove_style_all(ui_ContainerSleep);
    lv_obj_set_width(ui_ContainerSleep, 68);
    lv_obj_set_height(ui_ContainerSleep, 45);
    lv_obj_set_x(ui_ContainerSleep, -158);
    lv_obj_set_y(ui_ContainerSleep, -216);
    lv_obj_set_align(ui_ContainerSleep, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_ContainerSleep, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_ImageSleep = lv_img_create(ui_ContainerSleep);
    lv_img_set_src(ui_ImageSleep, &ui_img_1965945753);
    lv_obj_set_width(ui_ImageSleep, LV_SIZE_CONTENT);   /// 50
    lv_obj_set_height(ui_ImageSleep, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_align(ui_ImageSleep, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageSleep, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_ImageSleep, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_ImageSleep, 230);

    ui_Container4 = lv_obj_create(ui_ScreenHome);
    lv_obj_remove_style_all(ui_Container4);
    lv_obj_set_width(ui_Container4, 800);
    lv_obj_set_height(ui_Container4, 480);
    lv_obj_set_align(ui_Container4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Container4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Image1, ui_event_Image1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_LabelDay, ui_event_LabelDay, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImageWiFi, ui_event_ImageWiFi, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Slider1, ui_event_Slider1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button4, ui_event_Button4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button5, ui_event_Button5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button6, ui_event_Button6, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button7, ui_event_Button7, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button8, ui_event_Button8, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button9, ui_event_Button9, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Slider3, ui_event_Slider3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ContainerSleep, ui_event_ContainerSleep, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Container4, ui_event_Container4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ScreenHome, ui_event_ScreenHome, LV_EVENT_ALL, NULL);

}
