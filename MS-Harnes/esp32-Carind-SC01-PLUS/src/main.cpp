//*********************************************************************************************************/
// 카인드 캠핑카 V1.0
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
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <RTClib.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

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
void comSend(String sendData);

RTC_DS1307 rtc;

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, ssidHotSpot, pswdHotSpot, hotSpotIP;
String ledNameOrigin[10] = {" ",      "무드등", "실내등", "트렁크", "어닝등",
                            "물펌프", "시거잭", "냉장고", "TV",     "인버터"};
String ledName[10] = {};

bool trunkModeFlag = false;

bool scrLockFlag = false;

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
bool rtcFlag = false;

bool dispLogoFlag = false;

char ssidAp[30] = "CARIND V6.0-SB-020";
char passwordAp[30] = "11240426020";

char msg[100];
char label_text[50];
char daysOfTheWeek[7][10] = {"일요일", "월요일", "화요일", "수요일",
                             "목요일", "금요일", "토요일"};
// char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
//                              "Thursday", "Friday", "Saturday"};

String strYear, strMonth, strDay, strTime, strMinute, strSec;
int intYear, intMonth, intDay, intTime, intMinute, intSec;

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
  }
}

void sleepOff(lv_event_t *e) {
  sleepFlag = false;
  lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
  s = "SLEEP:0";
  comSend(s);
}

void sleepButton(lv_event_t *e) {
  sleepFlag = true;
  // lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
  // lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
  lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
  // s = "SLEEP:1";
  // comSend(s);
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

    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelDay, label_text);

    if (!now.isPM())
      s = "Am";
    else
      s = "Pm";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelAmPm, label_text);

    s = now.hour() % 12;
    if (s == "0")
      s = "12";
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelTime, label_text);

    s = now.minute();
    if (s.length() == 1) {
      s = '0';
      s += now.minute();
    }
    // s.toCharArray(label_text, sizeof(label_text));
    s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelMin, label_text);

    // Serial.print("s.length(); : ");
    // Serial.println(s.length());
    // now.twelveHour();
  }
}

void ScreenHomeLoaded(lv_event_t *e) { Serial.println("ScreenHomeLoaded..."); }

void logoDisplay(lv_event_t *e) {
  Serial.println("Logo Icon Click...");
  dispLogoCount = 110;
  dispLogoFlag = false;
}

void logoClick(lv_event_t *e) {
  Serial.println("Logo Screen Click...");
  dispLogoCount = 0;
  dispLogoFlag = false;
  tft.setBrightness(map(bright, 0, 100, 0, 255));
}

void masterModeSelect(lv_event_t *e) {

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
  }
  EEPROM.commit();
  motor(motorTime);
}

void led1Long(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  motor(50);
  delay(30);
  motor(50);
}

void led2Long(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  motor(50);
  delay(30);
  motor(50);
}

void led3Long(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  motor(50);
  delay(30);
  motor(50);
}

void led4Long(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  dispLogoCount = 0;
  dispLogoFlag = false;
  String sendData;

  s = lv_label_get_text(ui_LabelPWM);
  // comSend(s);
  Serial.print("Slider... ");
  Serial.println(s);

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
  case 3:                 // LED 3
    if (!trunkModeFlag) { // 카니발 모드이면 Return...
      ledPwm[3] = s.toInt();
      sendData = "LED3:";
    }
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
  Serial.print("sendData : ");
  Serial.println(sendData);
  delay(50);
}

void brightChangeSlider(lv_event_t *e) { // 화면 밝기 조절...
  dispLogoCount = 0;
  dispLogoFlag = false;
  bright = lv_slider_get_value(ui_Slider3);
  Serial.println("Bright Slider... ");
  Serial.println(bright);
  if (bright < 10)
    tft.setBrightness(1);
  else
    tft.setBrightness(map(bright, 0, 100, 0, 255));
};

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

  Serial.println("ledButton1...");

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
    Serial.print("Button1 ledPwm[1] : ");
    Serial.println(ledPwm[ledSelect]);
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
  motor(motorTime);
}

void ledButton2(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  motor(motorTime);
}

void ledButton3(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  ledSelect = 3;
  state = lv_obj_get_state(ui_Button3);

  Serial.println("ledButton3...");

  buttonOutLine();
  lv_obj_set_style_outline_opa(ui_Button3, 255,
                               LV_PART_MAIN | LV_STATE_DEFAULT);

  if (state == CHECKED) {
    ledFlag[ledSelect] = true;
    if (!trunkModeFlag) { // 스타리아 모드일때...
      s = ledPwm[ledSelect];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[ledSelect], LV_ANIM_OFF);
      ledName[ledSelect].toCharArray(label_text,
                                     ledName[ledSelect].length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
    }
  } else {
    if (!trunkModeFlag) { // 스타리아 모드일때...
      ledFlag[ledSelect] = false;
      lv_label_set_text(ui_LabelPWM, "0");
      lv_slider_set_value(ui_Slider1, 1, LV_ANIM_OFF);
      ledName[ledSelect].toCharArray(label_text,
                                     ledName[ledSelect].length() + 1);
      lv_label_set_text(ui_LabelLED, label_text);
    }
  }

  s = "LED3:";
  if (!trunkModeFlag)
    s += state == CHECKED ? ledPwm[ledSelect] : 0;
  else
    s += state == CHECKED ? 100 : 0;
  comSend(s);
  ledAllProcess();
  motor(motorTime);
}

void ledButton4(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  motor(motorTime);
}

void pumpButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  Serial.println("PUMP Button...");

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
  motor(motorTime);
}

void cigarButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  Serial.println("Cigar Button...");

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
  motor(motorTime);
}

void frigeButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  Serial.println("Frige Button...");

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
  motor(motorTime);
}

void tvButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  Serial.println("TV Button...");

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
  motor(motorTime);
}

void invertorButton(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
  Serial.println("Invertor Button...");

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
  motor(motorTime);
}

void brightQR(lv_event_t *e) {
  dispLogoCount = 0;
  dispLogoFlag = false;
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
  rs485Count = 0;
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
      s = ledPwm[3];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[3], LV_ANIM_OFF);
    } else {
      ledFlag[3] = false;
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    }
  } else if (_division == "LED4") {
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
      s = ledPwm[4];
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_LabelPWM, label_text);
      lv_slider_set_value(ui_Slider1, ledPwm[4], LV_ANIM_OFF);
    } else {
      ledFlag[4] = false;
      lv_obj_clear_state(ui_Button4, LV_STATE_CHECKED);
    }
  } else if (_division == "LED_ALL") {
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
    buttonOutLine();
    lv_obj_set_style_outline_opa(ui_Button5, 255,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    comInitFlag = true;
    if (_value != "0") {
      pumpFlag = true;
      lv_obj_add_state(ui_Button5, LV_STATE_CHECKED);
    } else {
      pumpFlag = false;
      lv_obj_clear_state(ui_Button5, LV_STATE_CHECKED);
    }
  } else if (_division == "CIGAR") {
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
    if (_value == "1") { // 취침모드 이면...
      sleepFlag = true;

      lv_obj_clear_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
      s = "SLEEP:1";
      comSend(s);
    } else { // 취침모드가 아니면...
      sleepFlag = false;
      lv_obj_add_flag(ui_Container4, LV_OBJ_FLAG_HIDDEN);
      s = "SLEEP:0";
      comSend(s);
    }
  } else if (_division == "TRUNK_MODE") {
    comInitFlag = true;
    if (_value != "0") {
      trunkModeFlag = true;
      lv_obj_add_flag(ui_Image8, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Image10, LV_OBJ_FLAG_HIDDEN); // Trunk Icon
    } else {
      trunkModeFlag = false;
      lv_obj_clear_flag(ui_Image8, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Image10, LV_OBJ_FLAG_HIDDEN); // Trunk Icon
    }
    if (trunkModeFlag)
      Serial.println("카니발 모드...");
    else
      Serial.println("스타리아 모드...");
  } else if (_division == "LED_NAME") {
    ledNameOrigin[_value.toInt()] = _flag;
  } else if (_division == "NAME_SAVE") {
    for (int i = 1; i < 10; i++) {
      ledName[i] = ledNameOrigin[i];
      Serial.print("ledNameOrigin[");
      Serial.print(i);
      Serial.print("] : ");
      Serial.println(ledNameOrigin[i]);
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
    intYear = _value.toInt();
  } else if (_division == "T_MONTH") {
    intMonth = _value.toInt();
  } else if (_division == "T_DAY") {
    intDay = _value.toInt();
  } else if (_division == "T_HOUR") {
    intTime = _value.toInt();
  } else if (_division == "T_MIN") {
    intMinute = _value.toInt();
  } else if (_division == "T_SEC") {
    intSec = _value.toInt();
    Serial.print("From Main Time : ");
    Serial.print(intYear);
    Serial.print("년 ");
    Serial.print(intMonth);
    Serial.print("월 ");
    Serial.print(intDay);
    Serial.print("일 ");
    Serial.print(intTime);
    Serial.print("시 ");
    Serial.print(intMinute);
    Serial.print("분 ");
    Serial.print(intSec);
    Serial.println("초");

    rtc.adjust(DateTime(intYear, intMonth, intDay, intTime, intMinute, intSec));
  }
}

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
    //   lv_obj_add_flag(ui_ImageWiFi,
    //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    //   if (wifiCount > 100)
    //     wifiCount = 10;
    // } else if (wifiCount == 0) { // WiFi 가 연결 되어 있으면...
    //   if (networkFlag == AP_MODE)
    //     lv_obj_clear_flag(ui_ImageWiFi,
    //                       LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // }
    Serial.print("WiFi Count : ");
    Serial.println(wifiCount);
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
  Serial.print("LED9 Name : ");
  Serial.println(ledName[9]);
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

  digitalWrite(MOTOR, LOW);
  digitalWrite(RTS, LOW);

  EEPROM.begin(400);

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  Wire1.begin(12, 11, 400000); // Input Board 통신용...

  int ahtStatus = 0;

  for (int i = 0; i < 3; i++) {
    if (rtc.begin(&Wire1) != true) {
      Serial.println("RTC Not Found");
      delay(50);
    } else {
      rtcFlag = true;
      Serial.println("RTC found");
      break;
    }
  }

  if (rtcFlag)
    rtc.writeSqwPinMode(DS1307_SquareWave1HZ);

  ssidHotSpot = EEPROM.readString(SSID_HOTSPOT_EEPROM);
  pswdHotSpot = EEPROM.readString(PSWD_HOTSPOT_EEPROM);

  masterModeFlag = EEPROM.readBool(MASTER_SLAVE_EEPROM);
  // networkFlag = EEPROM.readBool(NETWORK_EEPROM);

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

  tft.setBrightness(255);
  lv_obj_set_style_opa(ui_ImageQR, 127, 0);

  for (int i = 0; i < 150; i++) {
    lv_timer_handler();
    delay(5);
  }

  ledNameInit();
  clockRead();

  // if (networkFlag) // true : Tethering Mode, false : WiFi
  // {                // 핫스팟 Mode...
  //   s = "핫스팟 네트워크:";
  //   s.toCharArray(label_text, s.length() + 1);
  //   lv_label_set_text(ui_Label23, label_text);
  //   ssidHotSpot.toCharArray(label_text, ssidHotSpot.length() + 1);
  //   lv_label_set_text(ui_LabelSSID, label_text);

  //   s = "핫스팟 비밀번호:";
  //   s.toCharArray(label_text, s.length() + 1);
  //   lv_label_set_text(ui_Label24, label_text);
  //   pswdHotSpot.toCharArray(label_text, pswdHotSpot.length() + 1);
  //   lv_label_set_text(ui_LabelPSWD, label_text);
  //   lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
  //   lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
  // } else { // WiFi Mode...
  //   s = "와이파이 네트워크:";
  //   s.toCharArray(label_text, s.length() + 1);
  //   lv_label_set_text(ui_Label23, label_text);

  //   s = "와이파이 비밀번호:";
  //   s.toCharArray(label_text, s.length() + 1);
  //   lv_label_set_text(ui_Label24, label_text);
  // }

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

  clockRead();

  Serial.println("Setup done");
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

void loop() {

  lv_timer_handler(); /* let the GUI do its work */

  if (loopCount > 180) { // 약 1초 마다 실실행...

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

    Serial.print("RS485 Count : ");
    Serial.println(rs485Count);

    if (dispLogoCount > 100 && !dispLogoFlag) {
      lv_disp_load_scr(ui_ScreenLogo);
      dispLogoFlag = true;
      tft.setBrightness(5);
      if (dispLogoCount > 200)
        dispLogoCount = 101;
    }

    dispLogoCount++;
    Serial.print("DispLogoCount : ");
    Serial.println(dispLogoCount);
  }

  delay(5);
  loopCount++;
}
