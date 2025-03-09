/* 2025년 3월 5일

불필요 주석 삭제...

카인드 캠핑카 V3.0 프로그램
https://squareline.io/  camtrol@naver.com   gustnr99**

WiFi.status() == 1  // 연결 않됨...  (Station Mode)
WiFi.status() == 3  // 연결됨...  (Station Mode)

    프로그램 내용
    1. 화면(ST7789) 은 좌우 스와 온도 :이프로 메인 Display,
       QR Code만 표시 한다.메인 화면은 TextArea를 사용 한다.
    2. WiFi는 기존의 Station 모드를 기본으로 하되 다음을 준수 한다.
      a.Tab에서는 SSID 직접 선택 방식으로 한다.
        2024년 1월 15일 현재 구현 않됨...
        (QR Code 적용 해야 함.)
      b.휴대폰에서는 SSID 선택 방식으로 한다.
    3. 8채널(조명)아나로그 입력 및 출력 구현 됨.
    4. 3채널(인버터, TV, 냉장고)입력 및 출력 구현 됨.
    5. 아나로그 지령과 WiFi 지령을 구분하여 제어 됨.

  SquareLine UI Insert 하기
    - SQL의 Library 폴더에 있는 ui 폴더를 VSC의 src 폴더에
      Drag & Drop 한다.

    추 후 할일
    1. 준비된 Display제품의 RS485 통신 내용 구현 해야함.
    2. AP 통신을 위한 방안 마련 해야 함.

*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "ui/ui.h"
#include <WiFi.h>

#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define STATION_MODE true
#define AP_MODE false

#define CHECKED 3 // Station WiFi Status Connected...

#define ENABLE 5 // RS485 Enable...

// #define PWM_ON 255
#define PWM_ON 1023
#define ON HIGH
#define OFF LOW

#define COM_LED 1

#define LED1 14
#define LED2 21
#define LED3 47
#define LED4 48

// #define TRUNK_DOOR 8
#define TRUNK_DOOR 46

#define SWITCH_1 17
#define SWITCH_2 16
#define SWITCH_3 20 // OFF : Carind, ON : MS
// #define SWITCH_4 19 // Serial No. 006 ~ 010 번 까지 해당...
#define SWITCH_4 18 // OFF : 스타리아, ON : 카니발

#define PUMP 45
#define CIGAR 35
#define FRIGE 36
#define TV 37
#define INVERTOR 38

#define AP_SSID_EEPROM 20
#define AP_PSWD_EEPROM 70
#define ST_SSID_EEPROM 120
#define ST_PSWD_EEPROM 170

// EEPROM Address...
#define NETWORK_EEPROM 2 // bool

// setting PWM properties
const int pwmFreq = 240;
const int ledPwmCh[6] = {0, 0, 1, 2, 3};
const int resolution = 10;

int ledCh[6] = {0, LED1, LED2, LED3, LED4};
int lvPrintCount = 0;

uint16_t ledPwm[6] = {100, 100, 100, 100, 100, 100};
int loopCount = 55;
uint8_t rs485Count = 0;
uint8_t wifiCount = 0;

unsigned long previousMillis = 0;

char ssidAp[30] = "카인드 V2.0-CD-001";
char passwordAp[30] = "18250101001";
char text[50];
char msg[100];

String apSSID, apPSWD, stSSID, stPSWD, hotSpotIP;

bool ledComFlag = false; // LED COM Flag

bool networkFlag = false;     // Network Select Switch...
bool hotSpotFlag = false;     // Station Connect Flag...
bool wifiStartFlag = false;   // WiFi Start Flag...
bool wifiConnectFlag = false; // WiFi Connect Flag...
bool apModeStartFlag = false; // Station Connect Flag...
bool rs485Flag = false;       // RS485 Connect Flag...

bool wifiFlag = false; // Analog, WiFi 구분...

bool ledFlag[6] = {0, 0, 0, 0, 0, 0};

bool ledAllFlag = false;
bool sleepFlag = false;

bool pumpFlag = false;
bool cigarFlag = false;
bool frigeFlag = false;
bool tvFlag = false;
bool invertorFlag = false;

bool companyFlag = false;
bool trunkDoorFlag = false;

bool switch3 = false;
bool switch4 = false;

bool logoFlag = false;

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload, s;

WebServer server(80);

// 0.96" 80 x 160 ST7735S
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7735S _panel_instance;
  lgfx::Bus_SPI _bus_instance; // SPIバスのインスタンス

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI3_HOST; // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or
                                // SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      cfg.spi_mode = 3;         // 2 SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 60000000; // 送信時のSPIクロック (最大80MHz,
      // 80MHzを整数で割った値に丸められます)
      cfg.freq_read = 16000000; // 受信時のSPIクロック
      cfg.spi_3wire = false; // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock = true; // トランザクションロックを使用する場合はtrueを設定
      // cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定
      cfg.dma_channel = 0; // 使用するDMAチャンネルを設定
                           // (0=DMA不使用 / 1=1ch / 2=ch /
                           // SPI_DMA_CH_AUTO=自動設定)
      cfg.pin_sclk = 2;    // SPIのSCLKピン番号を設定
      cfg.pin_mosi = 42;   // SPIのMOSIピン番号を設定
      cfg.pin_miso = -1;   // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc = 41;     // SPIのD/Cピン番号を設定  (-1 = disable)
      _bus_instance.config(cfg); // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {
      auto cfg =
          _panel_instance.config(); // 表示パネル設定用の構造体を取得します。
      cfg.pin_cs = 39; // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst = 40; // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)
      cfg.panel_width = 160;  // 172, 240 実際に表示可能な幅
      cfg.panel_height = 160; // 320, 280 実際に表示可能な高さ
      // cfg.offset_x = -14;     // パネルのX方向オフセット量
      cfg.offset_x = 14; // パネルのX方向オフセット量
      cfg.offset_y = 0;  // パネルのY方向オフセット量
      cfg.offset_rotation = 2; // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel = 8; // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits =
          1;               // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable = true; // データ読出しが可能な場合 trueに設定
      cfg.invert = true; // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order =
          false; // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit =
          false;             // 16bitパラレルやSPIでデータ長を16bit単位で送信するパネルの場合
                             // trueに設定
      cfg.bus_shared = true; // SDカードとバスを共有している場合
      // trueに設定(drawJpgFile等でバス制御を行います)
      cfg.memory_width = 160; // ドライバICがサポートしている最大の幅
      cfg.memory_height = 160; // ドライバICがサポートしている最大の高さ
      _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

void wifiStart();

void ledDisp(byte rep, byte wait) {
  for (rep; rep > 0; rep--) {
    digitalWrite(COM_LED, HIGH);
    delay(wait);
    digitalWrite(COM_LED, LOW);
    delay(wait);
  }
}

void comSend(String sendData) {
  digitalWrite(ENABLE, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(ENABLE, LOW);
}

void lv_print(String msg) {
  if (lvPrintCount != 0)
    lv_textarea_add_text(ui_TextArea1, "\n");
  if (lvPrintCount > 50) {
    lv_textarea_set_text(ui_TextArea1, "");
    lvPrintCount = 0;
  }
  char _text[50];
  msg.toCharArray(_text, sizeof(_text));
  lv_textarea_add_text(ui_TextArea1, _text);
  lvPrintCount++;
}

void logoScreenExe() {
  loopCount = 55;
  if (logoFlag) {
    logoFlag = false;
    _ui_screen_change(&ui_ScreenText, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0,
                      &ui_ScreenText_screen_init);
    lv_disp_load_scr(ui_ScreenText);
  }
}

void serialEvent2() {
  rs485Count = 0;
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
  if (value != 0) {
    if (trunkDoorFlag)
      // 카니발 모드 일 때...
      pwm = map(value, 1, 100, 1, PWM_ON);
    else
      // 스타리아 모드일 때...
      pwm = PWM_ON;
  }

  if (chanel == "LED1") {
    logoScreenExe();
    ledcWrite(ledPwmCh[1], pwm);
    ledPwm[1] = value;
    if (value != 0) {
      ledFlag[1] = true;
    } else {
      ledFlag[1] = false;
      ledAllFlag = false;
    }
    s = "무드등 ";
    ledFlag[1] ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "LED2") {
    logoScreenExe();
    ledcWrite(ledPwmCh[2], pwm);
    ledPwm[2] = value;
    if (value != 0) {
      ledFlag[2] = true;
    } else {
      ledFlag[2] = false;
      ledAllFlag = false;
    }
    s = "실내등 ";
    ledFlag[2] ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "LED3") {
    logoScreenExe();
    ledcWrite(ledPwmCh[3], pwm);
    ledPwm[3] = value;
    if (value != 0) {
      ledFlag[3] = true;
    } else {
      ledFlag[3] = false;
      ledAllFlag = false;
    }
    trunkDoorFlag ? s = "예 비" : s = "트렁크 ";
    ledFlag[3] ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "LED4") {
    logoScreenExe();
    ledcWrite(ledPwmCh[4], pwm);
    ledPwm[4] = value;
    if (value != 0) {
      ledFlag[4] = true;
    } else {
      ledFlag[4] = false;
      ledAllFlag = false;
    }
    trunkDoorFlag ? s = "트렁크등 " : s = "어닝등 ";
    ledFlag[4] ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "PUMP") { // 물펌프
    logoScreenExe();
    if (trunkDoorFlag) {
      lv_print("트렁크 열림.");
      digitalWrite(TRUNK_DOOR, HIGH);
      delay(500);
      digitalWrite(TRUNK_DOOR, LOW);
      pumpFlag = false;
      s = "PUMP:";
      s += pumpFlag;
      s += ':';
      s += pumpFlag;
      comSend(s);
    } else {
      value == 1 ? digitalWrite(PUMP, HIGH) : digitalWrite(PUMP, LOW);
      if (value) {
        pumpFlag = true;
      } else {
        pumpFlag = false;
      }
      s = "물펌프 ";
      pumpFlag ? s += "ON" : s += "OFF";
      lv_print(s);
    }
  } else if (chanel == "CIGAR") { // 시거잭
    logoScreenExe();
    value == 1 ? digitalWrite(CIGAR, HIGH) : digitalWrite(CIGAR, LOW);
    if (value) {
      cigarFlag = true;
    } else {
      cigarFlag = false;
    }
    trunkDoorFlag ? s = "시거잭1 " : s = "시거잭 ";
    cigarFlag ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "FRIGE") { // 냉장고
    logoScreenExe();
    value == 1 ? digitalWrite(FRIGE, HIGH) : digitalWrite(FRIGE, LOW);
    if (value) {
      frigeFlag = true;
    } else {
      frigeFlag = false;
    }
    trunkDoorFlag ? s = "시거잭2 " : s = "냉장고 ";
    frigeFlag ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "TV") { // TV
    logoScreenExe();
    value == 1 ? digitalWrite(TV, HIGH) : digitalWrite(TV, LOW);
    if (value) {
      tvFlag = true;
    } else {
      tvFlag = false;
    }
    trunkDoorFlag ? s = "상부등 " : s = "TV ";
    tvFlag ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "INV") { // 인버터
    logoScreenExe();
    value == 1 ? digitalWrite(INVERTOR, HIGH) : digitalWrite(INVERTOR, LOW);
    if (value) {
      invertorFlag = true;
    } else {
      invertorFlag = false;
    }
    s = "인버터 ";
    invertorFlag ? s += "ON" : s += "OFF";
    lv_print(s);
  } else if (chanel == "SLEEP") {
    if (value) {
      sleepFlag = true;
    } else
      sleepFlag = false;
  } else if (chanel == "LED_ALL") {
    if (value != 0) {
      ledAllFlag = true;

      ledFlag[1] = true;
      ledFlag[2] = true;
      // if (!trunkDoorFlag)
      ledFlag[3] = true;
      ledFlag[4] = true;

      ledPwm[1] = 100;
      ledPwm[2] = 100;
      // if (!trunkDoorFlag)
      ledPwm[3] = 100;
      ledPwm[4] = 100;
    } else {
      ledAllFlag = false;

      ledFlag[1] = false;
      ledFlag[2] = false;
      // if (!trunkDoorFlag)
      ledFlag[3] = false;
      ledFlag[4] = false;

      ledPwm[1] = 0;
      ledPwm[2] = 0;
      // if (!trunkDoorFlag)
      ledPwm[3] = 0;
      ledPwm[4] = 0;
    }
    ledcWrite(ledPwmCh[1], pwm);
    ledcWrite(ledPwmCh[2], pwm);
    // if (!trunkDoorFlag)
    ledcWrite(ledPwmCh[3], pwm);
    ledcWrite(ledPwmCh[4], pwm);
    // if (trunkDoorFlag) {
    //   ledFlag[3] = false;
    //   s = "LED3:";
    //   s += 0;
    //   s += ':';
    //   s += 0;
    //   comSend(s);
    // }
  } else if (chanel == "REQUEST") {
    Serial.println("REQUEST receive OK.................");
    s = "LED1:";
    s += ledPwm[1];
    s += ':';
    s += ledFlag[1];
    comSend(s);
    delay(5);
    comSend(s);
    s = "LED2:";
    s += ledPwm[2];
    s += ':';
    s += ledFlag[2];
    comSend(s);
    s = "LED3:";
    s += ledPwm[3];
    s += ':';
    s += ledFlag[3];
    comSend(s);
    s = "LED4:";
    s += ledPwm[4];
    s += ':';
    s += ledFlag[4];
    comSend(s);
    s = "PUMP:";
    s += pumpFlag;
    s += ':';
    s += pumpFlag;
    comSend(s);
    s = "CIGAR:";
    s += cigarFlag;
    s += ':';
    s += cigarFlag;
    comSend(s);
    s = "FRIGE:";
    s += frigeFlag;
    s += ':';
    s += frigeFlag;
    comSend(s);
    s = "TV:";
    s += tvFlag;
    s += ':';
    s += tvFlag;
    comSend(s);
    s = "INV:";
    s += invertorFlag;
    s += ':';
    s += invertorFlag;
    comSend(s);
  } else if (chanel == "AP_SSID") { // Access Point Mode
    Serial.println();
    Serial.print("AP_SSID String Length : ");
    Serial.println(str2.length());

    if (str2.length() == 22) {
      Serial.println("AP_SSID Receive OK...");
      if (EEPROM.readString(AP_SSID_EEPROM) != str2) {
        apSSID = str2;
        EEPROM.writeString(AP_SSID_EEPROM, str2);
        EEPROM.commit();
      }
      str2.toCharArray(ssidAp, str2.length());
      Serial.println("AP_SSID Change OK...");
    }
  } else if (chanel == "AP_PSWD") { // Access Point Mode
    Serial.println();
    Serial.print(", AP_PSWD String Length : ");
    Serial.println(str2.length());
    if (str2.length() == 12) {
      Serial.println("AP_PSWD Receive OK...");
      if (EEPROM.readString(AP_PSWD_EEPROM) != str2) {
        apPSWD = str2;
        EEPROM.writeString(AP_PSWD_EEPROM, str2);
        EEPROM.commit();
        logoScreenExe();
        apSSID = EEPROM.readString(AP_SSID_EEPROM);
        apPSWD = EEPROM.readString(AP_PSWD_EEPROM);
        apSSID.toCharArray(ssidAp, apSSID.length());
        apPSWD.toCharArray(passwordAp, apPSWD.length());
        lv_label_set_text(ui_LabelSSID, ssidAp);
        lv_label_set_text(ui_LabelPSWD, passwordAp);
        lv_disp_load_scr(ui_ScreenHome);
      }
      str2.toCharArray(passwordAp, str2.length());
      Serial.println("AP_PSWD Change OK...");
      wifiStart();
    }
  } else if (chanel == "ST_SSID") { // Station Mode
    Serial.print(", ST_SSID String Length : ");
    Serial.print(str2.length());
    if (EEPROM.readString(ST_SSID_EEPROM) != str2) {
      stSSID = str2;
      EEPROM.writeString(ST_SSID_EEPROM, str2);
      EEPROM.commit();
    }
    str2.toCharArray(ssidAp, str2.length());
  } else if (chanel == "ST_PSWD") { // Station Mode
    Serial.print(", ST_PSWD String Length : ");
    Serial.println(str2.length());
    if (EEPROM.readString(ST_PSWD_EEPROM) != str2) {
      stPSWD = str2;
      EEPROM.writeString(ST_PSWD_EEPROM, str2);
      EEPROM.commit();
    }
    str2.toCharArray(passwordAp, str2.length());
    wifiStart();
  } else if (chanel == "COM") {
    if (!rs485Flag) {

      wifiStartFlag = false; // WiFi 통신 해제...

      // if (networkFlag == AP_MODE)
      //   WiFi.softAPdisconnect(); // 통신이 계속 되고 있으면 WiFi 종료...
      // else
      //   WiFi.disconnect();

      rs485Flag = true;
    }
    s = "COM:1";
    comSend(s);
  }

  Serial.print("Command: ");
  Serial.print(chanel);
  Serial.print(", Value: ");
  Serial.print(str2);
  Serial.print(", PWM: ");
  Serial.println(pwm);
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
  String ledName = root["LED_NAME"];
  String nowTime = root["NOW_TIME"];
  int chanel = root["Chanel"];
  int value = root["Value"];

  if (division == "LED") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;

    if (value == 0) {
      ledFlag[chanel] = false;
      ledcWrite(ledPwmCh[chanel], 0);
    } else {
      ledFlag[chanel] = true;
      ledPwm[chanel] = value;
      if (trunkDoorFlag)
        // 카니발 모드 일 때...
        ledcWrite(ledPwmCh[chanel], map(value, 1, 100, 1, PWM_ON));
      else {
        // 스타리아 모드 일 때...
        ledcWrite(ledPwmCh[chanel], PWM_ON);
        ledPwm[chanel] = 100;
      }
    }

    if (!ledFlag[1] && !ledFlag[2] && !ledFlag[3] && !ledFlag[4])
      ledAllFlag = false;

    Serial.print("ledPwm[");
    Serial.print(chanel);
    Serial.print("] = ");
    Serial.println(ledPwm[chanel]);

    s = "LED";
    s += chanel;
    s += ":";
    s += ledPwm[chanel];
    s += ":";
    s += ledFlag[chanel];
    comSend(s);
    Serial.print("S - ");
    Serial.println(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "LED_ALL") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    if (value == 1) { // All LED On...
      ledAllFlag = true;
      for (int i = 1; i < 5; i++) {
        ledcWrite(ledPwmCh[i], PWM_ON);
        ledPwm[i] = 100;
        ledFlag[i] = true;
      }
    } else { // All LED Off...
      ledAllFlag = false;
      for (int i = 1; i < 5; i++) {
        ledcWrite(ledPwmCh[i], 0);
        ledFlag[i] = false;
      }
    }
    s = "LED_ALL:";
    s += ledAllFlag;
    s += ":";
    s += ledAllFlag;
    comSend(s);
    // if (trunkDoorFlag) {
    //   ledcWrite(ledPwmCh[3], 0);
    //   ledFlag[3] = false;
    // }

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "PUMP") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    pumpFlag = true;
    // s = "PUMP:";
    // s += pumpFlag;
    // s += ':';
    // s += pumpFlag;
    // comSend(s);
    if (trunkDoorFlag) {
      comSend("PUMP:1:1");
      digitalWrite(TRUNK_DOOR, HIGH);
      delay(500);
      digitalWrite(TRUNK_DOOR, LOW);
      comSend("PUMP:0:0");
      pumpFlag = false;
      // s = "PUMP:";
      // s += pumpFlag;
      // s += ':';
      // s += pumpFlag;
      // comSend(s);
    } else {
      if (value) {
        digitalWrite(PUMP, HIGH);
        pumpFlag = true;
      } else {
        digitalWrite(PUMP, LOW);
        pumpFlag = false;
      }
      s = "PUMP:";
      s += pumpFlag;
      s += ':';
      s += pumpFlag;
      comSend(s);
    }

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "CIGAR") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    if (value) {
      digitalWrite(CIGAR, HIGH);
      cigarFlag = true;
    } else {
      digitalWrite(CIGAR, LOW);
      cigarFlag = false;
    }

    s = "CIGAR:";
    s += cigarFlag;
    s += ":";
    s += cigarFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "FRIGE") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    if (value) {
      digitalWrite(FRIGE, HIGH);
      frigeFlag = true;
    } else {
      digitalWrite(FRIGE, LOW);
      frigeFlag = false;
    }

    s = "FRIGE:";
    s += frigeFlag;
    s += ":";
    s += frigeFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "TV") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    // digitalWrite(WIFI_LED, HIGH);
    if (value) {
      digitalWrite(TV, HIGH);
      tvFlag = true;
    } else {
      digitalWrite(TV, LOW);
      tvFlag = false;
    }

    s = "TV:";
    s += tvFlag;
    s += ":";
    s += tvFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "INVERTOR") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    if (value) {
      digitalWrite(INVERTOR, HIGH);
      invertorFlag = true;
    } else {
      digitalWrite(INVERTOR, LOW);
      invertorFlag = false;
    }

    s = "INVERTOR:";
    s += invertorFlag;
    s += ":";
    s += invertorFlag;
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "SLEEP") {
    logoScreenExe();
    // wifiFlag = true;
    // remoteFlag = true;
    // ledFlag[1] = false;
    // ledFlag[2] = false;
    // ledFlag[3] = false;
    // ledFlag[4] = false;
    if (value) {
      // ledcWrite(ledPwmCh[1], 0);
      // ledcWrite(ledPwmCh[2], 0);
      // ledcWrite(ledPwmCh[3], 0);
      // ledcWrite(ledPwmCh[4], 0);
      sleepFlag = true;
    } else
      sleepFlag = false;

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";

    s = "SLEEP:";
    s += value;
    s += ':';
    s += value;
    comSend(s);
  } else if (division == "LED_NAME") {
    Serial.print("division : ");
    Serial.print(division);
    Serial.print(", Chanel : ");
    Serial.print(chanel);
    Serial.print(", LED_NAME : ");
    Serial.println(ledName);

    if (chanel != 10) {
      s = "LED_NAME:";
      s += chanel;
      s += ':';
      s += ledName;
      comSend(s);
    } else {
      s = "NAME_SAVE:";
      s += 1;
      s += ':';
      s += 1;
      comSend(s);
    }
  } else if (division == "TIME") {
    String week;
    week = nowTime[20];
    week += nowTime[21];
    week += nowTime[22];
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
    Serial.print("초 ");
    Serial.print(week);
    Serial.println(" 요일 ");

    int year = ((nowTime[2] - '0') * 10) + (nowTime[3] - '0') + 2000;
    int month = ((nowTime[5] - '0') * 10) + (nowTime[6] - '0');
    int day = ((nowTime[8] - '0') * 10) + (nowTime[9]) - '0';
    int hour = ((nowTime[11] - '0') * 10) + (nowTime[12] - '0');
    int min = ((nowTime[14] - '0') * 10) + (nowTime[15] - '0');
    int sec = ((nowTime[17] - '0') * 10) + (nowTime[18] - '0');

    s = "T_YEAR:";
    s += year;
    comSend(s);

    s = "T_MONTH:";
    s += month;
    comSend(s);

    s = "T_DAY:";
    s += day;
    comSend(s);

    s = "T_HOUR:";
    s += hour;
    comSend(s);

    s = "T_MIN:";
    s += min;
    comSend(s);

    s = "T_WEEK:";
    if (week == "Mon")
      s += 0;
    else if (week == "Tue")
      s += 1;
    else if (week == "Wed")
      s += 2;
    else if (week == "Thu")
      s += 3;
    else if (week == "Fri")
      s += 4;
    else if (week == "Sat")
      s += 5;
    else if (week == "Sun")
      s += 6;
    comSend(s);

    s = "T_SEC:";
    s += sec;
    comSend(s);

    Serial.print("Year : ");
    Serial.println(year);
    Serial.print("Month : ");
    Serial.println(month);
    Serial.print("Day : ");
    Serial.println(day);
    Serial.print("Hour : ");
    Serial.println(hour);
    Serial.print("Min : ");
    Serial.println(min);
    Serial.print("Sec : ");
    Serial.println(sec);
    Serial.print("Week : ");
    Serial.println(week);
  } else if (division == "QR_OK") {
    comSend("QR_OK:1");
  }

  server.send(200, "text/plain", s);

} // handleJson()

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  server.send(200, "text/plain", "");
}

void handleRequest() {
  wifiCount = 0;
  s = "";
  s = s + "{" + "\"ledValue0\":" + ledPwm[0] + ", " + "\"ledValue1\":" +
      ledPwm[1] + ", " + "\"ledValue2\":" + ledPwm[2] + ", " +
      "\"ledValue3\":" + ledPwm[3] + ", " + "\"ledValue4\":" + ledPwm[4] +
      ", " + "\"ledFlag0\":" + ledFlag[0] + ", " + "\"ledFlag1\":" +
      ledFlag[1] + ", " + "\"ledFlag2\":" + ledFlag[2] + ", " +
      "\"ledFlag3\":" + ledFlag[3] + ", " + "\"ledFlag4\":" + ledFlag[4] +
      ", " + "\"PUMP\":" + pumpFlag + ", " + "\"CIGAR\":" + cigarFlag + ", " +
      "\"FRIGE\":" + frigeFlag + ", " + "\"TV\":" + tvFlag + ", " +
      "\"INVERTOR\":" + invertorFlag + ", " + "\"ledAllFlag\":" + ledAllFlag +
      ", " + "\"sleepFlag\":" + sleepFlag + ", " + "\"companyFlag\":" +
      companyFlag + ", " + "\"trunkFlag\":" + trunkDoorFlag + "}";

  server.send(200, "text/plain", s);

} // handleRequest()

void wifiStart() { // WiFi Mode Start...

  String msg;

  wifiStartFlag = true; // WiFi 통신 시작...
  rs485Flag = false;

  // if (networkFlag == AP_MODE) {

  // if (!apModeStartFlag) {
  WiFi.mode(WIFI_MODE_AP);

  Serial.println(F("setupWifi()..."));
  Serial.println(F("Access Point(WiFi) Mode Start..."));
  Serial.print("ssidAp : ");
  Serial.print(ssidAp);
  Serial.print(", passwordAp : ");
  Serial.println(passwordAp);

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
  apModeStartFlag = true;
  // }
  // }
  // else {
  //   Serial.println(F("setupWifi()..."));
  //   Serial.println(F("Hot Spot Mode Start..."));
  //   Serial.println();
  //   Serial.print("Hot Spot SSID: ");
  //   Serial.print(ssidAp);
  //   Serial.print(", Hot Spot PSWD: ");
  //   Serial.println(passwordAp);

  //   WiFi.mode(WIFI_STA);
  //   WiFi.begin(ssidAp, passwordAp);
  //   Serial.print("Connecting to WiFi ..");

  //   if (WiFi.status() == WL_CONNECTED) {
  //     hotSpotFlag = true;
  //     hotSpotIP = WiFi.localIP().toString();
  //     Serial.println();
  //     Serial.print("HotSpotIP : ");
  //     Serial.println(hotSpotIP);
  //   }

  //   Serial.println();
  //   Serial.println("\nConnected to the WiFi network");
  //   Serial.print("Local ESP32 IP: ");
  //   Serial.println(WiFi.localIP());
  //   Serial.print("Subnet Mask: ");
  //   Serial.println(WiFi.subnetMask());
  //   Serial.print("Gateway IP: ");
  //   Serial.println(WiFi.gatewayIP());
  //   Serial.print("DNS 1: ");
  //   Serial.println(WiFi.dnsIP(0));
  //   Serial.print("DNS 2: ");
  //   Serial.println(WiFi.dnsIP(1));
  // }
  server.on("/json", handleJson);
  server.on("/root", handleRoot);
  server.on("/Request", handleRequest);

  server.begin();
}

void dipSW_read(bool select) {

  if (!select) {
    if (digitalRead(SWITCH_3)) {
      companyFlag = false; // 카인드 모드
      lv_obj_clear_flag(ui_LabelCount, LV_OBJ_FLAG_HIDDEN);
      Serial.println("CarInd 모드");
      lv_print("CarInd 모드.");
    } else {
      lv_obj_add_flag(ui_LabelCount, LV_OBJ_FLAG_HIDDEN);
      companyFlag = true; // MS 모드
      Serial.println("MS 모드");
      lv_print("MS 모드.");
    }
  } else {
    if (digitalRead(SWITCH_4)) {
      trunkDoorFlag = false; // 트렁크 스타리아 모드
      Serial.println("스타리아 모드");
      lv_print("스타리아 모드.");
    } else {
      trunkDoorFlag = true; // 트렁크 카니발 모드
      Serial.println("카니발 모드");
      lv_print("카니발 모드.");
    }
  }
}

static LGFX tft; // LGFXのインスタンスを作成。

static const uint16_t screenWidth = 160;  // 172, 240
static const uint16_t screenHeight = 160; // 320, 280

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

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

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}

void setup() {

  pinMode(ENABLE, OUTPUT); // RS485 ENABLE

  // pinMode(39, OUTPUT);

  pinMode(TRUNK_DOOR, OUTPUT);
  pinMode(SWITCH_1, INPUT_PULLUP);
  pinMode(SWITCH_2, INPUT_PULLUP);
  pinMode(SWITCH_3, INPUT_PULLUP); // ON:MS, OFF:카인드
  pinMode(SWITCH_4, INPUT_PULLUP); // ON:카니발, OFF

  pinMode(COM_LED, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(PUMP, OUTPUT);
  pinMode(CIGAR, OUTPUT);
  pinMode(FRIGE, OUTPUT);
  pinMode(TV, OUTPUT);
  pinMode(INVERTOR, OUTPUT);

  digitalWrite(COM_LED, LOW);
  digitalWrite(ENABLE, LOW); // RS485 ENABLE

  digitalWrite(TRUNK_DOOR, LOW); // Trunk Door

  // configure LED PWM
  ledcSetup(ledPwmCh[1], pwmFreq, resolution);
  ledcSetup(ledPwmCh[2], pwmFreq, resolution);
  ledcSetup(ledPwmCh[3], pwmFreq, resolution);
  ledcSetup(ledPwmCh[4], pwmFreq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledCh[1], ledPwmCh[1]);
  ledcAttachPin(ledCh[2], ledPwmCh[2]);
  ledcAttachPin(ledCh[3], ledPwmCh[3]);
  ledcAttachPin(ledCh[4], ledPwmCh[4]);

  ledcWrite(ledPwmCh[1], 0);
  ledcWrite(ledPwmCh[2], 0);
  ledcWrite(ledPwmCh[3], 0);
  ledcWrite(ledPwmCh[4], 0);

  digitalWrite(PUMP, LOW);
  digitalWrite(CIGAR, LOW);
  digitalWrite(FRIGE, LOW);
  digitalWrite(TV, LOW);
  digitalWrite(INVERTOR, LOW);

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

  EEPROM.begin(250);

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 4, 6);

  int ahtStatus = 0;

  String LVGL_Arduino = "카인드캠핑카 콘트롤러! ";

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  apSSID = EEPROM.readString(AP_SSID_EEPROM);
  apPSWD = EEPROM.readString(AP_PSWD_EEPROM);
  stSSID = EEPROM.readString(ST_SSID_EEPROM);
  stPSWD = EEPROM.readString(ST_PSWD_EEPROM);

  // networkFlag = EEPROM.readBool(NETWORK_EEPROM);

  if (networkFlag == STATION_MODE) {
    Serial.println("Network Select Station Mode...");

    stSSID.toCharArray(ssidAp, stSSID.length());
    stPSWD.toCharArray(passwordAp, stPSWD.length());

  } else {
    Serial.println("Network Select AP Mode...");

    apSSID.toCharArray(ssidAp, apSSID.length());
    apPSWD.toCharArray(passwordAp, apPSWD.length());
    lv_label_set_text(ui_LabelSSID, ssidAp);
    lv_label_set_text(ui_LabelPSWD, passwordAp);
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

  lv_timer_handler(); /* let the GUI do its work */

  ledDisp(10, 30);

  dipSW_read(false);
  dipSW_read(true);
  switch3 = digitalRead(SWITCH_3);
  switch4 = digitalRead(SWITCH_4);
  lv_print("수신 대기중...");

  delay(1000);
  comSend("COM:1");
  delay(10);
  comSend("REQ_NET:1");

  wifiStart();
  Serial.println("WiFi Started....");
}

void loop() {

  server.handleClient();

  lv_timer_handler(); /* let the GUI do its work */

  if (millis() - previousMillis >= 2000) {
    Serial.print("WiFi.status() : ");
    Serial.println(WiFi.status());

    Serial.print("WifiConnectFlag : ");
    Serial.println(wifiStartFlag ? "WiFi Start..." : "WiFi Stop...");

    previousMillis = millis();

    if (wifiStartFlag) {
      ledDisp(3, 20);
    } else {
      if (ledComFlag) {
        digitalWrite(COM_LED, LOW);
        ledComFlag = false;
      } else {
        digitalWrite(COM_LED, HIGH);
        ledComFlag = true;
      }
    }
    if (!digitalRead(0)) {
      delay(700);
      if (!digitalRead(0)) {
        EEPROM.writeString(AP_SSID_EEPROM, "  ");
        EEPROM.writeString(AP_PSWD_EEPROM, "  ");
        EEPROM.commit();
        Serial.println("REQ_NET !");
        comSend("REQ_NET:1");
      }
      ledDisp(10, 30);
      // lv_disp_load_scr(ui_ScreenHome);
      // loopCount = 55;
      // logoFlag = false;
    }

    if (switch3 != digitalRead(SWITCH_3)) {
      dipSW_read(false);
      switch3 = digitalRead(SWITCH_3);
      logoScreenExe();
    } else if (switch4 != digitalRead(SWITCH_4)) {
      dipSW_read(true);
      switch4 = digitalRead(SWITCH_4);
      logoScreenExe();
    }
    if (loopCount <= 0 && !logoFlag && !companyFlag) {
      logoFlag = true;
      _ui_screen_change(&ui_ScreenLogo, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0,
                        &ui_ScreenLogo_screen_init);
      lv_disp_load_scr(ui_ScreenLogo);
    }

    if (loopCount < 0)
      loopCount = 0;
    Serial.print("\nloopCount : ");
    Serial.println(loopCount);
    Serial.print("logoFlag : ");
    Serial.println(logoFlag);
    Serial.print("rs485Count : ");
    Serial.println(rs485Count);
    Serial.print("wifiCount : ");
    Serial.println(wifiCount);
    s = loopCount;
    s.toCharArray(text, s.length() + 1);
    lv_label_set_text(ui_LabelCount, text);
    loopCount--;

    if (rs485Count > 2) {
      lv_obj_add_flag(ui_ImageNetWork, LV_OBJ_FLAG_HIDDEN);
      if (rs485Count > 100)
        rs485Count = 10;
    } else {
      lv_obj_clear_flag(ui_ImageNetWork, LV_OBJ_FLAG_HIDDEN);
    }
    if (wifiCount > 2) {
      lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
      if (wifiCount > 100)
        wifiCount = 10;
    } else {
      lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    }
    rs485Count++;
    wifiCount++;
  }
  delay(5);
}
