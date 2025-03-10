// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.6
// Project name: SQL_LM_LED_8048S050-V3.0

#ifndef _SQL_LM_LED_8048S050_V3_0_UI_H
#define _SQL_LM_LED_8048S050_V3_0_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "lv_i18n.h"
#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_ScreenHome
void ui_ScreenHome_screen_init(void);
void ui_event_ScreenHome(lv_event_t * e);
extern lv_obj_t * ui_ScreenHome;
void ui_event_Container1(lv_event_t * e);
extern lv_obj_t * ui_Container1;
void ui_event_Image1(lv_event_t * e);
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_ImageSwitch;
extern lv_obj_t * ui_ImageDoor;
extern lv_obj_t * ui_ImageBattery;
extern lv_obj_t * ui_ImageNetwork;
void ui_event_ContainerSetting(lv_event_t * e);
extern lv_obj_t * ui_ContainerSetting;
extern lv_obj_t * ui_ImageSetting;
extern lv_obj_t * ui_Label7;
void ui_event_ContainerWiFi(lv_event_t * e);
extern lv_obj_t * ui_ContainerWiFi;
extern lv_obj_t * ui_ImageWiFi;
extern lv_obj_t * ui_LabelBright;
extern lv_obj_t * ui_LabelBGBright;
void ui_event_Slider1(lv_event_t * e);
extern lv_obj_t * ui_Slider1;
void ui_event_Slider3(lv_event_t * e);
extern lv_obj_t * ui_Slider3;
extern lv_obj_t * ui_LabelLED;
extern lv_obj_t * ui_LabelBr;
extern lv_obj_t * ui_PanelButton;
void ui_event_Button1(lv_event_t * e);
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Image6;
extern lv_obj_t * ui_Label1;
void ui_event_Button4(lv_event_t * e);
extern lv_obj_t * ui_Button4;
extern lv_obj_t * ui_Image9;
extern lv_obj_t * ui_Label4;
void ui_event_Button2(lv_event_t * e);
extern lv_obj_t * ui_Button2;
extern lv_obj_t * ui_Image7;
extern lv_obj_t * ui_Label2;
void ui_event_Button5(lv_event_t * e);
extern lv_obj_t * ui_Button5;
extern lv_obj_t * ui_Image15;
extern lv_obj_t * ui_Label5;
void ui_event_Button3(lv_event_t * e);
extern lv_obj_t * ui_Button3;
extern lv_obj_t * ui_Image8;
extern lv_obj_t * ui_Label3;
void ui_event_Button6(lv_event_t * e);
extern lv_obj_t * ui_Button6;
extern lv_obj_t * ui_Image16;
extern lv_obj_t * ui_Label21;
extern lv_obj_t * ui_Container7;
extern lv_obj_t * ui_Container10;
extern lv_obj_t * ui_Container9;
extern lv_obj_t * ui_Container8;
extern lv_obj_t * ui_Container11;
extern lv_obj_t * ui_Container14;
extern lv_obj_t * ui_Container13;
extern lv_obj_t * ui_Container12;
void ui_event_PanelMessage(lv_event_t * e);
extern lv_obj_t * ui_PanelMessage;
extern lv_obj_t * ui_LabelMessage;
void ui_event_Button11(lv_event_t * e);
extern lv_obj_t * ui_Button11;
extern lv_obj_t * ui_Label10;
extern lv_obj_t * ui_ContainerMasking;
extern lv_obj_t * ui_Container16;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenQR
void ui_ScreenQR_screen_init(void);
extern lv_obj_t * ui_ScreenQR;
extern lv_obj_t * ui_Label23;
extern lv_obj_t * ui_LabelSSID;
extern lv_obj_t * ui_LabelPSWD;
extern lv_obj_t * ui_Label24;
extern lv_obj_t * ui_ImageQR;
void ui_event_Slider2(lv_event_t * e);
extern lv_obj_t * ui_Slider2;
extern lv_obj_t * ui_Label37;
extern lv_obj_t * ui_Label39;
void ui_event_Container2(lv_event_t * e);
extern lv_obj_t * ui_Container2;
void ui_event_Switch1(lv_event_t * e);
extern lv_obj_t * ui_Switch1;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenUserSet
void ui_ScreenUserSet_screen_init(void);
void ui_event_ScreenUserSet(lv_event_t * e);
extern lv_obj_t * ui_ScreenUserSet;
void ui_event_Button21(lv_event_t * e);
extern lv_obj_t * ui_Button21;
extern lv_obj_t * ui_Label18;
void ui_event_Button22(lv_event_t * e);
extern lv_obj_t * ui_Button22;
extern lv_obj_t * ui_Label19;
void ui_event_Roller1(lv_event_t * e);
extern lv_obj_t * ui_Roller1;
extern lv_obj_t * ui_Container4;
extern lv_obj_t * ui_Container3;
extern lv_obj_t * ui_Container5;
extern lv_obj_t * ui_Container6;
void ui_event_SliderRed(lv_event_t * e);
extern lv_obj_t * ui_SliderRed;
void ui_event_SliderGreen(lv_event_t * e);
extern lv_obj_t * ui_SliderGreen;
void ui_event_SliderBlue(lv_event_t * e);
extern lv_obj_t * ui_SliderBlue;
void ui_event_Button7(lv_event_t * e);
extern lv_obj_t * ui_Button7;
extern lv_obj_t * ui_Label50;
void ui_event_Button8(lv_event_t * e);
extern lv_obj_t * ui_Button8;
extern lv_obj_t * ui_Label11;
void ui_event_Button24(lv_event_t * e);
extern lv_obj_t * ui_Button24;
extern lv_obj_t * ui_Label12;
void ui_event_Button25(lv_event_t * e);
extern lv_obj_t * ui_Button25;
extern lv_obj_t * ui_Label14;
void ui_event_Button26(lv_event_t * e);
extern lv_obj_t * ui_Button26;
extern lv_obj_t * ui_Label28;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenSetting
void ui_ScreenSetting_screen_init(void);
void ui_event_ScreenSetting(lv_event_t * e);
extern lv_obj_t * ui_ScreenSetting;
void ui_event_Button9(lv_event_t * e);
extern lv_obj_t * ui_Button9;
extern lv_obj_t * ui_Label15;
void ui_event_Button12(lv_event_t * e);
extern lv_obj_t * ui_Button12;
extern lv_obj_t * ui_Label16;
extern lv_obj_t * ui_Label17;
void ui_event_SwitchMood(lv_event_t * e);
extern lv_obj_t * ui_SwitchMood;
extern lv_obj_t * ui_Label33;
extern lv_obj_t * ui_Label34;
extern lv_obj_t * ui_Label45;
void ui_event_SwitchSide(lv_event_t * e);
extern lv_obj_t * ui_SwitchSide;
extern lv_obj_t * ui_Label46;
extern lv_obj_t * ui_Label47;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenLogo
void ui_ScreenLogo_screen_init(void);
void ui_event_ScreenLogo(lv_event_t * e);
extern lv_obj_t * ui_ScreenLogo;
extern lv_obj_t * ui_Image2;
// CUSTOM VARIABLES

// SCREEN: ui_ScreenLogo2
void ui_ScreenLogo2_screen_init(void);
void ui_event_ScreenLogo2(lv_event_t * e);
extern lv_obj_t * ui_ScreenLogo2;
extern lv_obj_t * ui_Image3;
// CUSTOM VARIABLES

// EVENTS

extern lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS
LV_IMG_DECLARE(ui_img_1332874776);    // assets/LM-Logo.png
LV_IMG_DECLARE(ui_img_switch_png);    // assets/switch.png
LV_IMG_DECLARE(ui_img_door_png);    // assets/door.png
LV_IMG_DECLARE(ui_img_battery_png);    // assets/battery.png
LV_IMG_DECLARE(ui_img_network_png);    // assets/network.png
LV_IMG_DECLARE(ui_img_setting_png);    // assets/setting.png
LV_IMG_DECLARE(ui_img_wifi_png);    // assets/wifi.png
LV_IMG_DECLARE(ui_img_1448386974);    // assets/CamTrol Icon-61.png
LV_IMG_DECLARE(ui_img_tv_png);    // assets/tv.png
LV_IMG_DECLARE(ui_img_316324357);    // assets/QR-Code-064.png
LV_IMG_DECLARE(ui_img_2128645090);    // assets/LM-Logo 2.png
LV_IMG_DECLARE(ui_img_618194214);    // assets/logo-3.png
LV_IMG_DECLARE(ui_img_44959014);    // assets/CamTrol Icon-54.png
LV_IMG_DECLARE(ui_img_366657795);    // assets/CamTrol Icon-62.png
LV_IMG_DECLARE(ui_img_1965946780);    // assets/Icon-18.png
LV_IMG_DECLARE(ui_img_1543939845);    // assets/Icon-24.png
LV_IMG_DECLARE(ui_img_invertor_png);    // assets/invertor.png
LV_IMG_DECLARE(ui_img_limousine_png);    // assets/LIMOUSINE.png
LV_IMG_DECLARE(ui_img_1438240077);    // assets/LM-Logo 1.png
LV_IMG_DECLARE(ui_img_1446094990);    // assets/logo-65.png
LV_IMG_DECLARE(ui_img_1719749116);    // assets/QR-Code-001.png
LV_IMG_DECLARE(ui_img_1119053922);    // assets/QR-Code-007.png
LV_IMG_DECLARE(ui_img_765407899);    // assets/QR-Code-035.png
LV_IMG_DECLARE(ui_img_37325770);    // assets/QR-Code-036.png
LV_IMG_DECLARE(ui_img_316326407);    // assets/QR-Code-044.png
LV_IMG_DECLARE(ui_img_1119049822);    // assets/QR-Code-047.png
LV_IMG_DECLARE(ui_img_1847130928);    // assets/QR-Code-052.png
LV_IMG_DECLARE(ui_img_1187410730);    // assets/QR-Code-069.png
LV_IMG_DECLARE(ui_img_1719747066);    // assets/QR-Code-021.png
LV_IMG_DECLARE(ui_img_105685651);    // assets/QR-Code-028.png

// FONTS
LV_FONT_DECLARE(ui_font_DigitalPlaySt12);
LV_FONT_DECLARE(ui_font_SOYOMapleBold12);
LV_FONT_DECLARE(ui_font_SOYOMapleBold14);
LV_FONT_DECLARE(ui_font_SOYOMapleBold18);
LV_FONT_DECLARE(ui_font_SOYOMapleBold22);
LV_FONT_DECLARE(ui_font_SOYOMapleBold26);

// UI INIT
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
