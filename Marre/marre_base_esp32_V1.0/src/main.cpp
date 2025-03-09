/* 2024년 9월 19일

마레 캠핑카 V7.0 프로그램
https://squareline.io/  camtrol@naver.com   gustnr99**

    프로그램 내용
    - 최종 통합 System Controller
    - 무시동 히터, 에어컨 RS485 통신
      추 후 5V 공급 처리...

  SquareLine UI Insert 하기
    - SQL의 Library 폴더에 있는 ui 폴더를 VSC의 src 폴더에
      Drag & Drop 한다.

*/

#include "Arduino.h"
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "ui/ui.h"
#include <WiFi.h>
#include <ADS1X15.h>
#include <Adafruit_AHTX0.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define STATION_MODE true
#define AP_MODE false

#define CHECKED 3 // Station WiFi Status Connected...

#define ENABLE 5 // RS485 Enable...

#define AUX_IN 15 // ESP32 Analog In

// #define PWM_ON 255
#define PWM_ON 1023
#define ON HIGH
#define OFF LOW

#define LED1 35
#define LED2 36
#define LED3 37
// #define LED3 13
#define LED4 14
#define LED5 21
#define LED6 47
#define LED7 48
#define LED8 45

// #define WIFI_LED 35

#define HUMAN 13
#define INVERTOR 1
#define TV 41
#define FRIGE 38
#define POP_UP 39
#define POP_DOWN 40

// EEPROM Address...
#define AUTO_ON_1_EEPROM 0
#define AUTO_ON_2_EEPROM 1
#define AUTO_ON_3_EEPROM 2
#define AUTO_ON_4_EEPROM 4
#define AUTO_ON_5_EEPROM 5
#define AUTO_ON_6_EEPROM 6
#define AUTO_ON_7_EEPROM 7
#define AUTO_ON_8_EEPROM 8
#define AUTO_ON_9_EEPROM 9
#define AUTO_ON_INV_EEPROM 10
#define AUTO_ON_TV_EEPROM 11
#define AUTO_ON_FRIGE_EEPROM 12

#define VOLT_ADJ_EEPROM 13
#define AP_SSID_EEPROM 20
#define AP_PSWD_EEPROM 70

#define LED1_NAME_EEPROM 100 // Unsigned Long
#define LED2_NAME_EEPROM 110 // Unsigned Long
#define LED3_NAME_EEPROM 120 // Unsigned Long
#define LED4_NAME_EEPROM 130 // Unsigned Long
#define LED5_NAME_EEPROM 140 // Unsigned Long
#define LED6_NAME_EEPROM 150 // Unsigned Long
#define LED7_NAME_EEPROM 160 // Unsigned Long
#define LED8_NAME_EEPROM 170 // Unsigned Long

// EEPROM Address...
#define NETWORK_EEPROM 2 // bool

// setting PWM properties
const int pwmFreq = 240;
const int ledPwmCh[9] = {0, 0, 1, 2, 3, 4, 5, 6, 7};
const int resolution = 10;

int ledCh[9] = {0, LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8};
int lvPrintCount = 0;

uint16_t pwmValue[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t pwmPushValue[9] = {100, 100, 100, 100, 100, 100, 100, 100, 100};
uint16_t aux_read;

float aux_v, main_v, temperature, humidity;
float aux_v_push, temperature_push, humidity_push;
float voltAdjust;

unsigned long previousMillis = 0;

uint8_t comCount, popCount = 0;
uint16_t globalCount = 0;
uint16_t chanelCount = 1;
int16_t aux_adc, main_adc;

char _label_text[30];

char ssidAp[30] = "마레 V6.0-SB-016";
char passwordAp[30] = "11240426016";
char msg[100];

String ledNameOrigin[9] = {" ",      "팝업등", "침상등", "사이드", "전면등",
                           "후면등", "철재장", "무드등", "어닝등"};
String ledName[9] = {};

String apSSID, apPSWD, stSSID, stPSWD, hotSpotIP;

bool networkFlag = false;     // Network Select Switch...
bool hotSpotFlag = false;     // Station Connect Flag...
bool wifiStartFlag = false;   // WiFi Start Flag...
bool apModeStartFlag = false; // Station Connect Flag...
// bool rs485Flag = false;       // RS485 Connect Flag...
bool human = false; // Human Sensor ...

bool wifiFlag = false; // Analog, WiFi 구분...

bool ledFlag[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

bool ledAllFlag = false;

bool invertorFlag = false;
bool tvFlag = false;
bool frigeFlag = false;
bool sleepFlag = false;

bool popActiveFlag = false;
bool popUpFlag = false;
bool popStopFlag = true;
bool popDownFlag = false;

void wifiStart();

// ESP32 S3는 반드시 아래 방식으로 해야 함.
TwoWire I2Ctwo = TwoWire(1);

Adafruit_AHTX0 myAHT10;
ADS1115 ads_1(0x48, &I2Ctwo);

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload, s;

WebServer server(80);

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance; // SPI BUS Interface
  // lgfx::Light_PWM _light_instance;
  lgfx::Touch_CST816S _touch_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST; // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or
                                // SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // cfg.spi_mode = 2;         // SPI通信モードを設定 (0 ~ 3)
      cfg.spi_mode = 0;          // SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 80000000; // 送信時のSPIクロック (最大80MHz,
      // 80MHzを整数で割った値に丸められます)
      cfg.freq_read = 20000000; // 受信時のSPIクロック
      cfg.spi_3wire = false; // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock = true; // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定
                                         // (0=DMA不使用 / 1=1ch / 2=ch /
                                         // SPI_DMA_CH_AUTO=自動設定)
      cfg.pin_sclk = 46;                 // SPIのSCLKピン番号を設定
      cfg.pin_mosi = 17;                 // SPIのMOSIピン番号を設定
      cfg.pin_miso = -1; // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc = 9;    // SPIのD/Cピン番号を設定  (-1 = disable)
      _bus_instance.config(cfg); // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {
      auto cfg =
          _panel_instance.config(); // 表示パネル設定用の構造体を取得します。
      cfg.pin_cs = 18; // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst = 3; // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)
      cfg.panel_width = 240;  // 実際に表示可能な幅
      cfg.panel_height = 280; // 実際に表示可能な高さ
      cfg.offset_x = 0;       // パネルのX方向オフセット量
      cfg.offset_y = 20;      // パネルのY方向オフセット量
      cfg.offset_rotation = 0; // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits =
          1;               // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable = true; // データ読出しが可能な場合 trueに設定
      cfg.invert = true; // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order = true; // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit =
          false;             // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合
                             // trueに設定
      cfg.bus_shared = true; // SDカードとバスを共有している場合
      // trueに設定(drawJpgFile等でバス制御を行います)
      _panel_instance.config(cfg);
    }
    // {
    //   auto cfg =
    //       _light_instance.config(); //
    //       バックライト設定用の構造体を取得します。
    //   cfg.pin_bl = 16; // バックライトが接続されているピン番号
    //   cfg.invert = false; // バックライトの輝度を反転させる場合 true
    //   cfg.freq = 5000;     // バックライトのPWM周波数
    //   cfg.pwm_channel = 0; // 使用するPWMのチャンネル番号
    //   _light_instance.config(cfg);
    //   _panel_instance.setLight(
    //       &_light_instance); // バックライトをパネルにセットします。
    // }

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.y_min = 0;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      // I2C connection
      // cfg.i2c_port = I2C_NUM_0;
      cfg.pin_sda = 10;
      cfg.pin_scl = 8;
      cfg.pin_int = 20;
      cfg.pin_rst = 19;
      cfg.x_max = 240;
      cfg.y_max = 280;
      cfg.freq = 10000;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

static LGFX tft; // LGFXのインスタンスを作成。

static const uint16_t screenWidth = 280;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

void comSend(String sendData) {

  digitalWrite(ENABLE, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(ENABLE, LOW);
}

void lv_print(String msg) {
  lvPrintCount++;
  if (lvPrintCount > 50) {
    lv_textarea_set_text(ui_TextArea1, "");
    lvPrintCount = 0;
  }
  char _text[50];
  msg.toCharArray(_text, sizeof(_text));
  lv_textarea_add_text(ui_TextArea1, _text);
  lv_textarea_add_char(ui_TextArea1, '\n');
}

void serialEvent2() {
  uint16_t value;
  uint16_t pwm;

  String eventString = Serial2.readStringUntil('\n');
  Serial.print("EventString: ");
  Serial.println(eventString);

  uint8_t first = eventString.indexOf(":"); // 첫 번째 콤마 위치

  String chanel = eventString.substring(0, first); // 첫 번째 토큰 (0, 3)
  String str2 = eventString.substring(
      first + 1, eventString.length()); // 두 번째 토큰 (4, 7)

  value = str2.toInt();
  if (value != 0)
    pwm = map(value, 1, 100, 1, PWM_ON);

  if (chanel == "LED1") {
    ledcWrite(ledPwmCh[1], pwm);
    pwmPushValue[1] = value;
    if (value != 0) {
      ledFlag[1] = true;
    } else {
      ledFlag[1] = false;
      ledAllFlag = false;
    }
    s = "조명 1";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED2") {
    ledcWrite(ledPwmCh[2], pwm);
    pwmPushValue[2] = value;
    if (value != 0) {
      ledFlag[2] = true;
    } else {
      ledFlag[2] = false;
      ledAllFlag = false;
    }
    s = "조명 2";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED3") {
    ledcWrite(ledPwmCh[3], pwm);
    pwmPushValue[3] = value;
    if (value != 0) {
      ledFlag[3] = true;
    } else {
      ledFlag[3] = false;
      ledAllFlag = false;
    }
    s = "조명 3";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED4") {
    ledcWrite(ledPwmCh[4], pwm);
    pwmPushValue[4] = value;
    if (value != 0) {
      ledFlag[4] = true;
    } else {
      ledFlag[4] = false;
      ledAllFlag = false;
    }
    s = "조명 4";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED5") {
    ledcWrite(ledPwmCh[5], pwm);
    pwmPushValue[5] = value;
    if (value != 0) {
      ledFlag[5] = true;
    } else {
      ledFlag[5] = false;
      ledAllFlag = false;
    }
    s = "조명 5";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED6") {
    ledcWrite(ledPwmCh[6], pwm);
    pwmPushValue[6] = value;
    if (value != 0) {
      ledFlag[6] = true;
    } else {
      ledFlag[6] = false;
      ledAllFlag = false;
    }
    s = "조명 6";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED7") {
    ledcWrite(ledPwmCh[7], pwm);
    pwmPushValue[7] = value;
    if (value != 0) {
      ledFlag[7] = true;
    } else {
      ledFlag[7] = false;
      ledAllFlag = false;
    }
    s = "조명 7";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED8") {
    ledcWrite(ledPwmCh[8], pwm);
    pwmPushValue[8] = value;
    if (value != 0) {
      ledFlag[8] = true;
    } else {
      ledFlag[8] = false;
      ledAllFlag = false;
    }
    s = "조명 8";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED_ALL") {
    ledcWrite(ledPwmCh[1], pwm);
    ledcWrite(ledPwmCh[2], pwm);
    ledcWrite(ledPwmCh[3], pwm);
    ledcWrite(ledPwmCh[4], pwm);
    ledcWrite(ledPwmCh[5], pwm);
    ledcWrite(ledPwmCh[6], pwm);
    ledcWrite(ledPwmCh[7], pwm);
    ledcWrite(ledPwmCh[8], pwm);

    if (value != 0) {
      ledAllFlag = true;
      ledFlag[1] = true;
      ledFlag[2] = true;
      ledFlag[3] = true;
      ledFlag[4] = true;
      ledFlag[5] = true;
      ledFlag[6] = true;
      ledFlag[7] = true;
      ledFlag[8] = true;
      pwmPushValue[1] = 100;
      pwmPushValue[2] = 100;
      pwmPushValue[3] = 100;
      pwmPushValue[4] = 100;
      pwmPushValue[5] = 100;
      pwmPushValue[6] = 100;
      pwmPushValue[7] = 100;
      pwmPushValue[8] = 100;
      s = "전체등 켜짐 : ";
      s += value;
      s += "...";
      s.toCharArray(_label_text, s.length() + 1);
      lv_print(_label_text);
    } else {
      ledAllFlag = false;
      ledFlag[1] = false;
      ledFlag[2] = false;
      ledFlag[3] = false;
      ledFlag[4] = false;
      ledFlag[5] = false;
      ledFlag[6] = false;
      ledFlag[7] = false;
      ledFlag[8] = false;
      pwmPushValue[1] = 0;
      pwmPushValue[2] = 0;
      pwmPushValue[3] = 0;
      pwmPushValue[4] = 0;
      pwmPushValue[5] = 0;
      pwmPushValue[6] = 0;
      pwmPushValue[7] = 0;
      pwmPushValue[8] = 0;
      lv_print("전체등    꺼짐...");
    }
  } else if (chanel == "INV") {
    value == 1 ? digitalWrite(INVERTOR, HIGH) : digitalWrite(INVERTOR, LOW);
    if (value) {
      invertorFlag = true;
      lv_print("인버터 켜짐...");
    } else {
      invertorFlag = false;
      lv_print("인버터    꺼짐...");
    }
  } else if (chanel == "TV") {
    value == 1 ? digitalWrite(TV, HIGH) : digitalWrite(TV, LOW);
    if (value) {
      tvFlag = true;
      lv_print("TV 켜짐...");
    } else {
      tvFlag = false;
      lv_print("TV    꺼짐...");
    }
  } else if (chanel == "FRIGE") {
    value == 1 ? digitalWrite(FRIGE, HIGH) : digitalWrite(FRIGE, LOW);
    if (value) {
      frigeFlag = true;
      lv_print("냉장고 켜짐...");
    } else {
      frigeFlag = false;
      lv_print("냉장고    꺼짐...");
    }
  } else if (chanel == "REQUEST") {
    comCount = 0;
    Serial.println("REQUEST receive OK.................");

    wifiStartFlag = false;
    s = "VOLT";
    s += ':';
    s += String(aux_v_push, 1);
    s += " V";
    comSend(s);
  } else if (chanel == "AP_SSID") { // Access Point Mode
    Serial.print("AP_SSID String Length : ");
    Serial.println(str2.length());
    if (EEPROM.readString(AP_SSID_EEPROM) != str2) {
      apSSID = str2;
      EEPROM.writeString(AP_SSID_EEPROM, str2);
      EEPROM.commit();
    }
  } else if (chanel == "AP_PSWD") { // Access Point Mode
    Serial.print(", AP_PSWD String Length : ");
    Serial.print(str2.length());
    if (EEPROM.readString(AP_PSWD_EEPROM) != str2) {
      apPSWD = str2;
      EEPROM.writeString(AP_PSWD_EEPROM, str2);
      EEPROM.commit();
    }
  } else if (chanel == "HUMAN") { // Access Point Mode
    s = "HUMAN";
    s += ':';
    s += human;
    comSend(s);
  } else if (chanel == "POP_UP") {
    popActiveFlag = true;
    if (value) {
      popUpFlag = true;
      popStopFlag = false;
      if (popDownFlag) {
        digitalWrite(POP_DOWN, LOW);
        delay(500);
      }
      digitalWrite(POP_UP, HIGH);
      popDownFlag = false;
    } else {
      popUpFlag = false;
      digitalWrite(POP_UP, LOW);
    }

    lv_print("팝업 텐트 상승...");
  } else if (chanel == "POP_DOWN") {
    popActiveFlag = true;
    if (value) {
      popDownFlag = true;
      popStopFlag = false;
      if (popUpFlag) {
        digitalWrite(POP_UP, LOW);
        delay(500);
      }
      digitalWrite(POP_DOWN, HIGH);
      popUpFlag = false;
    } else {
      digitalWrite(POP_DOWN, LOW);
      popDownFlag = false;
    }

    lv_print("팝업 텐트 하강...");
  } else if (chanel == "POP_STOP") {
    popActiveFlag = false;
    if (value) {
      popUpFlag = false;
      popStopFlag = true;
      popDownFlag = false;
      digitalWrite(POP_UP, LOW);
      digitalWrite(POP_DOWN, LOW);
    } else {
      popStopFlag = false;
    }

    // comSend("POP_STOP:1");

    lv_print("팝업 텐트 정지...");
  } else if (chanel == "COM") {
    Serial.println("COM receive OK.................");
    s = "VOLT";
    s += ':';
    s += String(aux_v_push, 1);
    s += " V";
    comSend(s);
  }

  Serial.print("Command: ");
  Serial.print(chanel);
  Serial.print(", Value: ");
  Serial.print(str2);
  Serial.print(", PWM: ");
  Serial.println(pwm);

  // comCount = 0;
}

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

    // Serial.println(btn_push_count);
  }
}

void btnPowerOnExeSave(lv_event_t *e) {

  if (lv_obj_get_state(ui_onSwitch1) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_1_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_1_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch2) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_2_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_2_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch3) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_3_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_3_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch4) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_4_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_4_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch5) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_5_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_5_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch6) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_6_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_6_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch7) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_7_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_7_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch8) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_8_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_8_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch9) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_9_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_9_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch10) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_INV_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_INV_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch11) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_TV_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_TV_EEPROM, false);

  if (lv_obj_get_state(ui_onSwitch12) == LV_STATE_CHECKED)
    EEPROM.writeBool(AUTO_ON_FRIGE_EEPROM, true);
  else
    EEPROM.writeBool(AUTO_ON_FRIGE_EEPROM, false);

  EEPROM.commit();
}

void panelVolt(lv_event_t *e) {
  s = String(voltAdjust, 2);
  s.toCharArray(_label_text, sizeof(_label_text));
  lv_label_set_text(ui_volt, _label_text);
}

void networkScreenLoaded(lv_event_t *e) {
  if (networkFlag == STATION_MODE) {
    hotSpotIP.toCharArray(_label_text, sizeof(_label_text));
    lv_label_set_text(ui_LabelNetwork, _label_text);
  }
}

void networkSelectSwitch(lv_event_t *e) {
  Serial.println("Network Select Switch...");

  if (apModeStartFlag)
    apModeStartFlag = false;

  if (lv_obj_get_state(ui_Switch1) == CHECKED) {
    EEPROM.writeBool(NETWORK_EEPROM, STATION_MODE);
    networkFlag = STATION_MODE;
    Serial.println("Network Select Station Mode...");
    stSSID.toCharArray(_label_text, stSSID.length());
    lv_label_set_text(ui_LabelSSID, _label_text);
    stPSWD.toCharArray(_label_text, stPSWD.length());
    lv_label_set_text(ui_LabelPSWD, _label_text);
    hotSpotIP.toCharArray(_label_text, sizeof(_label_text));
    lv_label_set_text(ui_LabelNetwork, _label_text);
  } else {
    EEPROM.writeBool(NETWORK_EEPROM, AP_MODE);
    networkFlag = AP_MODE;
    Serial.println("Network Select AP Mode...");
    apSSID.toCharArray(_label_text, apSSID.length());
    lv_label_set_text(ui_LabelSSID, _label_text);
    apPSWD.toCharArray(_label_text, apPSWD.length());
    lv_label_set_text(ui_LabelPSWD, _label_text);
    s = "-  네트워크 선택  -";
    s.toCharArray(_label_text, s.length() + 1);
    lv_label_set_text(ui_LabelNetwork, _label_text);
  }
  EEPROM.commit();

  if (!networkFlag) { // 와이파이 모드 일때...
    apSSID.toCharArray(ssidAp, apSSID.length());
    apPSWD.toCharArray(passwordAp, apPSWD.length());
  } else { // 핫스팟 모드 일때...
    stSSID.toCharArray(ssidAp, stSSID.length());
    stPSWD.toCharArray(passwordAp, stPSWD.length());
  }
  if (comCount != 0) {
    WiFi.disconnect();
  }
}

void voltPlus(lv_event_t *e) {
  voltAdjust += 0.01;
  if (voltAdjust > 10.0)
    voltAdjust = 10.0;
  s = String(voltAdjust, 2);
  s.toCharArray(_label_text, sizeof(_label_text));
  lv_label_set_text(ui_volt, _label_text);
}

void voltMinus(lv_event_t *e) {
  voltAdjust -= 0.01;
  if (voltAdjust < 0.0)
    voltAdjust = 0.0;
  s = String(voltAdjust, 2);
  s.toCharArray(_label_text, sizeof(_label_text));
  lv_label_set_text(ui_volt, _label_text);
}

void voltSave(lv_event_t *e) {
  EEPROM.writeFloat(VOLT_ADJ_EEPROM, voltAdjust);
  EEPROM.commit();
}

void Screen2Load(lv_event_t *e) {
  lv_label_set_text(ui_Label2, ssidAp);
  lv_label_set_text(ui_Label3, passwordAp);
}

void opacityChange(lv_event_t *e) {
  lv_opa_t rollValue;
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *obj = lv_event_get_target(e);
  if (code == 8) {
    char buf[32];
    lv_roller_get_selected_str(obj, buf, sizeof(buf));
    LV_LOG_USER("Selected value: %s", buf);
    // rollValue = atoi(buf);
    rollValue = map(atoi(buf), 5, 100, 250, 0);
  }
  lv_obj_set_style_img_recolor(ui_Image1, lv_color_hex(0x000000),
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_Image1, rollValue,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
  Serial.print("Slider 값: ");
  Serial.println(rollValue);
  Serial.print("Event Code: ");
  Serial.println(code);
}

void informationRead() {

  float Voltage = 0.0;
  sensors_event_t adsHumidity, adsTemp;

  aux_adc = ads_1.readADC(0);

  Voltage = (aux_adc * 0.00188898575);
  aux_v = round(Voltage * 10) / 10;
  aux_v += voltAdjust;

  // if (isnan(aux_adc)) {
  //   Voltage = (aux_adc * 0.00188898575);
  //   aux_v = round(Voltage * 10) / 10;
  //   aux_v += voltAdjust;
  //   Serial.print("ADS1115 Value...");
  // } else {
  //   aux_read = analogRead(AUX_IN);
  //   Serial.print("ESP ADC Value : ");
  //   Serial.println(aux_read);
  //   Serial.print("ESP ADC Volt : ");
  //   Serial.println(aux_read * (3.2 / 4095.0));
  //   Serial.print("ADC CON Volt : ");
  //   aux_v = aux_read * 0.00542846064366;
  //   Serial.println(aux_v);
  // }

  Serial.print(" 전압 : ");
  Serial.println(aux_v);

  myAHT10.getEvent(
      &adsHumidity,
      &adsTemp); // populate temp and humidity objects with fresh data

  Serial.print(" 온도 : ");
  temperature = round(adsTemp.temperature * 10) / 10;
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print(" 습도 : ");
  humidity = round(adsHumidity.relative_humidity * 10) / 10;
  Serial.print(humidity);
  Serial.println(" %");

  if (aux_v != aux_v_push) {
    // if ((aux_v != aux_v_push) || (temperature != temperature_push) ||
    //     (humidity != humidity_push)) {

    if (aux_v != aux_v_push) {
      aux_v_push = aux_v;
      s = "VOLT";
      s += ':';
      s += String(aux_v_push, 1);
      s += " V";
      comSend(s);
    }

    // if (temperature != temperature_push) {
    //   temperature_push = temperature;
    //   s = "TEMP";
    //   s += ':';
    //   s += String(temperature_push, 1);
    //   // s += " 도";
    //   comSend(s);
    // }

    // if (humidity != humidity_push) {
    //   humidity_push = humidity;
    //   s = "HUMI";
    //   s += ':';
    //   s += String(humidity_push, 1);
    //   // s += " %";
    //   comSend(s);
    // }
  }
}

void handleJson() {

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
    if (sleepFlag)
      sleepFlag = false;
    wifiFlag = true;
    if (value == 0) {
      ledFlag[chanel] = false;
      ledcWrite(ledPwmCh[chanel], 0);
    } else {
      ledFlag[chanel] = true;
      pwmPushValue[chanel] = value;
      ledcWrite(ledPwmCh[chanel], map(value, 1, 100, 1, PWM_ON));
    }
    Serial.print("pwmPushValue[");
    Serial.print(chanel);
    Serial.print("] = ");
    Serial.println(pwmPushValue[chanel]);

    s = "조명 ";
    s += chanel;
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);

    s = "LED";
    s += chanel;
    s += ":";
    s += pwmPushValue[chanel];
    s += ':';
    s += ledFlag[chanel];
    comSend(s);

    if (!ledFlag[1] && !ledFlag[2] && !ledFlag[3] && !ledFlag[4] &&
        !ledFlag[5] && !ledFlag[6] && !ledFlag[7] && !ledFlag[8] && ledAllFlag)
      ledAllFlag = false;

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "ALL") {
    wifiFlag = true;
    if (value == 1) { // All LED On...
      ledAllFlag = true;
      for (int i = 1; i < 9; i++) {
        ledcWrite(ledPwmCh[i], PWM_ON);
        pwmPushValue[i] = 100;
        ledFlag[i] = true;
      }
      lv_print("전체등 켜짐...");
    } else { // All LED Off...
      ledAllFlag = false;
      for (int i = 1; i < 9; i++) {
        ledcWrite(ledPwmCh[i], 0);
        ledFlag[i] = false;
      }
      lv_print("전체등    꺼짐...");
    }

    s = "LED_ALL:";
    // s += chanel;
    // s += ":";
    s += 100;
    s += ':';
    s += ledAllFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "INVERTOR") {
    wifiFlag = true;
    if (value) {
      digitalWrite(INVERTOR, HIGH);
      lv_print("인버터 켜짐...");
      invertorFlag = true;
    } else {
      digitalWrite(INVERTOR, LOW);
      lv_print("인버터    꺼짐...");
      invertorFlag = false;
    }
    s = "INV:";
    s += invertorFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "TV") {
    wifiFlag = true;
    if (value) {
      digitalWrite(TV, HIGH);
      lv_print("TV 켜짐...");
      tvFlag = true;
    } else {
      digitalWrite(TV, LOW);
      lv_print("TV    꺼짐...");
      tvFlag = false;
    }
    s = "TV:";
    s += tvFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "FRIGE") {
    wifiFlag = true;
    if (value) {
      digitalWrite(FRIGE, HIGH);
      lv_print("냉장고 켜짐...");
      frigeFlag = true;
    } else {
      digitalWrite(FRIGE, LOW);
      lv_print("냉장고    꺼짐...");
      frigeFlag = false;
    }
    s = "FRIGE:";
    s += frigeFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "POP_UP") {
    popActiveFlag = true;
    if (value) {
      popUpFlag = true;
      popStopFlag = false;
      if (popDownFlag) {
        digitalWrite(POP_DOWN, LOW);
        delay(500);
      }
      digitalWrite(POP_UP, HIGH);
      popDownFlag = false;
    } else {
      popUpFlag = false;
      digitalWrite(POP_UP, LOW);
    }

    lv_print("팝업 텐트 상승...");

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "POP_STOP") {
    popActiveFlag = false;
    if (value) {
      popStopFlag = true;
      popUpFlag = false;
      popDownFlag = false;
      digitalWrite(POP_UP, LOW);
      digitalWrite(POP_DOWN, LOW);
    } else {
      popStopFlag = false;
    }

    lv_print("팝업 텐트 정지...");

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "POP_DOWN") {
    popActiveFlag = true;
    if (value) {
      popDownFlag = true;
      popStopFlag = false;
      if (popUpFlag) {
        digitalWrite(POP_UP, LOW);
        delay(500);
      }
      digitalWrite(POP_DOWN, HIGH);
      popUpFlag = false;
    } else {
      digitalWrite(POP_DOWN, LOW);
      popDownFlag = false;
    }

    lv_print("팝업 텐트 하강...");

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "SLEEP") {
    if (value == 1) {
      lv_print("취침 모드 시작...");
      ledAllFlag = false;
      for (int i = 1; i < 9; i++) {
        if (ledFlag[i]) {
          ledcWrite(ledPwmCh[i], 0);
        }
        // comCount = 0;
        ledFlag[i] = false;
      }
      sleepFlag = true;
    } else {
      lv_print("취침 모드 해제...");
      sleepFlag = false;
    }

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "QR_OK") {
    comSend("QR_OK:1");
    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "LED_NAME") {
    switch (chanel) {
    case 1:
      ledName[1] = _ledName;
      Serial.print("ledName[1]: ");
      Serial.println(ledName[1]);
      s = "LED_NAME:";
      s += "1";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 2:
      ledName[2] = _ledName;
      Serial.print("ledName[2]: ");
      Serial.println(ledName[2]);
      s = "LED_NAME:";
      s += "2";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 3:
      ledName[3] = _ledName;
      Serial.print("ledName[3]: ");
      Serial.println(ledName[3]);
      s = "LED_NAME:";
      s += "3";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 4:
      ledName[4] = _ledName;
      Serial.print("ledName[4]: ");
      Serial.println(ledName[4]);
      s = "LED_NAME:";
      s += "4";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 5:
      ledName[5] = _ledName;
      Serial.print("ledName[5]: ");
      Serial.println(ledName[5]);
      s = "LED_NAME:";
      s += "5";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 6:
      ledName[6] = _ledName;
      Serial.print("ledName[6]: ");
      Serial.println(ledName[6]);
      s = "LED_NAME:";
      s += "6";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 7:
      ledName[7] = _ledName;
      Serial.print("ledName[7]: ");
      Serial.println(ledName[7]);
      s = "LED_NAME:";
      s += "7";
      s += ":";
      s += _ledName;
      comSend(s);
      break;
    case 8:
      ledName[8] = _ledName;
      Serial.print("ledName[8]: ");
      Serial.println(ledName[8]);
      s = "LED_NAME:";
      s += "8";
      s += ":";
      s += _ledName;
      comSend(s);
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

      ledName[1].toCharArray(_label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_Label1, _label_text);
      ledName[2].toCharArray(_label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_Label2, _label_text);
      ledName[3].toCharArray(_label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_Label3, _label_text);
      ledName[4].toCharArray(_label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_Label4, _label_text);
      ledName[5].toCharArray(_label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_Label5, _label_text);
      ledName[6].toCharArray(_label_text, ledName[6].length() + 1);
      lv_label_set_text(ui_Label6, _label_text);
      ledName[7].toCharArray(_label_text, ledName[7].length() + 1);
      lv_label_set_text(ui_Label7, _label_text);
      ledName[8].toCharArray(_label_text, ledName[8].length() + 1);
      lv_label_set_text(ui_Label8, _label_text);
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

      ledName[1].toCharArray(_label_text, ledName[1].length() + 1);
      lv_label_set_text(ui_Label1, _label_text);
      ledName[2].toCharArray(_label_text, ledName[2].length() + 1);
      lv_label_set_text(ui_Label2, _label_text);
      ledName[3].toCharArray(_label_text, ledName[3].length() + 1);
      lv_label_set_text(ui_Label3, _label_text);
      ledName[4].toCharArray(_label_text, ledName[4].length() + 1);
      lv_label_set_text(ui_Label4, _label_text);
      ledName[5].toCharArray(_label_text, ledName[5].length() + 1);
      lv_label_set_text(ui_Label5, _label_text);
      ledName[6].toCharArray(_label_text, ledName[6].length() + 1);
      lv_label_set_text(ui_Label6, _label_text);
      ledName[7].toCharArray(_label_text, ledName[7].length() + 1);
      lv_label_set_text(ui_Label7, _label_text);
      ledName[8].toCharArray(_label_text, ledName[8].length() + 1);
      lv_label_set_text(ui_Label8, _label_text);
      break;
    default:
      break;
    }
  } else if (division == "TIME") {
    s = "TIME:";
    s += nowTime;

    comSend(s);
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
  }

  server.send(200, "text/plain", s);

} // handleJson()

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  server.send(200, "text/plain", "");
}

void handleRequest() {

  s = "";
  s = s + "{" + "\"aux_v\":" + aux_v + ", " + "\"Temperature\":" + temperature +
      ", " + "\"Humidity\":" + humidity + ", " + "\"ledValue1\":" +
      pwmPushValue[1] + ", " + "\"ledValue2\":" + pwmPushValue[2] + ", " +
      "\"ledValue3\":" + pwmPushValue[3] + ", " + "\"ledValue4\":" +
      pwmPushValue[4] + ", " + "\"ledValue5\":" + pwmPushValue[5] + ", " +
      "\"ledValue6\":" + pwmPushValue[6] + ", " + "\"ledValue7\":" +
      pwmPushValue[7] + ", " + "\"ledValue8\":" + pwmPushValue[8] + ", " +
      "\"ledFlag1\":" + ledFlag[1] + ", " + "\"ledFlag2\":" + ledFlag[2] +
      ", " + "\"ledFlag3\":" + ledFlag[3] + ", " + "\"ledFlag4\":" +
      ledFlag[4] + ", " + "\"ledFlag5\":" + ledFlag[5] + ", " +
      "\"ledFlag6\":" + ledFlag[6] + ", " + "\"ledFlag7\":" + ledFlag[7] +
      ", " + "\"ledFlag8\":" + ledFlag[8] + ", " + "\"invertor\":" +
      invertorFlag + ", " + "\"tv\":" + tvFlag + ", " + "\"frige\":" +
      frigeFlag + ", " + "\"ledAllFlag\":" + ledAllFlag + ", " +
      "\"sleepFlag\":" + sleepFlag + ", " + "\"popUp\":" + popUpFlag + ", " +
      "\"popStop\":" + popStopFlag + ", " + "\"popDown\":" + popDownFlag + "}";

  server.send(200, "text/plain", s);
  // comCount = 0;

} // handleRequest()

void wifiStart() { // WiFi Mode Start...

  String msg;

  wifiStartFlag = true; // WiFi 통신 시작...

  WiFi.mode(WIFI_MODE_AP);

  Serial.println(F("setupWifi()..."));
  Serial.println(F("Access Point(WiFi) Mode Start..."));

  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAP(ssidAp, passwordAp); // Access Point mode begin...
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // WiFi Icon 표시...
  lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
  s = "와이파이 모드 시작...\n";
  s.toCharArray(_label_text, s.length() + 1);
  lv_textarea_add_text(ui_TextArea1, _label_text);

  Serial.println();
  Serial.print("SSID: ");
  Serial.print(ssidAp);
  Serial.print(", PSWD: ");
  Serial.println(passwordAp);
  Serial.print(F("ESP32 IP as soft AP : "));
  Serial.println(WiFi.softAPIP());
  apModeStartFlag = true;
  server.on("/json", handleJson);
  server.on("/root", handleRoot);
  server.on("/Request", handleRequest);

  server.begin();
}

void ledNameInit() {
  ledName[1] = EEPROM.readString(LED1_NAME_EEPROM);
  if (ledName[1] == "") {
    ledName[1] = ledNameOrigin[1];
    EEPROM.writeString(LED1_NAME_EEPROM, ledName[1]);
  }
  ledName[2] = EEPROM.readString(LED2_NAME_EEPROM);
  if (ledName[2] == "") {
    ledName[2] = ledNameOrigin[2];
    EEPROM.writeString(LED2_NAME_EEPROM, ledName[2]);
  }
  ledName[3] = EEPROM.readString(LED3_NAME_EEPROM);
  if (ledName[3] == "") {
    ledName[3] = ledNameOrigin[3];
    EEPROM.writeString(LED3_NAME_EEPROM, ledName[3]);
  }
  ledName[4] = EEPROM.readString(LED4_NAME_EEPROM);
  if (ledName[4] == "") {
    ledName[4] = ledNameOrigin[4];
    EEPROM.writeString(LED4_NAME_EEPROM, ledName[4]);
  }
  ledName[5] = EEPROM.readString(LED5_NAME_EEPROM);
  if (ledName[5] == "") {
    ledName[5] = ledNameOrigin[5];
    EEPROM.writeString(LED5_NAME_EEPROM, ledName[5]);
  }
  ledName[6] = EEPROM.readString(LED6_NAME_EEPROM);
  if (ledName[6] == "") {
    ledName[6] = ledNameOrigin[6];
    EEPROM.writeString(LED6_NAME_EEPROM, ledName[6]);
  }
  ledName[7] = EEPROM.readString(LED7_NAME_EEPROM);
  if (ledName[7] == "") {
    ledName[7] = ledNameOrigin[7];
    EEPROM.writeString(LED7_NAME_EEPROM, ledName[7]);
  }
  ledName[8] = EEPROM.readString(LED8_NAME_EEPROM);
  if (ledName[8] == "") {
    ledName[8] = ledNameOrigin[8];
    EEPROM.writeString(LED8_NAME_EEPROM, ledName[8]);
  }

  EEPROM.commit();

  ledName[1].toCharArray(_label_text, ledName[1].length() + 1);
  lv_label_set_text(ui_Label16, _label_text);
  ledName[2].toCharArray(_label_text, ledName[2].length() + 1);
  lv_label_set_text(ui_Label17, _label_text);
  ledName[3].toCharArray(_label_text, ledName[3].length() + 1);
  lv_label_set_text(ui_Label18, _label_text);
  ledName[4].toCharArray(_label_text, ledName[4].length() + 1);
  lv_label_set_text(ui_Label19, _label_text);
  ledName[5].toCharArray(_label_text, ledName[5].length() + 1);
  lv_label_set_text(ui_Label20, _label_text);
  ledName[6].toCharArray(_label_text, ledName[6].length() + 1);
  lv_label_set_text(ui_Label21, _label_text);
  ledName[7].toCharArray(_label_text, ledName[7].length() + 1);
  lv_label_set_text(ui_Label22, _label_text);
  ledName[8].toCharArray(_label_text, ledName[8].length() + 1);
  lv_label_set_text(ui_Label23, _label_text);

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

void setup() {

  pinMode(ENABLE, OUTPUT);
  pinMode(HUMAN, INPUT);

  pinMode(7, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);

  pinMode(INVERTOR, OUTPUT);
  pinMode(TV, OUTPUT);
  pinMode(FRIGE, OUTPUT);
  pinMode(POP_UP, OUTPUT);
  pinMode(POP_DOWN, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
  digitalWrite(ENABLE, LOW);
  digitalWrite(TV, LOW);
  digitalWrite(FRIGE, LOW);
  digitalWrite(POP_UP, LOW);
  digitalWrite(POP_DOWN, LOW);

  digitalWrite(7, LOW);
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
  digitalWrite(35, LOW);
  digitalWrite(36, LOW);

  // configure LED PWM
  ledcSetup(ledPwmCh[1], pwmFreq, resolution);
  ledcSetup(ledPwmCh[2], pwmFreq, resolution);
  ledcSetup(ledPwmCh[3], pwmFreq, resolution);
  ledcSetup(ledPwmCh[4], pwmFreq, resolution);
  ledcSetup(ledPwmCh[5], pwmFreq, resolution);
  ledcSetup(ledPwmCh[6], pwmFreq, resolution);
  ledcSetup(ledPwmCh[7], pwmFreq, resolution);
  ledcSetup(ledPwmCh[8], pwmFreq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledCh[1], ledPwmCh[1]);
  ledcAttachPin(ledCh[2], ledPwmCh[2]);
  ledcAttachPin(ledCh[3], ledPwmCh[3]);
  ledcAttachPin(ledCh[4], ledPwmCh[4]);
  ledcAttachPin(ledCh[5], ledPwmCh[5]);
  ledcAttachPin(ledCh[6], ledPwmCh[6]);
  ledcAttachPin(ledCh[7], ledPwmCh[7]);
  ledcAttachPin(ledCh[8], ledPwmCh[8]);

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(
      my_print); /* register print function for debugging */
#endif

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
  delay(5);
  lv_timer_handler();

  EEPROM.begin(250);

  // Wire.begin(42, 2); // Input Board 통신용...
  I2Ctwo.begin(42, 2); // Input Board 통신용...
  Serial.begin(115200);
  // Serial2.begin(19200, SERIAL_8N1, 4, 6);
  Serial2.begin(115200, SERIAL_8N1, 4, 6);

  int ahtStatus = 0;

  ahtStatus = 0;
  while (ads_1.begin() != true) {
    Serial.println(F("Could not find ADS1115 Check wiring"));
    lv_print("아나로그 회로 점검 요망...");
    delay(100);
    if (ahtStatus > 5)
      break;
    ahtStatus++;
  }
  Serial.println("ADS1115 found");
  lv_print("아나로그 회로 정상...");

  ahtStatus = 0;
  while (myAHT10.begin(&I2Ctwo, 0, 0x38) != true) {
    Serial.println(F("Could not find AHT10 Check wiring"));
    lv_print("온/습도 센서 점검 요망...");

    delay(100);
    if (ahtStatus > 5)
      break;
    ahtStatus++;
  }
  Serial.println("AHT10 found");
  lv_print("온/습도 센서 정상...");

  voltAdjust = EEPROM.readFloat(VOLT_ADJ_EEPROM);
  if (isnan(voltAdjust)) {
    voltAdjust = 0.0;
    EEPROM.writeFloat(VOLT_ADJ_EEPROM, 0.0);
    EEPROM.commit();
  }

  // digitalWrite(WIFI_LED, LOW);

  String LVGL_Arduino = "마레캠핑카 콘트롤러! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch() + '\n';

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");
  lv_print("마레캠핑카 콘트롤러 시작...");

  apSSID = EEPROM.readString(AP_SSID_EEPROM);
  apPSWD = EEPROM.readString(AP_PSWD_EEPROM);

  networkFlag = EEPROM.readBool(NETWORK_EEPROM);

  if (networkFlag == STATION_MODE) {
    Serial.println("Network Select Station Mode...");

    stSSID.toCharArray(ssidAp, stSSID.length());
    stPSWD.toCharArray(passwordAp, stPSWD.length());

    stSSID.toCharArray(_label_text, stSSID.length());
    lv_label_set_text(ui_LabelSSID, _label_text);
    stPSWD.toCharArray(_label_text, stPSWD.length());
    lv_label_set_text(ui_LabelPSWD, _label_text);
    lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
  } else {
    Serial.println("Network Select AP Mode...");

    apSSID.toCharArray(ssidAp, apSSID.length());
    apPSWD.toCharArray(passwordAp, apPSWD.length());

    apSSID.toCharArray(_label_text, apSSID.length());
    lv_label_set_text(ui_LabelSSID, _label_text);
    apPSWD.toCharArray(_label_text, apPSWD.length());
    lv_label_set_text(ui_LabelPSWD, _label_text);
    lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
  }

  if (EEPROM.readBool(AUTO_ON_1_EEPROM)) {
    lv_obj_add_state(ui_onSwitch1, LV_STATE_CHECKED);
    ledFlag[1] = true;
    ledcWrite(ledPwmCh[1], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_2_EEPROM)) {
    lv_obj_add_state(ui_onSwitch2, LV_STATE_CHECKED);
    ledFlag[2] = true;
    ledcWrite(ledPwmCh[2], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_3_EEPROM)) {
    lv_obj_add_state(ui_onSwitch3, LV_STATE_CHECKED);
    ledFlag[3] = true;
    ledcWrite(ledPwmCh[3], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_4_EEPROM)) {
    lv_obj_add_state(ui_onSwitch4, LV_STATE_CHECKED);
    ledFlag[4] = true;
    ledcWrite(ledPwmCh[4], PWM_ON);
  }
  if (EEPROM.readBool(AUTO_ON_5_EEPROM)) {
    lv_obj_add_state(ui_onSwitch5, LV_STATE_CHECKED);
    ledFlag[5] = true;
    ledcWrite(ledPwmCh[5], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_6_EEPROM)) {
    lv_obj_add_state(ui_onSwitch6, LV_STATE_CHECKED);
    ledFlag[6] = true;
    ledcWrite(ledPwmCh[6], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_7_EEPROM)) {
    lv_obj_add_state(ui_onSwitch7, LV_STATE_CHECKED);
    ledFlag[7] = true;
    ledcWrite(ledPwmCh[7], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_8_EEPROM)) {
    lv_obj_add_state(ui_onSwitch8, LV_STATE_CHECKED);
    ledFlag[8] = true;
    ledcWrite(ledPwmCh[8], PWM_ON);
  }

  if (EEPROM.readBool(AUTO_ON_9_EEPROM)) {
    lv_obj_add_state(ui_onSwitch9, LV_STATE_CHECKED);
    ledAllFlag = true;
    ledFlag[1] = true;
    ledFlag[2] = true;
    ledFlag[3] = true;
    ledFlag[4] = true;
    ledFlag[5] = true;
    ledFlag[6] = true;
    ledFlag[7] = true;
    ledFlag[8] = true;
    ledcWrite(ledPwmCh[1], PWM_ON);
    ledcWrite(ledPwmCh[2], PWM_ON);
    ledcWrite(ledPwmCh[3], PWM_ON);
    ledcWrite(ledPwmCh[4], PWM_ON);
    ledcWrite(ledPwmCh[5], PWM_ON);
    ledcWrite(ledPwmCh[6], PWM_ON);
    ledcWrite(ledPwmCh[7], PWM_ON);
    ledcWrite(ledPwmCh[8], PWM_ON);
  }

  ledNameInit();

  if (EEPROM.readBool(AUTO_ON_INV_EEPROM)) {
    lv_obj_add_state(ui_onSwitch10, LV_STATE_CHECKED);
    invertorFlag = true;
    digitalWrite(INVERTOR, HIGH);
  }

  if (EEPROM.readBool(AUTO_ON_TV_EEPROM)) {
    lv_obj_add_state(ui_onSwitch11, LV_STATE_CHECKED);
    tvFlag = true;
    digitalWrite(TV, HIGH);
  }

  if (EEPROM.readBool(AUTO_ON_FRIGE_EEPROM)) {
    lv_obj_add_state(ui_onSwitch12, LV_STATE_CHECKED);
    frigeFlag = true;
    digitalWrite(FRIGE, HIGH);
  }

  Serial.print("AP SSID : ");
  Serial.print(apSSID);
  Serial.print(", AP PSWD : ");
  Serial.println(apPSWD);
  Serial.print("ST SSID : ");
  Serial.print(stSSID);
  Serial.print(", ST PSWD : ");
  Serial.println(stPSWD);

  Serial.println("Setup done.....");
  Serial.println();
  // wifiStart();

  if (ledFlag[1]) {
    s = "LED1:";
    s += pwmPushValue[1];
    s += ':';
    s += ledFlag[1];
    comSend("COM:1");
    comSend(s);
  }
  if (ledFlag[2]) {
    s = "LED2:";
    s += pwmPushValue[2];
    s += ':';
    s += ledFlag[2];
    comSend(s);
  }
  if (ledFlag[3]) {
    s = "LED3:";
    s += pwmPushValue[3];
    s += ':';
    s += ledFlag[3];
    comSend(s);
  }
  if (ledFlag[4]) {
    s = "LED4:";
    s += pwmPushValue[4];
    s += ':';
    s += ledFlag[4];
    comSend(s);
  }
  if (ledFlag[5]) {
    s = "LED5:";
    s += pwmPushValue[5];
    s += ':';
    s += ledFlag[5];
    comSend(s);
  }
  if (ledFlag[6]) {
    s = "LED6:";
    s += pwmPushValue[6];
    s += ':';
    s += ledFlag[6];
    comSend(s);
  }
  if (ledFlag[7]) {
    s = "LED7:";
    s += pwmPushValue[7];
    s += ':';
    s += ledFlag[7];
    comSend(s);
  }
  if (ledFlag[8]) {
    s = "LED8:";
    s += pwmPushValue[8];
    s += ':';
    s += ledFlag[8];
    comSend(s);
  }
  if (invertorFlag) {
    s = "INV:";
    s += invertorFlag;
    comSend(s);
  }
  if (tvFlag) {
    s = "TV:";
    s += tvFlag;
    comSend(s);
  }
  if (frigeFlag) {
    s = "FRIGE:";
    s += frigeFlag;
    comSend(s);
  }
  if (ledFlag[1] && ledFlag[2] && ledFlag[3] && ledFlag[4] && ledFlag[5] &&
      ledFlag[6] && ledFlag[7] && ledFlag[8]) {
    ledAllFlag = true;
    s = "LED_ALL:";
    s += ledAllFlag;
    comSend(s);
  }
}

void loop() {

  lv_timer_handler(); /* let the GUI do its work */

  if (wifiStartFlag)
    server.handleClient();

  if (millis() - previousMillis >= 2000) {
    Serial.print("WiFi.status() : ");
    Serial.println(WiFi.status());

    Serial.print("WifiConnectFlag : ");
    Serial.println(wifiStartFlag ? "WiFi Start..." : "WiFi Stop...");

    Serial.print("Communication Count : ");
    Serial.println(comCount);

    informationRead();

    if ((networkFlag == STATION_MODE) && (WiFi.status() == WL_CONNECTED)) {
      hotSpotIP = WiFi.localIP().toString();
      hotSpotIP.toCharArray(_label_text, sizeof(_label_text));
      lv_label_set_text(ui_Label15, _label_text);
      lv_obj_clear_flag(ui_PanelIP, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_ImageHotSpot, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(ui_PanelIP, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_ImageHotSpot, LV_OBJ_FLAG_HIDDEN);
    }

    previousMillis = millis();

    s = String(aux_v, 1);
    s += " V,   ";
    s += String(temperature, 1);
    s += " 'C,   ";
    s += String(humidity, 1);
    s += " %";
    s.toCharArray(_label_text, sizeof(_label_text));
    lv_label_set_text(ui_Label1, _label_text);

    comCount++;
    if (popActiveFlag)
      popCount++;
  }

  if (popCount > 60 && popActiveFlag) { // 팝업 구동 후 약 3분 이면 정지...
    popCount = 0;
    popUpFlag = false;
    popDownFlag = false;
    popStopFlag = true;
    popActiveFlag = false;
    digitalWrite(POP_UP, LOW);
    digitalWrite(POP_DOWN, LOW);
    comSend("POP_STOP:1");
    lv_print("팝업 텐트 정지...");
  }

  if (comCount > 3) {
    if (!wifiStartFlag)
      wifiStart();
    if (comCount > 1000)
      comCount = 10;
  }

  if (!wifiStartFlag) {
    if (networkFlag == AP_MODE)
      WiFi.softAPdisconnect(); // 통신이 계속 되고 있으면 WiFi 종료...
    else
      WiFi.disconnect();

    lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ImageHotSpot, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PanelIP, LV_OBJ_FLAG_HIDDEN);
  }

  if (human != digitalRead(HUMAN)) {
    human = digitalRead(HUMAN);
    if (human) {
      human = true;
      Serial.println("Human on...");
    } else {
      human = false;
      Serial.println("Human off...");
    }
    s = "HUMAN";
    s += ':';
    s += human;
    comSend(s);
  }

  delay(5);
  globalCount++;
}
