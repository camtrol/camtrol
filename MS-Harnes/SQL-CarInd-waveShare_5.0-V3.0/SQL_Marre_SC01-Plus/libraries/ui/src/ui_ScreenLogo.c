// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQL-CarInd-waveShare_5.0-V3.0

#include "ui.h"

void ui_ScreenLogo_screen_init(void)
{
    ui_ScreenLogo = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenLogo, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image13 = lv_img_create(ui_ScreenLogo);
    lv_img_set_src(ui_Image13, &ui_img_54682230);
    lv_obj_set_width(ui_Image13, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image13, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Image13, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image13, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image13, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image13, 220);

    lv_obj_add_event_cb(ui_ScreenLogo, ui_event_ScreenLogo, LV_EVENT_ALL, NULL);

}
