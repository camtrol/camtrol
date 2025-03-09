
/*******************************************************************************
 * https://www.remove.bg/ko
 * LVGL Widgets
 * This is a widgets demo for LVGL - Light and Versatile Graphics Library
 * import from: https://github.com/lvgl/lv_demos.git
 *
 * Dependent libraries:
 * LVGL: https://github.com/lvgl/lvgl.git

 * Touch libraries:
 * FT6X36: https://github.com/strange-v/FT6X36.git
 * GT911: https://github.com/TAMCTec/gt911-arduino.git
 * XPT2046: https://github.com/PaulStoffregen/XPT2046_Touchscreen.git
 *
 * LVGL Configuration file:
 * Copy your_arduino_path/libraries/lvgl/lv_conf_template.h
 * to your_arduino_path/libraries/lv_conf.h
 * Then find and set:
 * #define LV_COLOR_DEPTH     16
 * #define LV_TICK_CUSTOM     1
 *
 * For SPI display set color swap can be faster, parallel screen don't set!
 * #define LV_COLOR_16_SWAP   1
 *
 * Optional: Show CPU usage and FPS count
 * #define LV_USE_PERF_MONITOR 1
 ******************************************************************************/

#include "Arduino.h"
#include "ui/ui.h" // this is the ui generated with lvgl / squareline editor
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define TFT_BL 2

#define RTS 11 // RS485 Enable
// #define ON_STATE 3
// #define OFF_STATE 2

#define ON 1
#define OFF 0

#define CHECKED 3
#define UNCHECKED 2

#define SLAVE_MODE false
#define MASTER_MODE true

#define STATION_MODE true
#define AP_MODE false

// EEPROM Address
#define MASTER_SLAVE_EEPROM 2 // bool
#define LOGO_CHANGE_EEPROM 4  // bool
#define LED1_NAME_EEPROM 10   // Unsigned Long
#define LED2_NAME_EEPROM 20   // Unsigned Long
#define LED3_NAME_EEPROM 30   // Unsigned Long
#define LED4_NAME_EEPROM 40   // Unsigned Long
#define LED5_NAME_EEPROM 50   // Unsigned Long

// #define BG_VALUE_EEPROM 50 //

#define SSID_HOTSPOT_EEPROM 110 // HOT Spot SSID String
#define PSWD_HOTSPOT_EEPROM 160 // HOT Spot PSWD String

void eventSec();
void wifiStart();
void comSend(String sendData);

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, ssidHotSpot, pswdHotSpot, hotSpotIP;
String ledNameOrigin[6] = {" ",      "메인등", "무드등",
                           "사이드", "공조등", "플로어"};
String ledName[6] = {};

WebServer server(80);

bool comFlag, switchState, doorState, batState = false;
bool dataReceiveFlag = false;

// bool dataReceiveFlag = false; // 최초 Data 수신 Check...

bool neo1Flag; // 무드등 true:NEO, false: 일반
bool neo2Flag; // 사이드 true:NEO, false: 일반
bool doorFlag; // 사이드 true:NEO, false: 일반
byte neo1Mode, neo2Mode, neoSelect;
byte neo1ModePush, neo2ModePush;
byte logoChangeCounter = 0;

bool comInitFlag = false;

bool ledFlag[6] = {false, false, false, false, false, false};
bool tvFlag, logoFlag = false;
bool logoChangeFlag;

bool masterModeFlag = false;
bool networkFlag = false;
bool networkChangeFlag = false;
bool hotSpotIpSetFlag = false;

char ssidAp[30] = "엘엠 V1.0-LM6-001";
char passwordAp[30] = "17240719001";

char msg[100];
char label_text[50];

uint8_t loopCount = 0;
uint8_t readStatus = 0;

uint8_t bright;

byte neo1startNumber; // Neo 1 시작 번호
byte neo2startNumber; // Neo 2 시작 번호
byte neo1TotalNumber; // Neo 1 총 갯수
byte neo2TotalNumber; // Neo 2 총 갯수

uint8_t sliderRedColor[3], sliderGreenColor[3], sliderBlueColor[3];
uint16_t lvState;
uint16_t logoCounter = 0;
// uint32_t chgColor;

int wifiCount = 10;
uint8_t ledPwm[6] = {100, 100, 100, 100, 100, 100};
int countValue = 0;
int ledSelect = 9;
int rs485ComCount = 1000;

// float tvAdj;

/* More dev device declaration:
 * https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class:
 * https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
// Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class:
 * https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
// Arduino_GFX *gfx = new Arduino_ILI9341(bus, DF_GFX_RST, 0 /* rotation */,
// false /* IPS */);

// ESP32-8048S050 IPS LCD 800x480
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */,
    GFX_NOT_DEFINED /* SDA */, 40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */,
    42 /* PCLK */, 45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */,
    14 /* R4 */, 5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */,
    4 /* G5 */, 8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
    );

// ESP32-8048S050 IPS LCD 800x480
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus, 800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */,
    4 /* hsync_pulse_width */, 8 /* hsync_back_porch */, 480 /* height */,
    0 /* vsync_polarity */, 8 /* vsync_front_porch */,
    4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */,
    true /* auto_flush */);

#endif /* !defined(DISPLAY_DEV_KIT) */

#include "touch.h"

void logoChangeClick(lv_event_t *e) {
  logoChangeCounter++;
  Serial.println("Logo Clicked...");
  Serial.print("Logo Counter : ");
  Serial.println(logoChangeCounter);
  if (logoChangeCounter > 10) {
    if (logoChangeFlag) {
      logoChangeFlag = false;
      lv_disp_load_scr(ui_ScreenLogo);
    } else {
      logoChangeFlag = true;
      lv_disp_load_scr(ui_ScreenLogo2);
    }

    logoChangeCounter = 0;
    EEPROM.writeBool(LOGO_CHANGE_EEPROM, logoChangeFlag);
    EEPROM.commit();
  }
  delay(100);
}

void SliderRedReleased(lv_event_t *e) {
  Serial.println("Slider RED Released...");

  s = "NEO_R_SAVE:";
  s += sliderRedColor[neoSelect];
  s += ':';
  s += neoSelect;
  comSend(s);
}

void SliderGreenReleased(lv_event_t *e) {
  Serial.println("Slider GREEN Released...");

  s = "NEO_G_SAVE:";
  s += sliderGreenColor[neoSelect];
  s += ':';
  s += neoSelect;
  comSend(s);
}

void SliderBlueReleased(lv_event_t *e) {
  Serial.println("Slider BLUE Released...");

  s = "NEO_B_SAVE:";
  s += sliderBlueColor[neoSelect];
  s += ':';
  s += neoSelect;
  comSend(s);
}

void neoMode1(lv_event_t *e) {
  Serial.println("NEO Mode 1 Select...");

  lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);

  lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button26, LV_STATE_CHECKED);

  if (ledSelect == 2) {
    neo1Mode = 0;
    s = "LED2:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1Mode;
    s += ':';
    s += neo1Mode;
    comSend(s);
  } else if (ledSelect == 3) {
    neo2Mode = 0;
    s = "LED3:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2Mode;
    s += ':';
    s += neo2Mode;
    comSend(s);
  }

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);
}

void neoMode2(lv_event_t *e) {
  Serial.println("NEO Mode 2 Select...");

  lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);

  lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button26, LV_STATE_CHECKED);

  if (ledSelect == 2) {
    neo1Mode = 1;
    s = "LED2:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1Mode;
    s += ':';
    s += neo1Mode;
    comSend(s);
  } else if (ledSelect == 3) {
    neo2Mode = 1;
    s = "LED3:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2Mode;
    s += ':';
    s += neo2Mode;
    comSend(s);
  }

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);
}

void neoMode3(lv_event_t *e) {
  Serial.println("NEO Mode 3 Select...");

  lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);

  lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_add_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button26, LV_STATE_CHECKED);

  if (ledSelect == 2) {
    neo1Mode = 2;
    s = "LED2:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1Mode;
    s += ':';
    s += neo1Mode;
    comSend(s);
  } else if (ledSelect == 3) {
    neo2Mode = 2;
    s = "LED3:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2Mode;
    s += ':';
    s += neo2Mode;
    comSend(s);
  }

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);
}

void neoMode4(lv_event_t *e) {
  Serial.println("NEO Mode 4 Select...");

  lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);

  lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_add_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button26, LV_STATE_CHECKED);

  if (ledSelect == 2) {
    neo1Mode = 3;
    s = "LED2:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1Mode;
    s += ':';
    s += neo1Mode;
    comSend(s);
  } else if (ledSelect == 3) {
    neo2Mode = 3;
    s = "LED3:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2Mode;
    s += ':';
    s += neo2Mode;
    comSend(s);
  }

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);
}

void neoMode5(lv_event_t *e) {
  Serial.println("NEO Mode 5 Select...");

  lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);

  lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_add_state(ui_Button26, LV_STATE_CHECKED);

  if (ledSelect == 2) {
    neo1Mode = 7;
    s = "LED2:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1Mode;
    s += ':';
    s += neo1Mode;
    comSend(s);
  } else if (ledSelect == 3) {
    neo2Mode = 7;
    s = "LED3:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2Mode;
    s += ':';
    s += neo2Mode;
    comSend(s);
  }

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);
}

void setLEDPwm(lv_event_t *e) {
  Serial.println("PWM Slide Released...");

  // if (ledSelect == 1)
  //   return;

  s = "PWM_SET:";
  s += ledPwm[ledSelect];
  s += ":";
  s += ledSelect;
  comSend(s);
}

void logoFlagReset(lv_event_t *e) {
  Serial.println("Logo Long click...");

  logoFlag = false;
  lv_disp_load_scr(ui_ScreenHome);
  ledcWrite(0, map(bright, 1, 100, 1, 255));
  logoCounter = 0;
}

void logoClick(lv_event_t *e) {
  Serial.println("Logo Long click...");

  logoFlag = true;
  ledcWrite(0, 50);
  if (logoChangeFlag)
    lv_disp_load_scr(ui_ScreenLogo2);
  else
    lv_disp_load_scr(ui_ScreenLogo);
  // logoCounter = 0;
}

// void saveTVAdj(lv_event_t *e) {
//   Serial.println("Save TV Adj Button...");
//   s = "SAVE_ADJ:"; // NEO 1 Mode
//   s += 1;
//   comSend(s);
// }

// void tvVoltAdjustSlider(lv_event_t *e) {
//   Serial.println("Tv Volt Adj Slider...");
//   tvAdj = lv_slider_get_value(ui_Slider4);
//   s = "TV_ADJ:"; // NEO 1 Mode
//   s += tvAdj;
//   comSend(s);
//   tvAdj /= 100;
//   s = tvAdj;
//   s += " V";
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelTvAdj, label_text);
// }

void buttonOutLine() {
  lv_obj_set_style_outline_opa(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void userSettingCancel(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("User Setting Cancel...");

  // s = "NEO_1_MODE:";
  // s += neo1Mode;
  // s += ':';
  // s += neo1Mode;
  // comSend(s);
  // delay(5);
  // s = "NEO_2_MODE:";
  // s += neo1Mode;
  // s += ':';
  // s += neo1Mode;
  // comSend(s);

  // buttonOutLine();
  // if (ledSelect == 2) {
  //   lv_obj_set_style_outline_opa(ui_Button2, 255,
  //                                LV_PART_MAIN | LV_STATE_DEFAULT);
  //   s = "NEO_1_MODE:";
  //   s += neo1Mode;
  //   s += ':';
  //   s += neo1Mode;
  //   comSend(s);
  // } else if (ledSelect == 3) {
  //   lv_obj_set_style_outline_opa(ui_Button3, 255,
  //                                LV_PART_MAIN | LV_STATE_DEFAULT);
  //   s = "NEO_2_MODE:";
  //   s += neo1Mode;
  //   s += ':';
  //   s += neo1Mode;
  //   comSend(s);
  // }

  // if (neoSelect == 1) {
  //   neo1Mode = neo1ModePush;
  //   lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
  //   s = "LED2:";
  //   s += 0;
  //   comSend(s);
  // } else {
  //   neo2Mode = neo2ModePush;
  //   lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
  //   s = "LED3:";
  //   s += 0;
  //   comSend(s);
  // }
}

void userSetting(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("User Setting...");

  neo1Mode = neo1ModePush;
  neo2Mode = neo1ModePush;

  if (neoSelect == 1) {
    // sliderRedColor[1] = sliderRedColor[0];
    // sliderGreenColor[1] = sliderGreenColor[0];
    // sliderBlueColor[1] = sliderBlueColor[0];
    s = "N1_MODE:"; // NEO 1 Mode
    s += neo1Mode;
    comSend(s);
    delay(5);
    s = "N1_RED:"; // NEO 1 Slider RED Value
    s += sliderRedColor[1];
    comSend(s);
    delay(5);
    s = "N1_GREEN:"; // NEO 1 Slider GREEN Value
    s += sliderGreenColor[1];
    comSend(s);
    delay(5);
    s = "N1_BLUE:"; // NEO 1 Slider BLUE Value
    s += sliderBlueColor[1];
    comSend(s);
    delay(5);
  } else {
    // sliderRedColor[2] = sliderRedColor[0];
    // sliderGreenColor[2] = sliderGreenColor[0];
    // sliderBlueColor[2] = sliderBlueColor[0];
    s = "NEO_2_MODE:"; // NEO 2 Mode
    s += neo2Mode;
    comSend(s);
    delay(5);
    s = "N2_RED:"; // NEO 2 Slider RED Value
    s += sliderRedColor[2];
    comSend(s);
    delay(5);

    s = "N2_GREEN:"; // NEO 2 Slider GREEN Value
    s += sliderGreenColor[2];
    comSend(s);
    delay(5);

    s = "N2_BLUE:"; // NEO 2 Slider BLUE Value
    s += sliderBlueColor[2];
    comSend(s);
    delay(5);
  }
  s = "USER_SET:"; // NEO 2 Slider BLUE Value
  s += 1;
  comSend(s);
  delay(5);
  // EEPROM.commit();
}

void factorySettingCancel(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Factory setting Cancel...");

  ledFlag[2] = false;
  ledFlag[3] = false;
  lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
  s = "LED2:";
  s += 0;
  s += ":";
  s += 0;
  comSend(s);
  s = "LED3:";
  s += 0;
  s += ":";
  s += 0;
  comSend(s);
}

void factorySetting(lv_event_t *e) {
  Serial.println("Factory setting...");

  ledFlag[2] = true;
  ledFlag[3] = true;
  s = ledPwm[ledSelect];
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
  lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "NEO_1_FLAG:"; // NEO 1 Flag
  s += neo1Flag;
  comSend(s);
  delay(5);
  s = "NEO_2_FLAG:"; // NEO 2 Flag
  s += neo2Flag;
  comSend(s);
  // delay(5);
  // s = "NEO_1_START:"; // NEO 1 Start Number
  // s += neo1startNumber;
  // comSend(s);
  // delay(5);
  // s = "NEO_2_START:"; // NEO 2 Start Number
  // s += neo2startNumber;
  // comSend(s);
  // delay(5);
  // s = "NEO_1_TOTAL:"; // NEO 1 Total Number
  // s += neo1TotalNumber;
  // comSend(s);
  // delay(5);
  // s = "NEO_2_TOTAL:"; // NEO 2 Total Number
  // s += neo2TotalNumber;
  // comSend(s);
  // delay(5);
  // s = "DOOR_TYPE:"; // NEO 2 Total Number
  // s += doorFlag;
  // comSend(s);
  delay(5);
  s = "FACTORY_SAVE:"; // NEO 1 Flag
  s += 1;
  comSend(s);
  // s = "LED2:";
  // s += 0;
  // comSend(s);
  // s = "LED3:";
  // s += 0;
  // comSend(s);
}

void ScreenUserSetLoad(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Screen User Set Loaded ...");
}

void ScreenSettingLoad(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Screen User Set Loaded ...");
}

// void neoTestButton(lv_event_t *e) { Serial.println("Neo Test Button ..."); }

void tvOutButton(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("TV Out Button ...");
  lvState = lv_obj_get_state(ui_Button6);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (lvState == 3) {
    s = "정지";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label10, label_text);
  } else {
    s = "출력";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label10, label_text);
  }

  s = "TV_FORCED:";
  if (lvState == 3) {
    tvFlag = true;
    s += '1';
  } else {
    tvFlag = false;
    s += '0';
  }
  comSend(s);
}

void rollerNeoMode(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Neo Roller Select ...");

  neo1ModePush = lv_roller_get_selected(ui_Roller1);
  if (neo1ModePush == 0) {
    lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
  } else {
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
  }
  // s = "NEO_1_MODE:";
  // s += neo1ModePush;
  // s += ':';
  // s += neo1ModePush;
  // comSend(s);
  // delay(5);
  // s = "NEO_2_MODE:";
  // s += neo1ModePush;
  // s += ':';
  // s += neo1ModePush;
  // comSend(s);

  buttonOutLine();
  if (ledSelect == 2) {
    lv_obj_set_style_outline_opa(ui_Button2, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    s = "LED2:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1ModePush;
    s += ':';
    s += neo1ModePush;
    comSend(s);
  } else if (ledSelect == 3) {
    lv_obj_set_style_outline_opa(ui_Button3, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    s = "LED3:";
    s += ledPwm[ledSelect];
    s += ":";
    s += 1;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo1ModePush;
    s += ':';
    s += neo1ModePush;
    comSend(s);
  }

  Serial.print("Roller 1 Selected : ");
  Serial.println(neo1ModePush);
}

// void startNeoPlus1(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Start Neo Plus 1...");
//   if (neo1startNumber == 10)
//     return;
//   neo1startNumber++;
//   s = neo1startNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEOStartNum1, label_text);
// }

// void startNeoMinus1(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Start Neo Minus 1...");
//   if (neo1startNumber == 0)
//     return;
//   neo1startNumber--;
//   s = neo1startNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEOStartNum1, label_text);
// }

// void numberNeoPlus1(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Number Neo Plus 1...");
//   if (neo1TotalNumber == 50)
//     return;
//   neo1TotalNumber += 5;
//   s = neo1TotalNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEONumber1, label_text);
// }

// void numberNeoMinus1(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Number Neo Minus 1...");
//   if (neo1TotalNumber == 10)
//     return;
//   neo1TotalNumber -= 5;
//   s = neo1TotalNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEONumber1, label_text);
// }

// void startNeoPlus2(lv_event_t *e) {
//   Serial.println("Start Neo Plus 2...");
//   if (neo2startNumber == 10)
//     return;
//   neo2startNumber++;
//   s = neo2startNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEOStartNum2, label_text);
// }

// void startNeoMinus2(lv_event_t *e) {
//   Serial.println("Start Neo Minus 2...");
//   if (neo2startNumber == 0)
//     return;
//   neo2startNumber--;
//   s = neo2startNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEOStartNum2, label_text);
// }

// void numberNeoPlus2(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Number Neo Plus 2...");
//   if (neo2TotalNumber == 50)
//     return;
//   neo2TotalNumber += 5;
//   s = neo2TotalNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEONumber2, label_text);
// }

// void numberNeoMinus2(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Number Neo Minus 2...");
//   if (neo2TotalNumber == 10)
//     return;
//   neo2TotalNumber -= 5;
//   s = neo2TotalNumber;
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelNEONumber2, label_text);
// }

void switchMood(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Switch Mood...");

  if (lv_obj_get_state(ui_SwitchMood) == CHECKED) {
    Serial.println("Mood LED NEO...");
    // lv_obj_clear_flag(ui_Slider1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Container7,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // lv_obj_clear_flag(ui_ContainerMood,
    //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_add_flag(ui_Image7, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    neo1Flag = true;
  } else {
    Serial.println("Mood LED General...");
    // lv_obj_add_flag(ui_Slider1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui_Container7,
                    LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // lv_obj_add_flag(ui_ContainerMood,
    //                 LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image7, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    neo1Flag = false;
  }
}

void switchSide(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Switch Side...");

  if (lv_obj_get_state(ui_SwitchSide) == CHECKED) {
    Serial.println("Side LED NEO...");
    lv_obj_clear_flag(ui_Container11,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // lv_obj_clear_flag(ui_ContainerSide,
    //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_add_flag(ui_Image8, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    neo2Flag = true;
  } else {
    Serial.println("Side LED General...");
    lv_obj_add_flag(ui_Container11,
                    LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // lv_obj_add_flag(ui_ContainerSide,
    //                 LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image8, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    neo2Flag = false;
  }
}

// void SwitchDoor(lv_event_t *e) {
//   logoCounter = 0;
//   Serial.println("Switch Door...");

//   if (lv_obj_get_state(ui_SwitchDoor) == CHECKED) {
//     doorFlag = true;
//     Serial.println("Door B Type...");
//   } else {
//     doorFlag = false;
//     Serial.println("Door A Type...");
//   }
// }

void buttonTest(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Button Test...");

  s = "neo1startNumber:";
  s += neo1startNumber;
  comSend(s);
  s = "neo1TotalNumber:";
  s += neo1TotalNumber;
  comSend(s);
  s = "neo2startNumber:";
  s += neo2startNumber;
  comSend(s);
  s = "neo2TotalNumber:";
  s += neo2TotalNumber;
  comSend(s);
  s = "NEO_TEST:";
  s += 1;
  comSend(s);
  s = "NEO_TEST:";
  s += 1;
  comSend(s);
}

void BGBright(lv_event_t *e) { // 화면 밝기 조절...
  logoCounter = 0;
  bright = lv_slider_get_value(ui_Slider3);
  Serial.println("Bright Slider... ");
  Serial.println(bright);
  if (bright < 10)
    ledcWrite(0, 2); // 제일 낮은 밝기임...
  else
    ledcWrite(0, map(bright, 1, 100, 1, 255));
}

void setBright(lv_event_t *e) { // 화면 밝기 조절...
  Serial.println("Set Bright Slider... ");

  s = "BG_VALUE:"; // NEO 1 Mode
  s += bright;
  comSend(s);
}

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  Serial.println("handleRoot...");
  server.send(200, "text/plain", "");
}

void handleJson() {
  Serial.println("handleJson...");

  server.arg("j").toCharArray(msg, 2000);
  Serial.println();
  Serial.println(F("handleJson()..."));
  Serial.println(msg);
  deserializeJson(doc, msg);
  root = doc.as<JsonObject>();

  String division = root["division"];
  String ssidParse = root["ssid"];
  String passwordParse = root["password"];
  String _ledName = root["LED_NAME"];
  String nowTime = root["NOW_TIME"];
  int chanel = root["Chanel"];
  int value = root["Value"];

  if (division == "LED") {
    s = value;
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, value, LV_ANIM_OFF);

    switch (chanel) {
    case 1:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button1, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[1] = false;
        lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[1] = true;
        lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
        ledPwm[1] = value;
      }
      s = "LED1:";
      s += ledFlag[1] == true ? ledPwm[1] : 0;
      comSend(s);

      s = ledName[1];

      break;
    case 2:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button2, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[2] = false;
        lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[2] = true;
        lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
        ledPwm[2] = value;
      }
      s = "LED2:";
      s += ledFlag[2] == true ? ledPwm[2] : 0;
      comSend(s);

      s = ledName[2];
      break;
    case 3:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button3, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[3] = false;
        lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[3] = true;
        lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
        ledPwm[3] = value;
      }
      s = "LED3:";
      s += ledFlag[3] == true ? ledPwm[3] : 0;
      comSend(s);

      s = ledName[3];
      break;
    case 4:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button4, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[4] = false;
        lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[4] = true;
        lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
        ledPwm[4] = value;
      }
      s = "LED4:";
      s += ledFlag[4] == true ? ledPwm[4] : 0;
      comSend(s);

      s = ledName[4];
      break;
    case 5:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button5, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[5] = false;
        lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[5] = true;
        lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
        ledPwm[5] = value;
      }
      s = "LED5:";
      s += ledFlag[5] == true ? ledPwm[5] : 0;
      comSend(s);

      s = ledName[5];
      break;
    default:
      break;
    }
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else if (division == "TV") {
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button6, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!value) { // 값이 0 이면...
      tvFlag = false;
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    } else { // 값이 1 이면...
      tvFlag = true;
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    }
    s = "TV:";
    s += tvFlag == true ? '1' : '0';
    comSend(s);
  }

  else if (division == "LED_NAME") {
    switch (chanel) {
    case 1:
      ledName[1] = _ledName;
      break;
    case 2:
      ledName[2] = _ledName;
      break;
    case 3:
      ledName[3] = _ledName;
      break;
    case 4:
      ledName[4] = _ledName;
      break;
    case 10:
      Serial.println("LED Name Save...");
      EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
      EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
      EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
      EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
      EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
      EEPROM.commit();
      ledName[1].toCharArray(label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_Label1, label_text);
      ledName[2].toCharArray(label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_Label2, label_text);
      ledName[3].toCharArray(label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_Label3, label_text);
      ledName[4].toCharArray(label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_Label4, label_text);
      ledName[5].toCharArray(label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_Label5, label_text);
      break;
    case 0:
      ledName[1] = ledNameOrigin[1];
      ledName[2] = ledNameOrigin[2];
      ledName[3] = ledNameOrigin[3];
      ledName[4] = ledNameOrigin[4];
      ledName[5] = ledNameOrigin[5];
      EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
      EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
      EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
      EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
      EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
      EEPROM.commit();
      ledName[1].toCharArray(label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_Label1, label_text);
      ledName[2].toCharArray(label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_Label2, label_text);
      ledName[3].toCharArray(label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_Label3, label_text);
      ledName[4].toCharArray(label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_Label4, label_text);
      ledName[5].toCharArray(label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_Label5, label_text);
      break;
    default:
      break;
    }
  } else if (division == "QR_OK") {
    lv_disp_load_scr(ui_ScreenHome);
  }

  s = "";
  s = s + "{\"result\":" + "\"Receive OK\"}";
  server.send(200, "text/plain", s);

} // handleJson()

void handleRequest() {

  wifiCount = 0;

  Serial.println("handleRequest...");

  s = "";
  s = s + "{" + "\"COM_FLAG\":" + comFlag + ", " + "\"SWITCH\":" + switchState +
      ", " + "\"DOOR\":" + doorState + ", " + "\"BAT\":" + batState + ", " +
      "\"ledValue1\":" + ledPwm[1] + ", " + "\"ledValue2\":" + ledPwm[2] +
      ", " + "\"ledValue3\":" + ledPwm[3] + ", " + "\"ledValue4\":" +
      ledPwm[4] + ", " + "\"ledFlag1\":" + ledFlag[1] + ", " + "\"ledFlag2\":" +
      ledFlag[2] + ", " + "\"ledFlag3\":" + ledFlag[3] + ", " +
      "\"ledFlag4\":" + ledFlag[4] + ", " + "\"floor\":" + ledFlag[5] + ", " +
      "\"tv\":" + tvFlag + "}";

  server.send(200, "text/plain", s);

} // handleRequest()

// void colorInit();

void ScreenHomeLoaded(lv_event_t *e) { Serial.println("ScreenHomeLoaded..."); }

void settingClick(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Setting Click...");

  comSend("REQ_NEO:1:1");
  // comSend("REQUEST");
}

void wifiClick(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("WiFi Click...");
  if (networkFlag == STATION_MODE) {
    hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
    lv_label_set_text(ui_LabelMessage, label_text);
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
  }
}

void masterModeSelect(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("MasterModeSelect... ");

  if (lv_obj_get_state(ui_Switch1) == CHECKED) {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, MASTER_MODE);
    masterModeFlag = MASTER_MODE;
    Serial.println("Master Mode Selected...");

    // lv_obj_clear_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label7, LV_OBJ_FLAG_HIDDEN);

    if (networkFlag == AP_MODE) {
      lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    }

  } else {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, SLAVE_MODE);
    masterModeFlag = SLAVE_MODE;
    Serial.println("Slave Mode Selected...");

    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label7, LV_OBJ_FLAG_HIDDEN);
  }
  EEPROM.commit();
}

void SliderRed(lv_event_t *e) { // ScreenUserSet SliderRed
  logoCounter = 0;
  sliderRedColor[neoSelect] = lv_slider_get_value(ui_SliderRed);

  s = "NEO ";
  s += neoSelect;
  s += " Slider RED : ";
  Serial.print(s);
  Serial.println(sliderRedColor[neoSelect], HEX);

  s = "NEO_R:";
  s += sliderRedColor[neoSelect];
  s += ':';
  s += neoSelect;
  comSend(s);
}

void SliderGreen(lv_event_t *e) { // ScreenUserSet SliderGreen
  logoCounter = 0;
  sliderGreenColor[neoSelect] = lv_slider_get_value(ui_SliderGreen);

  s = "NEO ";
  s += neoSelect;
  s += " Slider GREEN : ";
  Serial.print(s);
  Serial.println(sliderGreenColor[neoSelect], HEX);

  s = "NEO_G:";
  s += sliderGreenColor[neoSelect];
  s += ':';
  s += neoSelect;
  comSend(s);
}

void SliderBlue(lv_event_t *e) { // ScreenUserSet SliderBlue
  logoCounter = 0;
  sliderBlueColor[neoSelect] = lv_slider_get_value(ui_SliderBlue);

  s = "NEO ";
  s += neoSelect;
  s += " Slider BLUE : ";
  Serial.print(s);
  Serial.println(sliderBlueColor[neoSelect], HEX);

  s = "NEO_B:";
  s += sliderBlueColor[neoSelect];
  s += ':';
  s += neoSelect;
  comSend(s);
}

void SettingsSave(lv_event_t *e) {
  Serial.println("Settings Save...");

  char buf[10];

  lv_roller_get_selected_str(ui_Roller1, buf, sizeof(buf));
  s = buf;
  EEPROM.commit();
}

void panelMessageClick(lv_event_t *e) {
  logoCounter = 0;
  Serial.print("PanelMessage Click...");
}

void sleepButton(lv_event_t *e) {
  Serial.println("sleepButton... ");

  lv_timer_handler();
  delay(500);
}

void led1Left(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("LED 1 Bright...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (!ledFlag[1] || ledSelect == 1)
  //   return;
  ledSelect = 1;
  s = ledPwm[ledSelect];
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "BTN_SEL:";
  s += ledSelect;
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void led2Left(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("LED 2 Bright...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  ledSelect = 2;
  s = ledPwm[ledSelect];
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "BTN_SEL:";
  s += ledSelect;
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void led2GestureRight(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("LED 2 Setting...");
  ledSelect = 2;
  neoSelect = 1;

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (neo1Flag) { // 무드등 LED 가 NEO 면 실행
    neoSelect = 1;
    lv_roller_set_selected(ui_Roller1, neo1Mode, LV_ANIM_ON);
    lv_slider_set_value(ui_SliderRed, sliderRedColor[1], LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderGreen, sliderGreenColor[1], LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderBlue, sliderBlueColor[1], LV_ANIM_OFF);
    if (neo1Mode == 0) {
      lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    }
    // sliderRedColor[0] = sliderRedColor[1];
    // sliderGreenColor[0] = sliderGreenColor[1];
    // sliderBlueColor[0] = sliderBlueColor[1];
    lv_disp_load_scr(ui_ScreenUserSet);
  } else {
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button2, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!ledFlag[2] || ledSelect == 2)
      return;
    ledSelect = 2;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }

  lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button26, LV_STATE_CHECKED);

  switch (neo1Mode) {
  case 0:
    lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    // do something
    break;
  case 1:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    // do something
    break;
  case 2:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button24, LV_STATE_CHECKED);
    // do something
    break;
  case 3:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button25, LV_STATE_CHECKED);
    // do something
    break;
  case 7:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button26, LV_STATE_CHECKED);
    // do something
    break;
  default:
    break;
    // do something
  }
}

void led3Left(lv_event_t *e) {
  // return;
  logoCounter = 0;
  Serial.println("LED 3 Bright...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (!ledFlag[3] || ledSelect == 3)
  //   return;
  ledSelect = 3;

  Serial.print("LED Select : ");
  Serial.print(ledSelect);
  Serial.print(",  NEO 1 Mode : ");
  Serial.print(neo1Mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2Mode);

  s = ledPwm[ledSelect];
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "BTN_SEL:";
  s += ledSelect;
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void led3GestureRight(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("LED 3 Setting...");
  ledSelect = 3;
  neoSelect = 2;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (neo2Flag) { // 사이드 LED 가 NEO 면 실행
    neoSelect = 2;
    lv_roller_set_selected(ui_Roller1, neo2Mode, LV_ANIM_ON);
    lv_slider_set_value(ui_SliderRed, sliderRedColor[2], LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderGreen, sliderGreenColor[2], LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderBlue, sliderBlueColor[2], LV_ANIM_OFF);
    if (neo1Mode == 0) {
      lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    }
    // sliderRedColor[0] = sliderRedColor[2];
    // sliderGreenColor[0] = sliderGreenColor[2];
    // sliderBlueColor[0] = sliderBlueColor[2];
    lv_disp_load_scr(ui_ScreenUserSet);
  } else {
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button3, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!ledFlag[3] || ledSelect == 3)
      return;
    ledSelect = 3;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }

  lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button24, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button25, LV_STATE_CHECKED);
  lv_obj_clear_state(ui_Button26, LV_STATE_CHECKED);

  switch (neo2Mode) {
  case 0:
    lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    // do something
    break;
  case 1:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    // do something
    break;
  case 2:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button24, LV_STATE_CHECKED);
    // do something
    break;
  case 3:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button25, LV_STATE_CHECKED);
    // do something
    break;
  case 7:
    lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(ui_Button26, LV_STATE_CHECKED);
    // do something
    break;
  default:
    break;
    // do something
  }
}

void led4Left(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("LED 4 Bright...");
  // lv_obj_add_flag(ui_Slider1, LV_OBJ_FLAG_CLICKABLE);
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (!ledFlag[4] || ledSelect == 4)
  //   return;
  ledSelect = 4;
  s = ledPwm[ledSelect];
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "BTN_SEL:";
  s += ledSelect;
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void led5Left(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("LED 5 Bright...");
  // lv_obj_add_flag(ui_Slider1, LV_OBJ_FLAG_CLICKABLE);
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (!ledFlag[4] || ledSelect == 4)
  //   return;
  ledSelect = 5;
  Serial.println("ledPwm[ledSelect] : ");
  Serial.println(ledPwm[ledSelect]);
  s = ledPwm[ledSelect];
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "BTN_SEL:";
  s += ledSelect;
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void comSend(String sendData) {
  digitalWrite(RTS, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(RTS, LOW);
}

void pwmChangeSlider(lv_event_t *e) {
  Serial.println("PWM Change Slider...");

  // if (ledSelect == 1)
  //   return;

  logoCounter = 0;
  String sendData;

  s = lv_label_get_text(ui_LabelBright);
  // comSend(s);
  Serial.print("Slider... ");
  Serial.println(s);

  lv_timer_handler();

  switch (ledSelect) {
  case 0: // 취침모드...
    break;
  case 1: // LED 1
    ledPwm[1] = s.toInt();
    sendData = "LED1:";
    lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    break;
  case 2: // LED 2
    ledPwm[2] = s.toInt();
    sendData = "LED2:";
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    break;
  case 3: // LED 3
    ledPwm[3] = s.toInt();
    sendData = "LED3:";
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    break;
  case 4: // LED 4
    ledPwm[4] = s.toInt();
    sendData = "LED4:";
    lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    break;
  case 5: // LED 5
    ledPwm[5] = s.toInt();
    sendData = "LED5:";
    lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    break;
  default:
    break;
  }
  sendData += s.toInt();
  comSend(sendData);
}

void ledButton1(lv_event_t *e) {
  logoChangeCounter = 0;
  logoCounter = 0;
  ledSelect = 1;
  lvState = lv_obj_get_state(ui_Button1);

  Serial.println("ledButton 1...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (lvState == CHECKED) { // On 일때...
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    Serial.print("Button1 ledPwm[1] : ");
    Serial.println(ledPwm[ledSelect]);
  } else { // Off 일때...
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED1:";
  s += ledPwm[ledSelect];
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void ledButton2(lv_event_t *e) {
  logoChangeCounter = 0;
  logoCounter = 0;
  ledSelect = 2;
  lvState = lv_obj_get_state(ui_Button2);

  Serial.println("ledButton 2...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (lvState == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED2:";
  s += ledPwm[ledSelect];
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void ledButton3(lv_event_t *e) {
  logoChangeCounter = 0;
  logoCounter = 0;
  ledSelect = 3;
  lvState = lv_obj_get_state(ui_Button3);

  Serial.println("ledButton 3...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (lvState == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED3:";
  s += ledPwm[ledSelect];
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void ledButton4(lv_event_t *e) {
  logoChangeCounter = 0;
  logoCounter = 0;
  ledSelect = 4;
  lvState = lv_obj_get_state(ui_Button4);

  Serial.println("ledButton 4...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (lvState == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED4:";
  s += ledPwm[ledSelect];
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void ledButton5(lv_event_t *e) {
  logoChangeCounter = 0;
  logoCounter = 0;
  ledSelect = 5;
  lvState = lv_obj_get_state(ui_Button5);

  Serial.println("ledButton 5...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (lvState == CHECKED) { // On 일때...
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    Serial.print("Button 5 ledPwm[5] : ");
    Serial.println(ledPwm[ledSelect]);
  } else { // Off 일때...
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED5:";
  s += ledPwm[ledSelect];
  s += ":";
  s += lvState == CHECKED ? 1 : 0;
  comSend(s);
  delay(10);
  comSend(s);
}

void tvButton(lv_event_t *e) {
  logoChangeCounter = 0;
  logoCounter = 0;
  lvState = lv_obj_get_state(ui_Button6);

  Serial.println("tvButton...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "TV:";
  if (lvState == 3) {
    tvFlag = true;
    s += '1';
  } else {
    tvFlag = false;
    s += '0';
  }
  comSend(s);
  comSend(s);
  delay(100);
  comSend(s);
  comSend(s);
}

void brightQR(lv_event_t *e) {
  logoCounter = 0;
  uint8_t opa = lv_slider_get_value(ui_Slider2);
  Serial.print("brightQR... ");
  Serial.println(opa);
  lv_obj_set_style_opa(ui_ImageQR, opa, 0);
}

void serialEvent2() {
  // Serial.println("serialEvent2...");
  rs485ComCount = 0;
  lv_obj_clear_flag(ui_ImageNetwork,
                    LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
  comFlag = true;

  String eventString = Serial2.readStringUntil('\n');
  Serial2.flush();

  int index1 = eventString.indexOf(':');
  int index2 = eventString.indexOf(':', index1 + 1);
  int index3 = eventString.length();

  String _division = eventString.substring(0, index1);
  String _value = eventString.substring(index1 + 1, index2);
  String _flag = eventString.substring(index2 + 1, index3 - 1);

  Serial.println(eventString);
  Serial.print("Division : ");
  Serial.print(_division);
  Serial.print(", Value : ");
  Serial.print(_value);
  Serial.print(", Flag : ");
  Serial.println(_flag);
  Serial.println();

  if (_division == "LED1") {
    comInitFlag = true;

    ledSelect = 1;
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button1, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);

    if (_value.toInt() != 0 && _flag == "1") {
      ledFlag[1] = true;
      ledPwm[1] = _value.toInt();
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    } else if (_flag == "0") {
      ledFlag[1] = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    }
    s = _value.toInt();
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, _value.toInt(), LV_ANIM_OFF);
  } else if (_division == "LED2") {
    comInitFlag = true;

    ledSelect = 2;
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button2, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);

    if (_value.toInt() != 0 && _flag == "1") {
      ledFlag[2] = true;
      ledPwm[2] = _value.toInt();
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    } else if (_flag == "0") {
      ledFlag[2] = false;
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    }
    s = _value.toInt();
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, _value.toInt(), LV_ANIM_OFF);
  } else if (_division == "LED3") {
    comInitFlag = true;

    ledSelect = 3;
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button3, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);

    if (_value.toInt() != 0 && _flag == "1") {
      ledFlag[3] = true;
      ledPwm[3] = _value.toInt();
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    } else if (_flag == "0") {
      ledFlag[3] = false;
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    }
    s = _value.toInt();
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, _value.toInt(), LV_ANIM_OFF);
  } else if (_division == "LED4") {
    comInitFlag = true;

    ledSelect = 4;
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button4, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);

    if (_value.toInt() != 0 && _flag == "1") {
      ledFlag[4] = true;
      ledPwm[4] = _value.toInt();
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    } else if (_flag == "0") {
      ledFlag[4] = false;
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    }
    s = _value.toInt();
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, _value.toInt(), LV_ANIM_OFF);
  } else if (_division == "LED5") {
    comInitFlag = true;

    ledSelect = 5;
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button5, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);

    if (_value.toInt() != 0 && _flag == "1") {
      ledFlag[5] = true;
      ledPwm[5] = _value.toInt();
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    } else if (_flag == "0") {
      ledFlag[5] = false;
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    }
    s = _value.toInt();
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, _value.toInt(), LV_ANIM_OFF);
  } else if (_division == "TV") {
    comInitFlag = true;

    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button6, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);

    if (_value != "0") {
      tvFlag = true;
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    } else {
      tvFlag = false;
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    }
  } else if (_division == "NEO_1_FLAG") {
    comInitFlag = true;
    if (_value == "1") {
      neo1Flag = true;
      // lv_obj_clear_flag(ui_ContainerMood,
      //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_add_state(ui_SwitchMood, LV_STATE_CHECKED);
      lv_obj_clear_flag(ui_Container7,
                        LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_add_flag(ui_Image7, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    } else {
      neo1Flag = false;
      // lv_obj_add_flag(ui_ContainerMood,
      //                 LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_clear_state(ui_SwitchMood, LV_STATE_CHECKED);
      lv_obj_add_flag(ui_Container7,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_clear_flag(ui_Image7,
                        LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    }
  } else if (_division == "NEO_2_FLAG") {
    comInitFlag = true;
    if (_value == "1") {
      neo2Flag = true;
      // lv_obj_clear_flag(ui_ContainerSide,
      //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_add_state(ui_SwitchSide, LV_STATE_CHECKED);
      lv_obj_clear_flag(ui_Container11,
                        LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_add_flag(ui_Image8, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    } else {
      neo2Flag = false;
      // lv_obj_add_flag(ui_ContainerSide,
      //                 LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_clear_state(ui_SwitchSide, LV_STATE_CHECKED);
      lv_obj_add_flag(ui_Container11,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_clear_flag(ui_Image8,
                        LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    }
  } else if (_division == "NEO_1_R_COLOR") {
    comInitFlag = true;
    sliderRedColor[1] = byte(_value.toInt());
  } else if (_division == "NEO_1_G_COLOR") {
    comInitFlag = true;
    sliderGreenColor[1] = byte(_value.toInt());
  } else if (_division == "NEO_1_B_COLOR") {
    comInitFlag = true;
    sliderBlueColor[1] = byte(_value.toInt());
  } else if (_division == "NEO_2_R_COLOR") {
    comInitFlag = true;
    sliderRedColor[2] = byte(_value.toInt());
  } else if (_division == "NEO_2_G_COLOR") {
    comInitFlag = true;
    sliderGreenColor[2] = byte(_value.toInt());
  } else if (_division == "NEO_2_B_COLOR") {
    comInitFlag = true;
    sliderBlueColor[2] = byte(_value.toInt());
  } else if (_division == "NEO_1_MODE") {
    comInitFlag = true;
    neo1Mode = byte(_value.toInt());
  } else if (_division == "NEO_2_MODE") {
    comInitFlag = true;
    neo2Mode = byte(_value.toInt());
  } else if (_division == "DOOR_TYPE") {
    comInitFlag = true;
    if (_value != "0") {
      doorFlag = true;
    } else {
      doorFlag = false;
    }
  } else if (_division == "SWITCH") {
    comInitFlag = true;
    if (_value == "1") {
      lv_obj_clear_flag(ui_ImageSwitch, LV_OBJ_FLAG_HIDDEN);
      switchState = true;
    } else {
      lv_obj_add_flag(ui_ImageSwitch, LV_OBJ_FLAG_HIDDEN);
      switchState = false;
    }
  } else if (_division == "DOOR") {
    comInitFlag = true;
    if (_value == "1") {
      lv_obj_clear_flag(ui_ImageDoor, LV_OBJ_FLAG_HIDDEN);
      doorState = true;
    } else {
      lv_obj_add_flag(ui_ImageDoor, LV_OBJ_FLAG_HIDDEN);
      doorState = false;
    }
  } else if (_division == "BG_VALUE") {
    comInitFlag = true;
    bright = _value.toInt();
    ledcWrite(0, map(bright, 1, 100, 1, 255));
    s = bright;
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBGBright, label_text);
    lv_slider_set_value(ui_Slider3, bright, LV_ANIM_OFF);
    Serial.print("BG_VALUE : ");
    Serial.print(bright);
  } else if (_division == "BAT") {
    comInitFlag = true;
    if (_value == "1") {
      lv_obj_clear_flag(ui_ImageBattery, LV_OBJ_FLAG_HIDDEN);
      batState = true;
    } else {
      lv_obj_add_flag(ui_ImageBattery, LV_OBJ_FLAG_HIDDEN);
      batState = false;
    }
  } else if (_division == "TV_ERROR") {
    comInitFlag = true;
    if (_value == "1") {
      lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
    }
  } else if (_division == "BTN_SEL") {
    comInitFlag = true;
    buttonOutLine();
    ledSelect = _value.toInt();
    // switch (_value.toInt()) {
    switch (ledSelect) {
    case 1:
      // s = ledPwm[ledSelect];
      // s.toCharArray(label_text, s.length() + 1);
      // lv_label_set_text(ui_LabelBright, label_text);
      // lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
      // ledName[ledSelect].toCharArray(label_text,
      //                                ledName[ledSelect].length() + 1);
      // lv_label_set_text(ui_LabelLED, label_text);
      lv_obj_set_style_outline_opa(ui_Button1, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 2:
      s = ledPwm[ledSelect];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
      ledName[ledSelect].toCharArray(label_text,
                                     ledName[ledSelect].length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
      lv_obj_set_style_outline_opa(ui_Button2, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 3:
      s = ledPwm[ledSelect];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
      ledName[ledSelect].toCharArray(label_text,
                                     ledName[ledSelect].length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
      lv_obj_set_style_outline_opa(ui_Button3, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 4:
      s = ledPwm[ledSelect];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
      ledName[ledSelect].toCharArray(label_text,
                                     ledName[ledSelect].length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
      lv_obj_set_style_outline_opa(ui_Button4, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 5:
      lv_obj_set_style_outline_opa(ui_Button5, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 6:
      lv_obj_set_style_outline_opa(ui_Button6, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    default:
      break;
    }
  } else if (_division == "QR_OK") {
    lv_disp_load_scr(ui_ScreenHome);
  } else if (_division == "BAT_LOW") {
    lv_disp_load_scr(ui_ScreenHome);
    ledcWrite(0, map(bright, 1, 100, 1, 255));
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
    lv_timer_handler();
    while (1) ;
  } else if (_division == "SEND_OK") {
    comInitFlag = true;

    Serial.println("");
    Serial.print("neo1ModeFlag : ");
    Serial.print(neo1Flag);
    Serial.print(",  neo2ModeFlag : ");
    Serial.println(neo2Flag);

    Serial.print("neo1Start : ");
    Serial.print(neo1startNumber);
    Serial.print(",  neo2Start : ");
    Serial.println(neo2startNumber);

    Serial.print("neo1Total : ");
    Serial.print(neo1TotalNumber);
    Serial.print(",  neo2Total : ");
    Serial.println(neo2TotalNumber);

    Serial.print("neo1_r_color : ");
    Serial.print(sliderRedColor[1]);
    Serial.print(",  neo1_g_color : ");
    Serial.print(sliderGreenColor[1]);
    Serial.print(",  neo1_b_color : ");
    Serial.println(sliderBlueColor[1]);

    Serial.print("neo2_r_color : ");
    Serial.print(sliderRedColor[2]);
    Serial.print(",  neo2_g_color : ");
    Serial.print(sliderGreenColor[2]);
    Serial.print(",  neo2_b_color : ");
    Serial.println(sliderBlueColor[2]);

    Serial.print("neo1_mode : ");
    Serial.print(neo1Mode);
    Serial.print(",  neo2_mode : ");
    Serial.println(neo2Mode);

    Serial.print("doorType : ");
    Serial.println(doorFlag);
  }

  if (masterModeFlag == MASTER_MODE)
    comSend("RCV_OK:1");
  // server.handleClient();
}

void wifiStart() { // WiFi Mode Start...

  String msg;

  if (networkFlag == AP_MODE) {
    WiFi.mode(WIFI_MODE_AP);

    Serial.println(F("setupWifi()..."));
    Serial.println(F("Access Point(WiFi) Mode Start..."));

    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAP(ssidAp, passwordAp); // Access Point mode begin...
    WiFi.softAPConfig(local_ip, gateway, subnet);

    Serial.println();
    Serial.print("SSID: ");
    Serial.print(ssidAp);
    Serial.print(", PSWD: ");
    Serial.println(passwordAp);
    Serial.print(F("ESP32 IP as soft AP : "));
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println(F("setupWifi()..."));
    Serial.println(F("Hot Spot Mode Start..."));
    Serial.println();
    Serial.print("Hot Spot SSID: ");
    Serial.print(ssidHotSpot);
    Serial.print(", Hot Spot PSWD: ");
    Serial.println(pswdHotSpot);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    WiFi.begin(ssidHotSpot, pswdHotSpot);
    Serial.print("Connecting to WiFi ..");
    // lv_obj_clear_flag(ui_ImageHotSpot,
    //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // s = "핫스팟 연결중 입니다...";
    // s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelMessage, label_text);
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);

    for (int i = 1; i < 10; i++) {
      // s = i;
      // s.toCharArray(label_text, s.length() + 1);
      // lv_label_set_text(ui_LabelSecCount, label_text);
      if (WiFi.status() == WL_CONNECTED)
        break;
      lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      lv_timer_handler();
      delay(900);
      lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      lv_timer_handler();
      delay(100);

      Serial.print('.');
    }

    // s = " ";
    // s.toCharArray(label_text, s.length() + 1);
    // lv_label_set_text(ui_LabelSecCount, label_text);

    if (WiFi.status() == WL_CONNECTED) {
      hotSpotIP = WiFi.localIP().toString();
      hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
      lv_label_set_text(ui_LabelMessage, label_text);
      Serial.println();
      Serial.print("HotSpotIP : ");
      Serial.println(hotSpotIP);
    } else {
      s = "핫스팟 네트워크 정보 확인...";
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelMessage, label_text);
    }
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);

    Serial.println();
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS 1: ");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2: ");
    Serial.println(WiFi.dnsIP(1));
  }
}

void eventSec() {
  Serial.println("1 Sec Event...");

  if (countValue++ > 1) {

    if (wifiCount > 2) { // WiFi 가 연결 안되어 있으면...
      lv_obj_add_flag(ui_ImageWiFi,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      if (wifiCount > 100)
        wifiCount = 10;
    } else if (wifiCount == 0) { // WiFi 가 연결 되어 있으면...
      if (networkFlag == AP_MODE)
        lv_obj_clear_flag(ui_ImageWiFi,
                          LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    }
    Serial.print("WiFi Count : ");
    Serial.println(wifiCount);
    countValue = 0;
    wifiCount++;
  }
}

void ledNameInit() {
  Serial.print("EEPROM Length : ");
  Serial.println(EEPROM.readString(LED1_NAME_EEPROM).length());

  if (EEPROM.readString(LED1_NAME_EEPROM).length() < 20) {
    ledName[1] = EEPROM.readString(LED1_NAME_EEPROM);
    ledName[2] = EEPROM.readString(LED2_NAME_EEPROM);
    ledName[3] = EEPROM.readString(LED3_NAME_EEPROM);
    ledName[4] = EEPROM.readString(LED4_NAME_EEPROM);
    ledName[5] = EEPROM.readString(LED5_NAME_EEPROM);
  } else {
    ledName[1] = ledNameOrigin[1];
    ledName[2] = ledNameOrigin[2];
    ledName[3] = ledNameOrigin[3];
    ledName[4] = ledNameOrigin[4];
    ledName[5] = ledNameOrigin[5];
  }
  ledName[1].toCharArray(label_text, ledName[1].length() + 1);
  lv_label_set_text(ui_Label1, label_text);
  ledName[2].toCharArray(label_text, ledName[2].length() + 1);
  lv_label_set_text(ui_Label2, label_text);
  ledName[3].toCharArray(label_text, ledName[3].length() + 1);
  lv_label_set_text(ui_Label3, label_text);
  ledName[4].toCharArray(label_text, ledName[4].length() + 1);
  lv_label_set_text(ui_Label4, label_text);
  ledName[5].toCharArray(label_text, ledName[5].length() + 1);
  lv_label_set_text(ui_Label5, label_text);

  Serial.print("LED1 Name Length : ");
  Serial.println(ledNameOrigin[1].length() + 1);

  Serial.print("LED1 Name : ");
  Serial.println(ledName[1]);
  Serial.print("LED2 Name : ");
  Serial.println(ledName[2]);
  Serial.print("LED3 Name : ");
  Serial.println(ledName[3]);
  Serial.print("LED4 Name : ");
  Serial.println(ledName[4]);
  Serial.print("LED5 Name : ");
  Serial.println(ledName[5]);
}

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w,
                            h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  if (touch_has_signal()) {
    if (touch_touched()) {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;

      // Serial.print("Data x ");
      // Serial.println(touch_last_x);

      // Serial.print("Data y ");
      // Serial.println(touch_last_y);
    } else if (touch_released()) {
      data->state = LV_INDEV_STATE_REL;
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  pinMode(RTS, OUTPUT);

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial2.begin(115200, SERIAL_8N1, 12, 13);

  Serial.flush();
  Serial2.flush();

  digitalWrite(RTS, LOW);

  EEPROM.begin(250);

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  // Serial.print("comInitFlag : ");
  // Serial.println(comInitFlag);
  // comSend("REQUEST:1");

  // delay(50);
  // Serial.print("comInitFlag : ");
  // Serial.println(comInitFlag);
  // Serial.println("Main Board Com OK !!!");

  // EEPROM Read Process...
  ssidHotSpot = EEPROM.readString(SSID_HOTSPOT_EEPROM);
  pswdHotSpot = EEPROM.readString(PSWD_HOTSPOT_EEPROM);

  masterModeFlag = EEPROM.readBool(MASTER_SLAVE_EEPROM);

  // 카인드 Logo를 기본으로 함...
  logoChangeFlag = EEPROM.readBool(LOGO_CHANGE_EEPROM);

  // if (EEPROM.readByte(BG_VALUE_EEPROM) > 100) {
  //   EEPROM.writeByte(BG_VALUE_EEPROM, 100);
  //   EEPROM.commit();
  //   bright = 100;
  // }
  // bright = EEPROM.readByte(BG_VALUE_EEPROM);

  // ledNameInit();
  ledName[1] = ledNameOrigin[1];
  ledName[2] = ledNameOrigin[2];
  ledName[3] = ledNameOrigin[3];
  ledName[4] = ledNameOrigin[4];
  ledName[5] = ledNameOrigin[5];

  // Init Display
  lv_init();
  gfx->begin();
  // lv_init();
  delay(10);
  touch_init();

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 240, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, 3);
// ledcWrite(0, 255);
// digitalWrite(TFT_BL, HIGH);
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();
#ifdef ESP32
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(
      sizeof(lv_color_t) * screenWidth * screenHeight / 4,
      MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
  disp_draw_buf =
      (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 4);
#endif
  if (!disp_draw_buf) {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  } else {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL,
                          screenWidth * screenHeight / 4);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
  }

  // LV Initial 이전에 lv_ 명령 금지...
  ui_init();

  // ledcWrite(0, map(bright, 1, 100, 1, 255));
  // s = bright;
  // s.toCharArray(label_text, s.length() + 1);
  // lv_label_set_text(ui_LabelBGBright, label_text);
  // lv_slider_set_value(ui_Slider3, bright, LV_ANIM_OFF);

  if (networkFlag) // true : Tethering Mode, false : WiFi
  {                // 핫스팟 Mode...
    s = "핫스팟 네트워크:";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label23, label_text);
    ssidHotSpot.toCharArray(label_text, ssidHotSpot.length() + 1);
    lv_label_set_text(ui_LabelSSID, label_text);

    s = "핫스팟 비밀번호:";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label24, label_text);
    pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
    lv_label_set_text(ui_LabelPSWD, label_text);
    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
  } else { // WiFi Mode...

    s = "와이파이 비밀번호:";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label24, label_text);
  }

  if (masterModeFlag) // true : Master Mode, false : Slave Mode
  {
    lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
    // lv_obj_clear_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label7, LV_OBJ_FLAG_HIDDEN);

    if (networkFlag == AP_MODE) {
      lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    }
  } else {
    lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label7, LV_OBJ_FLAG_HIDDEN);
  }

  s = lv_label_get_text(ui_LabelSSID);
  s.toCharArray(ssidAp, s.length() + 1);

  s = lv_label_get_text(ui_LabelPSWD);
  s.toCharArray(passwordAp, s.length() + 1);

  // if (masterModeFlag == MASTER_MODE) {
  //   wifiStart();
  //   server.on("/json", handleJson);
  //   server.on("/root", handleRoot);
  //   server.on("/Request", handleRequest);
  //   server.begin();
  // }

  // clockRead();

  // Serial.print("BACK_COLOR_EEPROM : ");
  // Serial.println(EEPROM.readUInt(BACK_COLOR_EEPROM), HEX);

  Serial.println("Setup done");

  comSend("COM:0");
  delay(10);

  s = "AP_SSID:";
  s += ssidAp;
  comSend(s);
  delay(10);

  s = "AP_PSWD:";
  s += passwordAp;
  comSend(s);
  delay(100);

  Serial.print("AP_SSID:");
  Serial.println(ssidAp);
  Serial.print("AP_PSWD:");
  Serial.println(passwordAp);

  // ledcWrite(0, 5);
  // s = 5;
  ledcWrite(0, 255);
  s = 100;
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBGBright, label_text);
  // lv_slider_set_value(ui_Slider3, bright, LV_ANIM_OFF);
  lv_slider_set_value(ui_Slider3, 100, LV_ANIM_OFF);

  lv_obj_add_flag(ui_PanelMessage,
                  LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);

  // Serial2.flush();
  // comSend("REQ_NEO:1:1");
  // delay(50);
  // delay(100);
  // comSend("REQ_DATA:1:1");
  // for (int i = 0; i > 400; i++) {
  //   if (Serial2.available() > 0)
  //     serialEvent2();
  //   delay(10);
  // }
}

void loop() {

  if (!dataReceiveFlag) {
    delay(100);
    comSend("REQ_DATA:1:1");
    for (int i = 0; i > 400; i++) {
      if (Serial2.available() > 0)
        serialEvent2();
      delay(10);
    }
    dataReceiveFlag = true;
  } else {
    lv_timer_handler(); /* let the GUI do its work */

    if (loopCount > 150) { // 약 1초 마다 실행...

      comSend("REQ_FLAG:1:1");

      if ((networkFlag == STATION_MODE) && (WiFi.status() != WL_CONNECTED)) {
        hotSpotIpSetFlag = false;
        WiFi.begin(ssidHotSpot, pswdHotSpot);
        Serial.println("Reconnecting to WiFi...");
      } else if (!hotSpotIpSetFlag && (WiFi.status() == WL_CONNECTED)) {
        hotSpotIpSetFlag = true;
        hotSpotIP = WiFi.localIP().toString();
        hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
        lv_label_set_text(ui_LabelMessage, label_text);
        Serial.print("Loop HotSpotIP : ");
        Serial.println(hotSpotIP);
        lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
        lv_timer_handler();
        delay(2000);
        lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      }
      loopCount = 0;
    }

    if (rs485ComCount > 1000) { // LAN Icon 표시 여부...
      lv_obj_add_flag(ui_ImageNetwork,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      rs485ComCount = 10;
      comInitFlag = false;
      comFlag = false;
    }

    if (logoFlag == false) {
      if (logoCounter > 15000) {
        ledcWrite(0, 50);
        if (logoChangeFlag)
          lv_disp_load_scr(ui_ScreenLogo2);
        else
          lv_disp_load_scr(ui_ScreenLogo);
        logoFlag = true;
      }
    }

    delay(5);
    loopCount++;
    logoCounter++;
    rs485ComCount++;

    // Serial.print("rs485ComCount : ");
    // Serial.println(rs485ComCount);
    // Serial.print("logoCounter : ");
    // Serial.print(logoCounter);
    // Serial.print(", logoFlag : ");
    // Serial.println(logoFlag);
  }
}
