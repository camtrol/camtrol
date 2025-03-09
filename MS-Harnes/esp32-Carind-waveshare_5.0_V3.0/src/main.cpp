
/*******************************************************************************
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

// #include <RTClib.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <ESP_IOExpander_Library.h>
#include "waveshare_pcf85063a.h"

// Set current time
static datetime_t Set_Time = {.year = 2024,
                              .month = 12,
                              .day = 9,
                              .dotw = 1, // Day of the week, 5 means Friday
                              .hour = 22,
                              .min = 6,
                              .sec = 0};

char datetime_str[256]; // Variable to store time string
datetime_t Now_time;    // Current time variable
int Alarm_flag = 0;     // Alarm flag

/*******************************************************************************
 ******************************************************************************/

#define I2C_MASTER_NUM 0

// Extend(CH422) IO Pin define
#define TP_RST 1  // Touch Reset
#define LCD_BL 2  // Back Light Reset
#define LCD_RST 3 // LCD Reset
#define SD_CS 4

#define TFT_BL 13

#define ON_STATE 3
#define OFF_STATE 2

#define ON 1
#define OFF 0

#define CHECKED 3
#define UNCHECKED 2

#define SLAVE_MODE false
#define MASTER_MODE true

#define STATION_MODE true
#define AP_MODE false

// EEPROM Address...
#define SLEEP_COUNT_EEPROM 0    // Byte
#define SCR_LOCK_COUNT_EEPROM 1 // Byte
#define MASTER_SLAVE_EEPROM 2   // bool
#define NETWORK_EEPROM 3        // bool
#define LED1_NAME_EEPROM 4      // Unsigned Long
#define LED2_NAME_EEPROM 24     // Unsigned Long
#define LED3_NAME_EEPROM 44     // Unsigned Long
#define LED4_NAME_EEPROM 64     // Unsigned Long
#define LED5_NAME_EEPROM 84     // Unsigned Long
#define LED6_NAME_EEPROM 104    // Unsigned Long
#define LED7_NAME_EEPROM 124    // Unsigned Long
#define LED8_NAME_EEPROM 144    // Unsigned Long
#define LED9_NAME_EEPROM 164    // Unsigned Long

#define SSID_HOTSPOT_EEPROM 184 // HOT Spot SSID String
#define PSWD_HOTSPOT_EEPROM 234 // HOT Spot PSWD String

void eventSec();
// void wifiStart();
void comSend(String sendData);
void clockRead();

// TwoWire I2Ctwo = TwoWire(1);

// RTC_DS1307 rtc;

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, ssidHotSpot, pswdHotSpot, hotSpotIP;
String ledNameOrigin[10] = {" ",        "무드등", "실내등",   "예  비",
                            "트렁크등", "트렁크", "시거잭 1", "시거잭 2",
                            "상부등",   "인버터"};
String ledName[10] = {};

// WebServer server(80);

bool trunkModeFlag = false;

// bool scrLockFlag = false;

bool comMaskFlag = false;

bool comInitFlag = false;
bool serialEventMask = false;

bool ledFlag[9] = {false, false, false, false, false,
                   false, false, false, false};
bool ledAllFlag = false;

bool pumpFlag = false;
bool cigarFlag = false;
bool frigeFlag = false;
bool tvFlag = false;
bool invertorFlag = false;

bool sleepFlag = false;
// bool rs485Flag = false;

// bool wiFiFlag = false;

bool masterModeFlag = false;
bool networkFlag = false;
bool networkChangeFlag = false;
bool hotSpotIpSetFlag = false;

bool ahtFlag = false;
bool dispLogoFlag = false;
// bool rtcFlag = false;

char ssidAp[30] = "CARIND V6.0-SB-020";
char passwordAp[30] = "11240426020";

char msg[100];
char label_text[50];
String daysOfTheWeek[7] = {"월요일", "화요일", "수요일", "목요일",
                           "금요일", "토요일", "일요일"};

uint8_t loopCount = 0;
uint8_t readStatus = 0;
uint8_t bright = 100;
uint8_t btnNameClickCount = 0;

uint16_t state;             // 3.5' Button State...
uint16_t rs485Count = 0;    // rs485 Count
uint16_t dispLogoCount = 0; // Display Logo Count

int wifiCount = 10;
uint8_t ledPwm[9] = {100, 100, 100, 100, 100, 100, 100, 100, 100};
int countValue = 0;
int ledSelect = 9;
int motorTime = 30;

float aux_v = 14.4, temp = 20.0, humi = 50.0;
bool popUpFlag = false;
bool popDownFlag = false;
bool popStopFlag = false;

byte sleepCount;

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

      // USBSerial.print("Data x ");
      // USBSerial.println(touch_last_x);

      // USBSerial.print("Data y ");
      // USBSerial.println(touch_last_y);
    } else if (touch_released()) {
      data->state = LV_INDEV_STATE_REL;
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void wifiClick(lv_event_t *e) {
  dispLogoFlag = true;
  lv_obj_add_flag(ui_LabelBtnInit, LV_OBJ_FLAG_HIDDEN);
}

void screenQrExit(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
}

void nowTimeExit(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
}

void nowTimeSave(lv_event_t *e) {
  USBSerial.println("Now Time Save...");

  uint8_t year, month, day, hour, min, sec = 0;
  // DateTime now = rtc.now();
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

  Set_Time.year = year + 2000;
  Set_Time.month = month;
  Set_Time.day = day;
  Set_Time.dotw = lv_roller_get_selected(ui_RollerWeek);
  Set_Time.hour = hour;
  Set_Time.min = min;
  // Set_Time.sec = year + 2000;
  PCF85063A_Set_All(Set_Time); // Set current time
  clockRead();
  // rtc.adjust(DateTime(year + 2000, month, day, hour, min, sec));
  // USBSerial.print("Year : ");
  // USBSerial.println(year);
}

void timeChanged(lv_event_t *e) {
  // dispLogoCount = 0;
  // dispLogoFlag = false;
  USBSerial.println("Time Changed...");
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
  lv_roller_get_selected_str(ui_RollerWeek, buf, sizeof(buf));
  s += buf;
  s += "    ";
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
  dispLogoFlag = true;
  USBSerial.println("Time Clicked...");
  ledcWrite(0, 0);
  // if (rtcFlag) {
  // DateTime now = rtc.now();

  PCF85063A_Read_now(&Now_time); // Read current time

  lv_roller_set_selected(ui_RollerYear, Now_time.year - 2024, LV_ANIM_ON);
  lv_roller_set_selected(ui_RollerMonth, Now_time.month - 1, LV_ANIM_ON);
  lv_roller_set_selected(ui_RollerDay, Now_time.day - 1, LV_ANIM_ON);
  lv_roller_set_selected(ui_RollerWeek, Now_time.dotw, LV_ANIM_ON);
  lv_roller_set_selected(ui_RollerHour, Now_time.hour, LV_ANIM_ON);
  lv_roller_set_selected(ui_RollerMin, Now_time.min, LV_ANIM_ON);

  // s.toCharArray(label_text, s.length() + 1);
  // lv_label_set_text(ui_LabelMin, label_text);

  timeChanged(e);
  ledcWrite(0, map(bright, 0, 100, 255, 0));
}

void logoDisplay(lv_event_t *e) {
  USBSerial.println("Logo Icon Click...");
  dispLogoCount = 110;
  dispLogoFlag = false;
}

void logoClick(lv_event_t *e) {
  USBSerial.println("Logo Screen Click...");
  dispLogoCount = 0;
  dispLogoFlag = false;
  ledcWrite(0, map(bright, 0, 100, 255, 0));
}

void nameInitClick(lv_event_t *e) {
  USBSerial.print("Button Name Change Click : ");
  btnNameClickCount++;
  USBSerial.println(btnNameClickCount);
  if (btnNameClickCount > 10) {
    lv_obj_clear_flag(ui_LabelBtnInit, LV_OBJ_FLAG_HIDDEN);
    for (int i = 1; i < 10; i++) {
      ledName[i] = ledNameOrigin[i];
    }
    btnNameClickCount = 0;
    EEPROM.writeString(LED1_NAME_EEPROM, ledNameOrigin[1]);
    EEPROM.writeString(LED2_NAME_EEPROM, ledNameOrigin[2]);
    EEPROM.writeString(LED3_NAME_EEPROM, ledNameOrigin[3]);
    EEPROM.writeString(LED4_NAME_EEPROM, ledNameOrigin[4]);
    EEPROM.writeString(LED5_NAME_EEPROM, ledNameOrigin[5]);
    EEPROM.writeString(LED6_NAME_EEPROM, ledNameOrigin[6]);
    EEPROM.writeString(LED7_NAME_EEPROM, ledNameOrigin[7]);
    EEPROM.writeString(LED8_NAME_EEPROM, ledNameOrigin[8]);
    EEPROM.writeString(LED9_NAME_EEPROM, ledNameOrigin[9]);
    EEPROM.commit();

    ledNameOrigin[1].toCharArray(label_text, ledNameOrigin[1].length() + 1);
    lv_label_set_text(ui_Label1, label_text);
    ledNameOrigin[2].toCharArray(label_text, ledNameOrigin[2].length() + 1);
    lv_label_set_text(ui_Label2, label_text);
    ledNameOrigin[3].toCharArray(label_text, ledNameOrigin[3].length() + 1);
    lv_label_set_text(ui_Label3, label_text);
    ledNameOrigin[4].toCharArray(label_text, ledNameOrigin[4].length() + 1);
    lv_label_set_text(ui_Label4, label_text);
    ledNameOrigin[5].toCharArray(label_text, ledNameOrigin[5].length() + 1);
    lv_label_set_text(ui_Label5, label_text);
    ledNameOrigin[6].toCharArray(label_text, ledNameOrigin[6].length() + 1);
    lv_label_set_text(ui_Label6, label_text);
    ledNameOrigin[7].toCharArray(label_text, ledNameOrigin[7].length() + 1);
    lv_label_set_text(ui_Label7, label_text);
    ledNameOrigin[8].toCharArray(label_text, ledNameOrigin[8].length() + 1);
    lv_label_set_text(ui_Label8, label_text);
    ledNameOrigin[9].toCharArray(label_text, ledNameOrigin[9].length() + 1);
    lv_label_set_text(ui_Label9, label_text);

    // lv_disp_load_scr(ui_ScreenHome);
  }
}

void sleepOff(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  sleepFlag = false;
  ledcWrite(0, map(bright, 0, 100, 250, 0));
  lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
  s = "SLEEP:0";
  comSend(s);
}

void sleepButton(lv_event_t *e) {
  USBSerial.println("Sleep Button Click");
  dispLogoFlag = true;
  sleepFlag = true;
  // lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
  ledcWrite(0, 255);
  lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
  s = "SLEEP:1";
  comSend(s);
}

void buttonOutLine() {
  lv_obj_set_style_outline_opa(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void clockRead() {
  PCF85063A_Read_now(&Now_time);           // Read current time
  datetime_to_str(datetime_str, Now_time); // Convert to string format

  s = (Now_time.year) - 2000;
  s += "년 ";

  s += Now_time.month;
  s += "월 ";

  s += Now_time.day;
  s += "일  ";
  s += daysOfTheWeek[Now_time.dotw];

  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelDay, label_text);

  s = Now_time.hour;
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelTime, label_text);

  if (Now_time.min >= 10)
    s = Now_time.min;
  else {
    s = "0";
    s += Now_time.min;
  }
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelMin, label_text);
}

void ScreenHomeLoaded(lv_event_t *e) {
  USBSerial.println("ScreenHomeLoaded...");
}

void masterModeSelect(lv_event_t *e) {

  USBSerial.println("MasterModeSelect... ");

  if (lv_obj_get_state(ui_Switch1) == CHECKED) {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, MASTER_MODE);
    masterModeFlag = MASTER_MODE;
    USBSerial.println("Master Mode Selected...");

    // lv_obj_clear_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);

    if (networkFlag == AP_MODE) {
      lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    }

  } else {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, SLAVE_MODE);
    masterModeFlag = SLAVE_MODE;
    USBSerial.println("Slave Mode Selected...");

    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_add_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
  }
  EEPROM.commit();
}

void led1Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[1] || ledSelect == 1)
    return;
  ledSelect = 1;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelPWM, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led2Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[2] || ledSelect == 2)
    return;
  ledSelect = 2;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelPWM, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led3Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[3] || ledSelect == 3)
    return;
  ledSelect = 3;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelPWM, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void led4Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[4] || ledSelect == 4)
    return;
  ledSelect = 4;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelPWM, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
}

void comSend(String sendData) {
  // lv_timer_handler();
  // delay(5);
  // s = command;
  // s += ':';
  // s += value;
  Serial2.println(sendData);
  Serial2.flush();
}

void pwmChangeSlider(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  String sendData;

  s = lv_label_get_text(ui_LabelPWM);
  // comSend(s);
  USBSerial.print("Slider... ");
  USBSerial.println(s);

  // if (trunkModeFlag) // 카니발 모드이면 Return...
  //   return;

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
  default:
    break;
  }
  sendData += s.toInt();
  comSend(sendData);
  USBSerial.print("sendData : ");
  USBSerial.println(sendData);
  delay(50);
}

void brightChangeSlider(lv_event_t *e) { // 화면 밝기 조절...
  dispLogoCount = 0;
  dispLogoFlag = false;
  bright = lv_slider_get_value(ui_Slider3);
  USBSerial.println("Bright Slider... ");
  USBSerial.println(bright);
  if (bright < 10)
    ledcWrite(0, 250);
  else
    ledcWrite(0, map(bright, 0, 100, 255, 0));
}

void ledAllProcess() {
  return;
  if (!ledFlag[1] && !ledFlag[2] && !ledFlag[3] && !ledFlag[4]) {
    ledAllFlag = false;
  } else if (ledFlag[1] && ledFlag[2] && ledFlag[3] && ledFlag[4]) {
    ledAllFlag = true;
  }
}

void ledButton1(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  ledSelect = 1;
  state = lv_obj_get_state(ui_Button1);

  USBSerial.println("ledButton1...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button1, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) { // On 일때...
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelPWM, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    USBSerial.print("Button1 ledPwm[1] : ");
    USBSerial.println(ledPwm[ledSelect]);
  } else { // Off 일때...
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelPWM, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED1:";
  s += state == CHECKED ? ledPwm[1] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton2(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  ledSelect = 2;
  state = lv_obj_get_state(ui_Button2);

  USBSerial.println("ledButton2...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelPWM, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelPWM, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }

  s = "LED2:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton3(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  ledSelect = 3;
  state = lv_obj_get_state(ui_Button3);

  USBSerial.println("ledButton3...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelPWM, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelPWM, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED3:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void ledButton4(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  ledSelect = 4;
  state = lv_obj_get_state(ui_Button4);

  USBSerial.println("ledButton4...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelPWM, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelPWM, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED4:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
}

void pumpButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  USBSerial.println("PUMP Button...");

  ledSelect = 5;
  state = lv_obj_get_state(ui_Button5);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (state == CHECKED)
    pumpFlag = true;
  else
    pumpFlag = false;

  s = "PUMP:";
  s += state == CHECKED ? 1 : 0;
  comSend(s);
}

void cigarButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  USBSerial.println("Cigar Button...");

  ledSelect = 6;
  state = lv_obj_get_state(ui_Button6);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED)
    cigarFlag = true;
  else
    cigarFlag = false;
  s = "CIGAR:";
  s += state == CHECKED ? 1 : 0;
  comSend(s);
}

void frigeButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  USBSerial.println("Frige Button...");

  ledSelect = 7;
  state = lv_obj_get_state(ui_Button7);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED)
    frigeFlag = true;
  else
    frigeFlag = false;

  s = "FRIGE:";
  s += state == CHECKED ? 1 : 0;
  comSend(s);
}

void tvButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  USBSerial.println("TV Button...");

  ledSelect = 8;
  state = lv_obj_get_state(ui_Button8);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button8, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED)
    tvFlag = true;
  else
    tvFlag = false;

  s = "TV:";
  s += state == CHECKED ? 1 : 0;
  comSend(s);
}

void invertorButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  USBSerial.println("Invertor Button...");

  ledSelect = 9;
  state = lv_obj_get_state(ui_Button9);

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button9, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED)
    invertorFlag = true;
  else
    invertorFlag = false;

  s = "INV:";
  s += state == CHECKED ? 1 : 0;
  comSend(s);
}

void brightQR(lv_event_t *e) {
  uint8_t opa = lv_slider_get_value(ui_Slider2);
  USBSerial.print("brightQR... ");
  USBSerial.println(opa);
  lv_obj_set_style_opa(ui_ImageQR, opa, 0);
}

// void screenSleepIntro(lv_event_t *e) {
//   USBSerial.println("ScreenSleepIntro...");
//   scrLockFlag = false;
// }

void chgScreen2(lv_event_t *e) { USBSerial.println("chgScreen2..."); }

void homeLoad() {
  if (dispLogoFlag) {
    lv_disp_load_scr(ui_ScreenHome);
    ledcWrite(0, map(bright, 0, 100, 255, 0));
  }
  dispLogoCount = 0;
  dispLogoFlag = false;
}

void serialEvent2() {
  rs485Count = 0;
  if (comMaskFlag)
    return;

  USBSerial.println("serialEvent2...");

  String eventString = Serial2.readStringUntil('\n');
  Serial2.flush();

  int index1 = eventString.indexOf(':');
  int index2 = eventString.indexOf(':', index1 + 1);
  int index3 = eventString.length();

  String _division = eventString.substring(0, index1);
  String _value = eventString.substring(index1 + 1, index2);
  String _flag = eventString.substring(index2 + 1, index3 - 1);

  USBSerial.println(eventString);
  USBSerial.print("Division : ");
  USBSerial.print(_division);
  USBSerial.print(", Value : ");
  USBSerial.print(_value);
  USBSerial.print(", Flag : ");
  USBSerial.println(_flag);
  USBSerial.println();

  if (_division == "LED1") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button1, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    s = ledName[1];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    comInitFlag = true;
    // if (_value != "0") {
    if (_flag == "1") {
      ledFlag[1] = true;
      ledPwm[1] = _value.toInt();
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
      s = ledPwm[1];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[1], LV_ANIM_OFF);
    } else {
      ledFlag[1] = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    }
  } else if (_division == "LED2") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button2, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    s = ledName[2];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    comInitFlag = true;
    // if (_value != "0") {
    if (_flag == "1") {
      ledFlag[2] = true;
      ledPwm[2] = _value.toInt();
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
      s = ledPwm[2];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[2], LV_ANIM_OFF);
    } else {
      ledFlag[2] = false;
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    }
  } else if (_division == "LED3") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button3, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    s = ledName[3];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    comInitFlag = true;
    // if (_value != "0") {
    if (_flag == "1") {
      ledFlag[3] = true;
      ledPwm[3] = _value.toInt();
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
      s = ledPwm[2];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[3], LV_ANIM_OFF);
    } else {
      ledFlag[3] = false;
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    }
  } else if (_division == "LED4") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button4, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    s = ledName[4];
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    comInitFlag = true;
    // if (_value != "0") {
    if (_flag == "1") {
      ledFlag[4] = true;
      ledPwm[4] = _value.toInt();
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
      s = ledPwm[2];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[4], LV_ANIM_OFF);
    } else {
      ledFlag[4] = false;
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    }
  } else if (_division == "LED_ALL") {
    homeLoad();
    comInitFlag = true;
    if (_value != "0") {
      ledAllFlag = true;
      ledFlag[1] = true;
      ledFlag[2] = true;
      ledFlag[3] = true;
      ledFlag[4] = true;
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
      // comSend("LED_ALL:1:1");
    } else {
      ledAllFlag = false;
      ledFlag[1] = false;
      ledFlag[2] = false;
      ledFlag[3] = false;
      ledFlag[4] = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      // comSend("LED_ALL:0:0");
    }
  } else if (_division == "PUMP") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button5, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    comInitFlag = true;

    pumpFlag = false;

    if (_value != "0") {
      pumpFlag = true;
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
      // delay(500);
      // lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    } else {
      pumpFlag = false;
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    }

  } else if (_division == "CIGAR") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button6, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    comInitFlag = true;
    if (_value != "0") {
      cigarFlag = true;
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    } else {
      cigarFlag = false;
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    }
  } else if (_division == "FRIGE") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button7, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    comInitFlag = true;
    if (_value != "0") {
      frigeFlag = true;
      lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    } else {
      frigeFlag = false;
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
    }
  } else if (_division == "TV") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button8, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    comInitFlag = true;
    if (_value != "0") {
      tvFlag = true;
      lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    } else {
      tvFlag = false;
      lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
    }
  } else if (_division == "INVERTOR") {
    homeLoad();
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button9, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    comInitFlag = true;
    if (_value != "0") {
      invertorFlag = true;
      lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
    } else {
      invertorFlag = false;
      lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);
    }
  } else if (_division == "SLEEP") {
    dispLogoFlag = true;
    if (_value == "1") { // 취침모드 이면...
      sleepFlag = true;
      ledcWrite(0, 255);
      lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
      s = "SLEEP:1";
      comSend(s);
    } else { // 취침모드가 아니면...
      sleepFlag = false;
      ledcWrite(0, map(bright, 0, 100, 250, 0));
      lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
      s = "SLEEP:0";
      comSend(s);
    }
  } else if (_division == "TRUNK_MODE") {
    comInitFlag = true;
    if (_value == "1") {
      trunkModeFlag = true;
    } else {
      trunkModeFlag = false;
    }
    if (trunkModeFlag) {
      USBSerial.println("카니발 모드...");
    } else
      USBSerial.println("스타리아 모드...");
  } else if (_division == "LED_NAME") {
    ledNameOrigin[_value.toInt()] = _flag;
  } else if (_division == "NAME_SAVE") {
    for (int i = 1; i < 10; i++) {
      ledName[i] = ledNameOrigin[i];
      USBSerial.print("ledNameOrigin[");
      USBSerial.print(i);
      USBSerial.print("] : ");
      USBSerial.println(ledNameOrigin[i]);
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
    ledName[6].toCharArray(label_text, ledName[6].length() + 1);
    lv_label_set_text(ui_Label6, label_text);
    ledName[7].toCharArray(label_text, ledName[7].length() + 1);
    lv_label_set_text(ui_Label7, label_text);
    ledName[8].toCharArray(label_text, ledName[8].length() + 1);
    lv_label_set_text(ui_Label8, label_text);
    ledName[9].toCharArray(label_text, ledName[9].length() + 1);
    lv_label_set_text(ui_Label9, label_text);
    EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
    EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
    EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
    EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
    EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
    EEPROM.writeString(LED6_NAME_EEPROM, ledName[6]);
    EEPROM.writeString(LED7_NAME_EEPROM, ledName[7]);
    EEPROM.writeString(LED8_NAME_EEPROM, ledName[8]);
    EEPROM.writeString(LED9_NAME_EEPROM, ledName[9]);

    EEPROM.commit();
  } else if (_division == "QR_OK") {
    lv_disp_load_scr(ui_ScreenHome);
  } else if (_division == "T_YEAR") {
    Set_Time.year = _value.toInt();
  } else if (_division == "T_MONTH") {
    Set_Time.month = _value.toInt();
  } else if (_division == "T_DAY") {
    Set_Time.day = _value.toInt();
  } else if (_division == "T_HOUR") {
    Set_Time.hour = _value.toInt();
  } else if (_division == "T_MIN") {
    Set_Time.min = _value.toInt();
  } else if (_division == "T_WEEK") {
    Set_Time.dotw = _value.toInt();
  } else if (_division == "T_SEC") {
    Set_Time.sec = _value.toInt();
    PCF85063A_Set_All(Set_Time); // Set current time
  } else if (_division == "REQ_NET") {
    comSend("COM:1");
    delay(5);

    s = "AP_SSID:";
    s += ssidAp;
    delay(5);
    comSend(s);
    delay(10);

    s = "AP_PSWD:";
    s += passwordAp;
    delay(5);
    comSend(s);
    delay(10);
  }
}

void eventSec() {
  USBSerial.println("1 Sec Event...");

  if (countValue++ > 1) {

    // if (rtcFlag) {
    //   DateTime now = rtc.now();

    //   USBSerial.print(now.year(), DEC);
    //   USBSerial.print('/');
    //   USBSerial.print(now.month(), DEC);
    //   USBSerial.print('/');
    //   USBSerial.print(now.day(), DEC);
    //   USBSerial.print(" (");
    //   USBSerial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    //   USBSerial.print(") ");
    //   USBSerial.print(now.hour(), DEC);
    //   USBSerial.print(':');
    //   USBSerial.print(now.minute(), DEC);
    //   USBSerial.print(':');
    //   USBSerial.print(now.second(), DEC);
    //   USBSerial.println();
    // }

    clockRead();

    // if (wifiCount > 2) { // WiFi 가 연결 안되어 있으면...
    //   lv_obj_add_flag(ui_ImageWiFi,
    //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    //   if (wifiCount > 100)
    //     wifiCount = 10;
    // } else if (wifiCount == 0) { // WiFi 가 연결 되어 있으면...
    //   if (networkFlag == AP_MODE)
    //     lv_obj_clear_flag(ui_ImageWiFi,
    //                       LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // }
    USBSerial.print("WiFi Count : ");
    USBSerial.println(wifiCount);
    countValue = 0;
    wifiCount++;
  }
}

void ledNameInit() {

  ledName[1] = EEPROM.readString(LED1_NAME_EEPROM);
  ledName[2] = EEPROM.readString(LED2_NAME_EEPROM);
  ledName[3] = EEPROM.readString(LED3_NAME_EEPROM);
  ledName[4] = EEPROM.readString(LED4_NAME_EEPROM);
  ledName[5] = EEPROM.readString(LED5_NAME_EEPROM);
  ledName[6] = EEPROM.readString(LED6_NAME_EEPROM);
  ledName[7] = EEPROM.readString(LED7_NAME_EEPROM);
  ledName[8] = EEPROM.readString(LED8_NAME_EEPROM);
  ledName[9] = EEPROM.readString(LED9_NAME_EEPROM);

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
  ledName[6].toCharArray(label_text, ledName[6].length() + 1);
  lv_label_set_text(ui_Label6, label_text);
  ledName[7].toCharArray(label_text, ledName[7].length() + 1);
  lv_label_set_text(ui_Label7, label_text);
  ledName[8].toCharArray(label_text, ledName[8].length() + 1);
  lv_label_set_text(ui_Label8, label_text);
  ledName[9].toCharArray(label_text, ledName[9].length() + 1);
  lv_label_set_text(ui_Label9, label_text);

  lv_timer_handler();

  USBSerial.print("LED1 Name Length : ");
  USBSerial.println(ledNameOrigin[1].length() + 1);

  USBSerial.print("LED1 Name : ");
  USBSerial.println(ledName[1]);
  USBSerial.print("LED2 Name : ");
  USBSerial.println(ledName[2]);
  USBSerial.print("LED3 Name : ");
  USBSerial.println(ledName[3]);
  USBSerial.print("LED4 Name : ");
  USBSerial.println(ledName[4]);
  USBSerial.print("LED5 Name : ");
  USBSerial.println(ledName[5]);
  USBSerial.print("LED6 Name : ");
  USBSerial.println(ledName[6]);
  USBSerial.print("LED7 Name : ");
  USBSerial.println(ledName[7]);
  USBSerial.print("LED8 Name : ");
  USBSerial.println(ledName[8]);
  USBSerial.print("LED9 Name : ");
  USBSerial.println(ledName[9]);
}

void setup() {
  delay(50);

  Wire.begin(8, 9, 100000);
  ESP_IOExpander_CH422G *expander = new ESP_IOExpander_CH422G(
      (i2c_port_t)I2C_MASTER_NUM, ESP_IO_EXPANDER_I2C_CH422G_ADDRESS,
      I2C_MASTER_SCL_IO, I2C_MASTER_SDA_IO);
  expander->init();
  expander->begin();
  USBSerial.println("Set the IO0-7 pin to output mode.");
  expander->enableAllIO_Output();
  expander->digitalWrite(TP_RST, HIGH);
  expander->digitalWrite(LCD_RST, HIGH); // Set reset pins high

  // Serial.begin(115200, SERIAL_8N1, 16, 15);
  USBSerial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 43, 44);

  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 240, 8);
  ledcAttachPin(TFT_BL, 0);

  // digitalWrite(TFT_BL, LOW);
  ledcWrite(0, 255);
  delay(100);

  // Serial.begin(115200); /* prepare for possible serial debug */

  USBSerial.flush();
  Serial2.flush();

  EEPROM.begin(400);

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  USBSerial.println(LVGL_Arduino);
  USBSerial.println("I am LVGL_Arduino");

  PCF85063A_Init(); // Initialize PCF85063A RTC module
  delay(50);
  pinMode(RTC_MASTER_INT_IO, INPUT_PULLUP);

  // expander->digitalWrite(TP_RST, LOW);
  // delay(100);
  // expander->digitalWrite(TP_RST, HIGH);

  int ahtStatus = 0;

  // rtcFlag = rtc.begin(&I2Ctwo);
  // delay(10);
  // delay(50);

  // if (rtcFlag) {
  //   USBSerial.println("RTC Found...");
  //   rtcFlag = true;
  // } else {
  //   USBSerial.println("RTC Not Found...");
  // }

  ssidHotSpot = EEPROM.readString(SSID_HOTSPOT_EEPROM);
  pswdHotSpot = EEPROM.readString(PSWD_HOTSPOT_EEPROM);

  masterModeFlag = EEPROM.readBool(MASTER_SLAVE_EEPROM);
  // networkFlag = EEPROM.readBool(NETWORK_EEPROM);

  USBSerial.print("Master Slave Mode : ");
  USBSerial.println(masterModeFlag);
  USBSerial.print("Network Mode : ");
  USBSerial.println(networkFlag);

  USBSerial.print("Master Slave Mode : ");
  USBSerial.println(masterModeFlag == MASTER_MODE ? "Master mode"
                                                  : "Slave Mode");
  USBSerial.print("Network Mode : ");
  USBSerial.println(networkFlag == STATION_MODE ? "Station mode" : "AP Mode");

  if (EEPROM.readByte(SLEEP_COUNT_EEPROM) > 100) {
    EEPROM.writeByte(SLEEP_COUNT_EEPROM, 3);
    EEPROM.commit();
  } else
    sleepCount = EEPROM.readByte(SLEEP_COUNT_EEPROM);

  // #ifdef TFT_BL
  // pinMode(TFT_BL, OUTPUT);
  // ledcSetup(0, 240, 8);
  // ledcAttachPin(TFT_BL, 0);
  // ledcWrite(0, 255);
  // #endif

  // expander->digitalWrite(TP_RST, LOW);
  // delay(100);
  // expander->digitalWrite(TP_RST, HIGH);
  // delay(10);

  // Init Display
  lv_init();
  gfx->begin();
  touch_init();

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
    USBSerial.println("LVGL disp_draw_buf allocate failed!");
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

    // delay(100);

    ui_init();

    // delay(100);
    // lv_obj_set_style_opa(ui_ImageQR, 127, 0);
    // lv_timer_handler();

    ledNameInit();
    clockRead();

    if (networkFlag) // true : Tethering Mode, false : WiFi
    {                // 핫스팟 Mode...
      s = "핫스팟 네트워크:";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_Label23, label_text);
      // ssidHotSpot.toCharArray(label_text, sizeof(label_text));
      ssidHotSpot.toCharArray(label_text, ssidHotSpot.length() + 1);
      lv_label_set_text(ui_LabelSSID, label_text);
      // lv_textarea_set_text(ui_TextAreaSSID, label_text);

      s = "핫스팟 비밀번호:";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_Label24, label_text);
      // pswdHotSpot.toCharArray(label_text, sizeof(label_text));
      pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
      lv_label_set_text(ui_LabelPSWD, label_text);
      // lv_textarea_set_text(ui_TextAreaPSWD, label_text);
      lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    } else { // WiFi Mode...
      // lv_obj_clear_state(ui_SwitchNetwork, LV_STATE_CHECKED);
      s = "와이파이 네트워크:";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_Label23, label_text);
      // lv_label_set_text(ui_Label25, ssidAp);

      s = "와이파이 비밀번호:";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_Label24, label_text);
      // lv_label_set_text(ui_Label26, passwordAp);
    }
    // ssidHotSpot.toCharArray(label_text, ssidHotSpot.length() + 1);
    // lv_textarea_set_text(ui_TextAreaSSID, label_text);
    // pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
    // lv_textarea_set_text(ui_TextAreaPSWD, label_text);

    if (!masterModeFlag) // true : Master Mode, false : Slave Mode
    {
      lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);

      lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);

      lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);

      if (networkFlag == AP_MODE) {
        lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
      }
    }

    s = lv_label_get_text(ui_LabelSSID);
    s.toCharArray(ssidAp, s.length() + 1);

    s = lv_label_get_text(ui_LabelPSWD);
    s.toCharArray(passwordAp, s.length() + 1);

    // wifiStart();

    // server.on("/json", handleJson);
    // server.on("/root", handleRoot);
    // server.on("/Request", handleRequest);

    // server.begin();

    bright = lv_slider_get_value(ui_Slider3);

    clockRead();

    // USBSerial.print("BACK_COLOR_EEPROM : ");
    // USBSerial.println(EEPROM.readUInt(BACK_COLOR_EEPROM), HEX);

    USBSerial.println("Setup done");
    Serial2.flush();

    s = "AP_SSID:";
    s += ssidAp;
    comSend("COM:1");
    delay(5);
    comSend(s);
    delay(10);

    s = "AP_PSWD:";
    s += passwordAp;
    comSend("COM:1");
    delay(5);
    comSend(s);
    delay(10);

    // s = "ST_SSID:";
    // s += ssidHotSpot;
    // comSend(s);
    // delay(10);

    // s = "ST_PSWD:";
    // s += pswdHotSpot;
    // comSend(s);
    // delay(10);
  }
  ledcWrite(0, 0);
}

void loop() {

  lv_timer_handler(); /* let the GUI do its work */

  if (loopCount > 180) { // 약 1초 마다 실실행...
    // int8_t rssi = WiFi.RSSI();
    // USBSerial.print("RSSI: ");

    // if (pumpFlag && trunkModeFlag) {
    //   lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    // }

    PCF85063A_Read_now(&Now_time);           // Read current time
    datetime_to_str(datetime_str, Now_time); // Convert to string format
    USBSerial.print("Now_time is ");
    USBSerial.println(datetime_str);       // Print current time
    USBSerial.print(Now_time.year - 2000); // Print current time
    USBSerial.print("년 ");                // Print current time
    USBSerial.print(Now_time.month);       // Print current time
    USBSerial.print("월 ");                // Print current time
    USBSerial.print(Now_time.day);         // Print current time
    USBSerial.print("일 ");                // Print current time
    USBSerial.print(Now_time.dotw);        // Print current time
    USBSerial.print("요일 ");              // Print current time
    USBSerial.print(Now_time.hour);        // Print current time
    USBSerial.print("시 ");                // Print current time
    USBSerial.print(Now_time.min);         // Print current time
    USBSerial.print("분 ");                // Print current time
    USBSerial.print(Now_time.sec);         // Print current time
    USBSerial.println("초 ");              // Print current time

    if (Alarm_flag == 1) { // If alarm flag is set
      Alarm_flag = 0;      // Reset alarm flag
      // Restart the alarm. Comment out this function if it only needs to run
      // once
      PCF85063A_Enable_Alarm(); // Re-enable the alarm
      USBSerial.println(
          "The alarm clock goes off."); // Print alarm goes off message
    }

    // if (networkChangeFlag) {
    //   WiFi.disconnect();
    //   wifiStart();
    //   networkChangeFlag = false;
    // }

    // USBSerial.println(rssi);

    if (!comMaskFlag)
      eventSec();

    loopCount = 0;

    comSend("COM:1");
    if (!comInitFlag) // 초기에 메인 콘트롤러로 부터 내용 가져오기...
    {
      comSend("REQUEST:1");
      // for (int i = 0; i < 500; i++)
      //   delay(5);
    }
    rs485Count++;

    if (rs485Count > 6)
      lv_obj_add_flag(ui_ImageNetwork, LV_OBJ_FLAG_HIDDEN);
    else
      lv_obj_clear_flag(ui_ImageNetwork, LV_OBJ_FLAG_HIDDEN);

    USBSerial.print("RS485 Count : ");
    USBSerial.println(rs485Count);

    if (dispLogoCount > 100 && !dispLogoFlag) {
      lv_disp_load_scr(ui_ScreenLogo);
      dispLogoFlag = true;
      ledcWrite(0, 230);
      if (dispLogoCount > 200)
        dispLogoCount = 101;
    }

    dispLogoCount++;
    USBSerial.print("DispLogoCount : ");
    USBSerial.println(dispLogoCount);
  }

  delay(5);
  loopCount++;
}
