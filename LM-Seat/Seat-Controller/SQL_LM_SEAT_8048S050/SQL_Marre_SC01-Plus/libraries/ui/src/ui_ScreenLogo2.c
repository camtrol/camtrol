// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQL_LM_SEAT_8048S050

#include "ui.h"

void ui_ScreenLogo2_screen_init(void)
{
    ui_ScreenLogo2 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_ScreenLogo2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image10 = lv_img_create(ui_ScreenLogo2);
    lv_img_set_src(ui_Image10, &ui_img_618194214);
    lv_obj_set_width(ui_Image10, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Image10, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Image10, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image10, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image10, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Image10, 230);

    lv_obj_add_event_cb(ui_ScreenLogo2, ui_event_ScreenLogo2, LV_EVENT_ALL, NULL);

}
