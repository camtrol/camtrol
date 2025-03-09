
/*******************************************************************************
 * 풀하우스 모터홈 프로그램...
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
//#include "lv_demo_widgets.h"
#include <Arduino.h>
#include "FS.h"
#include "SPI.h"
#include "SD.h"
#include <lvgl.h>
#include "ui.h"
#include <Wire.h>
// #include <WiFi.h>
// #include <WebServer.h>
#include <Arduino_GFX_Library.h>
// #include <Adafruit_AHTX0.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP_IOExpander_Library.h>
#include "waveshare_pcf85063a.h"

/*******************************************************************************
 ******************************************************************************/
#define I2C_MASTER_NUM 0

// Extend(CH422) IO Pin define
#define TP_RST 1 // Touch Reset
#define LCD_RST 3

#define TFT_BL 13

#define ON_STATE 3
#define OFF_STATE 2

#define ON 1
#define OFF 0

#define CHECKED 3
#define UNCHECKED 2

#define MASTER_MODE true
#define SLAVE_MODE false

// #define STATION_MODE true
// #define AP_MODE false

// EEPROM Address...
// #define SLEEP_COUNT_EEPROM 0    // Byte
// #define SCR_LOCK_COUNT_EEPROM 1 // Byte
#define MASTER_SLAVE_EEPROM 2 // bool
// #define NETWORK_EEPROM 3      // bool
#define LED1_NAME_EEPROM 10 // Unsigned Long
#define LED2_NAME_EEPROM 20 // Unsigned Long
#define LED3_NAME_EEPROM 30 // Unsigned Long
#define LED4_NAME_EEPROM 40 // Unsigned Long
#define LED5_NAME_EEPROM 50 // Unsigned Long
#define LED6_NAME_EEPROM 60 // Unsigned Long

void comSend(String sendData);
void PanelButton_Animation(lv_obj_t *TargetObject, int delay);
void PanelDisplay_Animation(lv_obj_t *TargetObject, int delay);

RTC_DS1307 rtc;
// Adafruit_AHTX0 myAHT10;

// sensors_event_t adsHumidity, adsTemp;

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, hotSpotIP;
String ledNameOrigin[7] = {" ",      "실내등", "간접등", "주방등",
                           "벙커등", "욕실등", "어닝등"};
String ledName[7] = {};

String outNameOrigin[7] = {" ",        "현관등", "물펌프", "맥스팬",
                           "전기실팬", "보일러", "냉장고"};
String outName[7] = {};

// WebServer server(80);

bool screenHomeFlag = true;

bool ledFlag[9] = {false, false, false, false, false,
                   false, false, false, false};
bool outFlag[7] = {false, false, false, false, false, false, false};
bool ledAllFlag = false;

bool clnValveFlag = false;
bool wstValveFlag = false;
bool heaterFlag = false;
bool livingFlag = false;
bool motor2Flag = false;
bool boilerFlag = false;
bool invertorFlag = false;

bool sleepFlag = false;

bool masterModeFlag;

bool ahtFlag = false;
bool rtcFlag = false;

// byte heaterFanValue, coolerFanValue;

char ssidAp[30] = "풀하우스 V6.0-SB-020";
char passwordAp[30] = "11240426020";

char msg[100];
char label_text[50];
char daysOfTheWeek[7][10] = {"일요일", "월요일", "화요일", "수요일",
                             "목요일", "금요일", "토요일"};

String strYear, strMonth, strDay, strTime, strMinute, strSec;

uint8_t displayCount = 0;
uint16_t loopCount = 0;
uint16_t heatTempValue = 100;
uint16_t coolTempValue = 100;
float setTemp = 25.0;
float nowTemp;

// uint8_t sliderRedColor, sliderGreenColor, sliderBlueColor = 0;
uint16_t state;

uint16_t rs485Count = 0;
uint8_t ledPwm[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
uint8_t btnNameClickCount = 0;
int countValue = 0;
int ledSelect = 9;
int bright = 0;
int heatValue = 0;
int livingValue = 0;

float temperature, humidity;
// float floatHeatTemp, floatCoolTemp;

TwoWire I2Ctwo = TwoWire(1);

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

// WaveShare 7" LCD 800x480   // GFX_NOT_DEFINED
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */,
    GFX_NOT_DEFINED /* SDA */, 5 /* DE */, 3 /* VSYNC */, 46 /* HSYNC */,
    7 /* PCLK */, 1 /* R3 */, 2 /* R4 */, 42 /* R5 */, 41 /* R6 */, 40 /* R7 */,
    39 /* G2 */, 0 /* G3 */, 45 /* G4 */, 48 /* G5 */, 47 /* G6 */, 21 /* G7 */,
    14 /* B3 */, 38 /* B4 */, 18 /* B5 */, 17 /* B6 */, 10 /* B7 */
    );

// WaveShare 7" LCD 800x480   // GFX_NOT_DEFINED
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus, 800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */,
    4 /* hsync_pulse_width */, 8 /* hsync_back_porch */, 480 /* height */,
    0 /* vsync_polarity */, 8 /* vsync_front_porch */,
    4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */,
    true /* auto_flush */);

#endif /* !defined(DISPLAY_DEV_KIT) */

#include "touch.h"

void clockRead() {
  if (rtcFlag) {
    DateTime now = rtc.now();

    s = now.year() - 2000;
    s += "년 ";

    s += now.month();
    s += "월 ";

    s += now.day();
    s += "일  ";

    s += daysOfTheWeek[now.dayOfTheWeek()];

    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelDay, label_text);

    if (!now.isPM())
      s = "Am";
    else
      s = "Pm";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelAmPm, label_text);

    s = now.hour() % 12;
    if (s == "0")
      s = "12";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelTime, label_text);

    s = now.minute();
    if (s.length() == 1) {
      s = '0';
      s += now.minute();
    }
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelMin, label_text);
  }
}

void nowTimeSave(lv_event_t *e) {
  Serial.println("Now Time Save...");

  uint8_t year, month, day, hour, min, sec = 0;
  DateTime now = rtc.now();
  char buf[10];

  lv_roller_get_selected_str(ui_RollerYear, buf, sizeof(buf));
  s = buf;
  year = s.toInt() - 2000;
  lv_roller_get_selected_str(ui_RollerMonth, buf, sizeof(buf));
  s = buf;
  month = s.toInt();
  lv_roller_get_selected_str(ui_RollerDay, buf, sizeof(buf));
  s = buf;
  day = s.toInt();
  lv_roller_get_selected_str(ui_RollerHour, buf, sizeof(buf));
  s = buf;
  hour = s.toInt();
  lv_roller_get_selected_str(ui_RollerMin, buf, sizeof(buf));
  s = buf;
  min = s.toInt();

  rtc.adjust(DateTime(year + 2000, month, day, hour, min, sec));
  Serial.print("Year : ");
  Serial.println(year);
}

void timeChanged(lv_event_t *e) {
  Serial.println("Time Changed...");
  // String timeString;
  char buf[10];
  lv_roller_get_selected_str(ui_RollerYear, buf, sizeof(buf));
  s = buf;
  s += "년 ";
  lv_roller_get_selected_str(ui_RollerMonth, buf, sizeof(buf));
  s += buf;
  s += "월 ";
  lv_roller_get_selected_str(ui_RollerDay, buf, sizeof(buf));
  s += buf;
  s += "일    ";
  lv_roller_get_selected_str(ui_RollerHour, buf, sizeof(buf));
  s += buf;
  s += "시 ";
  lv_roller_get_selected_str(ui_RollerMin, buf, sizeof(buf));
  s += buf;
  s += "분";

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelNowTime, label_text);
}

void timeClick(lv_event_t *e) {
  Serial.println("Time Clicked...");
  ledcWrite(0, 0);
  if (rtcFlag) {
    DateTime now = rtc.now();

    lv_roller_set_selected(ui_RollerYear, now.year() - 2024, LV_ANIM_ON);
    lv_roller_set_selected(ui_RollerMonth, now.month() - 1, LV_ANIM_ON);
    lv_roller_set_selected(ui_RollerDay, now.day() - 1, LV_ANIM_ON);
    lv_roller_set_selected(ui_RollerHour, now.hour(), LV_ANIM_ON);
    lv_roller_set_selected(ui_RollerMin, now.minute(), LV_ANIM_ON);

    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelMin, label_text);
  }
  timeChanged(e);
  ledcWrite(0, bright);
}

void nameInitClick(lv_event_t *e) {
  Serial.println("Button Name Change Click...");
  btnNameClickCount++;
  if (btnNameClickCount > 10) {
    lv_obj_clear_flag(ui_LabelBtnInit, LV_OBJ_FLAG_HIDDEN);
    for (int i = 1; i < 10; i++) {
      ledName[i] = ledNameOrigin[i];
    }
    btnNameClickCount = 0;
    // EEPROM.writeString(LED1_NAME_EEPROM, ledNameOrigin[1]);
    // EEPROM.writeString(LED2_NAME_EEPROM, ledNameOrigin[2]);
    // EEPROM.writeString(LED3_NAME_EEPROM, ledNameOrigin[3]);
    // EEPROM.writeString(LED4_NAME_EEPROM, ledNameOrigin[4]);
    // EEPROM.writeString(LED5_NAME_EEPROM, ledNameOrigin[5]);
    // EEPROM.writeString(LED6_NAME_EEPROM, ledNameOrigin[6]);
    // EEPROM.writeString(LED7_NAME_EEPROM, ledNameOrigin[7]);
    // EEPROM.writeString(LED8_NAME_EEPROM, ledNameOrigin[8]);
    // EEPROM.commit();
    // ledName[1].toCharArray(label_text, ledName[1].length() + 1);
    // lv_label_set_text(ui_Label1, label_text);
    // ledName[2].toCharArray(label_text, ledName[2].length() + 1);
    // lv_label_set_text(ui_Label2, label_text);
    // ledName[3].toCharArray(label_text, ledName[3].length() + 1);
    // lv_label_set_text(ui_Label3, label_text);
    // ledName[4].toCharArray(label_text, ledName[4].length() + 1);
    // lv_label_set_text(ui_Label4, label_text);
    // ledName[5].toCharArray(label_text, ledName[5].length() + 1);
    // lv_label_set_text(ui_Label5, label_text);
    // ledName[6].toCharArray(label_text, ledName[6].length() + 1);
    // lv_label_set_text(ui_Label6, label_text);
    // ledName[7].toCharArray(label_text, ledName[7].length() + 1);
    // lv_label_set_text(ui_Label7, label_text);
    // ledName[8].toCharArray(label_text, ledName[8].length() + 1);
    // lv_label_set_text(ui_Label8, label_text);
  }
}

void comSend(String sendData) {
  // lv_timer_handler();
  // digitalWrite(RTS, HIGH);
  // delay(5);
  // s = command;
  // s += ':';
  // s += value;
  if (masterModeFlag == SLAVE_MODE)
    return;

  Serial2.println(sendData);
  Serial2.flush();

  // digitalWrite(RTS, LOW);
}

// void tempRead() {
//   myAHT10.getEvent(
//       &adsHumidity,
//       &adsTemp); // populate temp and humidity objects with fresh data

//   Serial.print(" 온도 : ");
//   temperature = round(adsTemp.temperature * 10) / 10;
//   // Serial.print(adsTemp.temperature);
//   Serial.print(temperature);
//   Serial.println(" °C");
//   Serial.print(" 습도 : ");
//   humidity = round(adsHumidity.relative_humidity * 10) / 10;
//   // Serial.print(adsHumidity.relative_humidity);
//   Serial.print(humidity);
//   Serial.println(" %");

//   s = String(temperature, 1);
//   s += " °C";
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelTemp, label_text);

//   s = String(humidity, 1);
//   s += " %";
//   s.toCharArray(label_text, s.length() + 1);
//   lv_label_set_text(ui_LabelHumi, label_text);
// }

void eventSec() {
  Serial.println("1 Sec Event...");

  if (countValue++ > 1) {

    if (rtcFlag) {
      DateTime now = rtc.now();

      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(" (");
      Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
      Serial.print(") ");
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.println();
    }

    clockRead();

    // if (wifiCount > 2) { // WiFi 가 연결 안되어 있으면...
    //   if (wifiCount > 100)
    //     wifiCount = 10;
    // } else if (wifiCount == 0) { // WiFi 가 연결 되어 있으면...
    // }
    // countValue = 0;
    // wifiCount++;
  }
}

void buttonOutLine() {
  lv_obj_set_style_outline_opa(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_opa(ui_outButton1, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_outButton2, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_outButton3, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_outButton4, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_outButton6, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_opa(ui_clnButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_wstButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_invButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_heaterButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_livingButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_bunkerButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_boilerButton, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
}

void wifiClick(lv_event_t *e) { Serial.println("WiFi Click..."); }

void ScreenHomeLoaded(lv_event_t *e) {
  Serial.println("ScreenHomeLoaded...");
  displayCount = 0;
  screenHomeFlag = true;
  EEPROM.commit();
}

void screenAirHeaterLoaded(lv_event_t *e) {
  Serial.println("Screen Air Heater Loaded...");
  displayCount = 0;
  screenHomeFlag = false;
  comSend("AIR_HEATER:1");
}

void screenQRLoaded(lv_event_t *e) {
  Serial.println("Screen QR Loaded...");
  displayCount = 0;
  screenHomeFlag = false;
}

void sleepButton(lv_event_t *e) {
  Serial.println("Sleep Button Long...");

  if (sleepFlag) {
    sleepFlag = false;
    lv_obj_clear_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Image23, LV_OBJ_FLAG_HIDDEN);
  } else { // Sleep Mode 진입...
    sleepFlag = true;
    ledcWrite(0, 10);
    lv_obj_add_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Image23, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_outButton1, LV_STATE_CHECKED);
  }

  Serial.print("Sleep Mode : ");
  Serial.println(sleepFlag);
}

void led1Long(lv_event_t *e) {
  Serial.println("ledButton 1 Long...");

  ledSelect = 1;
  // if (!ledFlag[1])
  //   return;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (ledFlag[ledSelect])
  s = ledPwm[ledSelect];
  // else {
  //   ledPwm[ledSelect] = 0;
  //   s = '0';
  // }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led2Long(lv_event_t *e) {
  Serial.println("ledButton 2 Long...");

  ledSelect = 2;
  // if (!ledFlag[2])
  //   return;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  // if (ledFlag[ledSelect])
  s = ledPwm[ledSelect];
  // else {
  //   ledPwm[ledSelect] = 0;
  //   s = '0';
  // }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led3Long(lv_event_t *e) {
  Serial.println("ledButton 3 Long...");

  ledSelect = 3;
  // if (!ledFlag[3])
  //   return;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (ledFlag[ledSelect])
  s = ledPwm[ledSelect];
  // else {
  //   ledPwm[ledSelect] = 0;
  //   s = '0';
  // }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led4Long(lv_event_t *e) {
  Serial.println("ledButton 4 Long...");

  ledSelect = 4;
  // if (!ledFlag[4])
  //   return;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (ledFlag[ledSelect])
  s = ledPwm[ledSelect];
  // else {
  //   ledPwm[ledSelect] = 0;
  //   s = '0';
  // }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led5Long(lv_event_t *e) {
  Serial.println("ledButton 5 Long...");

  ledSelect = 5;
  // if (!ledFlag[5])
  //   return;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (ledFlag[ledSelect])
  s = ledPwm[ledSelect];
  // else {
  //   ledPwm[ledSelect] = 0;
  //   s = '0';
  // }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led6Long(lv_event_t *e) {
  Serial.println("ledButton 6 Long...");

  ledSelect = 6;
  // if (!ledFlag[6])
  //   return;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // if (ledFlag[ledSelect])
  s = ledPwm[ledSelect];
  // else {
  //   ledPwm[ledSelect] = 0;
  //   s = '0';
  // }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led7Long(lv_event_t *e) {
  Serial.println("ledButton 7 Long...");

  ledSelect = 7;

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (ledFlag[ledSelect])
    s = ledPwm[ledSelect];
  else {
    ledPwm[ledSelect] = 0;
    s = '0';
  }

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  s = "전체등";
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void ledAllProcess() {
  if (!ledFlag[1] && !ledFlag[2] && !ledFlag[3] && !ledFlag[4] && !ledFlag[5] &&
      !ledFlag[6]) {
    ledAllFlag = false;
    lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
  } else if (ledFlag[1] && ledFlag[2] && ledFlag[3] && ledFlag[4] &&
             ledFlag[5] && ledFlag[6]) {
    ledAllFlag = true;
    lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
  }
}

void ledButton1(lv_event_t *e) {
  ledSelect = 1;
  state = lv_obj_get_state(ui_Button1);

  Serial.println("ledButton1...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) { // On 일때...
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    Serial.print("Button1 ledPwm[1] : ");
    Serial.println(ledPwm[ledSelect]);
  } else { // Off 일때...
    ledFlag[ledSelect] = false;
    // lv_label_set_text(ui_LabelBright, "0");
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED1:";
  s += state == CHECKED ? ledPwm[1] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton2(lv_event_t *e) {
  ledSelect = 2;
  state = lv_obj_get_state(ui_Button2);

  Serial.println("ledButton2...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    // lv_label_set_text(ui_LabelBright, "0");
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED2:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton3(lv_event_t *e) {
  ledSelect = 3;
  state = lv_obj_get_state(ui_Button3);

  Serial.println("ledButton3...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    // lv_label_set_text(ui_LabelBright, "0");
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED3:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton4(lv_event_t *e) {
  ledSelect = 4;
  state = lv_obj_get_state(ui_Button4);

  Serial.println("ledButton4...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    // lv_label_set_text(ui_LabelBright, "0");
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED4:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton5(lv_event_t *e) {
  ledSelect = 5;
  state = lv_obj_get_state(ui_Button5);

  Serial.println("ledButton5...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    // lv_label_set_text(ui_LabelBright, "0");
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED5:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton6(lv_event_t *e) {
  ledSelect = 6;
  state = lv_obj_get_state(ui_Button6);

  Serial.println("ledButton6...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    // lv_label_set_text(ui_LabelBright, "0");
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED6:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

// void ledButton7(lv_event_t *e) {
//   ledSelect = 7;
//   state = lv_obj_get_state(ui_Button7);

//   Serial.println("ledButton7...");

//   buttonOutLine();
//   lv_obj_set_style_outline_opa(ui_Button7, 255,
//                                LV_PART_MAIN | LV_STATE_DEFAULT);

//   if (state == CHECKED) {
//     ledFlag[ledSelect] = true;
//     s = ledPwm[ledSelect];
//     // s.toCharArray(label_text, sizeof(label_text));
//     s.toCharArray(label_text, s.length() + 1);
//     lv_label_set_text(ui_LabelBright, label_text);
//     lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
//     // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
//     ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() +
//     1);
//     lv_label_set_text(ui_LabelLED, label_text);
//   } else {
//     ledFlag[ledSelect] = false;
//     lv_label_set_text(ui_LabelBright, "0");
//     lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
//     // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
//     ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() +
//     1);
//     lv_label_set_text(ui_LabelLED, label_text);
//   }
//   s = "LED7:";
//   s += state == CHECKED ? ledPwm[ledSelect] : 0;
//   comSend(s);
//   ledAllProcess();
// }

// void ledButton8(lv_event_t *e) {
//   ledSelect = 8;
//   state = lv_obj_get_state(ui_Button8);

//   Serial.println("ledButton8...");

//   buttonOutLine();
//   lv_obj_set_style_outline_opa(ui_Button8, 255,
//                                LV_PART_MAIN | LV_STATE_DEFAULT);

//   if (state == CHECKED) {
//     ledFlag[ledSelect] = true;
//     s = ledPwm[ledSelect];
//     // s.toCharArray(label_text, sizeof(label_text));
//     s.toCharArray(label_text, s.length() + 1);
//     lv_label_set_text(ui_LabelBright, label_text);
//     lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
//     // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
//     ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() +
//     1);
//     lv_label_set_text(ui_LabelLED, label_text);
//   } else {
//     ledFlag[ledSelect] = false;
//     lv_label_set_text(ui_LabelBright, "0");
//     lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
//     // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
//     ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() +
//     1);
//     lv_label_set_text(ui_LabelLED, label_text);
//   }
//   s = "LED8:";
//   s += state == CHECKED ? ledPwm[ledSelect] : 0;
//   comSend(s);
//   ledAllProcess();
// }

void ledPwmSlide(lv_event_t *e) {
  Serial.println("LED PWM Slide...");

  // lv_timer_handler();
  String slide = lv_label_get_text(ui_LabelBright);

  ledFlag[ledSelect] = true;

  switch (ledSelect) {
  case 0: // 취침모드...
    break;
  case 1: // LED 1
    ledPwm[1] = slide.toInt();
    s = "LED1:";
    lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    // comSend("LED1", s.toInt());
    break;
  case 2: // LED 2
    ledPwm[2] = slide.toInt();
    s = "LED2:";
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    // comSend("LED2", s.toInt());
    break;
  case 3: // LED 3
    ledPwm[3] = slide.toInt();
    s = "LED3:";
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    // comSend("LED3", s.toInt());
    break;
  case 4: // LED 4
    ledPwm[4] = slide.toInt();
    s = "LED4:";
    lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    // comSend("LED4", s.toInt());
    break;
  case 5: // LED 5
    ledPwm[5] = slide.toInt();
    s = "LED5:";
    lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    // comSend("LED5", s.toInt());
    break;
  case 6: // LED 6
    ledPwm[6] = slide.toInt();
    s = "LED6:";
    lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    // comSend("LED6", s.toInt());
    break;

  case 7: // LED All
    ledPwm[1] = slide.toInt();
    ledPwm[2] = slide.toInt();
    ledPwm[3] = slide.toInt();
    ledPwm[4] = slide.toInt();
    ledPwm[5] = slide.toInt();
    // ledPwm[6] = slide.toInt();
    ledPwm[7] = slide.toInt();
    ledFlag[1] = true;
    ledFlag[2] = true;
    ledFlag[3] = true;
    ledFlag[4] = true;
    ledFlag[5] = true;
    // ledFlag[6] = true;
    ledFlag[7] = true;

    lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    // lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);

    s = "LED_ALL:";
    break;
  default:
    break;
  }

  s += slide.toInt();
  comSend(s);
  Serial.print("s : ");
  Serial.println(s);
  delay(70);
}

void ledButtonAll(lv_event_t *e) {
  ledSelect = 7;
  state = lv_obj_get_state(ui_Button7);

  Serial.println("ledButton9...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  // if (ledAllFlag)
  //   ledAllFlag = false;
  // else
  //   ledAllFlag = true;

  if (state != CHECKED) {
    // if (!ledAllFlag) {
    ledFlag[1] = false;
    ledFlag[2] = false;
    ledFlag[3] = false;
    ledFlag[4] = false;
    ledFlag[5] = false;
    // ledFlag[6] = false;
    ledFlag[7] = false;

    // ledPwm[1] = 0;
    // ledPwm[2] = 0;
    // ledPwm[3] = 0;
    // ledPwm[4] = 0;
    // ledPwm[5] = 0;
    // ledPwm[6] = 0;
    // ledPwm[7] = 0;

    ledAllFlag = false;

    lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    // lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);

    // s = '0';
    // s.toCharArray(label_text, s.length() + 1);
    // lv_label_set_text(ui_LabelBright, label_text);
    // lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
  } else { // 값이 1 이상이면...
    ledFlag[1] = true;
    ledFlag[2] = true;
    ledFlag[3] = true;
    ledFlag[4] = true;
    ledFlag[5] = true;
    // ledFlag[6] = true;
    ledFlag[7] = true;

    ledPwm[1] = 100;
    ledPwm[2] = 100;
    ledPwm[3] = 100;
    ledPwm[4] = 100;
    ledPwm[5] = 100;
    // ledPwm[6] = 100;
    ledPwm[7] = 100;

    ledAllFlag = true;
    lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    // lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    // lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    // lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    // lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
    s = 100;
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, 100, LV_ANIM_OFF);
  }
  s = "전체등";
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "LED_ALL:";
  s += ledAllFlag == true ? ledPwm[ledSelect] : 0;
  comSend(s);
  delay(5);
}

void outButton1(lv_event_t *e) {
  Serial.println("OUT Button 1...");
  state = lv_obj_get_state(ui_outButton1);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_outButton1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "OUT1:";
  if (state == 3) {
    outFlag[1] = true;
    s += '1';
  } else {
    outFlag[1] = false;
    s += '0';
  }
  comSend(s);
}
void outButton2(lv_event_t *e) {
  Serial.println("OUT Button 2...");
  state = lv_obj_get_state(ui_outButton2);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_outButton2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "OUT2:";
  if (state == 3) {
    outFlag[2] = true;
    s += '1';
  } else {
    outFlag[2] = false;
    s += '0';
  }
  comSend(s);
}

void outButton3(lv_event_t *e) {
  Serial.println("OUT Button 3...");
  state = lv_obj_get_state(ui_outButton3);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_outButton3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "OUT3:";
  if (state == 3) {
    outFlag[3] = true;
    s += '1';
  } else {
    outFlag[3] = false;
    s += '0';
  }
  comSend(s);
}

void outButton4(lv_event_t *e) {
  Serial.println("OUT Button 4...");
  state = lv_obj_get_state(ui_outButton4);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_outButton4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "OUT4:";
  if (state == 3) {
    outFlag[4] = true;
    s += '1';
  } else {
    outFlag[4] = false;
    s += '0';
  }
  comSend(s);
}

// void outButton5(lv_event_t *e) { Serial.println("Out Button 5 Click..."); }

void outButton6(lv_event_t *e) {
  Serial.println("OUT Button 6...");
  state = lv_obj_get_state(ui_outButton6);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_outButton6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "OUT6:";
  if (state == 3) {
    outFlag[6] = true;
    s += '1';
  } else {
    outFlag[6] = false;
    s += '0';
  }
  comSend(s);
}

void cleanDrainButton(lv_event_t *e) {
  Serial.println("Clean Drain Button Click...");
  state = lv_obj_get_state(ui_clnButton);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_clnButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "clnValve:";
  if (state == 3) {
    clnValveFlag = true;
    s += '1';
  } else {
    clnValveFlag = false;
    s += '0';
  }
  comSend(s);
}

void wasteDrainButton(lv_event_t *e) {
  Serial.println("Waste Drain Button Click...");
  state = lv_obj_get_state(ui_wstButton);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_wstButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "wstValve:";
  if (state == 3) {
    wstValveFlag = true;
    s += '1';
  } else {
    wstValveFlag = false;
    s += '0';
  }
  comSend(s);
}

void tempDownButton(lv_event_t *e) {
  Serial.println("Temp down Click...");
  setTemp = setTemp - 0.5;
  if (setTemp < 18.0)
    setTemp = 18.0;
  // s = setTemp;
  s = String(setTemp, 1);
  s += " °C";
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelSetTemp, label_text);
  Serial.print("setTemp : ");
  Serial.println(s);
  s = "SET_TEMP:";
  s += setTemp * 10;
  comSend(s);
}

void tempUpButton(lv_event_t *e) {
  Serial.println("Temp Up Button Click...");
  setTemp = setTemp + 0.5;
  if (setTemp > 50.0)
    setTemp = 50.0;
  // s = setTemp;
  s = String(setTemp, 1);
  s += " °C";
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelSetTemp, label_text);
  Serial.print("setTemp : ");
  Serial.println(s);
  s = "SET_TEMP:";
  s += setTemp * 10;
  comSend(s);
}

void heaterSlide(lv_event_t *e) {
  Serial.println("Heater Slider...");

  heatValue = lv_slider_get_value(ui_SliderHeater);

  if (!heaterFlag)
    lv_obj_add_state(ui_heaterButton, LV_STATE_CHECKED);

  s = "HEATER:";
  s += heatValue;
  comSend(s);

  delay(50);
}

void livingSlide(lv_event_t *e) {
  Serial.println("Living Slider...");

  livingValue = lv_slider_get_value(ui_SliderLiving);

  if (!livingFlag)
    lv_obj_add_state(ui_livingButton, LV_STATE_CHECKED);

  s = "LIVING:";
  s += livingValue;
  comSend(s);

  delay(50);
}

void heaterButton(lv_event_t *e) {
  Serial.println("Heater Button Click...");
  state = lv_obj_get_state(ui_heaterButton);

  int heatValue = lv_slider_get_value(ui_SliderHeater);

  buttonOutLine();

  lv_obj_set_style_outline_opa(ui_heaterButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "HEATER:";
  if (state == CHECKED) {
    heaterFlag = true;
    heatValue = lv_slider_get_value(ui_SliderHeater);
    s += heatValue;
  } else {
    heaterFlag = false;
    s += heaterFlag;
  }
  comSend(s);
}

void livingButton(lv_event_t *e) {
  Serial.println("Living Button Click...");
  state = lv_obj_get_state(ui_livingButton);

  int livingValue = lv_slider_get_value(ui_SliderHeater);

  buttonOutLine();

  lv_obj_set_style_outline_opa(ui_livingButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "LIVING:";
  if (state == CHECKED) {
    livingFlag = true;
    livingValue = lv_slider_get_value(ui_SliderLiving);
    s += livingValue;
  } else {
    livingFlag = false;
    s += livingFlag;
  }
  comSend(s);
}

void bunkerButton(lv_event_t *e) {
  Serial.println("Bunker Button Click...");
  state = lv_obj_get_state(ui_bunkerButton);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_bunkerButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "BUNKER:";
  if (state == 3) {
    motor2Flag = true;
    s += "100";
  } else {
    motor2Flag = false;
    s += '0';
  }
  comSend(s);
}

void boilerButton(lv_event_t *e) {
  state = lv_obj_get_state(ui_boilerButton);

  Serial.println("Boiler Button...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_boilerButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "BOILER:";
  if (state == 3) {
    boilerFlag = true;
    s += '1';
  } else {
    boilerFlag = false;
    s += '0';
  }
  comSend(s);
}

void invButton(lv_event_t *e) {
  state = lv_obj_get_state(ui_invButton);

  Serial.println("invButton...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_invButton, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "INVERTOR:";
  if (state == 3) {
    invertorFlag = true;
    s += '1';
  } else {
    invertorFlag = false;
    s += '0';
  }
  comSend(s);
}

void brightSlide(lv_event_t *e) { // 화면 밝기 조절...
  Serial.println("Bright Slide...");
  bright = lv_slider_get_value(ui_Slider3);

  if (bright < 10)
    bright = 10;

  if (sleepFlag) {
    sleepFlag = false;
    lv_obj_clear_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Image23, LV_OBJ_FLAG_HIDDEN);
  }

  ledcWrite(0, bright);

  Serial.println("Bright Slider... ");
  Serial.println(bright);
}

void brightQR(lv_event_t *e) {
  uint8_t opa = lv_slider_get_value(ui_Slider2);
  displayCount = 0;
  Serial.print("brightQR... ");
  Serial.println(opa);
  lv_obj_set_style_opa(ui_ImageQR, opa, 0);
}

void masterModeSelect(lv_event_t *e) {

  Serial.println("MasterModeSelect... ");
  displayCount = 0;

  if (lv_obj_get_state(ui_Switch1) == CHECKED) {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, MASTER_MODE);
    masterModeFlag = MASTER_MODE;
    Serial.println("Master Mode Selected...");

    lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);

  } else {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, SLAVE_MODE);
    masterModeFlag = SLAVE_MODE;
    Serial.println("Slave Mode Selected...");

    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
  }
}

void keyboardClick(lv_event_t *e) { Serial.println("Keyboard Click..."); }

void chgScreen2(lv_event_t *e) { Serial.println("chgScreen2..."); }

void serialEvent2() {
  rs485Count = 0;

  Serial.println("serialEvent2...");

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
    buttonOutLine();
    if (_flag != "0") {
      ledFlag[1] = true;
      ledPwm[1] = _value.toInt();
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    } else {
      ledFlag[1] = false;
      // ledPwm[1] = 0;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    }
    s = ledPwm[1];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[1], LV_ANIM_OFF);
    s = ledName[1];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button1, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LED2") {
    buttonOutLine();
    if (_flag != "0") {
      ledFlag[2] = true;
      ledPwm[2] = _value.toInt();
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    } else {
      ledFlag[2] = false;
      // ledPwm[2] = 0;
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    }
    s = ledPwm[2];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[2], LV_ANIM_OFF);
    s = ledPwm[2];
    s.toCharArray(label_text, s.length() + 1);
    s = ledName[2];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button2, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LED3") {
    buttonOutLine();
    if (_flag != "0") {
      ledFlag[3] = true;
      ledPwm[3] = _value.toInt();
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    } else {
      ledFlag[3] = false;
      // ledPwm[3] = 0;
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    }
    s = ledPwm[3];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[3], LV_ANIM_OFF);
    s = ledPwm[3];
    s.toCharArray(label_text, s.length() + 1);
    s = ledName[3];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button3, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LED4") {
    buttonOutLine();
    if (_flag != "0") {
      ledFlag[4] = true;
      ledPwm[4] = _value.toInt();
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    } else {
      ledFlag[4] = false;
      // ledPwm[4] = 0;
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    }
    s = ledPwm[4];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[4], LV_ANIM_OFF);
    s = ledPwm[4];
    s.toCharArray(label_text, s.length() + 1);
    s = ledName[4];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button4, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LED5") {
    buttonOutLine();
    if (_flag != "0") {
      ledFlag[5] = true;
      ledPwm[5] = _value.toInt();
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    } else {
      ledFlag[5] = false;
      // ledPwm[5] = 0;
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    }
    s = ledPwm[5];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[5], LV_ANIM_OFF);
    s = ledPwm[5];
    s.toCharArray(label_text, s.length() + 1);
    s = ledName[5];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button5, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LED6") {
    buttonOutLine();
    if (_flag != "0") {
      ledFlag[6] = true;
      ledPwm[6] = _value.toInt();
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    } else {
      ledFlag[6] = false;
      // ledPwm[6] = 0;
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    }
    s = ledPwm[6];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[6], LV_ANIM_OFF);
    s = ledPwm[6];
    s.toCharArray(label_text, s.length() + 1);
    s = ledName[6];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button6, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "SLEEP") {
    if (_flag.toInt() == 0) {
      sleepFlag = false;
      lv_obj_clear_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Image23, LV_OBJ_FLAG_HIDDEN);
      ledcWrite(0, bright);
      if (_value.toInt() == 1) {
        lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
        lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
        lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
        lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
        lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
        lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
      }
    } else { // Sleep Mode 진입...
      sleepFlag = true;
      ledcWrite(0, 10);
      lv_obj_add_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Image23, LV_OBJ_FLAG_HIDDEN);

      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_outButton1, LV_STATE_CHECKED);
    }
  } else if (_division == "OUT") {
    int out_Ch, out_Flag;
    out_Ch = _value.toInt();
    out_Flag = _flag.toInt();

    buttonOutLine();

    switch (out_Ch) {
    case 1:
      if (out_Flag == 1) {
        outFlag[out_Flag] = true;
        lv_obj_add_state(ui_outButton1, LV_STATE_CHECKED);
      } else {
        outFlag[out_Flag] = false;
        lv_obj_clear_state(ui_outButton1, LV_STATE_CHECKED);
      }
      lv_obj_set_style_outline_opa(ui_outButton1, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 2:
      if (out_Flag == 1) {
        outFlag[out_Flag] = true;
        lv_obj_add_state(ui_outButton2, LV_STATE_CHECKED);
      } else {
        outFlag[out_Flag] = false;
        lv_obj_clear_state(ui_outButton2, LV_STATE_CHECKED);
      }
      lv_obj_set_style_outline_opa(ui_outButton2, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 3:
      if (out_Flag == 1) {
        outFlag[out_Flag] = true;
        lv_obj_add_state(ui_outButton3, LV_STATE_CHECKED);
      } else {
        outFlag[out_Flag] = false;
        lv_obj_clear_state(ui_outButton3, LV_STATE_CHECKED);
      }
      lv_obj_set_style_outline_opa(ui_outButton3, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 4:
      if (out_Flag == 1) {
        outFlag[out_Flag] = true;
        lv_obj_add_state(ui_outButton4, LV_STATE_CHECKED);
      } else {
        outFlag[out_Flag] = false;
        lv_obj_clear_state(ui_outButton4, LV_STATE_CHECKED);
      }
      lv_obj_set_style_outline_opa(ui_outButton4, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 5:
      if (out_Flag == 1) {
        boilerFlag = true;
        lv_obj_add_state(ui_boilerButton, LV_STATE_CHECKED);
      } else {
        boilerFlag = false;
        lv_obj_clear_state(ui_boilerButton, LV_STATE_CHECKED);
      }
      lv_obj_set_style_outline_opa(ui_boilerButton, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 6:
      if (out_Flag == 1) {
        outFlag[out_Flag] = true;
        lv_obj_add_state(ui_outButton6, LV_STATE_CHECKED);
      } else {
        outFlag[out_Flag] = false;
        lv_obj_clear_state(ui_outButton6, LV_STATE_CHECKED);
      }
      lv_obj_set_style_outline_opa(ui_outButton6, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    default:
      break;
    }

    // if (_value != "0") {
    //   outFlag[_flag.toInt()] = true;
    // } else {
    //   outFlag[_flag.toInt()] = false;
    // }
    // s = ledPwm[6];
    // s.toCharArray(label_text, s.length() + 1);
    // lv_label_set_text(ui_LabelBright, label_text);
    // lv_slider_set_value(ui_Slider1, ledPwm[6], LV_ANIM_OFF);
    // s = ledPwm[6];
    // s.toCharArray(label_text, s.length() + 1);
    // s = ledName[6];
    // s.toCharArray(label_text, s.length() + 1);
    // lv_label_set_text(ui_LabelLED, label_text);
    // lv_obj_set_style_outline_opa(ui_Button6, 255,
    //                              LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LED_ALL") {
    buttonOutLine();
    if (_flag != "0") {
      ledAllFlag = true;
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
      // lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
      // lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
      // lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
      s = "100";
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      lv_slider_set_value(ui_Slider1, 100, LV_ANIM_OFF);
    } else {
      ledAllFlag = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
      // lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
      // lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
      // lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);
      // s = "0";
      // s.toCharArray(label_text, s.length() + 1);
      // lv_label_set_text(ui_LabelBright, label_text);
      // lv_slider_set_value(ui_Slider1, 0, LV_ANIM_OFF);
    }
    s = "전체등";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    lv_obj_set_style_outline_opa(ui_Button7, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "INV") {
    buttonOutLine();
    if (_value.toInt() != 0) {
      invertorFlag = true;
      lv_obj_add_state(ui_invButton, LV_STATE_CHECKED);
    } else {
      invertorFlag = false;
      lv_obj_clear_state(ui_invButton, LV_STATE_CHECKED);
    }
    Serial.print("INV Flag : ");
    Serial.println(_value);
    lv_obj_set_style_outline_opa(ui_invButton, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "CLEAN") {
    buttonOutLine();
    if (_value.toInt() != 0) {
      clnValveFlag = true;
      lv_obj_add_state(ui_clnButton, LV_STATE_CHECKED);
    } else {
      clnValveFlag = false;
      lv_obj_clear_state(ui_clnButton, LV_STATE_CHECKED);
    }
    Serial.print("CLEAN Flag : ");
    Serial.println(_value);
    lv_obj_set_style_outline_opa(ui_clnButton, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "WASTE") {
    buttonOutLine();
    if (_value.toInt() != 0) {
      wstValveFlag = true;
      lv_obj_add_state(ui_wstButton, LV_STATE_CHECKED);
    } else {
      wstValveFlag = false;
      lv_obj_clear_state(ui_wstButton, LV_STATE_CHECKED);
    }
    Serial.print("WASTE Flag : ");
    Serial.println(_value);
    lv_obj_set_style_outline_opa(ui_wstButton, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "BUNKER") {
    buttonOutLine();
    if (_value.toInt() != 0) {
      motor2Flag = true;
      lv_obj_add_state(ui_bunkerButton, LV_STATE_CHECKED);
    } else {
      motor2Flag = false;
      lv_obj_clear_state(ui_bunkerButton, LV_STATE_CHECKED);
    }
    Serial.print("BUNKER Flag : ");
    Serial.println(_value);
    lv_obj_set_style_outline_opa(ui_bunkerButton, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "HEATER") {
    buttonOutLine();
    if (_value.toInt() != 0) {
      lv_slider_set_value(ui_SliderHeater, _value.toInt(), LV_ANIM_OFF);
      lv_obj_add_state(ui_heaterButton, LV_STATE_CHECKED);
    } else {
      lv_obj_clear_state(ui_heaterButton, LV_STATE_CHECKED);
    }
    lv_obj_set_style_outline_opa(ui_heaterButton, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "LIVING") {
    buttonOutLine();
    if (_value.toInt() != 0) {
      lv_slider_set_value(ui_SliderLiving, _value.toInt(), LV_ANIM_OFF);
      lv_obj_add_state(ui_livingButton, LV_STATE_CHECKED);
    } else {
      lv_obj_clear_state(ui_livingButton, LV_STATE_CHECKED);
    }
    lv_obj_set_style_outline_opa(ui_livingButton, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  } else if (_division == "VOLT") {
    _value.toCharArray(label_text, _value.length());
    lv_label_set_text(ui_LabelVolt, label_text);
  } else if (_division == "QR_OK") {
    lv_disp_load_scr(ui_ScreenHome);
  } else if (_division == "TIME") {
    String strTime;
    int year, mon, day, hour, min;

    strTime = String(eventString[7]);
    strTime += String(eventString[8]);
    year = strTime.toInt();

    strTime = String(eventString[10]);
    strTime += String(eventString[11]);
    mon = strTime.toInt();

    strTime = String(eventString[13]);
    strTime += String(eventString[14]);
    day = strTime.toInt();

    strTime = String(eventString[16]);
    strTime += String(eventString[17]);
    hour = strTime.toInt();

    strTime = String(eventString[19]);
    strTime += String(eventString[20]);
    min = strTime.toInt();

    Serial.print(year);
    Serial.print("년 ");
    Serial.print(mon);
    Serial.print("월 ");
    Serial.print(day);
    Serial.print("일 ");
    Serial.print(hour);
    Serial.print("시 ");
    Serial.print(min);
    Serial.println("분 ");

    rtc.adjust(DateTime(year, mon, day, hour, min, 0));
  } else if (_division == "LED_NAME") {
    switch (_value.toInt()) {
    case 1:
      ledName[1] = _flag;
      ledName[1].toCharArray(label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_ledLabel1, label_text);
      EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
      break;
    case 2:
      ledName[2] = _flag;
      ledName[2].toCharArray(label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_ledLabel2, label_text);
      EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
      break;
    case 3:
      ledName[3] = _flag;
      ledName[3].toCharArray(label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_ledLabel3, label_text);
      EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
      break;
    case 4:
      ledName[4] = _flag;
      ledName[4].toCharArray(label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_ledLabel4, label_text);
      EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
      break;
    case 5:
      ledName[5] = _flag;
      ledName[5].toCharArray(label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_ledLabel5, label_text);
      EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
      break;
    case 6:
      ledName[6] = _flag;
      ledName[6].toCharArray(label_text, ledName[6].length() + 1);
      lv_label_set_text(ui_ledLabel6, label_text);
      EEPROM.writeString(LED6_NAME_EEPROM, ledName[6]);
      break;
    // case 7:
    // ledName[7] = _flag;
    // ledName[7].toCharArray(label_text, ledName[7].length() + 1);
    // lv_label_set_text(ui_Label7, label_text);
    // EEPROM.writeString(LED7_NAME_EEPROM, ledName[7]);
    // break;
    // case 8:
    //   ledName[8] = _flag;
    //   ledName[8].toCharArray(label_text, ledName[8].length() + 1);
    //   lv_label_set_text(ui_Label8, label_text);
    //   EEPROM.writeString(LED8_NAME_EEPROM, ledName[8]);
    //   break;
    default:
      break;
    }
    EEPROM.commit();
  } else if (_division == "SET_TEMP") {
    setTemp = _value.toFloat();
    s = String(setTemp, 1);
    s += " °C";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelSetTemp, label_text);
  } else if (_division == "TEMP") {
    // nowTemp = _value.toFloat();
    // s = String(nowTemp, 1);
    // s += " °C";
    // s.toCharArray(label_text, s.length() + 1);
    // lv_label_set_text(ui_LabelNowTemp, label_text);
    // nowTemp = _value.toFloat();
    s = _value;
    // s += " °C";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelNowTemp, label_text);
  } else if (_division == "CLN_VAL") {
    if (_value != "---")
      lv_bar_set_value(ui_BarClean, _value.toInt(), LV_ANIM_OFF);
    else
      lv_bar_set_value(ui_BarClean, 0, LV_ANIM_OFF);
    _value.toCharArray(label_text, _value.length() + 1);
    lv_label_set_text(ui_LabelClean, label_text);
  } else if (_division == "WST_VAL") {
    if (_value != "---")
      lv_bar_set_value(ui_BarWaste, _value.toInt(), LV_ANIM_OFF);
    else
      lv_bar_set_value(ui_BarWaste, 0, LV_ANIM_OFF);
    _value.toCharArray(label_text, _value.length() + 1);
    lv_label_set_text(ui_LabelWaste, label_text);
  }
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

void ledNameInit() {

  ledName[1] = EEPROM.readString(LED1_NAME_EEPROM);
  ledName[2] = EEPROM.readString(LED2_NAME_EEPROM);
  ledName[3] = EEPROM.readString(LED3_NAME_EEPROM);
  ledName[4] = EEPROM.readString(LED4_NAME_EEPROM);
  ledName[5] = EEPROM.readString(LED5_NAME_EEPROM);
  ledName[6] = EEPROM.readString(LED6_NAME_EEPROM);

  if (ledName[1] == "") {
    ledName[1] = ledNameOrigin[1];
    ledName[2] = ledNameOrigin[2];
    ledName[3] = ledNameOrigin[3];
    ledName[4] = ledNameOrigin[4];
    ledName[5] = ledNameOrigin[5];
    ledName[6] = ledNameOrigin[6];
  }

  ledName[1].toCharArray(label_text, ledName[1].length() + 1);
  lv_label_set_text(ui_ledLabel1, label_text);
  ledName[2].toCharArray(label_text, ledName[2].length() + 1);
  lv_label_set_text(ui_ledLabel2, label_text);
  ledName[3].toCharArray(label_text, ledName[3].length() + 1);
  lv_label_set_text(ui_ledLabel3, label_text);
  ledName[4].toCharArray(label_text, ledName[4].length() + 1);
  lv_label_set_text(ui_ledLabel4, label_text);
  ledName[5].toCharArray(label_text, ledName[5].length() + 1);
  lv_label_set_text(ui_ledLabel5, label_text);
  ledName[6].toCharArray(label_text, ledName[6].length() + 1);
  lv_label_set_text(ui_ledLabel6, label_text);

  // Serial.print("LED1 Name Length : ");
  // Serial.println(ledNameOrigin[1].length() + 1);

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
  Serial.print("LED6 Name : ");
  Serial.println(ledName[6]);
}

void setup() {
  delay(50);

  Wire.begin(8, 9, 100000);
  ESP_IOExpander_CH422G *expander = new ESP_IOExpander_CH422G(
      (i2c_port_t)I2C_MASTER_NUM, ESP_IO_EXPANDER_I2C_CH422G_ADDRESS,
      I2C_MASTER_SCL_IO, I2C_MASTER_SDA_IO);
  expander->init();
  expander->begin();
  Serial.println("Set the IO0-7 pin to output mode.");
  expander->enableAllIO_Output();
  expander->digitalWrite(TP_RST, HIGH);
  expander->digitalWrite(LCD_RST, HIGH); // Set reset pins high

  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1, 15, 16);

  Serial.flush();
  Serial2.flush();

  EEPROM.begin(250);

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  // delay(200);
  I2Ctwo.begin(12, 11, 100000); // Input Board 통신용...
  // delay(50);
  // I2Ctwo.clearWriteError();

  // myAHT10Flag = myAHT10.begin(&I2Ctwo, 0, 0x38);
  // delay(50);

  // if (myAHT10Flag)
  //   Serial.println("AHT Sensor Found...");
  // else
  //   Serial.println("AHT Sensor Not Found...");

  for (int i = 0; i < 3; i++) {
    if (rtc.begin(&I2Ctwo) != true) {
      Serial.println("RTC Not Found");
      delay(50);
    } else {
      rtcFlag = true;
      Serial.println("RTC Found");
      break;
    }
  }

  int ahtStatus = 0;

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 240, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, 0);
#endif

  // Init Display
  lv_init();
  gfx->begin();
  touch_init();
  // delay(50);

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

    ui_init();

    lv_timer_handler();

    rtc.writeSqwPinMode(DS1307_SquareWave1HZ);

    lv_obj_set_style_opa(ui_ImageQR, 127, 0);

    ledNameInit();
    clockRead();

    masterModeFlag = EEPROM.readBool(MASTER_SLAVE_EEPROM);

    Serial.print("Master Slave Mode : ");
    Serial.println(masterModeFlag == MASTER_MODE ? "Master mode"
                                                 : "Slave Mode");
    Serial.print("Master Slave bool Mode : ");
    Serial.println(masterModeFlag);

    if (masterModeFlag == MASTER_MODE) // true : Master Mode, false : SlaveMode
    {
      Serial.println("QR Display Master mode...");
      lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
      lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    } else {
      Serial.println("QR Display Slave mode...");
      lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
      lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    }
    lv_timer_handler();

    Serial.print("Master Slave bool Mode : ");
    Serial.println(masterModeFlag);

    Serial.println("Setup done");
    Serial2.flush();

    // s = lv_label_get_text(ui_LabelSSID);
    // s.toCharArray(ssidAp, s.length() + 1);

    // s = lv_label_get_text(ui_LabelPSWD);
    // s.toCharArray(passwordAp, s.length() + 1);

    // s = "AP_SSID:";
    // s += ssidAp;
    // comSend(s);
    // delay(10);
    // Serial.print("AP_SSID :");
    // Serial.println(s);

    // s = "AP_PSWD:";
    // s += passwordAp;
    // comSend(s);
    // delay(10);

    // Serial.print("AP_PSWD :");
    // Serial.println(s);

    // Serial.print("AP_SSID:");
    // Serial.println(ssidAp);
    // Serial.print("AP_PSWD:");
    // Serial.println(passwordAp);
  }
  // rtc.adjust(DateTime(2024, 10, 7, 20, 36, 0));

  // ahtStatus = 0;
  // while (myAHT10.begin(&I2Ctwo, 0, 0x38) != true) {
  //   Serial.println(F("Could not find AHT10 Check wiring"));

  //   delay(100);
  //   if (ahtStatus > 10)
  //     break;
  //   ahtStatus++;
  // }
  // Serial.println("AHT10 found");

  // tempRead();

  lv_disp_load_scr(ui_ScreenHome);

  delay(10);
  for (int i = 20; i < 255; i += 2) {
    lv_timer_handler();
    ledcWrite(0, i);
    // Serial.print("i = ");
    // Serial.println(i);
    delay(5);
  }
  // ledcWrite(0, 255);
  bright = lv_slider_get_value(ui_Slider3);
  ledSelect = 7;
  lv_obj_set_style_outline_opa(ui_Button7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = lv_label_get_text(ui_LabelSSID);
  s.toCharArray(ssidAp, s.length() + 1);

  s = lv_label_get_text(ui_LabelPSWD);
  s.toCharArray(passwordAp, s.length() + 1);

  comSend("COM:0");
  delay(10);

  s = "AP_SSID:";
  s += ssidAp;
  comSend(s);
  delay(10);
  Serial.print("AP_SSID :");
  Serial.println(s);

  s = "AP_PSWD:";
  s += passwordAp;
  comSend(s);
  delay(10);

  Serial.print("AP_PSWD :");
  Serial.println(s);

  Serial.print("AP_SSID:");
  Serial.println(ssidAp);
  Serial.print("AP_PSWD:");
  Serial.println(passwordAp);
}

void loop() {

  lv_timer_handler(); /* let the GUI do its work */

  if (loopCount > 500) { // 약 2초 마다 실실행...

    eventSec();
    // tempRead();

    Serial.print("Display Count : ");
    Serial.println(displayCount);

    loopCount = 0;

    if (screenHomeFlag) {
      // Home Screen 에서만 전송 ...
      comSend("REQUEST:1");
    }

    if (!screenHomeFlag)
      displayCount++;

    if (displayCount > 50) {
      lv_disp_load_scr(ui_ScreenHome);
    }
    if (rs485Count > 600) {
      lv_obj_add_flag(ui_ImageNetwork, LV_OBJ_FLAG_HIDDEN);
      rs485Count = 610;
    } else if (rs485Count < 600) {
      lv_obj_clear_flag(ui_ImageNetwork, LV_OBJ_FLAG_HIDDEN);
    }
  }

  // Serial.print("Rs485Count : ");
  // Serial.println(rs485Count);

  delay(5);
  loopCount++;
  rs485Count++;
}
