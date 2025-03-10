// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SQL-CarInd-waveShare_5.0-V3.0

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
void sec_Animation(lv_obj_t * TargetObject, int delay);
void secDot_Animation(lv_obj_t * TargetObject, int delay);
void PanelButton_Animation(lv_obj_t * TargetObject, int delay);
void PanelDisplay_Animation(lv_obj_t * TargetObject, int delay);


// SCREEN: ui_ScreenHome
void ui_ScreenHome_screen_init(void);
void ui_event_ScreenHome(lv_event_t * e);
lv_obj_t * ui_ScreenHome;
lv_obj_t * ui_Container1;
void ui_event_Image1(lv_event_t * e);
lv_obj_t * ui_Image1;
lv_obj_t * ui_Label27;
void ui_event_LabelDay(lv_event_t * e);
lv_obj_t * ui_LabelDay;
lv_obj_t * ui_LabelAmPm;
lv_obj_t * ui_LabelTime;
lv_obj_t * ui_LabelSec;
lv_obj_t * ui_LabelMin;
lv_obj_t * ui_ImageNetwork;
void ui_event_ImageWiFi(lv_event_t * e);
lv_obj_t * ui_ImageWiFi;
lv_obj_t * ui_LabelPWM;
void ui_event_Slider1(lv_event_t * e);
lv_obj_t * ui_Slider1;
lv_obj_t * ui_LabelLED;
lv_obj_t * ui_PanelLED;
void ui_event_Button1(lv_event_t * e);
lv_obj_t * ui_Button1;
lv_obj_t * ui_Image6;
lv_obj_t * ui_Label1;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;
lv_obj_t * ui_Image7;
lv_obj_t * ui_Label2;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
lv_obj_t * ui_Image8;
lv_obj_t * ui_Label3;
void ui_event_Button4(lv_event_t * e);
lv_obj_t * ui_Button4;
lv_obj_t * ui_Image11;
lv_obj_t * ui_Label4;
void ui_event_Button5(lv_event_t * e);
lv_obj_t * ui_Button5;
lv_obj_t * ui_Image2;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Image10;
void ui_event_Button6(lv_event_t * e);
lv_obj_t * ui_Button6;
lv_obj_t * ui_Image3;
lv_obj_t * ui_Label6;
void ui_event_Button7(lv_event_t * e);
lv_obj_t * ui_Button7;
lv_obj_t * ui_Image4;
lv_obj_t * ui_Image12;
lv_obj_t * ui_Label7;
void ui_event_Button8(lv_event_t * e);
lv_obj_t * ui_Button8;
lv_obj_t * ui_Image5;
lv_obj_t * ui_Label8;
void ui_event_Button9(lv_event_t * e);
lv_obj_t * ui_Button9;
lv_obj_t * ui_Image9;
lv_obj_t * ui_Label9;
void ui_event_Slider3(lv_event_t * e);
lv_obj_t * ui_Slider3;
lv_obj_t * ui_LabelBright2;
lv_obj_t * ui_LabelBright;
void ui_event_ContainerSleep(lv_event_t * e);
lv_obj_t * ui_ContainerSleep;
lv_obj_t * ui_ImageSleep;
void ui_event_Container4(lv_event_t * e);
lv_obj_t * ui_Container4;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenQR
void ui_ScreenQR_screen_init(void);
lv_obj_t * ui_ScreenQR;
lv_obj_t * ui_Label23;
void ui_event_LabelSSID(lv_event_t * e);
lv_obj_t * ui_LabelSSID;
lv_obj_t * ui_LabelPSWD;
lv_obj_t * ui_Label24;
void ui_event_Slider2(lv_event_t * e);
lv_obj_t * ui_Slider2;
lv_obj_t * ui_ImageQR;
void ui_event_Container2(lv_event_t * e);
lv_obj_t * ui_Container2;
lv_obj_t * ui_Label37;
lv_obj_t * ui_Label39;
void ui_event_Switch1(lv_event_t * e);
lv_obj_t * ui_Switch1;
lv_obj_t * ui_LabelBtnInit;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenLogo
void ui_ScreenLogo_screen_init(void);
void ui_event_ScreenLogo(lv_event_t * e);
lv_obj_t * ui_ScreenLogo;
lv_obj_t * ui_Image13;
// CUSTOM VARIABLES


// SCREEN: ui_ScreenClock
void ui_ScreenClock_screen_init(void);
lv_obj_t * ui_ScreenClock;
lv_obj_t * ui_Image14;
void ui_event_RollerYear(lv_event_t * e);
lv_obj_t * ui_RollerYear;
lv_obj_t * ui_Label10;
void ui_event_RollerMonth(lv_event_t * e);
lv_obj_t * ui_RollerMonth;
lv_obj_t * ui_Label13;
void ui_event_RollerDay(lv_event_t * e);
lv_obj_t * ui_RollerDay;
lv_obj_t * ui_Label14;
void ui_event_RollerWeek(lv_event_t * e);
lv_obj_t * ui_RollerWeek;
void ui_event_RollerHour(lv_event_t * e);
lv_obj_t * ui_RollerHour;
lv_obj_t * ui_Label15;
void ui_event_RollerMin(lv_event_t * e);
lv_obj_t * ui_RollerMin;
lv_obj_t * ui_Label16;
void ui_event_Button10(lv_event_t * e);
lv_obj_t * ui_Button10;
lv_obj_t * ui_Label11;
void ui_event_Button11(lv_event_t * e);
lv_obj_t * ui_Button11;
lv_obj_t * ui_Label12;
lv_obj_t * ui_Label17;
lv_obj_t * ui_LabelNowTime;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
const lv_img_dsc_t * ui_imgset_633900246[10] = {&ui_img_1965946780, &ui_img_1247291337, &ui_img_941216143, &ui_img_1294863193, &ui_img_1543939845, &ui_img_2022945322, &ui_img_1965945753, &ui_img_1190290745, &ui_img_140505863, &ui_img_213141187};
const lv_img_dsc_t * ui_imgset_1174140356[1] = {&ui_img_1446094990};
const lv_img_dsc_t * ui_imgset_1038066740[30] = {&ui_img_322907723, &ui_img_322913096, &ui_img_322914121, &ui_img_322919494, &ui_img_322912071, &ui_img_322917444, &ui_img_322918469, &ui_img_322923858, &ui_img_322916435, &ui_img_1889664027, &ui_img_1889671450, &ui_img_1889666077, &ui_img_1889665052, &ui_img_1889659679, &ui_img_1889667102, &ui_img_1889661729, &ui_img_1889660704, &ui_img_1889655315, &ui_img_1889662738, &ui_img_1808593792, &ui_img_1808601215, &ui_img_1808595842, &ui_img_1808594817, &ui_img_1808589444, &ui_img_1808596867, &ui_img_1808591494, &ui_img_1808590469, &ui_img_1808585080, &ui_img_1808592503, &ui_img_209895491};
const lv_img_dsc_t * ui_imgset_636852105[1] = {&ui_img_638533140};
const lv_img_dsc_t * ui_imgset_1171105555[1] = {&ui_img_861741736};
const lv_img_dsc_t * ui_imgset_1924419634[1] = {&ui_img_54682230};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void sec_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, true);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}
void secDot_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 35000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_angle);
    lv_anim_set_values(&PropertyAnimation_0, 600, 3600);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_angle);
    lv_anim_start(&PropertyAnimation_0);

}
void PanelButton_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);
    lv_anim_set_values(&PropertyAnimation_0, 0, 64);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}
void PanelDisplay_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);
    lv_anim_set_values(&PropertyAnimation_0, -140, -198);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////
void ui_event_ScreenHome(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SCREEN_LOADED) {
        ScreenHomeLoaded(e);
    }
    if(event_code == LV_EVENT_SCREEN_LOAD_START) {
        sec_Animation(ui_LabelSec, 0);
    }
}

void ui_event_Image1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        logoDisplay(e);
    }
}

void ui_event_LabelDay(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_screen_change(&ui_ScreenClock, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenClock_screen_init);
        timeClick(e);
    }
}

void ui_event_ImageWiFi(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_screen_change(&ui_ScreenQR, LV_SCR_LOAD_ANIM_NONE, 500, 0, &ui_ScreenQR_screen_init);
        wifiClick(e);
    }
}

void ui_event_Slider1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_slider_set_text_value(ui_LabelPWM, target, "", "");
        pwmChangeSlider(e);
    }
}

void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        led1Long(e);
    }
    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button1, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        ledButton1(e);
    }
}

void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        led2Long(e);
    }
    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button2, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        ledButton2(e);
    }
}

void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        led3Long(e);
    }
    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button3, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        ledButton3(e);
    }
}

void ui_event_Button4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button4, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        ledButton4(e);
    }
    if(event_code == LV_EVENT_LONG_PRESSED) {
        led4Long(e);
    }
}

void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button5, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        pumpButton(e);
    }
}

void ui_event_Button6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button6, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        cigarButton(e);
    }
}

void ui_event_Button7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button7, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        frigeButton(e);
    }
}

void ui_event_Button8(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button8, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        tvButton(e);
    }
}

void ui_event_Button9(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_SHORT_CLICKED) {
        _ui_state_modify(ui_Button9, LV_STATE_CHECKED, _UI_MODIFY_STATE_TOGGLE);
        invertorButton(e);
    }
}

void ui_event_Slider3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_slider_set_text_value(ui_LabelBright, target, "", "");
        brightChangeSlider(e);
    }
}

void ui_event_ContainerSleep(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_LONG_PRESSED) {
        _ui_flag_modify(ui_Container4, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        sleepButton(e);
    }
}

void ui_event_Container4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        sleepOff(e);
    }
}

void ui_event_LabelSSID(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        nameInitClick(e);
    }
}

void ui_event_Slider2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        brightQR(e);
    }
}

void ui_event_Container2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_ScreenHome, LV_SCR_LOAD_ANIM_NONE, 500, 0, &ui_ScreenHome_screen_init);
        _ui_flag_modify(ui_LabelBtnInit, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        screenQrExit(e);
    }
}

void ui_event_Switch1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        masterModeSelect(e);
    }
}

void ui_event_ScreenLogo(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        logoClick(e);
        _ui_screen_change(&ui_ScreenHome, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenHome_screen_init);
    }
}

void ui_event_RollerYear(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        timeChanged(e);
    }
}

void ui_event_RollerMonth(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        timeChanged(e);
    }
}

void ui_event_RollerDay(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        timeChanged(e);
    }
}

void ui_event_RollerWeek(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        timeChanged(e);
    }
}

void ui_event_RollerHour(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        timeChanged(e);
    }
}

void ui_event_RollerMin(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        timeChanged(e);
    }
}

void ui_event_Button10(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        nowTimeSave(e);
        _ui_screen_change(&ui_ScreenHome, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenHome_screen_init);
    }
}

void ui_event_Button11(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_ScreenHome, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_ScreenHome_screen_init);
        nowTimeExit(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_ScreenHome_screen_init();
    ui_ScreenQR_screen_init();
    ui_ScreenLogo_screen_init();
    ui_ScreenClock_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_ScreenHome);
}
