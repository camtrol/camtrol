// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.6
// Project name: SQL_Marre_ST7789-V3

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
void ui_event_Screen1(lv_event_t * e);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Panel1;
void ui_event_Image3(lv_event_t * e);
lv_obj_t * ui_Image3;
void ui_event_TextArea1(lv_event_t * e);
lv_obj_t * ui_TextArea1;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Panel2;
lv_obj_t * ui_Label7;
lv_obj_t * ui_volt;
lv_obj_t * ui_Label9;
void ui_event_Button1(lv_event_t * e);
lv_obj_t * ui_Button1;
lv_obj_t * ui_Label4;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;
lv_obj_t * ui_Label5;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
lv_obj_t * ui_Label6;
lv_obj_t * ui_ImageWiFi;
lv_obj_t * ui_ImageHotSpot;
lv_obj_t * ui_PanelIP;
lv_obj_t * ui_Label13;
lv_obj_t * ui_Label15;


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
void ui_event_Screen2(lv_event_t * e);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Image1;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Label3;
void ui_event_Roller1(lv_event_t * e);
lv_obj_t * ui_Roller1;


// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
void ui_event_Screen3(lv_event_t * e);
lv_obj_t * ui_Screen3;
void ui_event_Switch1(lv_event_t * e);
lv_obj_t * ui_Switch1;
lv_obj_t * ui_Label8;
lv_obj_t * ui_Label10;
lv_obj_t * ui_LabelNetwork;
lv_obj_t * ui_Label11;
lv_obj_t * ui_Label12;
lv_obj_t * ui_LabelSSID;
lv_obj_t * ui_LabelPSWD;


// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
void ui_event_Screen4(lv_event_t * e);
lv_obj_t * ui_Screen4;
lv_obj_t * ui_Panel3;
lv_obj_t * ui_Label14;
lv_obj_t * ui_Container1;
lv_obj_t * ui_onSwitch1;
lv_obj_t * ui_Label16;
lv_obj_t * ui_Container2;
lv_obj_t * ui_onSwitch2;
lv_obj_t * ui_Label17;
lv_obj_t * ui_Container3;
lv_obj_t * ui_onSwitch3;
lv_obj_t * ui_Label18;
lv_obj_t * ui_Container4;
lv_obj_t * ui_onSwitch4;
lv_obj_t * ui_Label19;
lv_obj_t * ui_Container5;
lv_obj_t * ui_onSwitch5;
lv_obj_t * ui_Label20;
lv_obj_t * ui_Container6;
lv_obj_t * ui_onSwitch6;
lv_obj_t * ui_Label21;
lv_obj_t * ui_Container7;
lv_obj_t * ui_onSwitch7;
lv_obj_t * ui_Label22;
lv_obj_t * ui_Container8;
lv_obj_t * ui_onSwitch8;
lv_obj_t * ui_Label23;
lv_obj_t * ui_Container12;
lv_obj_t * ui_onSwitch9;
lv_obj_t * ui_Label29;
lv_obj_t * ui_Container9;
lv_obj_t * ui_onSwitch10;
lv_obj_t * ui_Label24;
lv_obj_t * ui_Container10;
lv_obj_t * ui_onSwitch11;
lv_obj_t * ui_Label25;
void ui_event_Button4(lv_event_t * e);
lv_obj_t * ui_Button4;
lv_obj_t * ui_Label26;
void ui_event_Button5(lv_event_t * e);
lv_obj_t * ui_Button5;
lv_obj_t * ui_Label27;
lv_obj_t * ui_Container11;
lv_obj_t * ui_onSwitch12;
lv_obj_t * ui_Label28;
lv_obj_t * ui____initial_actions0;
const lv_img_dsc_t * ui_imgset_633900246[2] = {&ui_img_941217166, &ui_img_140512013};
const lv_img_dsc_t * ui_imgset_1038066740[5] = {&ui_img_273796629, &ui_img_273797654, &ui_img_273803043, &ui_img_273795620, &ui_img_354864566};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Screen1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 0, &ui_Screen3_screen_init);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 0, &ui_Screen4_screen_init);
    }
}
void ui_event_Image3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_flag_modify(ui_Panel2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}
void ui_event_TextArea1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_flag_modify(ui_Panel2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        panelVolt(e);
    }
}
void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        voltPlus(e);
    }
}
void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        voltMinus(e);
    }
}
void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        voltSave(e);
        _ui_flag_modify(ui_Panel2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}
void ui_event_Screen2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Screen1_screen_init);
    }
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        Screen2Load(e);
    }
}
void ui_event_Roller1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        opacityChange(e);
    }
}
void ui_event_Screen3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 0, &ui_Screen4_screen_init);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 0, &ui_Screen1_screen_init);
    }
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        networkScreenLoaded(e);
    }
}
void ui_event_Switch1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        networkSelectSwitch(e);
    }
}
void ui_event_Screen4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_OVER_LEFT, 500, 0, &ui_Screen1_screen_init);
    }
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 0, &ui_Screen3_screen_init);
    }
}
void ui_event_Button4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
        btnPowerOnExeSave(e);
    }
}
void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
