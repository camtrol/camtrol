//*********************************************************************************************************/
// 마래 캠핑카 V6.0
// https://squareline.io/  camtrol@naver.com   gustnr99**
// I2C 설명 Site :
// https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/
//
// PanelDisplay x 위치 변화 : -140 -> -198
// PanelButton  x 위치 변화 : 0 -> 64
//
//*********************************************************************************************************/

#define LGFX_USE_V1

#include "Arduino.h"
#include "ui/ui.h" // this is the ui generated with lvgl / squareline editor
// #include "../ui/src/ui.h"
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
// #include <Adafruit_AHTX0.h>
// #include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <ArduinoJson.h>
// #include <ESP32Ticker.h>
#include <EEPROM.h>
// #include <cstdint>
// #include <RtcDS1307.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 13
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

#define SEC 14
#define MOTOR 21
#define RTS 2
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
#define LED1_NAME_EEPROM 10     // Unsigned Long
#define LED2_NAME_EEPROM 20     // Unsigned Long
#define LED3_NAME_EEPROM 30     // Unsigned Long
#define LED4_NAME_EEPROM 40     // Unsigned Long
#define LED5_NAME_EEPROM 50     // Unsigned Long
#define LED6_NAME_EEPROM 60     // Unsigned Long
#define LED7_NAME_EEPROM 70     // Unsigned Long
#define LED8_NAME_EEPROM 80     // Unsigned Long

#define BACK_COLOR_EEPROM 90 // 배경색 Double
#define SW_ON_EEPROM 94      // 스위치 켜짐 Double
#define SW_OFF_EEPROM 98     // 스위치 꺼짐 Double

#define SSID_HOTSPOT_EEPROM 110 // HOT Spot SSID String
#define PSWD_HOTSPOT_EEPROM 160 // HOT Spot PSWD String

void eventSec();
void wifiStart();
void comSend(String sendData);
void PanelButton_Animation(lv_obj_t *TargetObject, int delay);
void PanelDisplay_Animation(lv_obj_t *TargetObject, int delay);

DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 rtc;

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, ssidHotSpot, pswdHotSpot, hotSpotIP;
String ledNameOrigin[9] = {" ",      "팝업등", "침상등", "사이드", "전면등",
                           "후면등", "철재장", "무드등", "어닝등"};
String ledName[9] = {};

WebServer server(80);

bool scrLockFlag = false;
bool gestureLockUpFlag = false;

bool comMaskFlag = false;

bool comInitFlag = false;
bool serialEventMask = false;

bool ledFlag[9] = {false, false, false, false, false,
                   false, false, false, false};
bool ledAllFlag = false;
bool invertorFlag = false;
bool tvFlag = false;
bool frigeFlag = false;

bool sleepFlag = false;
bool sleepStartFlag = false;

// bool wiFiFlag = false;

bool masterModeFlag = false;
bool networkFlag = false;
bool networkChangeFlag = false;
bool hotSpotIpSetFlag = false;

bool ahtFlag = false;
bool rtcFlag = false;
bool popUpFlag = false;
bool popStopFlag = true;
bool popDownFlag = false;

bool sensorDHT = false;

byte colorSelect = 1, scrLockCount;

char ssidAp[30] = "마레 V6.0-SB-020";
char passwordAp[30] = "11240426020";

char msg[100];
char label_text[50];
char daysOfTheWeek[7][10] = {"일요일", "월요일", "화요일", "수요일",
                             "목요일", "금요일", "토요일"};
// char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
//                              "Thursday", "Friday", "Saturday"};

String strYear, strMonth, strDay, strTime, strMinute, strSec;

uint8_t loopCount = 0;
uint8_t readStatus = 0;
uint8_t btnNameClickCount = 0;

uint8_t sliderRedColor, sliderGreenColor, sliderBlueColor = 0;
uint16_t state;
uint32_t backColor, switchOnColor, switchOffColor, chgColor;
uint32_t backColorPush, switchOnColorPush, switchOffColorPush;

int wifiCount = 10;
uint8_t ledPwm[10] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
int countValue = 0;
int ledSelect = 9;
int motorTime = 30;
int bright = 0;

byte sleepCount;

float temperature, humidity;
float aux_v;

// sensors_event_t adsHumidity, adsTemp;
// TwoWire I2Ctwo = TwoWire(1);
// Adafruit_AHTX0 myAHT10;
// RtcDS1307<TwoWire> Rtc(I2Ctwo);

// SETUP LGFX PARAMETERS FOR WT32-SC01-PLUS
class LGFX : public lgfx::LGFX_Device {

  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_Parallel8 _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_FT5x06
      _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();

      cfg.freq_write = 20000000;
      cfg.pin_wr = 47; // pin number connecting WR
      cfg.pin_rd = -1; // pin number connecting RD
      cfg.pin_rs = 0;  // Pin number connecting RS(D/C)
      cfg.pin_d0 = 9;  // pin number connecting D0
      cfg.pin_d1 = 46; // pin number connecting D1
      cfg.pin_d2 = 3;  // pin number connecting D2
      cfg.pin_d3 = 8;  // pin number connecting D3
      cfg.pin_d4 = 18; // pin number connecting D4
      cfg.pin_d5 = 17; // pin number connecting D5
      cfg.pin_d6 = 16; // pin number connecting D6
      cfg.pin_d7 = 15; // pin number connecting D7
      // cfg.i2s_port = I2S_NUM_0; // (I2S_NUM_0 or I2S_NUM_1)

      _bus_instance.config(cfg);              // Apply the settings to the bus.
      _panel_instance.setBus(&_bus_instance); // Sets the bus to the panel.
    }

    { // Set display panel control.
      auto cfg = _panel_instance
                     .config(); // Get the structure for display panel settings.

      cfg.pin_cs = -1; // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = 4; // TP_RST 와 연결됨...
      // cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 =
      // disable)
      cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 = disable)

      // * The following setting values ​​are set to general default values
      // ​​for each panel, and the pin number (-1 = disable) to which BUSY
      // is connected, so please try commenting out any unknown items.

      cfg.memory_width = 320;  // Maximum width supported by driver IC
      cfg.memory_height = 480; // Maximum height supported by driver IC
      cfg.panel_width = 320;   // actual displayable width
      cfg.panel_height = 480;  // actual displayable height
      cfg.offset_x = 0;        // Panel offset in X direction
      cfg.offset_y = 0;        // Panel offset in Y direction
      cfg.offset_rotation = 2; // was 2
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true; // was false
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true; // was false something to do with SD?

      _panel_instance.config(cfg);
    }

    { // Set backlight control. (delete if not necessary)
      auto cfg =
          _light_instance
              .config(); // Get the structure for backlight configuration.

      cfg.pin_bl = 45;     // pin number to which the backlight is connected
      cfg.invert = false;  // true to invert backlight brightness
      cfg.freq = 44100;    // backlight PWM frequency
      cfg.pwm_channel = 0; // PWM channel number to use (7??)

      _light_instance.config(cfg);
      _panel_instance.setLight(
          &_light_instance); // Sets the backlight to the panel.
    }

    //*
    { // Configure settings for touch screen control. (delete if not necessary)
      auto cfg = _touch_instance.config();

      cfg.x_min =
          0; // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max =
          319; // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min = 0; // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max =
          479; // Maximum Y value (raw value) obtained from the touchscreen
      cfg.pin_int = 7; // pin number to which INT is connected
      cfg.bus_shared =
          false; // set true if you are using the same bus as the screen
      cfg.offset_rotation = 0;

      // For I2C connection
      cfg.i2c_port = 0;    // Select I2C to use (0 or 1)
      cfg.i2c_addr = 0x38; // I2C device address number
      cfg.pin_sda = 6;     // pin number where SDA is connected
      cfg.pin_scl = 5;     // pin number to which SCL is connected
      cfg.freq = 400000;   // set I2C clock

      _touch_instance.config(cfg);
      _panel_instance.setTouch(
          &_touch_instance); // Set the touchscreen to the panel.
    }
    //*/
    setPanel(&_panel_instance); // Sets the panel to use.
  }
};

LGFX tft;

static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

// lv debugging can be set in lv_conf.h
// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print(const char *buf) {
//   Serial.printf(buf);
//   Serial.flush();
// }
// #endif

/*Change to your screen resolution*/

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX = 0, touchY = 0;

  // bool touched = false; // tft.getTouch( &touchX, &touchY, 600 );
  bool touched = tft.getTouch(&touchX, &touchY);

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    // Serial.print("Data x ");
    // Serial.println(touchX);

    // Serial.print("Data y ");
    // Serial.println(touchY);

    if (scrLockFlag) // 화면잠금Count Reload...
      scrLockCount = EEPROM.readByte(SCR_LOCK_COUNT_EEPROM);
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
    EEPROM.writeString(LED1_NAME_EEPROM, ledNameOrigin[1]);
    EEPROM.writeString(LED2_NAME_EEPROM, ledNameOrigin[2]);
    EEPROM.writeString(LED3_NAME_EEPROM, ledNameOrigin[3]);
    EEPROM.writeString(LED4_NAME_EEPROM, ledNameOrigin[4]);
    EEPROM.writeString(LED5_NAME_EEPROM, ledNameOrigin[5]);
    EEPROM.writeString(LED6_NAME_EEPROM, ledNameOrigin[6]);
    EEPROM.writeString(LED7_NAME_EEPROM, ledNameOrigin[7]);
    EEPROM.writeString(LED8_NAME_EEPROM, ledNameOrigin[8]);
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
    ledName[6].toCharArray(label_text, ledName[6].length() + 1);
    lv_label_set_text(ui_Label6, label_text);
    ledName[7].toCharArray(label_text, ledName[7].length() + 1);
    lv_label_set_text(ui_Label7, label_text);
    ledName[8].toCharArray(label_text, ledName[8].length() + 1);
    lv_label_set_text(ui_Label8, label_text);
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
  lv_obj_set_style_outline_opa(ui_Button8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Button12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
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
    sleepFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
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
      // s = "팝업등";
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
      // s = "침상등";
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

      // s = "사이드";
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

      // s = "전면등";
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

      // s = "후면등";
      s = ledName[5];
      break;
    case 6:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button6, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[6] = false;
        lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[6] = true;
        lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
        ledPwm[6] = value;
      }
      s = "LED6:";
      s += ledFlag[6] == true ? ledPwm[6] : 0;
      comSend(s);

      // s = "철재장";
      s = ledName[6];
      break;
    case 7:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button7, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[7] = false;
        lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[7] = true;
        lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
        ledPwm[7] = value;
      }
      s = "LED7:";
      s += ledFlag[7] == true ? ledPwm[7] : 0;
      comSend(s);

      // s = "무드등";
      s = ledName[7];
      break;
    case 8:
      buttonOutLine();
      lv_obj_set_style_outline_opa(ui_Button8, 255,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
      if (!value) { // 값이 0 이면...
        ledFlag[8] = false;
        lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
      } else { // 값이 1 이상이면...
        ledFlag[8] = true;
        lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
        ledPwm[8] = value;
      }
      s = "LED8:";
      s += ledFlag[8] == true ? ledPwm[8] : 0;
      comSend(s);

      // s = "어닝등";
      s = ledName[8];
      break;
    default:
      break;
    }
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "ALL") {
    s = "전체등";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);

    if (!value) { // 값이 0 이면...
      ledFlag[1] = false;
      ledFlag[2] = false;
      ledFlag[3] = false;
      ledFlag[4] = false;
      ledFlag[5] = false;
      ledFlag[6] = false;
      ledFlag[7] = false;
      ledFlag[8] = false;
      ledAllFlag = false;
      // ledPwm[1] = 0;
      // ledPwm[2] = 0;
      // ledPwm[3] = 0;
      // ledPwm[4] = 0;
      // ledPwm[5] = 0;
      // ledPwm[6] = 0;
      // ledPwm[7] = 0;
      // ledPwm[8] = 0;
      // ledPwm[9] = 0;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);

      lv_label_set_text(ui_LabelBright, "0");
      s = 1;

      lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    } else { // 값이 1 이상이면...
      ledFlag[1] = true;
      ledFlag[2] = true;
      ledFlag[3] = true;
      ledFlag[4] = true;
      ledFlag[5] = true;
      ledFlag[6] = true;
      ledFlag[7] = true;
      ledFlag[8] = true;
      ledAllFlag = true;
      sleepFlag = false;
      // ledPwm[1] = 100;
      // ledPwm[2] = 100;
      // ledPwm[3] = 100;
      // ledPwm[4] = 100;
      // ledPwm[5] = 100;
      // ledPwm[6] = 100;
      // ledPwm[7] = 100;
      // ledPwm[8] = 100;
      // ledPwm[9] = 100;
      lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
      tft.setBrightness(255);
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
      value = 100;
      s = value;
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      lv_slider_set_value(ui_Slider1, value, LV_ANIM_OFF);
    }
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button9, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    s = "LED_ALL:";
    s += ledAllFlag == true ? ledPwm[ledSelect] : 0;
    comSend(s);
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "INVERTOR") {
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button10, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!value) { // 값이 0 이면...
      invertorFlag = false;
      lv_obj_clear_state(ui_Button10, LV_STATE_CHECKED);
    } else { // 값이 1 이상이면...
      invertorFlag = true;
      lv_obj_add_state(ui_Button10, LV_STATE_CHECKED);
    }
    s = "INV:";
    s += invertorFlag == true ? '1' : '0';
    comSend(s);
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "TV") {
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button11, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!value) { // 값이 0 이면...
      tvFlag = false;
      lv_obj_clear_state(ui_Button11, LV_STATE_CHECKED);
    } else { // 값이 1 이상이면...
      tvFlag = true;
      lv_obj_add_state(ui_Button11, LV_STATE_CHECKED);
    }
    s = "TV:";
    s += tvFlag == true ? '1' : '0';
    comSend(s);
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "FRIGE") {
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button12, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    if (!value) { // 값이 0 이면...
      frigeFlag = false;
      lv_obj_clear_state(ui_Button12, LV_STATE_CHECKED);
    } else { // 값이 1 이상이면...
      frigeFlag = true;
      lv_obj_add_state(ui_Button12, LV_STATE_CHECKED);
    }
    s = "FRIGE:";
    s += frigeFlag == true ? '1' : '0';
    comSend(s);
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "POP_UP") {
    s = "POP_UP:";
    if (value == 1) {
      s += '1';
      popUpFlag = true;
      popDownFlag = false;
    }
    comSend(s);
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "POP_STOP") {
    s = "POP_STOP:";
    if (value == 1) {
      s += '1';
      popStopFlag = true;
      popUpFlag = false;
      popDownFlag = false;
    }
    comSend(s);

    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "POP_DOWN") {
    s = "POP_DOWN:";
    if (value == 1) {
      s += '1';
      popDownFlag = true;
      popUpFlag = false;
    }
    comSend(s);

    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "SLEEP") {
    if (value == 1) { // 취침모드 이면...
      ledAllFlag = false;
      sleepFlag = true;
      sleepStartFlag = true;
      sleepCount = EEPROM.readByte(SLEEP_COUNT_EEPROM);
      // lv_obj_clear_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
      // lv_obj_add_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
      // tft.setBrightness(1);
    } else { // 취침모드가 아니면...
      sleepFlag = false;
      sleepStartFlag = false;
      lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
      tft.setBrightness(255);
    }
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "LED_NAME") {
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
    case 5:
      ledName[5] = _ledName;
      break;
    case 6:
      ledName[6] = _ledName;
      break;
    case 7:
      ledName[7] = _ledName;
      break;
    case 8:
      ledName[8] = _ledName;
      break;
    case 10:
      Serial.println("LED Name Save...");
      EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
      EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
      EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
      EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
      EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
      EEPROM.writeString(LED6_NAME_EEPROM, ledName[6]);
      EEPROM.writeString(LED7_NAME_EEPROM, ledName[7]);
      EEPROM.writeString(LED8_NAME_EEPROM, ledName[8]);
      EEPROM.commit();
      // ledName[1].toCharArray(label_text, sizeof(label_text));
      ledName[1].toCharArray(label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_Label1, label_text);
      // ledName[2].toCharArray(label_text, sizeof(label_text));
      ledName[2].toCharArray(label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_Label2, label_text);
      // ledName[3].toCharArray(label_text, sizeof(label_text));
      ledName[3].toCharArray(label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_Label3, label_text);
      // ledName[4].toCharArray(label_text, sizeof(label_text));
      ledName[4].toCharArray(label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_Label4, label_text);
      // ledName[5].toCharArray(label_text, sizeof(label_text));
      ledName[5].toCharArray(label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_Label5, label_text);
      // ledName[6].toCharArray(label_text, sizeof(label_text));
      ledName[6].toCharArray(label_text, ledName[6].length() + 1);
      lv_label_set_text(ui_Label6, label_text);
      // ledName[7].toCharArray(label_text, sizeof(label_text));
      ledName[7].toCharArray(label_text, ledName[7].length() + 1);
      lv_label_set_text(ui_Label7, label_text);
      // ledName[8].toCharArray(label_text, sizeof(label_text));
      ledName[8].toCharArray(label_text, ledName[8].length() + 1);
      lv_label_set_text(ui_Label8, label_text);
      break;
    case 0:
      ledName[1] = ledNameOrigin[1];
      ledName[2] = ledNameOrigin[2];
      ledName[3] = ledNameOrigin[3];
      ledName[4] = ledNameOrigin[4];
      ledName[5] = ledNameOrigin[5];
      ledName[6] = ledNameOrigin[6];
      ledName[7] = ledNameOrigin[7];
      ledName[8] = ledNameOrigin[8];
      EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
      EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
      EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
      EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
      EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
      EEPROM.writeString(LED6_NAME_EEPROM, ledName[6]);
      EEPROM.writeString(LED7_NAME_EEPROM, ledName[7]);
      EEPROM.writeString(LED8_NAME_EEPROM, ledName[8]);
      EEPROM.commit();
      // ledName[1].toCharArray(label_text, sizeof(label_text));
      ledName[1].toCharArray(label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_Label1, label_text);
      // ledName[2].toCharArray(label_text, sizeof(label_text));
      ledName[2].toCharArray(label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_Label2, label_text);
      // ledName[3].toCharArray(label_text, sizeof(label_text));
      ledName[3].toCharArray(label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_Label3, label_text);
      // ledName[4].toCharArray(label_text, sizeof(label_text));
      ledName[4].toCharArray(label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_Label4, label_text);
      // ledName[5].toCharArray(label_text, sizeof(label_text));
      ledName[5].toCharArray(label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_Label5, label_text);
      // ledName[6].toCharArray(label_text, sizeof(label_text));
      ledName[6].toCharArray(label_text, ledName[6].length() + 1);
      lv_label_set_text(ui_Label6, label_text);
      // ledName[7].toCharArray(label_text, sizeof(label_text));
      ledName[7].toCharArray(label_text, ledName[7].length() + 1);
      lv_label_set_text(ui_Label7, label_text);
      // ledName[8].toCharArray(label_text, sizeof(label_text));
      ledName[8].toCharArray(label_text, ledName[8].length() + 1);
      lv_label_set_text(ui_Label8, label_text);
      break;
    default:
      break;
    }
  } else if (division == "TIME") {
    Serial.print("Receive Time : ");
    Serial.println(nowTime);
    Serial.print(nowTime[0]);
    Serial.print(nowTime[1]);
    Serial.print(nowTime[2]);
    Serial.print(nowTime[3]);
    Serial.print("년 ");
    Serial.print(nowTime[5]);
    Serial.print(nowTime[6]);
    Serial.print("월 ");
    Serial.print(nowTime[8]);
    Serial.print(nowTime[9]);
    Serial.print("일 ");
    Serial.print(nowTime[11]);
    Serial.print(nowTime[12]);
    Serial.print("시 ");
    Serial.print(nowTime[14]);
    Serial.print(nowTime[15]);
    Serial.print("분 ");
    Serial.print(nowTime[17]);
    Serial.print(nowTime[18]);
    Serial.println("초 ");

    if (rtcFlag) {
      uint16_t year, month, day, hour, min, sec;

      String strTime = String(nowTime[14]);
      strTime += String(nowTime[15]);

      DateTime now = rtc.now();

      if ((strTime.toInt()) == now.minute()) {
        Serial.println("현재 분 이 같습니다...");
      } else {
        Serial.println("현재 분 이 다릅니다...");
        strTime = String(nowTime[0]);
        strTime += String(nowTime[1]);
        strTime += String(nowTime[2]);
        strTime += String(nowTime[3]);
        year = strTime.toInt();
        strTime = String(nowTime[5]);
        strTime += String(nowTime[6]);
        month = strTime.toInt();
        strTime = String(nowTime[8]);
        strTime += String(nowTime[9]);
        day = strTime.toInt();
        strTime = String(nowTime[11]);
        strTime += String(nowTime[12]);
        hour = strTime.toInt();
        strTime = String(nowTime[14]);
        strTime += String(nowTime[15]);
        min = strTime.toInt();
        strTime = String(nowTime[17]);
        strTime += String(nowTime[18]);
        sec = strTime.toInt();

        rtc.adjust(DateTime(year, month, day, hour, min, sec));
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      }
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
  s = s + "{" + "\"aux_v\":" + aux_v + ", " + "\"Temperature\":" + temperature +
      ", " + "\"Humidity\":" + humidity + ", " + "\"ledValue1\":" + ledPwm[1] +
      ", " + "\"ledValue2\":" + ledPwm[2] + ", " + "\"ledValue3\":" +
      ledPwm[3] + ", " + "\"ledValue4\":" + ledPwm[4] + ", " +
      "\"ledValue5\":" + ledPwm[5] + ", " + "\"ledValue6\":" + ledPwm[6] +
      ", " + "\"ledValue7\":" + ledPwm[7] + ", " + "\"ledValue8\":" +
      ledPwm[8] + ", " + "\"ledFlag1\":" + ledFlag[1] + ", " + "\"ledFlag2\":" +
      ledFlag[2] + ", " + "\"ledFlag3\":" + ledFlag[3] + ", " +
      "\"ledFlag4\":" + ledFlag[4] + ", " + "\"ledFlag5\":" + ledFlag[5] +
      ", " + "\"ledFlag6\":" + ledFlag[6] + ", " + "\"ledFlag7\":" +
      ledFlag[7] + ", " + "\"ledFlag8\":" + ledFlag[8] + ", " +
      "\"invertor\":" + invertorFlag + ", " + "\"tv\":" + tvFlag + ", " +
      "\"frige\":" + frigeFlag + ", " + "\"ledAllFlag\":" + ledAllFlag + ", " +
      "\"sleepFlag\":" + sleepFlag + ", " + "\"popUp\":" + popUpFlag + ", " +
      "\"popStop\":" + popStopFlag + ", " + "\"popDown\":" + popDownFlag + "}";

  server.send(200, "text/plain", s);

} // handleRequest()

void motor(int time) {
  digitalWrite(MOTOR, ON);
  delay(time);
  digitalWrite(MOTOR, OFF);
}

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

void colorInit();

void gestureLockUp(lv_event_t *e) {
  Serial.println("GestureLockUp...");
  gestureLockUpFlag = true;
}

void gestureLockDown(lv_event_t *e) {
  Serial.println("GestureLockDown...");
  if (gestureLockUpFlag) {
    lv_obj_add_flag(ui_ContainerLock, LV_OBJ_FLAG_HIDDEN);
    gestureLockUpFlag = false;
    scrLockFlag = true;
    scrLockCount = EEPROM.readByte(SCR_LOCK_COUNT_EEPROM);
  }
}

void wifiPick(lv_event_t *e) {
  char buf[50];

  Serial.println("WifiPick...");
  lv_roller_get_selected_str(ui_RollerHotSpot, buf, sizeof(buf));
  lv_textarea_set_text(ui_TextAreaSSID, buf);
  motor(motorTime);
}

void hotSpotSelect(lv_event_t *e) {

  String hotSpotList;
  char roll_text[500] = "";

  Serial.println("HotSpotSelect...");

  lv_obj_add_state(ui_SwitchNetwork, LV_STATE_CHECKED);
  lv_obj_add_state(ui_Label41, LV_STATE_CHECKED);
  lv_obj_clear_flag(ui_PanelNetwork, LV_OBJ_FLAG_HIDDEN);

  lv_timer_handler();
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      lv_timer_handler();
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
      hotSpotList += WiFi.SSID(i);
      hotSpotList += '\n';
    }
    hotSpotList.toCharArray(roll_text, hotSpotList.length());
    lv_roller_set_options(ui_RollerHotSpot, roll_text, LV_ROLLER_MODE_INFINITE);
  }
  lv_obj_add_flag(ui_SpinnerHotSpot, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_RollerHotSpot, LV_OBJ_FLAG_HIDDEN);
}

void ScreenHomeLoaded(lv_event_t *e) { Serial.println("ScreenHomeLoaded..."); }

void ssidClick(lv_event_t *e) {
  Serial.println("SsidClick...");

  lv_obj_set_style_outline_opa(ui_TextAreaSSID, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_TextAreaPSWD, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_TEXT_UPPER);
  lv_keyboard_set_textarea(ui_Keyboard1, ui_TextAreaSSID);

  motor(motorTime);
}

void pswdClick(lv_event_t *e) {
  Serial.println("PswdClick...");

  lv_obj_set_style_outline_opa(ui_TextAreaSSID, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_TextAreaPSWD, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_keyboard_set_mode(ui_Keyboard1, LV_KEYBOARD_MODE_NUMBER);
  lv_keyboard_set_textarea(ui_Keyboard1, ui_TextAreaPSWD);
  lv_obj_add_flag(ui_RollerHotSpot, LV_OBJ_FLAG_HIDDEN);

  motor(motorTime);
}

void keyboardClick(lv_event_t *e) {
  Serial.println("Keyboard Click...");
  motor(motorTime);
}

void networkSave(lv_event_t *e) {
  Serial.println("Network Save...");

  lv_obj_add_flag(ui_RollerHotSpot, LV_OBJ_FLAG_HIDDEN);
  ssidHotSpot = lv_textarea_get_text(ui_TextAreaSSID);
  pswdHotSpot = lv_textarea_get_text(ui_TextAreaPSWD);

  EEPROM.writeString(SSID_HOTSPOT_EEPROM, ssidHotSpot);
  EEPROM.writeString(PSWD_HOTSPOT_EEPROM, pswdHotSpot);
  EEPROM.commit();

  if (lv_obj_get_state(ui_SwitchNetwork) == CHECKED) {
    // ssidHotSpot.toCharArray(label_text, sizeof(label_text));
    ssidHotSpot.toCharArray(label_text, ssidHotSpot.length() + 1);
    lv_label_set_text(ui_LabelSSID, label_text);

    // pswdHotSpot.toCharArray(label_text, sizeof(label_text));
    pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
    lv_label_set_text(ui_LabelPSWD, label_text);

    s = "핫스팟 네트워크:";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label23, label_text);
    s = "핫스팟 비밀번호:";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label24, label_text);
  }

  // WiFi.disconnect();
  motor(50);
  delay(30);
  motor(50);
}

void wifiClick(lv_event_t *e) {
  Serial.println("WiFi Click...");
  if (networkFlag == STATION_MODE) {
    // hotSpotIP.toCharArray(label_text, sizeof(label_text));
    hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
    lv_label_set_text(ui_LabelMessage, label_text);
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
  }
  motor(motorTime);
}

// void TextAreaSSIDClick(lv_event_t *e) {
//   Serial.println("TextAreaSSIDClick Click...");
// }
// void TextAreaPSWDClick(lv_event_t *e) {
//   Serial.println("TextAreaPSWDClick Click...");
// }
// void networkSave(lv_event_t *e) { Serial.println("networkSave Click..."); }

void masterModeSelect(lv_event_t *e) {

  Serial.println("MasterModeSelect... ");

  if (lv_obj_get_state(ui_Switch1) == CHECKED) {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, MASTER_MODE);
    masterModeFlag = MASTER_MODE;
    Serial.println("Master Mode Selected...");

    lv_obj_clear_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
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
    Serial.println("Slave Mode Selected...");

    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
  }
  EEPROM.commit();
  motor(motorTime);
}

void networkSelect(lv_event_t *e) {
  Serial.println("NetworkSelect... ");

  if (lv_obj_get_state(ui_SwitchNetwork) == CHECKED) {
    // lv_obj_clear_flag(ui_PanelNetwork, LV_OBJ_FLAG_HIDDEN);
    EEPROM.writeBool(NETWORK_EEPROM, STATION_MODE);
    networkFlag = STATION_MODE;

    s = "핫스팟 네트워크:";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label23, label_text);
    // ssidHotSpot.toCharArray(label_text, sizeof(label_text));
    ssidHotSpot.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelSSID, label_text);

    s = "핫스팟 비밀번호:";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label24, label_text);
    // pswdHotSpot.toCharArray(label_text, sizeof(label_text));
    pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
    lv_label_set_text(ui_LabelPSWD, label_text);
    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);

    Serial.println("Hot Spot Mode Selected...");
  } else {
    EEPROM.writeBool(NETWORK_EEPROM, AP_MODE);
    networkFlag = AP_MODE;

    s = "와이파이 네트워크:";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label23, label_text);
    lv_label_set_text(ui_LabelSSID, ssidAp);

    s = "와이파이 비밀번호:";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label24, label_text);
    lv_label_set_text(ui_LabelPSWD, passwordAp);
    lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);

    Serial.println("AP Mode Selected...");
  }
  EEPROM.commit();
  motor(motorTime);
}

// void ScreenQRLoaded(lv_event_t *e) { ; }

void colorSave(lv_event_t *e) {

  Serial.println("ColorSave...");

  backColor = backColorPush;
  switchOnColor = switchOnColorPush;
  switchOffColor = switchOffColorPush;
  EEPROM.writeUInt(BACK_COLOR_EEPROM, backColor);
  EEPROM.writeUInt(SW_ON_EEPROM, switchOnColor);
  EEPROM.writeUInt(SW_OFF_EEPROM, switchOffColor);
  EEPROM.commit();
  colorInit();
  motor(50);
  delay(30);
  motor(50);
}

void addColors() {

  switch (colorSelect) {
  case 1:
    backColorPush = (static_cast<uint32_t>(sliderRedColor) << 16) |
                    (static_cast<uint32_t>(sliderGreenColor) << 8) |
                    static_cast<uint32_t>(sliderBlueColor);
    lv_obj_set_style_bg_color(ui_Container5, lv_color_hex(backColorPush),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 2:
    switchOnColorPush = (static_cast<uint32_t>(sliderRedColor) << 16) |
                        (static_cast<uint32_t>(sliderGreenColor) << 8) |
                        static_cast<uint32_t>(sliderBlueColor);
    lv_obj_set_style_bg_color(ui_Button16, lv_color_hex(switchOnColorPush),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 3:
    switchOffColorPush = (static_cast<uint32_t>(sliderRedColor) << 16) |
                         (static_cast<uint32_t>(sliderGreenColor) << 8) |
                         static_cast<uint32_t>(sliderBlueColor);
    lv_obj_set_style_bg_color(ui_Button17, lv_color_hex(switchOffColorPush),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  default:
    break;
  }
}

void SliderRed(lv_event_t *e) { // ScreenColor / SliderRed
  sliderRedColor = lv_slider_get_value(ui_SliderRed);
  Serial.print("Slider RED : ");
  Serial.println(sliderRedColor, HEX);

  // chgColor = (static_cast<uint32_t>(sliderRedColor) << 16) |
  //            (static_cast<uint32_t>(sliderGreenColor) << 8) |
  //            static_cast<uint32_t>(sliderBlueColor);

  Serial.print("chgColor : ");
  Serial.println(chgColor, HEX);
  addColors();
}
void SliderGreen(lv_event_t *e) { // ScreenColor / SliderGreen
  sliderGreenColor = lv_slider_get_value(ui_SliderGreen);
  Serial.print("Slider Green : ");
  Serial.println(sliderGreenColor, HEX);

  // chgColor = (static_cast<uint32_t>(sliderRedColor) << 16) |
  //            (static_cast<uint32_t>(sliderGreenColor) << 8) |
  //            static_cast<uint32_t>(sliderBlueColor);

  Serial.print("chgColor : ");
  Serial.println(chgColor, HEX);
  addColors();
}
void SliderBlue(lv_event_t *e) { // ScreenColor / SliderBkue
  sliderBlueColor = lv_slider_get_value(ui_SliderBlue);
  Serial.print("Slider BLUE : ");
  Serial.println(sliderBlueColor, HEX);

  // chgColor = (static_cast<uint32_t>(sliderRedColor) << 16) |
  //            (static_cast<uint32_t>(sliderGreenColor) << 8) |
  //            static_cast<uint32_t>(sliderBlueColor);

  Serial.print("chgColor : ");
  Serial.println(chgColor, HEX);
  addColors();
}

void colorReset(lv_event_t *e) { // ScreenColor / Button31

  Serial.println("ColorReset...");

  uint8_t red, green, blue;

  colorSelect = 1;
  backColorPush = 0xFA9F10;
  switchOffColorPush = 0x1B91E9;
  switchOnColorPush = 0x00FFA1;

  lv_obj_set_style_outline_opa(ui_Container6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container7, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container8, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  // 메인 창의 바탕색...
  lv_obj_set_style_bg_color(ui_Container5, lv_color_hex(backColorPush),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // ScreenColor 창의 스위치 꺼짐색...
  lv_obj_set_style_bg_color(ui_Button16, lv_color_hex(switchOffColorPush),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // ScreenColor 창의 스위치 켜짐색...
  lv_obj_set_style_bg_color(ui_Button17, lv_color_hex(switchOnColorPush),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_add_state(ui_Checkbox1, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Checkbox2, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Checkbox3, LV_STATE_CHECKED);

  red = static_cast<uint8_t>((backColorPush >> 16) & 0xFF);
  s = red;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelRed, label_text);
  lv_slider_set_value(ui_SliderRed, red, LV_ANIM_OFF);

  green = static_cast<uint8_t>((backColorPush >> 8) & 0xFF);
  s = green;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelGreen, label_text);
  lv_slider_set_value(ui_SliderGreen, green, LV_ANIM_OFF);

  blue = static_cast<uint8_t>(backColorPush & 0xFF);
  s = blue;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBlue, label_text);
  lv_slider_set_value(ui_SliderBlue, blue, LV_ANIM_OFF);
}

void backGroundCheck(lv_event_t *e) { // ScreenColor / Container6
  uint8_t red, green, blue;

  red = static_cast<uint8_t>((backColorPush >> 16) & 0xFF);
  green = static_cast<uint8_t>((backColorPush >> 8) & 0xFF);
  blue = static_cast<uint8_t>(backColorPush & 0xFF);

  colorSelect = 1;
  lv_obj_set_style_outline_opa(ui_Container6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container7, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container8, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = red;
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelRed, label_text);
  lv_slider_set_value(ui_SliderRed, red, LV_ANIM_OFF);
  s = green;
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelGreen, label_text);
  lv_slider_set_value(ui_SliderGreen, green, LV_ANIM_OFF);
  s = blue;
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBlue, label_text);
  lv_slider_set_value(ui_SliderBlue, blue, LV_ANIM_OFF);
}

void switchOnCheck(lv_event_t *e) { // ScreenColor / Container8
  uint8_t red, green, blue;

  red = static_cast<uint8_t>((switchOnColorPush >> 16) & 0xFF);
  green = static_cast<uint8_t>((switchOnColorPush >> 8) & 0xFF);
  blue = static_cast<uint8_t>(switchOnColorPush & 0xFF);

  colorSelect = 2;
  lv_obj_set_style_outline_opa(ui_Container6, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container8, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_clear_state(ui_Checkbox1, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Checkbox2, LV_STATE_CHECKED);
  // lv_obj_add_state(ui_Checkbox3, LV_STATE_CHECKED);

  s = red;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelRed, label_text);
  lv_slider_set_value(ui_SliderRed, red, LV_ANIM_OFF);
  s = green;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelGreen, label_text);
  lv_slider_set_value(ui_SliderGreen, green, LV_ANIM_OFF);
  s = blue;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBlue, label_text);
  lv_slider_set_value(ui_SliderBlue, blue, LV_ANIM_OFF);
}

void switchOffCheck(lv_event_t *e) { // ScreenColor / Container7
  uint8_t red, green, blue;

  red = static_cast<uint8_t>((switchOffColorPush >> 16) & 0xFF);
  green = static_cast<uint8_t>((switchOffColorPush >> 8) & 0xFF);
  blue = static_cast<uint8_t>(switchOffColorPush & 0xFF);

  colorSelect = 3;
  lv_obj_set_style_outline_opa(ui_Container6, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container7, 0,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(ui_Container8, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_clear_state(ui_Checkbox1, LV_STATE_CHECKED);
  // lv_obj_add_state(ui_Checkbox2, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Checkbox3, LV_STATE_CHECKED);

  s = red;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelRed, label_text);
  lv_slider_set_value(ui_SliderRed, red, LV_ANIM_OFF);
  s = green;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelGreen, label_text);
  lv_slider_set_value(ui_SliderGreen, green, LV_ANIM_OFF);
  s = blue;
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBlue, label_text);
  lv_slider_set_value(ui_SliderBlue, blue, LV_ANIM_OFF);
}

void SleepTimeSave(lv_event_t *e) {
  Serial.println("SleepTimeSave...");

  char buf[10];

  lv_roller_get_selected_str(ui_Roller4, buf, sizeof(buf));
  s = buf;
  sleepCount = s.toInt();
  EEPROM.writeByte(SLEEP_COUNT_EEPROM, sleepCount);
  EEPROM.commit();

  lv_roller_get_selected_str(ui_Roller1, buf, sizeof(buf));
  s = buf;
  scrLockCount = s.toInt();
  if (scrLockCount == 0)
    scrLockFlag = false;
  else
    scrLockFlag = true;
  EEPROM.writeByte(SCR_LOCK_COUNT_EEPROM, scrLockCount);
  EEPROM.commit();

  Serial.print("Sleep Count : ");
  Serial.println(sleepCount);
  Serial.print(", Screen Lock Count : ");
  Serial.println(scrLockCount);

  motor(50);
  delay(30);
  motor(50);
}

void panelMessageClick(lv_event_t *e) {
  Serial.print("PanelMessage Click...");
  sleepFlag = false;
  sleepStartFlag = false;
  sleepCount = EEPROM.readByte(SLEEP_COUNT_EEPROM);
  s = "취침모드 해제...";
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelMessage, label_text);
  delay(600);
  lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
  motor(motorTime);
}

// void sleepLongPress(lv_event_t *e) {
//   Serial.print("SleepLongPress Click...");
//   lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN);
//   lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_HIDDEN);
//   lv_timer_handler();
// }

void sleepButton(lv_event_t *e) {
  Serial.println("sleepButton... ");

  if (sleepFlag) { // 취침모드가 아니면...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
    ledSelect = 9;
    s = "전체등";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else { // 취침모드 이면...
    sleepFlag = true;
    if (sleepCount != 0) {
      sleepStartFlag = true;
      s = "0";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      // lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
      lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
      s = "취침중";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
      s = "초 후에 취침모드로 진입 합니다.";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelMessage, label_text);
    } else {
      lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      sleepCount = EEPROM.readByte(SLEEP_COUNT_EEPROM);
      lv_obj_clear_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
      for (int i = 255; i > 0; i = i - 5) {
        tft.setBrightness(i);
        delay(50);
        server.handleClient();
      }
      ledSelect = 0;
      ledFlag[1] = false;
      ledFlag[2] = false;
      ledFlag[3] = false;
      ledFlag[4] = false;
      ledFlag[5] = false;
      ledFlag[6] = false;
      ledFlag[7] = false;
      ledFlag[8] = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);
      lv_label_set_text(ui_LabelBright, "0");
      lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
      s = "취침중";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
      tft.setBrightness(1);
      // lv_disp_load_scr(ui_ScreenClock);
      sleepStartFlag = false;
      ledAllFlag = false;
      comSend("LED_ALL:0");
    }
  }
  lv_timer_handler();
  delay(500);
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
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
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
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
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
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
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
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
}

void led5Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[5] || ledSelect == 5)
    return;
  ledSelect = 5;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
}

void led6Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[6] || ledSelect == 6)
    return;
  ledSelect = 6;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
}

void led7Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[7] || ledSelect == 7)
    return;
  ledSelect = 7;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
}

void led8Long(lv_event_t *e) {
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button8, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  if (!ledFlag[8] || ledSelect == 8)
    return;
  ledSelect = 8;
  s = ledPwm[ledSelect];
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelBright, label_text);
  lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
  // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
  ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  motor(50);
  delay(30);
  motor(50);
}

void comSend(String sendData) {
  // lv_timer_handler();
  digitalWrite(RTS, HIGH);
  // delay(5);
  // s = command;
  // s += ':';
  // s += value;
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(RTS, LOW);
}

void pwmChangeSlider(lv_event_t *e) {
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
    // comSend("LED1", s.toInt());
    break;
  case 2: // LED 2
    ledPwm[2] = s.toInt();
    sendData = "LED2:";
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    // comSend("LED2", s.toInt());
    break;
  case 3: // LED 3
    ledPwm[3] = s.toInt();
    sendData = "LED3:";
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    // comSend("LED3", s.toInt());
    break;
  case 4: // LED 4
    ledPwm[4] = s.toInt();
    sendData = "LED4:";
    lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    // comSend("LED4", s.toInt());
    break;
  case 5: // LED 5
    ledPwm[5] = s.toInt();
    sendData = "LED5:";
    lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    // comSend("LED5", s.toInt());
    break;
  case 6: // LED 6
    ledPwm[6] = s.toInt();
    sendData = "LED6:";
    lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    // comSend("LED6", s.toInt());
    break;
  case 7: // LED 7
    ledPwm[7] = s.toInt();
    sendData = "LED7:";
    lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    // comSend("LED7", s.toInt());
    break;
  case 8: // LED 8
    ledPwm[8] = s.toInt();
    sendData = "LED8:";
    lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    // comSend("LED8", s.toInt());
    break;
  case 9: // LED All
    ledPwm[9] = s.toInt();
    if (ledPwm[9] != 0) {
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
      lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
    }
    sendData = "LED_ALL:";
    break;
  default:
    break;
  }
  sendData += s.toInt();
  comSend(sendData);
}

void brightSlide(lv_event_t *e) { // 화면 밝기 조절...
  bright = lv_slider_get_value(ui_Slider3);
  Serial.println("Bright Slider... ");
  Serial.println(bright);
  if (bright < 10)
    tft.setBrightness(1);
  else
    tft.setBrightness(bright);
};

void ledAllProcess() {
  if (!ledFlag[1] && !ledFlag[2] && !ledFlag[3] && !ledFlag[4] && !ledFlag[5] &&
      !ledFlag[6] && !ledFlag[7] && !ledFlag[8]) {
    ledAllFlag = false;
    lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);
  } else if (ledFlag[1] && ledFlag[2] && ledFlag[3] && ledFlag[4] &&
             ledFlag[5] && ledFlag[6] && ledFlag[7] && ledFlag[8]) {
    ledAllFlag = true;
    lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
  }
}

void ledButton1(lv_event_t *e) {
  ledSelect = 1;
  state = lv_obj_get_state(ui_Button1);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

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
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED1:";
  s += state == CHECKED ? ledPwm[1] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton2(lv_event_t *e) {
  ledSelect = 2;
  state = lv_obj_get_state(ui_Button2);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton2...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button2, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED2:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton3(lv_event_t *e) {
  ledSelect = 3;
  state = lv_obj_get_state(ui_Button3);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton3...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED3:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton4(lv_event_t *e) {
  ledSelect = 4;
  state = lv_obj_get_state(ui_Button4);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton4...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button4, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED4:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton5(lv_event_t *e) {
  ledSelect = 5;
  state = lv_obj_get_state(ui_Button5);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton5...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button5, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED5:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton6(lv_event_t *e) {
  ledSelect = 6;
  state = lv_obj_get_state(ui_Button6);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton6...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button6, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED6:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton7(lv_event_t *e) {
  ledSelect = 7;
  state = lv_obj_get_state(ui_Button7);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton7...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button7, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED7:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton8(lv_event_t *e) {
  ledSelect = 8;
  state = lv_obj_get_state(ui_Button8);

  if (sleepFlag) { // 취침모드 이면 해제...
    sleepFlag = false;
    sleepStartFlag = false;
    lv_obj_add_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
    tft.setBrightness(255);
  }

  Serial.println("ledButton8...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button8, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    s = ledPwm[ledSelect];
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  } else {
    ledFlag[ledSelect] = false;
    lv_label_set_text(ui_LabelBright, "0");
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
    // ledName[ledSelect].toCharArray(label_text, sizeof(label_text));
    ledName[ledSelect].toCharArray(label_text, ledName[ledSelect].length() + 1);
    lv_label_set_text(ui_LabelLED, label_text);
  }
  s = "LED8:";
  s += state == CHECKED ? ledPwm[ledSelect] : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton9(lv_event_t *e) {
  comMaskFlag = true;
  ledSelect = 9;

  Serial.println("ledButton9...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button9, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (ledAllFlag)
    ledAllFlag = false;
  else
    ledAllFlag = true;

  // if (state != CHECKED) {
  if (!ledAllFlag) {
    ledFlag[1] = false;
    ledFlag[2] = false;
    ledFlag[3] = false;
    ledFlag[4] = false;
    ledFlag[5] = false;
    ledFlag[6] = false;
    ledFlag[7] = false;
    ledFlag[8] = false;
    ledAllFlag = false;
    lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);

    s = '0';
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
  } else { // 값이 1 이상이면...
    ledFlag[1] = true;
    ledFlag[2] = true;
    ledFlag[3] = true;
    ledFlag[4] = true;
    ledFlag[5] = true;
    ledFlag[6] = true;
    ledFlag[7] = true;
    ledFlag[8] = true;
    ledAllFlag = true;
    lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
    s = 100;
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelBright, label_text);
    lv_slider_set_value(ui_Slider1, 100, LV_ANIM_OFF);
  }
  s = "전체등";
  // s.toCharArray(label_text, sizeof(label_text));
  s.toCharArray(label_text, s.length() + 1);
  lv_label_set_text(ui_LabelLED, label_text);
  s = "LED_ALL:";
  s += ledAllFlag == true ? ledPwm[ledSelect] : 0;
  comSend(s);
  delay(5);
  // comSend("REQUEST:1");

  motor(motorTime);
  comMaskFlag = false;
}

void invButton(lv_event_t *e) {
  state = lv_obj_get_state(ui_Button10);

  Serial.println("invButton...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button10, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "INV:";
  if (state == 3) {
    invertorFlag = true;
    s += '1';
  } else {
    invertorFlag = false;
    s += '0';
  }
  comSend(s);
  motor(motorTime);
}

void tvButton(lv_event_t *e) {
  state = lv_obj_get_state(ui_Button11);

  Serial.println("tvButton...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button11, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "TV:";
  if (state == 3) {
    tvFlag = true;
    s += '1';
  } else {
    tvFlag = false;
    s += '0';
  }
  comSend(s);
  motor(motorTime);
}

void frigeButton(lv_event_t *e) {
  state = lv_obj_get_state(ui_Button12);

  Serial.println("frigeButton...");
  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button12, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  s = "FRIGE:";
  if (state == 3) {
    frigeFlag = true;
    s += '1';
  } else {
    frigeFlag = false;
    s += '0';
  }
  comSend(s);
  motor(motorTime);
}

void brightQR(lv_event_t *e) {
  uint8_t opa = lv_slider_get_value(ui_Slider2);
  Serial.print("brightQR... ");
  Serial.println(opa);
  lv_obj_set_style_opa(ui_ImageQR, opa, 0);
}

void screenSleepIntro(lv_event_t *e) {
  Serial.println("ScreenSleepIntro...");
  scrLockFlag = false;
}

void chgScreen2(lv_event_t *e) { Serial.println("chgScreen2..."); }

void serialEvent2() {

  if (comMaskFlag)
    return;

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
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[1] = true;
      ledPwm[1] = _value.toInt();
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    } else {
      ledFlag[1] = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    }
  } else if (_division == "LED2") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[2] = true;
      ledPwm[2] = _value.toInt();
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    } else {
      ledFlag[2] = false;
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    }
  } else if (_division == "LED3") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[3] = true;
      ledPwm[3] = _value.toInt();
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    } else {
      ledFlag[3] = false;
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    }
  } else if (_division == "LED4") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[4] = true;
      ledPwm[4] = _value.toInt();
      lv_obj_add_state(ui_Button4, LV_STATE_CHECKED);
    } else {
      ledFlag[4] = false;
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    }
  } else if (_division == "LED5") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[5] = true;
      ledPwm[5] = _value.toInt();
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    } else {
      ledFlag[5] = false;
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    }
  } else if (_division == "LED6") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[6] = true;
      ledPwm[6] = _value.toInt();
      lv_obj_add_state(ui_Button6, LV_STATE_CHECKED);
    } else {
      ledFlag[6] = false;
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
    }
  } else if (_division == "LED7") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[7] = true;
      ledPwm[7] = _value.toInt();
      lv_obj_add_state(ui_Button7, LV_STATE_CHECKED);
    } else {
      ledFlag[7] = false;
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
    }
  } else if (_division == "LED8") {
    comInitFlag = true;
    if (_flag == "1") {
      ledFlag[8] = true;
      ledPwm[8] = _value.toInt();
      lv_obj_add_state(ui_Button8, LV_STATE_CHECKED);
    } else {
      ledFlag[8] = false;
      lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
    }
  } else if (_division == "LED_ALL") {
    comInitFlag = true;
    if (_value != "0") {
      ledAllFlag = true;
      lv_obj_add_state(ui_Button9, LV_STATE_CHECKED);
      s = 100;
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelBright, label_text);
      lv_slider_set_value(ui_Slider1, 100, LV_ANIM_OFF);
    } else {
      ledAllFlag = false;
      lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);
    }
  } else if (_division == "INV") {
    comInitFlag = true;
    if (_value != "0") {
      invertorFlag = true;
      lv_obj_add_state(ui_Button10, LV_STATE_CHECKED);
    } else {
      invertorFlag = false;
      lv_obj_clear_state(ui_Button10, LV_STATE_CHECKED);
    }
  } else if (_division == "TV") {
    comInitFlag = true;
    if (_value != "0") {
      tvFlag = true;
      lv_obj_add_state(ui_Button11, LV_STATE_CHECKED);
    } else {
      tvFlag = false;
      lv_obj_clear_state(ui_Button11, LV_STATE_CHECKED);
    }
  } else if (_division == "FRIGE") {
    comInitFlag = true;
    if (_value != "0") {
      frigeFlag = true;
      lv_obj_add_state(ui_Button12, LV_STATE_CHECKED);
    } else {
      frigeFlag = false;
      lv_obj_clear_state(ui_Button12, LV_STATE_CHECKED);
    }
  } else if (_division == "VOLT") {
    _value.toCharArray(label_text, _value.length());
    lv_label_set_text(ui_LabelVolt, label_text);
    aux_v = _value.toFloat();
  } else if (_division == "POP_STOP") {
    popStopFlag = true;
    popUpFlag = false;
    popDownFlag = false;
  }
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
    lv_obj_clear_flag(ui_ImageHotSpot,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    s = "핫스팟 연결중 입니다...";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelMessage, label_text);
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);

    for (int i = 1; i < 10; i++) {
      s = i;
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelSecCount, label_text);
      // int _count = 0;
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

    s = " ";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelSecCount, label_text);

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

  if (sleepStartFlag) {
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
    s = sleepCount;
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelSecCount, label_text);
    if (sleepCount == 0) {
      lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      sleepCount = EEPROM.readByte(SLEEP_COUNT_EEPROM);
      lv_obj_clear_flag(ui_ImageSleepOn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_ImageSleepOff, LV_OBJ_FLAG_HIDDEN);
      for (int i = 255; i > 0; i = i - 5) {
        tft.setBrightness(i);
        delay(50);
        server.handleClient();
      }
      ledSelect = 0;
      ledFlag[1] = false;
      ledFlag[2] = false;
      ledFlag[3] = false;
      ledFlag[4] = false;
      ledFlag[5] = false;
      ledFlag[6] = false;
      ledFlag[7] = false;
      ledFlag[8] = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button6, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button7, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button8, LV_STATE_CHECKED);
      lv_obj_clear_state(ui_Button9, LV_STATE_CHECKED);
      lv_label_set_text(ui_LabelBright, "0");
      lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
      s = "취침중";
      // s.toCharArray(label_text, sizeof(label_text));
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
      tft.setBrightness(1);
      // lv_disp_load_scr(ui_ScreenClock);
      sleepStartFlag = false;
      ledAllFlag = false;
      comSend("LED_ALL:0");
    }

    --sleepCount;
  }

  if (countValue++ > 1) {

    // if (ahtFlag)
    //   myAHT10.getEvent(
    //       &adsHumidity,
    //       &adsTemp); // populate temp and humidity objects with fresh data
    // Serial.print(" 온도 : ");
    // temperature = round(adsTemp.temperature * 10) / 10;
    // Serial.print(temperature);
    // Serial.println(" °C");
    // Serial.print(" 습도 : ");
    // humidity = round(adsHumidity.relative_humidity * 10) / 10;
    // Serial.print(adsHumidity.relative_humidity);
    // Serial.print(humidity);
    // Serial.println(" %");

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

    if (sensorDHT) {
      humidity = dht.readHumidity();
      temperature = dht.readTemperature();
    } else {
      humidity = 0.0;
      temperature = 0.0;
    }

    s = String(temperature, 1);
    s += " 도";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelTemp, label_text);

    s = String(humidity, 1);
    s += " %";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelHumi, label_text);

    if (wifiCount > 2) { // WiFi 가 연결 안되어 있으면...
      lv_obj_add_flag(ui_ImageWiFi,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
      lv_obj_add_flag(ui_ImageHotSpot,
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

  if (scrLockFlag) {
    if (scrLockCount == 0) {
      lv_obj_clear_flag(ui_ContainerLock, LV_OBJ_FLAG_HIDDEN);
      scrLockFlag = false;
    } else
      scrLockCount--;
  }
  Serial.print("ScrLockFlag: ");
  Serial.print(scrLockFlag ? "True,  " : "False,  ");
  Serial.print("ScrLockCount: ");
  Serial.println(scrLockCount);
}

void ledNameInit() {

  // if (ledNameOrigin[0] == "reset") { // LED 이름 초기화...
  //   EEPROM.writeString(LED1_NAME_EEPROM, ledNameOrigin[1]);
  //   EEPROM.writeString(LED2_NAME_EEPROM, ledNameOrigin[2]);
  //   EEPROM.writeString(LED3_NAME_EEPROM, ledNameOrigin[3]);
  //   EEPROM.writeString(LED4_NAME_EEPROM, ledNameOrigin[4]);
  //   EEPROM.writeString(LED5_NAME_EEPROM, ledNameOrigin[5]);
  //   EEPROM.writeString(LED6_NAME_EEPROM, ledNameOrigin[6]);
  //   EEPROM.writeString(LED7_NAME_EEPROM, ledNameOrigin[7]);
  //   EEPROM.writeString(LED8_NAME_EEPROM, ledNameOrigin[8]);
  // } else {
  ledName[1] = EEPROM.readString(LED1_NAME_EEPROM);
  ledName[2] = EEPROM.readString(LED2_NAME_EEPROM);
  ledName[3] = EEPROM.readString(LED3_NAME_EEPROM);
  ledName[4] = EEPROM.readString(LED4_NAME_EEPROM);
  ledName[5] = EEPROM.readString(LED5_NAME_EEPROM);
  ledName[6] = EEPROM.readString(LED6_NAME_EEPROM);
  ledName[7] = EEPROM.readString(LED7_NAME_EEPROM);
  ledName[8] = EEPROM.readString(LED8_NAME_EEPROM);
  // }
  // EEPROM.commit();

  // ledName[1].toCharArray(label_text, sizeof(label_text));
  ledName[1].toCharArray(label_text, ledName[1].length() + 1);
  lv_label_set_text(ui_Label1, label_text);
  // ledName[2].toCharArray(label_text, sizeof(label_text));
  ledName[2].toCharArray(label_text, ledName[2].length() + 1);
  lv_label_set_text(ui_Label2, label_text);
  // ledName[3].toCharArray(label_text, sizeof(label_text));
  ledName[3].toCharArray(label_text, ledName[3].length() + 1);
  lv_label_set_text(ui_Label3, label_text);
  // ledName[4].toCharArray(label_text, sizeof(label_text));
  ledName[4].toCharArray(label_text, ledName[4].length() + 1);
  lv_label_set_text(ui_Label4, label_text);
  // ledName[5].toCharArray(label_text, sizeof(label_text));
  ledName[5].toCharArray(label_text, ledName[5].length() + 1);
  lv_label_set_text(ui_Label5, label_text);
  // ledName[6].toCharArray(label_text, sizeof(label_text));
  ledName[6].toCharArray(label_text, ledName[6].length() + 1);
  lv_label_set_text(ui_Label6, label_text);
  // ledName[7].toCharArray(label_text, sizeof(label_text));
  ledName[7].toCharArray(label_text, ledName[7].length() + 1);
  lv_label_set_text(ui_Label7, label_text);
  // ledName[8].toCharArray(label_text, sizeof(label_text));
  ledName[8].toCharArray(label_text, ledName[8].length() + 1);
  lv_label_set_text(ui_Label8, label_text);

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
  Serial.print("LED6 Name : ");
  Serial.println(ledName[6]);
  Serial.print("LED7 Name : ");
  Serial.println(ledName[7]);
  Serial.print("LED8 Name : ");
  Serial.println(ledName[8]);
}

void colorInit() {
  if (EEPROM.readUInt(BACK_COLOR_EEPROM) > 0xFFFFFF) { // 색상 초기화...
    backColor = 0xFA9F10;
    switchOnColor = 0x00FFA1;
    switchOffColor = 0x1B91E9;
    EEPROM.writeUInt(BACK_COLOR_EEPROM, backColor);
    EEPROM.writeUInt(SW_ON_EEPROM, switchOnColor);
    EEPROM.writeUInt(SW_OFF_EEPROM, switchOffColor);
    EEPROM.commit();
  } else {
    backColor = EEPROM.readUInt(BACK_COLOR_EEPROM);
    switchOnColor = EEPROM.readUInt(SW_ON_EEPROM);
    switchOffColor = EEPROM.readUInt(SW_OFF_EEPROM);
  }

  backColorPush = backColor;
  switchOnColorPush = switchOnColor;
  switchOffColorPush = switchOffColor;

  Serial.print("backColor : ");
  Serial.println(backColor, HEX);
  Serial.print("switchOnColor : ");
  Serial.println(switchOnColor, HEX);
  Serial.print("SwitchOffColor : ");
  Serial.println(switchOffColor, HEX);

  // 메인화면 바탕색...
  lv_obj_set_style_bg_color(ui_ScreenHome, lv_color_hex(backColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 1 Off 색...
  lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 2 Off 색...
  lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 3 Off 색...
  lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 4 Off 색...
  lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 5 Off 색...
  lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 6 Off 색...
  lv_obj_set_style_bg_color(ui_Button6, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 7 Off 색...
  lv_obj_set_style_bg_color(ui_Button7, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 8 Off 색...
  lv_obj_set_style_bg_color(ui_Button8, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 9 Off 색...
  lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 10 Off 색...
  lv_obj_set_style_bg_color(ui_Button10, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 11 Off 색...
  lv_obj_set_style_bg_color(ui_Button11, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 12 Off 색...
  lv_obj_set_style_bg_color(ui_Button12, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // 버튼 1 On 색...
  lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 2 On 색...
  lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 3 On 색...
  lv_obj_set_style_bg_color(ui_Button3, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 4 On 색...
  lv_obj_set_style_bg_color(ui_Button4, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 5 On 색...
  lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 6 On 색...
  lv_obj_set_style_bg_color(ui_Button6, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 7 On 색...
  lv_obj_set_style_bg_color(ui_Button7, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 8 On 색...
  lv_obj_set_style_bg_color(ui_Button8, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 9 On 색...
  lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 10 On 색...
  lv_obj_set_style_bg_color(ui_Button10, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 11 On 색...
  lv_obj_set_style_bg_color(ui_Button11, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // 버튼 12 On 색...
  lv_obj_set_style_bg_color(ui_Button12, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_CHECKED);
  // ScreenColor 창의 배경색...
  lv_obj_set_style_bg_color(ui_Container5, lv_color_hex(backColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // ScreenColor 창의 스위치 켜짐색...
  lv_obj_set_style_bg_color(ui_Button16, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // ScreenColor 창의 스위치 켜짐색...
  lv_obj_set_style_bg_color(ui_Button17, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // ScreenColor 창의 스위치 꺼짐색...
  lv_obj_set_style_bg_color(ui_Button16, lv_color_hex(switchOnColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  // ScreenColor 창의 스위치 꺼짐색...
  lv_obj_set_style_bg_color(ui_Button17, lv_color_hex(switchOffColor),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
}

void setup() {

  pinMode(SEC, INPUT);
  pinMode(MOTOR, OUTPUT);
  pinMode(RTS, OUTPUT);

  Serial.begin(115200); /* prepare for possible serial debug */
  // Serial2.begin(19200, SERIAL_8N1, 1, 42);
  Serial2.begin(115200, SERIAL_8N1, 1, 42);

  Serial.flush();
  Serial2.flush();
  // comSend("REQUEST:1");

  digitalWrite(MOTOR, LOW);
  digitalWrite(RTS, LOW);

  EEPROM.begin(250);

  // Serial.print("SLEEP_COUNT_EEPROM : ");
  // Serial.println(EEPROM.readUInt(SLEEP_COUNT_EEPROM));

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  int ahtStatus = 0;

  // Wire1.begin(12, 11, 100000); // Input Board 통신용...

  // for (int i = 0; i < 3; i++) {
  //   if (rtc.begin(&Wire1) != true) {
  //     Serial.println("RTC Not Found");
  //     delay(50);
  //   } else {
  //     rtcFlag = true;
  //     Serial.println("RTC found");
  //     break;
  //   }
  // }

  // if (rtcFlag)
  //   rtc.writeSqwPinMode(DS1307_SquareWave1HZ);

  ssidHotSpot = EEPROM.readString(SSID_HOTSPOT_EEPROM);
  pswdHotSpot = EEPROM.readString(PSWD_HOTSPOT_EEPROM);

  masterModeFlag = EEPROM.readBool(MASTER_SLAVE_EEPROM);
  networkFlag = EEPROM.readBool(NETWORK_EEPROM);

  Serial.print("Master Slave Mode : ");
  Serial.println(masterModeFlag);
  Serial.print("Network Mode : ");
  Serial.println(networkFlag);

  Serial.print("Master Slave Mode : ");
  Serial.println(masterModeFlag == MASTER_MODE ? "Master mode" : "Slave Mode");
  Serial.print("Network Mode : ");
  Serial.println(networkFlag == STATION_MODE ? "Station mode" : "AP Mode");

  if (EEPROM.readByte(SLEEP_COUNT_EEPROM) > 100) {
    EEPROM.writeByte(SLEEP_COUNT_EEPROM, 3);
    EEPROM.commit();
  } else
    sleepCount = EEPROM.readByte(SLEEP_COUNT_EEPROM);

  if (EEPROM.readByte(SCR_LOCK_COUNT_EEPROM) > 100) {
    EEPROM.writeByte(SCR_LOCK_COUNT_EEPROM, 0);
    EEPROM.commit();
  } else
    scrLockCount = EEPROM.readByte(SCR_LOCK_COUNT_EEPROM);

  lv_init();

  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();

  lv_obj_set_x(ui_PanelButton, 0);
  lv_obj_set_x(ui_PanelDisplay, -140);
  tft.setBrightness(255);
  lv_obj_set_style_opa(ui_ImageQR, 127, 0);
  PanelButton_Animation(ui_PanelButton, 1000);
  PanelDisplay_Animation(ui_PanelDisplay, 1000);

  Wire1.begin(12, 11, 100000); // Input Board 통신용...
  delay(50);
  for (int i = 0; i < 10; i++) {
    if (rtc.begin(&Wire1) != true) {
      Serial.println("RTC Not Found");
      delay(50);
    } else {
      rtcFlag = true;
      Serial.println("RTC found");
      break;
    }
  }
  clockRead();

  for (int i = 0; i < 150; i++) {
    lv_timer_handler();
    delay(5);
  }

  dht.begin();

  ledNameInit();
  colorInit();

  if (scrLockCount == 0) {
    scrLockFlag = false;
    lv_roller_set_selected(ui_Roller1, scrLockCount,
                           LV_ANIM_ON); // Screen Lock Time
  } else {
    scrLockFlag = true;
    lv_roller_set_selected(ui_Roller1, scrLockCount - 2,
                           LV_ANIM_ON); // Screen Lock Time
  }
  lv_roller_set_selected(ui_Roller4, sleepCount, LV_ANIM_ON); // Sleep Time

  if (networkFlag) // true : Tethering Mode, false : WiFi
  {                // 핫스팟 Mode...
    lv_obj_add_state(ui_SwitchNetwork, LV_STATE_CHECKED);
    lv_obj_add_state(ui_Label41, LV_STATE_CHECKED);
    lv_obj_clear_flag(ui_SpinnerHotSpot, LV_OBJ_FLAG_HIDDEN);
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
    lv_obj_clear_state(ui_SwitchNetwork, LV_STATE_CHECKED);
    s = "와이파이 네트워크:";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label23, label_text);

    s = "와이파이 비밀번호:";
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_Label24, label_text);
  }
  ssidHotSpot.toCharArray(label_text, ssidHotSpot.length() + 1);
  lv_textarea_set_text(ui_TextAreaSSID, label_text);
  pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
  lv_textarea_set_text(ui_TextAreaPSWD, label_text);

  if (!masterModeFlag) // true : Master Mode, false : Slave Mode
  {
    lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);

    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);

    lv_obj_clear_flag(ui_Label40, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_SwitchNetwork, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label41, LV_OBJ_FLAG_HIDDEN);
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

  wifiStart();

  server.on("/json", handleJson);
  server.on("/root", handleRoot);
  server.on("/Request", handleRequest);

  server.begin();

  clockRead();
  bright = lv_slider_get_value(ui_Slider3);

  Serial.print("BACK_COLOR_EEPROM : ");
  Serial.println(EEPROM.readUInt(BACK_COLOR_EEPROM), HEX);

  Serial.println("Setup done");
  Serial2.flush();

  s = "AP_SSID:";
  s += ssidAp;
  comSend("COM:1");
  comSend(s);
  delay(10);

  s = "AP_PSWD:";
  s += passwordAp;
  comSend(s);
  delay(10);

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  Serial.print("humidity : ");
  Serial.print(humidity);
  Serial.print(",  temperature : ");
  Serial.println(temperature);

  if (isnan(humidity)) {
    Serial.println("Humidity is nan...");
  }
  if (isnan(temperature)) {
    Serial.println("Humidity is nan...");
  }

  if (isnan(humidity) || isnan(temperature))
    sensorDHT = false;
  else
    sensorDHT = true;
}

void loop() {

  lv_timer_handler(); /* let the GUI do its work */
  server.handleClient();

  if (loopCount > 180) { // 약 1초 마다 실실행...
    int8_t rssi = WiFi.RSSI();
    Serial.print("RSSI: ");

    if (networkChangeFlag) {
      WiFi.disconnect();
      wifiStart();
      networkChangeFlag = false;
    }

    Serial.println(rssi);

    if (!comMaskFlag)
      eventSec();

    if (rssi != 0) // Hot Spot Icon 표시 여부...
      lv_obj_clear_flag(ui_ImageHotSpot,
                        LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    else
      lv_obj_add_flag(ui_ImageHotSpot,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);

    if ((networkFlag == STATION_MODE) && (WiFi.status() != WL_CONNECTED)) {
      hotSpotIpSetFlag = false;
      // WiFi.reconnect();
      // WiFi.begin("CamTrol V6.0", "01234567");
      WiFi.begin(ssidHotSpot, pswdHotSpot);
      Serial.println("Reconnecting to WiFi...");
    } else if (!hotSpotIpSetFlag && (WiFi.status() == WL_CONNECTED)) {
      hotSpotIpSetFlag = true;
      hotSpotIP = WiFi.localIP().toString();
      // hotSpotIP.toCharArray(label_text, sizeof(label_text));
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
    if (!comInitFlag) // 초기에 메인 콘트롤러로 부터 내용 가져오기...
    {
      comSend("REQUEST:1");
    }
    comSend("COM:1");
  }

  delay(5);
  loopCount++;
  // Serial.print("Loop Count : ");
  // Serial.print(loopCount);
}
