/* 2024년 6월 22일

LM Seat V1.1 조명 콘트롤 프로그램
https://squareline.io/  camtrol@naver.com   gustnr99**


해야 할 프로그램
    1. 시작 시 Switch, Door 입력 처리하기
    2. 무드등, 사이드, userSetting 화면처리 하기.

  SquareLine UI Insert 하기
    - SQL의 Library 폴더에 있는 ui 폴더를 VSC의 src 폴더에
      Drag & Drop 한다.

    추 후 할일
    1. 준비된 Display제품의 RS485 통신 내용 구현 해야함.
    2. AP 통신을 위한 방안 마련 해야 함.

*/

// #define _8574

#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#define STATION_MODE true
#define AP_MODE false

#define ENABLE 5 // RS485 Enable...

// #define PWM_ON 255
#define PWM_ON 1023
#define ON HIGH
#define OFF LOW

#define LED1 21
#define LED2 47
#define LED3 48
#define LED4 45
#define LED5 41
// #define FLOOR 41

#define LiPo 8
// #define POWER 17
#define SWITCH 19
#define DOOR 20

#define DOOR_TYPE 12

#define TV 38
#define FAN 40

#define NEO_PIN_1 15
#define NEO_PIN_2 16

#define NEO_1_RGB 9  // NEO 1 RGB Switch
#define NEO_2_RGB 10 // NEO 2 RGB Switch

#define SWITCH_LED 1
#define DOOR_LED 39

#define DOOR_A true  // 메인등 일반 PWM 제어...
#define DOOR_B false // 메인등 55' TV 제어...

#define NUMPIXELS 120

// EEPROM Address...
#define NETWORK_EEPROM 2 // bool

#define NEO_1_FLAG_EEPROM 10  // bool
#define NEO_1_START_EEPROM 12 // byte
#define NEO_1_TOTAL_EEPROM 14 // byte
#define NEO_1_RED_EEPROM 16   // byte
#define NEO_1_GREEN_EEPROM 18 // byte
#define NEO_1_BLUE_EEPROM 20  // byte

#define NEO_2_FLAG_EEPROM 24  // bool
#define NEO_2_START_EEPROM 26 // byte
#define NEO_2_TOTAL_EEPROM 28 // byte
#define NEO_2_RED_EEPROM 30   // byte
#define NEO_2_GREEN_EEPROM 32 // byte
#define NEO_2_BLUE_EEPROM 34  // byte

// #define DOOR_TYPE_EEPROM 36 // bool

#define TV_ADJ_EEPROM 37 // int16_t

#define NEO_1_MODE 40 // byte
#define NEO_2_MODE 42 // byte

#define BG_VALUE_EEPROM 43 // byte

#define AP_SSID_EEPROM 50
#define AP_PSWD_EEPROM 100
// #define ST_SSID_EEPROM 150
// #define ST_PSWD_EEPROM 200

#define LED_1_PWM_EEPROM 150  // byte
#define LED_2_PWM_EEPROM 151  // byte
#define LED_3_PWM_EEPROM 152  // byte
#define LED_4_PWM_EEPROM 153  // byte
#define LED_5_PWM_EEPROM 154  // byte
#define BTN_1_FLAG_EEPROM 155 // bool
#define BTN_2_FLAG_EEPROM 156 // bool
#define BTN_3_FLAG_EEPROM 157 // bool
#define BTN_4_FLAG_EEPROM 158 // bool
#define BTN_5_FLAG_EEPROM 159 // bool
#define BTN_6_FLAG_EEPROM 160 // bool
#define BTN_SELECT_EEPROM 161 // bool

Adafruit_NeoPixel strip1 =
    Adafruit_NeoPixel(NUMPIXELS, NEO_PIN_1, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel strip2 =
    Adafruit_NeoPixel(NUMPIXELS, NEO_PIN_2, NEO_BRG + NEO_KHZ800);

// setting PWM properties
const int pwmFreq = 240;
const int ledPwmCh[6] = {0, 0, 1, 2, 3, 4};
const int resolution = 10;

int ledCh[6] = {0, LED1, LED2, LED3, LED4, LED5};

int switchVolt, doorVolt;

uint8_t bright, btnSelect;
// uint8_t neo1Pwm, neo2Pwm = 100;
uint8_t loopCount = 0;
uint16_t stableCount = 600;
uint16_t pwmValue[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t pwmPushValue[9] = {100, 100, 100, 100, 100, 100, 100, 100, 100};

float tv_volt;

// NEO 변수...

float redStates[150];
float blueStates[150];
float greenStates[150];
float fadeRate = 0.96;

unsigned long previousMillis = 0;

uint8_t comCount = 0;
uint8_t switchOffCount = 0;
int16_t aux_adc, main_adc, tvAdj;

// byte neo_mode = 0;
byte neo1_mode, neo2_mode = 0;
byte neo1_r_color, neo1_g_color, neo1_b_color = 0;
byte neo2_r_color, neo2_g_color, neo2_b_color = 0;
byte neo1Start, neo2Start;
int16_t neo1Total = 250;
int16_t neo2Total = 250;

char _label_text[30];

char ssidAp[30] = "카인드 V1.0-LA-005";
char passwordAp[30] = "17240731005";
char msg[100];

String apSSID, apPSWD, stSSID, stPSWD, hotSpotIP;

bool networkFlag = false;     // Network Select Switch...
bool hotSpotFlag = false;     // Station Connect Flag...
bool wifiStartFlag = false;   // WiFi Start Flag...
bool wifiConnectFlag = false; // WiFi Connect Flag...
bool apModeStartFlag = false; // Station Connect Flag...
bool rs485Flag = false;       // RS485 Connect Flag...
bool rs485Enable = true;      // RS485 가능 Flag...

bool wifiFlag = false; // Analog, WiFi 구분...

bool ledFlag[6] = {0, 0, 0, 0, 0, 0};

// bool floorFlag = false;
bool tvFlag = false;

bool neoRandomFlag = true;

bool neo1TypeFlag, neo2TypeFlag, neo1rgbFlag, neo2rgbFlag;
bool neoModeChangeFlag = false;

bool switchFlag, doorFlag;
// bool switchFlagPush, doorFlag = true;
// bool switchFlag, doorFlagPush = false;
// bool doorFlag, doorFlagPush;
bool batLiPoFlag, batLiPoFlagPush;

void wifiStart();

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload, s;

WebServer server(80);

void comSend(String sendData) {

  rs485Enable = false;
  digitalWrite(ENABLE, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(ENABLE, LOW);
  rs485Enable = true;
}

void sendData() {
  // Touch로 data 전송...
  s = "LED1:";
  s += pwmPushValue[1];
  s += ':';
  s += ledFlag[1];
  Serial2.flush();
  comSend(s);
  s = "LED1:";
  s += pwmPushValue[1];
  s += ':';
  s += ledFlag[1];
  Serial2.flush();
  comSend(s);
  s = "LED2:";
  s += pwmPushValue[2];
  s += ':';
  s += ledFlag[2];
  Serial2.flush();
  comSend(s);
  s = "LED3:";
  s += pwmPushValue[3];
  s += ':';
  s += ledFlag[3];
  Serial2.flush();
  comSend(s);
  s = "LED4:";
  s += pwmPushValue[4];
  s += ':';
  s += ledFlag[4];
  Serial2.flush();
  comSend(s);
  s = "LED5:";
  s += ledFlag[5];
  s += ':';
  s += ledFlag[5];
  Serial2.flush();
  comSend(s);
  s = "TV:";
  s += tvFlag;
  s += ':';
  s += tvFlag;
  Serial2.flush();
  comSend(s);
  s = "NEO_1_FLAG:";
  s += neo1TypeFlag;
  s += ':';
  s += neo1TypeFlag;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_FLAG:";
  s += neo2TypeFlag;
  s += ':';
  s += neo2TypeFlag;
  Serial2.flush();
  comSend(s);
  s = "NEO_1_START:";
  s += neo1Start;
  s += ':';
  s += neo1Start;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_START:";
  s += neo2Start;
  s += ':';
  s += neo2Start;
  Serial2.flush();
  comSend(s);
  s = "NEO_1_TOTAL:";
  s += neo1Total;
  s += ':';
  s += neo1Total;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_TOTAL:";
  s += neo2Total;
  s += ':';
  s += neo2Total;
  Serial2.flush();
  comSend(s);
  s = "NEO_1_R_COLOR:";
  s += neo1_r_color;
  s += ':';
  s += neo1_r_color;
  Serial2.flush();
  comSend(s);
  s = "NEO_1_G_COLOR:";
  s += neo1_g_color;
  s += ':';
  s += neo1_g_color;
  comSend(s);
  delay(5);
  s = "NEO_1_B_COLOR:";
  s += neo1_b_color;
  s += ':';
  s += neo1_b_color;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_R_COLOR:";
  s += neo2_r_color;
  s += ':';
  s += neo2_r_color;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_G_COLOR:";
  s += neo2_g_color;
  s += ':';
  s += neo2_g_color;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_B_COLOR:";
  s += neo2_b_color;
  s += ':';
  s += neo2_b_color;
  Serial2.flush();
  comSend(s);
  s = "NEO_1_MODE:";
  s += neo1_mode;
  s += ':';
  s += neo1_mode;
  Serial2.flush();
  comSend(s);
  s = "NEO_2_MODE:";
  s += neo2_mode;
  s += ':';
  s += neo2_mode;
  Serial2.flush();
  comSend(s);
  s = "BG_VALUE:";
  s += bright;
  s += ':';
  s += bright;
  Serial2.flush();
  comSend(s);
  s = "BTN_SEL:";
  s += btnSelect;
  s += ':';
  s += btnSelect;
  Serial2.flush();
  comSend(s);
  s = "SWITCH:";
  s += switchFlag;
  s += ':';
  s += switchFlag;
  comSend(s);
  s = "DOOR:";
  s += doorFlag;
  s += ':';
  s += doorFlag;
  comSend(s);
  s = "SEND_OK:";
  s += true;
  s += ':';
  s += true;
  comSend(s);
}

void serialEvent2() {

  uint16_t value, colorValue;
  uint16_t pwm;

  String eventString = Serial2.readStringUntil('\n');
  Serial.print("EventString: ");
  Serial.println(eventString);

  uint8_t index1 = eventString.indexOf(":"); // 첫 번째 콤마 위치
  uint8_t index2 = eventString.indexOf(':', index1 + 1);
  uint8_t index3 = eventString.length();

  String _division = eventString.substring(0, index1);
  String _value = eventString.substring(index1 + 1, index2);
  String _flag = eventString.substring(index2 + 1, index3 - 1);

  value = _value.toInt();
  if (_flag != "0")
    pwm = map(value, 1, 100, 1, PWM_ON);

  if (_division == "LED1") {
    ledcWrite(ledPwmCh[1], pwm);
    pwmPushValue[1] = value;
    if (_flag != "0") {
      ledFlag[1] = true;
    } else {
      ledFlag[1] = false;
    }
    EEPROM.writeBool(BTN_1_FLAG_EEPROM, ledFlag[1]);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 1);
    EEPROM.commit();
  } else if (_division == "LED2") {
    if (neo1TypeFlag) {
      ledcWrite(ledPwmCh[2], PWM_ON);
      strip1.setBrightness(map(value, 1, 100, 1, 255));
      if (_flag != "0") {
        ledFlag[2] = true;
        if (neo1TypeFlag) {
          if (neo1_mode == 0) {
            for (uint16_t i = neo1Start; i < neo1Total; i++) {
              strip1.setPixelColor(
                  i, strip1.Color(neo1_r_color, neo1_g_color, neo1_b_color));
            }
            strip1.show();
            strip1.show();
          }
        }
      } else {
        ledcWrite(ledPwmCh[2], 0);
        ledFlag[2] = false;
      }
    } else {
      if (_flag != "0")
        ledFlag[2] = true;
      else
        ledFlag[2] = false;
      ledcWrite(ledPwmCh[2], pwm);
      strip1.clear();
      strip1.show();
    }
    delay(10);
    pwmPushValue[2] = value;
    EEPROM.writeByte(LED_2_PWM_EEPROM, pwmPushValue[2]);
    EEPROM.writeBool(BTN_2_FLAG_EEPROM, ledFlag[2]);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 2);
    EEPROM.commit();
  } else if (_division == "LED3") {
    if (neo2TypeFlag) {
      ledcWrite(ledPwmCh[3], PWM_ON);
      strip2.setBrightness(map(value, 1, 100, 1, 255));
      if (_flag != "0") {
        ledFlag[3] = true;
        if (neo2TypeFlag) {
          if (neo2_mode == 0) {
            for (uint16_t i = neo2Start; i < neo2Total; i++) {
              strip2.setPixelColor(
                  i, strip2.Color(neo2_r_color, neo2_g_color, neo2_b_color));
            }
            strip2.show();
            strip2.show();
          }
        }
      } else {
        ledcWrite(ledPwmCh[3], 0);
        ledFlag[3] = false;
      }
    } else {
      if (_flag != "0")
        ledFlag[3] = true;
      else
        ledFlag[3] = false;
      ledcWrite(ledPwmCh[3], pwm);
      strip2.clear();
      strip2.show();
    }
    delay(10);
    pwmPushValue[3] = value;
    EEPROM.writeByte(LED_3_PWM_EEPROM, pwmPushValue[3]);
    EEPROM.writeBool(BTN_3_FLAG_EEPROM, ledFlag[3]);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 3);
    EEPROM.commit();
  } else if (_division == "LED4") {
    ledcWrite(ledPwmCh[4], pwm);
    pwmPushValue[4] = value;
    if (_flag != "0") {
      ledFlag[4] = true;
    } else {
      ledFlag[4] = false;
    }
    EEPROM.writeBool(BTN_4_FLAG_EEPROM, ledFlag[4]);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 4);
    EEPROM.commit();
  } else if (_division == "LED5") {
    ledcWrite(ledPwmCh[5], pwm);
    pwmPushValue[5] = value;
    if (_flag != "0") {
      ledFlag[5] = true;
    } else {
      ledFlag[5] = false;
    }
    EEPROM.writeBool(BTN_5_FLAG_EEPROM, ledFlag[5]);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 5);
    EEPROM.commit();
  } else if (_division == "PWM_SET") {
    u_int8_t ch = _flag.toInt();
    if (ch == 1)
      EEPROM.writeByte(LED_1_PWM_EEPROM, value);
    else if (ch == 2)
      EEPROM.writeByte(LED_2_PWM_EEPROM, value);
    else if (ch == 3)
      EEPROM.writeByte(LED_3_PWM_EEPROM, value);
    else if (ch == 4)
      EEPROM.writeByte(LED_4_PWM_EEPROM, value);
    else if (ch == 5)
      EEPROM.writeByte(LED_5_PWM_EEPROM, value);
    EEPROM.commit();
  } else if (_division == "NEO_R") {
    if (_flag == "1") {
      neo1_r_color = byte(value);
      ledcWrite(ledPwmCh[2], PWM_ON);
      ledFlag[2] = true;
      for (uint16_t i = neo1Start; i < neo1Total; i++) {
        if (neo1rgbFlag)
          strip1.setPixelColor(
              i, strip1.Color(neo1_r_color, neo1_g_color, neo1_b_color));
        else
          strip1.setPixelColor(
              i, strip1.Color(neo1_g_color, neo1_b_color, neo1_r_color));
      }
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
    } else {
      neo2_r_color = byte(value);
      ledcWrite(ledPwmCh[3], PWM_ON);
      ledFlag[3] = true;
      for (uint16_t i = neo2Start; i < neo2Total; i++) {
        if (neo2rgbFlag)
          strip2.setPixelColor(
              i, strip2.Color(neo2_r_color, neo2_g_color, neo2_b_color));
        else
          strip2.setPixelColor(
              i, strip2.Color(neo2_g_color, neo2_b_color, neo2_r_color));
      }
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
    }
  } else if (_division == "NEO_G") {
    if (_flag == "1") {
      neo1_g_color = byte(value);
      ledcWrite(ledPwmCh[2], PWM_ON);
      ledFlag[2] = true;
      for (uint16_t i = neo1Start; i < neo1Total; i++) {
        if (neo1rgbFlag)
          strip1.setPixelColor(
              i, strip1.Color(neo1_r_color, neo1_g_color, neo1_b_color));
        else
          strip1.setPixelColor(
              i, strip1.Color(neo1_g_color, neo1_b_color, neo1_r_color));
      }
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
    } else {
      neo2_g_color = byte(value);
      ledcWrite(ledPwmCh[3], PWM_ON);
      ledFlag[2] = true;
      for (uint16_t i = neo2Start; i < neo2Total; i++) {
        if (neo2rgbFlag)
          strip2.setPixelColor(
              i, strip2.Color(neo2_r_color, neo2_g_color, neo2_b_color));
        else
          strip2.setPixelColor(
              i, strip2.Color(neo2_g_color, neo2_b_color, neo2_r_color));
      }
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
    }
  } else if (_division == "NEO_B") {
    if (_flag == "1") {
      neo1_b_color = byte(value);
      ledcWrite(ledPwmCh[2], PWM_ON);
      ledFlag[2] = true;
      for (uint16_t i = neo1Start; i < neo1Total; i++) {
        if (neo1rgbFlag)
          strip1.setPixelColor(
              i, strip1.Color(neo1_r_color, neo1_g_color, neo1_b_color));
        else
          strip1.setPixelColor(
              i, strip1.Color(neo1_g_color, neo1_b_color, neo1_r_color));
      }
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
    } else {
      neo2_b_color = byte(value);
      ledcWrite(ledPwmCh[3], PWM_ON);
      ledFlag[3] = true;
      for (uint16_t i = neo2Start; i < neo2Total; i++) {
        if (neo2rgbFlag)
          strip2.setPixelColor(
              i, strip2.Color(neo2_r_color, neo2_g_color, neo2_b_color));
        else
          strip2.setPixelColor(
              i, strip2.Color(neo2_g_color, neo2_b_color, neo2_r_color));
      }
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
    }
  } else if (_division == "TV") {
    if (value) {
      digitalWrite(TV, HIGH);
      tvFlag = true;
    } else {
      digitalWrite(TV, LOW);
      tvFlag = false;
    }
    EEPROM.writeBool(BTN_6_FLAG_EEPROM, tvFlag);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 6);
    EEPROM.commit();
  } else if (_division == "TV_FORCED") {
    if (value) {
      digitalWrite(TV, HIGH);
      tvFlag = true;
    } else {
      digitalWrite(TV, LOW);
      tvFlag = false;
    }
    s = "TV:";
    s += tvFlag;
    s += ':';
    s += tvFlag;
    comSend(s);
  } else if (_division == "REQ_NEO") {
    Serial.println("REQUEST NEO OK.................");
    comSend("COM:1:1");
    s = "NEO_1_FLAG:";
    s += neo1TypeFlag;
    s += ':';
    s += neo1TypeFlag;
    comSend(s);
    s = "NEO_2_FLAG:";
    s += neo2TypeFlag;
    s += ':';
    s += neo2TypeFlag;
    comSend(s);
    s = "NEO_1_R_COLOR:";
    s += neo1_r_color;
    s += ':';
    s += neo1_r_color;
    comSend(s);
    s = "NEO_1_G_COLOR:";
    s += neo1_g_color;
    s += ':';
    s += neo1_g_color;
    comSend(s);
    delay(5);
    s = "NEO_1_B_COLOR:";
    s += neo1_b_color;
    s += ':';
    s += neo1_b_color;
    comSend(s);
    s = "NEO_2_R_COLOR:";
    s += neo2_r_color;
    s += ':';
    s += neo2_r_color;
    comSend(s);
    s = "NEO_2_G_COLOR:";
    s += neo2_g_color;
    s += ':';
    s += neo2_g_color;
    comSend(s);
    s = "NEO_2_B_COLOR:";
    s += neo2_b_color;
    s += ':';
    s += neo2_b_color;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1_mode;
    s += ':';
    s += neo1_mode;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2_mode;
    s += ':';
    s += neo2_mode;
    comSend(s);
    // sendData();
  } else if (_division == "REQ_DATA") {
    Serial.println("REQUEST DATA OK.................");
    comSend("COM:1:1");
    s = "NEO_1_FLAG:";
    s += neo1TypeFlag;
    s += ':';
    s += neo1TypeFlag;
    comSend(s);
    s = "NEO_2_FLAG:";
    s += neo2TypeFlag;
    s += ':';
    s += neo2TypeFlag;
    comSend(s);
    s = "NEO_1_R_COLOR:";
    s += neo1_r_color;
    s += ':';
    s += neo1_r_color;
    comSend(s);
    s = "NEO_1_G_COLOR:";
    s += neo1_g_color;
    s += ':';
    s += neo1_g_color;
    comSend(s);
    delay(10);
    s = "NEO_1_B_COLOR:";
    s += neo1_b_color;
    s += ':';
    s += neo1_b_color;
    comSend(s);
    s = "NEO_2_R_COLOR:";
    s += neo2_r_color;
    s += ':';
    s += neo2_r_color;
    comSend(s);
    s = "NEO_2_G_COLOR:";
    s += neo2_g_color;
    s += ':';
    s += neo2_g_color;
    comSend(s);
    s = "NEO_2_B_COLOR:";
    s += neo2_b_color;
    s += ':';
    s += neo2_b_color;
    comSend(s);
    s = "NEO_1_MODE:";
    s += neo1_mode;
    s += ':';
    s += neo1_mode;
    comSend(s);
    s = "NEO_2_MODE:";
    s += neo2_mode;
    s += ':';
    s += neo2_mode;
    comSend(s);
    s = "LED1:";
    s += pwmPushValue[1];
    s += ':';
    s += ledFlag[1];
    comSend(s);
    s = "LED2:";
    s += pwmPushValue[2];
    s += ':';
    s += ledFlag[2];
    comSend(s);
    s = "LED3:";
    s += pwmPushValue[3];
    s += ':';
    s += ledFlag[3];
    comSend(s);
    s = "LED4:";
    s += pwmPushValue[4];
    s += ':';
    s += ledFlag[4];
    comSend(s);
    delay(50);
    s = "LED5:";
    s += pwmPushValue[5];
    s += ':';
    s += ledFlag[5];
    comSend(s);
    s = "TV:";
    s += tvFlag;
    s += ':';
    s += tvFlag;
    comSend(s);
    s = "BG_VALUE:";
    s += bright;
    s += ':';
    s += bright;
    comSend(s);
    s = "BTN_SEL:";
    s += btnSelect;
    s += ':';
    s += btnSelect;
    comSend(s);

  } else if (_division == "REQ_FLAG") {
    // Serial.println("REQUEST FLAG OK.................");
    s = "SWITCH:";
    s += switchFlag;
    s += ':';
    s += switchFlag;
    comSend(s);
    s = "DOOR:";
    s += doorFlag;
    s += ':';
    s += doorFlag;
    comSend(s);
    s = "BAT:";
    s += batLiPoFlag;
    s += ':';
    s += batLiPoFlag;
    comSend(s);
  } else if (_division == "AP_SSID") { // Access Point Mode
    // Serial.print("AP_SSID String Length : ");
    // Serial.println(_value.length());
    if (EEPROM.readString(AP_SSID_EEPROM) != _value) {
      apSSID = _value;
      EEPROM.writeString(AP_SSID_EEPROM, _value);
      EEPROM.commit();
    }
  } else if (_division == "AP_PSWD") { // Access Point Mode
    // Serial.print(", AP_PSWD String Length : ");
    // Serial.println(_value.length());
    if (EEPROM.readString(AP_PSWD_EEPROM) != _value) {
      apPSWD = _value;
      EEPROM.writeString(AP_PSWD_EEPROM, _value);
      EEPROM.commit();
    }
  } else if (_division == "ST_SSID") { // Station Mode
    Serial.print(", ST_SSID String Length : ");
    // Serial.print(_value.length());
    // if (EEPROM.readString(ST_SSID_EEPROM) != _value) {
    //   stSSID = _value;
    //   EEPROM.writeString(ST_SSID_EEPROM, _value);
    //   EEPROM.commit();
    // }
  } else if (_division == "ST_PSWD") { // Station Mode
    Serial.print(", ST_PSWD String Length : ");
    // Serial.println(_value.length());
    // if (EEPROM.readString(ST_PSWD_EEPROM) != _value) {
    //   stPSWD = _value;
    //   EEPROM.writeString(ST_PSWD_EEPROM, _value);
    //   EEPROM.commit();
    // }
  } else if (_division == "neo1startNumber") {
    neo1Start = int8_t(value);
  } else if (_division == "neo1TotalNumber") {
    neo1Total = int8_t(value);
  } else if (_division == "neo2startNumber") {
    neo2Start = int8_t(value);
  } else if (_division == "neo2TotalNumber") {
    neo2Total = int8_t(value);
  } else if (_division == "NEO_TEST") {
    Serial.print("neo1Start : ");
    Serial.print(neo1Start);
    Serial.print(",  neo1Total : ");
    Serial.println(neo1Total);
    Serial.print("neo2Start : ");
    Serial.print(neo2Start);
    Serial.print(",  neo2Total : ");
    Serial.println(neo2Total);
    strip1.clear();
    strip2.clear();
    if (neo1TypeFlag)
      strip1.show();
    if (neo2TypeFlag)
      strip2.show();
    ledcWrite(ledPwmCh[2], PWM_ON);
    ledcWrite(ledPwmCh[3], PWM_ON);
    for (uint16_t i = neo1Start; i < neo1Total; i++) {
      strip1.setPixelColor(i, strip1.Color(127, 127, 127));
    }
    if (neo1TypeFlag)
      strip1.show();
    delay(5);
    for (uint16_t i = neo2Start; i < neo2Total; i++) {
      strip2.setPixelColor(i, strip2.Color(127, 127, 127));
    }
    if (neo2TypeFlag)
      strip2.show();
  } else if (_division == "NEO_1_FLAG") {
    if (value == 1)
      neo1TypeFlag = true;
    else
      neo1TypeFlag = false;
  } else if (_division == "NEO_2_FLAG") {
    if (value == 1)
      neo2TypeFlag = true;
    else
      neo2TypeFlag = false;
  } else if (_division == "NEO_1_START") {
    neo1Start = byte(value);
  } else if (_division == "NEO_2_START") {
    neo2Start = byte(value);
  } else if (_division == "NEO_1_TOTAL") {
    neo1Total = byte(value);
  } else if (_division == "NEO_2_TOTAL") {
    neo2Total = byte(value);
  }

  else if (_division == "BG_VALUE") {
    EEPROM.writeByte(BG_VALUE_EEPROM, value);
    EEPROM.commit();
  } else if (_division == "FACTORY_SAVE") {
    strip1.clear();
    strip2.clear();
    if (neo1TypeFlag != EEPROM.readBool(NEO_1_FLAG_EEPROM)) {
      EEPROM.writeBool(NEO_1_FLAG_EEPROM, neo1TypeFlag);
    }
    if (neo2TypeFlag != EEPROM.readBool(NEO_2_FLAG_EEPROM)) {
      EEPROM.writeBool(NEO_2_FLAG_EEPROM, neo2TypeFlag);
    }
    // if (neo1Start != EEPROM.readByte(NEO_1_START_EEPROM)) {
    //   EEPROM.writeByte(NEO_1_START_EEPROM, neo1Start);
    // }
    // if (neo2Start != EEPROM.readByte(NEO_2_START_EEPROM)) {
    //   EEPROM.writeByte(NEO_2_START_EEPROM, neo2Start);
    // }

    EEPROM.commit();
  } else if (_division == "NEO_1_MODE") {
    neoModeChangeFlag = true;
    Serial.print("NEO 1 Mode : ");
    Serial.println(value);
    neo1_mode = byte(value);
    if (neo1_mode == 0) {
      for (uint16_t i = neo1Start; i < neo1Total; i++) {
        strip1.setPixelColor(
            i, strip1.Color(neo1_r_color, neo1_g_color, neo1_b_color));
      }
      strip1.show();
      strip1.show();
    }
    if (neo2_mode != 0) {
      neo2_mode = neo1_mode;
      s = "NEO_2_MODE:";
      s += neo2_mode;
      s += ':';
      s += neo2_mode;
      comSend(s);
    }
    EEPROM.writeByte(NEO_1_MODE, neo1_mode);
    EEPROM.commit();
  } else if (_division == "BTN_SEL") {
    EEPROM.writeByte(BTN_SELECT_EEPROM, byte(value));
    EEPROM.commit();
  } else if (_division == "N1_RED") {
    neo1_r_color = byte(value);
  } else if (_division == "N1_GREEN") {
    neo1_g_color = byte(value);
  } else if (_division == "N1_BLUE") {
    neo1_b_color = byte(value);
  } else if (_division == "NEO_2_MODE") {
    neoModeChangeFlag = true;
    Serial.print("NEO 1 Mode : ");
    Serial.println(value);
    neo2_mode = byte(value);
    if (neo2_mode == 0) {
      for (uint16_t i = neo2Start; i < neo2Total; i++) {
        strip2.setPixelColor(
            i, strip2.Color(neo2_r_color, neo2_g_color, neo2_b_color));
      }
      strip2.show();
      strip2.show();
    }
    if (neo1_mode != 0) {
      neo1_mode = neo2_mode;
      s = "NEO_1_MODE:";
      s += neo1_mode;
      s += ':';
      s += neo1_mode;
      comSend(s);
    }
    EEPROM.writeByte(NEO_2_MODE, neo2_mode);
    EEPROM.commit();
  } else if (_division == "N2_RED") {
    neo2_r_color = byte(value);
  } else if (_division == "N2_GREEN") {
    neo2_g_color = byte(value);
  } else if (_division == "N2_BLUE") {
    neo2_b_color = byte(value);
  } else if (_division == "RCV_OK") {
    comCount = 0;
  } else if (_division == "NEO_R_SAVE") {
    if (_flag == "1")
      EEPROM.writeByte(NEO_1_RED_EEPROM, value);
    else if (_flag == "2")
      EEPROM.writeByte(NEO_2_RED_EEPROM, value);
    EEPROM.commit();
    colorValue = _flag.toInt();
    Serial.print("Division : ");
    Serial.print(_division);
    Serial.print(", Value : ");
    Serial.print(value);
    Serial.print(", _flag : ");
    Serial.println(colorValue);
  } else if (_division == "NEO_G_SAVE") {
    if (_flag == "1")
      EEPROM.writeByte(NEO_1_GREEN_EEPROM, value);
    else if (_flag == "2")
      EEPROM.writeByte(NEO_2_GREEN_EEPROM, value);
    EEPROM.commit();
    colorValue = _flag.toInt();
    Serial.print("Division : ");
    Serial.print(_division);
    Serial.print(", Value : ");
    Serial.print(value);
    Serial.print(", _flag : ");
    Serial.println(colorValue);
  } else if (_division == "NEO_B_SAVE") {
    if (_flag == "1")
      EEPROM.writeByte(NEO_1_BLUE_EEPROM, value);
    else if (_flag == "2")
      EEPROM.writeByte(NEO_2_BLUE_EEPROM, value);
    EEPROM.commit();
    colorValue = _flag.toInt();
    Serial.print("Division : ");
    Serial.print(_division);
    Serial.print(", Value : ");
    Serial.print(value);
    Serial.print(", _flag : ");
    Serial.println(colorValue);
  } else if (_division == "USER_SET") {
    if (neo1_mode != EEPROM.readByte(NEO_1_MODE))
      EEPROM.writeByte(NEO_1_MODE, neo1_mode);
    if (neo2_mode != EEPROM.readByte(NEO_2_MODE))
      EEPROM.writeByte(NEO_2_MODE, neo2_mode);
    if (neo1_r_color != EEPROM.readByte(NEO_1_RED_EEPROM))
      EEPROM.writeByte(NEO_1_RED_EEPROM, neo1_r_color);
    if (neo1_g_color != EEPROM.readByte(NEO_1_GREEN_EEPROM))
      EEPROM.writeByte(NEO_1_GREEN_EEPROM, neo1_g_color);
    if (neo1_b_color != EEPROM.readByte(NEO_1_BLUE_EEPROM))
      EEPROM.writeByte(NEO_1_BLUE_EEPROM, neo1_b_color);
    if (neo2_r_color != EEPROM.readByte(NEO_2_RED_EEPROM))
      EEPROM.writeByte(NEO_2_RED_EEPROM, neo2_r_color);
    if (neo2_g_color != EEPROM.readByte(NEO_2_GREEN_EEPROM))
      EEPROM.writeByte(NEO_2_GREEN_EEPROM, neo2_g_color);
    if (neo2_b_color != EEPROM.readByte(NEO_2_BLUE_EEPROM))
      EEPROM.writeByte(NEO_2_BLUE_EEPROM, neo2_b_color);
    EEPROM.commit();

    Serial.print("neo1_r_color : ");
    Serial.print(neo1_r_color);
    Serial.print(",  neo1_g_color : ");
    Serial.print(neo1_g_color);
    Serial.print(",  neo1_b_color : ");
    Serial.println(neo1_b_color);

    Serial.print("neo2_r_color : ");
    Serial.print(neo2_r_color);
    Serial.print(",  neo2_g_color : ");
    Serial.print(neo2_g_color);
    Serial.print(",  neo2_b_color : ");
    Serial.println(neo2_b_color);

    Serial.print("NEO 1 Mode : ");
    Serial.print(neo1_mode);
    Serial.print(",  NEO 2 Mode : ");
    Serial.println(neo2_mode);

    Serial.println();
  }

  // if (analogRead(SWITCH) > 1000) {
  //   Serial.print("Switch Flag : True");
  //   switchFlag = true;
  //   // digitalWrite(POWER, HIGH);
  // } else {
  //   Serial.print(",  Switch Flag : False");
  //   switchFlag = false;
  // }

  // if (analogRead(DOOR) > 1000) {
  //   Serial.println(",  DOOR Flag : True");
  //   doorFlag = false;
  //   if (ledFlag[1] == false)
  //     ledcWrite(ledPwmCh[1], 0);
  // } else {
  //   Serial.println(",  DOOR Flag : False");
  //   doorFlag = true;
  //   if (ledFlag[1] == false)
  //     ledcWrite(ledPwmCh[1], PWM_ON);
  // }

  // if (digitalRead(LiPo)) {
  //   batLiPoFlag = true;
  //   Serial.println("Bat LiPo Flag : True");
  // } else {
  //   batLiPoFlag = false;
  //   Serial.println("Bat LiPo Flag : False");
  // }

  // if (!switchFlag) {
  //   switchOffCount++;
  //   if (switchOffCount > 3) {
  //     switchOffCount = 0;
  //   }
  // }

  // Serial.print("Command: ");
  // Serial.print(_division);
  // Serial.print(", Value: ");
  // Serial.print(_value);
  // Serial.print(", PWM: ");
  // Serial.println(pwm);

  comCount = 0;
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
  int chanel = root["Chanel"];
  int value = root["Value"];

  if (division == "LED") {
    wifiFlag = true;

    if (chanel == 2 && neo1TypeFlag) {
      Serial.print("NEO 2 Value : ");
      Serial.println(value);
      if (value != 0) {
        ledFlag[chanel] = true;
        ledcWrite(ledPwmCh[2], PWM_ON);
        strip1.setBrightness(map(value, 1, 100, 1, 255));
        pwmPushValue[chanel] = value;
      } else if (value == 0) {
        ledFlag[chanel] = false;
        ledcWrite(ledPwmCh[2], 0);
        strip1.clear();
        if (neo1TypeFlag)
          strip1.show();
      }
    } else if (chanel == 3 && neo2TypeFlag) {
      Serial.print("NEO 3 Value : ");
      Serial.println(value);
      if (value != 0) {
        ledFlag[chanel] = true;
        ledcWrite(ledPwmCh[3], PWM_ON);
        strip2.setBrightness(map(value, 1, 100, 1, 255));
        pwmPushValue[chanel] = value;
      } else if (value == 0) {
        ledcWrite(ledPwmCh[3], 0);
        ledFlag[chanel] = false;
        strip2.clear();
        if (neo2TypeFlag)
          strip2.show();
      }
    } else {
      if (value == 0) {
        ledFlag[chanel] = false;
        ledcWrite(ledPwmCh[chanel], 0);
      } else {
        ledFlag[chanel] = true;
        ledcWrite(ledPwmCh[chanel], map(value, 1, 100, 1, PWM_ON));
        pwmPushValue[chanel] = value;
      }
    }

    Serial.print("pwmPushValue[");
    Serial.print(chanel);
    Serial.print("] = ");
    Serial.println(pwmPushValue[chanel]);

    s = "LED";
    s += chanel;
    s += ":";
    s += value;
    s += ":";
    s += value == 0 ? 0 : 1;
    comSend(s);
    delay(5);
    comSend(s);
    delay(5);
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
    if (chanel == 1) {
      EEPROM.writeByte(LED_1_PWM_EEPROM, value);
      EEPROM.writeBool(BTN_1_FLAG_EEPROM, ledFlag[1]);
      EEPROM.writeByte(BTN_SELECT_EEPROM, 1);
    } else if (chanel == 2) {
      EEPROM.writeByte(LED_2_PWM_EEPROM, value);
      EEPROM.writeBool(BTN_2_FLAG_EEPROM, ledFlag[2]);
      EEPROM.writeByte(BTN_SELECT_EEPROM, 2);
    } else if (chanel == 3) {
      EEPROM.writeByte(LED_3_PWM_EEPROM, value);
      EEPROM.writeBool(BTN_3_FLAG_EEPROM, ledFlag[3]);
      EEPROM.writeByte(BTN_SELECT_EEPROM, 3);
    } else if (chanel == 4) {
      EEPROM.writeByte(LED_4_PWM_EEPROM, value);
      EEPROM.writeBool(BTN_4_FLAG_EEPROM, ledFlag[4]);
      EEPROM.writeByte(BTN_SELECT_EEPROM, 4);
    } else if (chanel == 5) {
      EEPROM.writeByte(LED_5_PWM_EEPROM, value);
      EEPROM.writeBool(BTN_5_FLAG_EEPROM, ledFlag[5]);
      EEPROM.writeByte(BTN_SELECT_EEPROM, 5);
    }
    EEPROM.commit();
  } else if (division == "TV") {
    wifiFlag = true;
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
    EEPROM.writeBool(BTN_6_FLAG_EEPROM, tvFlag);
    EEPROM.writeByte(BTN_SELECT_EEPROM, 6);
    EEPROM.commit();
  } else if (division == "QR_OK") {
    comSend("QR_OK:1");
    delay(5);
    comSend("QR_OK:1");
    delay(5);
    comSend("QR_OK:1");
  }

  server.send(200, "text/plain", s);

} // handleJson()

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  server.send(200, "text/plain", "");
}

void handleRequest() {

  s = "";
  s = s + "{" + "\"COM_FLAG\":" + rs485Flag + ", " + "\"SWITCH\":" +
      switchFlag + ", " + "\"DOOR\":" + doorFlag + ", " + "\"BAT\":" +
      batLiPoFlag + ", " + "\"ledValue1\":" + pwmPushValue[1] + ", " +
      "\"ledValue2\":" + pwmPushValue[2] + ", " + "\"ledValue3\":" +
      pwmPushValue[3] + ", " + "\"ledValue4\":" + pwmPushValue[4] + ", " +
      "\"ledValue5\":" + pwmPushValue[5] + ", " + "\"ledFlag1\":" + ledFlag[1] +
      ", " + "\"ledFlag2\":" + ledFlag[2] + ", " + "\"ledFlag3\":" +
      ledFlag[3] + ", " + "\"ledFlag4\":" + ledFlag[4] + ", " +
      "\"ledFlag5\":" + ledFlag[5] + ", " + "\"tv\":" + tvFlag + "}";

  server.send(200, "text/plain", s);
  // comCount = 0;

} // handleRequest()

void wifiStart() { // WiFi Mode Start...

  String msg;

  wifiStartFlag = true; // WiFi 통신 시작...
  // rs485Flag = false;

  if (networkFlag == AP_MODE) {

    if (!apModeStartFlag) {
      WiFi.mode(WIFI_MODE_AP);

      Serial.println(F("setupWifi()..."));
      Serial.println(F("Access Point(WiFi) Mode Start..."));

      IPAddress local_ip(192, 168, 4, 1);
      IPAddress gateway(192, 168, 4, 1);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.softAP(ssidAp, passwordAp); // Access Point mode begin...
      WiFi.softAPConfig(local_ip, gateway, subnet);

      Serial.println();
      Serial.print("SSID: '");
      Serial.print(ssidAp);
      Serial.print("', PSWD: '");
      Serial.print(passwordAp);
      Serial.println("'");
      Serial.print(F("ESP32 IP as soft AP : "));
      Serial.println(WiFi.softAPIP());
      apModeStartFlag = true;
    }
  } else {
    Serial.println(F("setupWifi()..."));
    Serial.println(F("Hot Spot Mode Start..."));
    Serial.println();
    Serial.print("Hot Spot SSID: ");
    Serial.print(ssidAp);
    Serial.print(", Hot Spot PSWD: ");
    Serial.println(passwordAp);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidAp, passwordAp);
    Serial.print("Connecting to WiFi ..");

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
  server.on("/json", handleJson);
  server.on("/root", handleRoot);
  server.on("/Request", handleRequest);

  server.begin();
}

void comProcess() {
  server.handleClient();

  if (Serial2.available() > 0)
    serialEvent2();

  if (digitalRead(DOOR) != doorFlag) {
    // delay(50);
    if (digitalRead(DOOR)) {
      // delay(500);
      doorFlag = true;
      digitalWrite(DOOR_LED, LOW);
      if (ledFlag[1] == false) {
        for (int i = 0; i < 1000; i = i + 50) {
          ledcWrite(ledPwmCh[1], i);
          delay(50);
        }
        ledcWrite(ledPwmCh[1], PWM_ON);
        // delay(2000);
      }
    } else {
      // delay(300);
      doorFlag = false;
      digitalWrite(DOOR_LED, HIGH);
      if (ledFlag[1] == false) {
        for (int i = PWM_ON; i > 0; i = i - 50) {
          ledcWrite(ledPwmCh[1], i);
          delay(30);
        }
        ledcWrite(ledPwmCh[1], 0);
        // delay(2000);
      }
    }
    delay(2000);

    Serial.print("Door flag : ");
    Serial.println(doorFlag);
  }

  // if (loopCount > 150) { // 약 1초 마다 실행...
  if (loopCount > 50) { // 약 1초 마다 실행...
                        // if (loopCount > 100) { // 약 1초 마다 실행...
    switchVolt = analogRead(SWITCH);
    if (switchVolt > 1000) {
      // Serial.print("Switch Flag : True");
      switchFlag = true;
      // digitalWrite(SWITCH_LED, LOW);
    } else {
      // Serial.print("Switch Flag : False");
      switchFlag = false;
      // digitalWrite(SWITCH_LED, HIGH);
    }

    // Serial.print("TV Flag : ");
    // Serial.print(tvFlag);
    // Serial.print(", ledFlag[5] : ");
    // Serial.println(ledFlag[5]);

    if (tvFlag || ledFlag[5])
      digitalWrite(FAN, HIGH);
    else
      digitalWrite(FAN, LOW);

    // if (digitalRead(DOOR) != doorFlag) {
    //   // delay(50);
    //   if (digitalRead(DOOR)) {
    //     // delay(500);
    //     doorFlag = true;
    //     digitalWrite(DOOR_LED, LOW);
    //     if (ledFlag[1] == false) {
    //       ledcWrite(ledPwmCh[1], PWM_ON);
    //     }
    //   } else {
    //     // delay(300);
    //     doorFlag = false;
    //     digitalWrite(DOOR_LED, HIGH);
    //     if (ledFlag[1] == false) {
    //       ledcWrite(ledPwmCh[1], 0);
    //     }
    //   }
    //   delay(300);

    //   Serial.print("Door flag : ");
    //   Serial.println(doorFlag);

    //   // s = "DOOR:";
    //   // s += doorFlag;
    //   // s += ':';
    //   // s += doorFlag;
    //   // comSend(s);
    // }

    if (digitalRead(LiPo)) {
      batLiPoFlag = true;
      // Serial.println("Bat LiPo Flag : True");
    } else {
      batLiPoFlag = false;
      // Serial.println("Bat LiPo Flag : False");
    }

    if (!switchFlag) {
      switchOffCount++;
      if (switchOffCount > 3) {
        switchOffCount = 0;
      }
    }
    // uint16_t tv_read = analogRead(13);
    // tv_volt = (tv_read * 0.00744109136) + 1.0;
    // Serial.println(tv_volt);
    // s = "TV_VOLT:";
    // s += String(tv_volt + (float(tvAdj) / 100), 1);
    // s += ':';
    // s += String(tv_volt + (float(tvAdj) / 100), 1);
    // s += " V";
    // comSend(s); // 1초 마다 Touch 측으로 회신...
    // s = "SWITCH:";
    // s += switchFlag;
    // s += ':';
    // s += switchFlag;
    // comSend(s); // 1초 마다 Touch 측으로 회신...
    // s = "DOOR:";
    // s += doorFlag;
    // s += ':';
    // s += doorFlag;
    // comSend(s); // 1초 마다 Touch 측으로 회신...
    // s = "BAT:";
    // s += batLiPoFlag;
    // s += ':';
    // s += batLiPoFlag;
    // comSend(s); // 1초 마다 Touch 측으로 회신...
    // s = "BAT:";
    // s += batLiPoFlag;
    // s += ':';
    // s += batLiPoFlag;
    // comSend(s); // 1초 마다 Touch 측으로 회신...
    // delay(5);
    // if (switchFlag != switchFlagPush || doorFlag != doorFlagPush ||
    //     batLiPoFlag != batLiPoFlagPush) {

    // if (switchFlag != switchFlagPush) {
    //   switchFlagPush = switchFlag;
    if (switchFlag) {
      // Serial.println("Switch Flag : True");
      digitalWrite(SWITCH_LED, LOW);
    } else {
      // Serial.println("Switch Flag : False");
      digitalWrite(SWITCH_LED, HIGH);
    }
    s = "SWITCH:";
    s += switchFlag;
    s += ':';
    s += switchFlag;
    // comSend(s);
    // } else if (doorFlag != doorFlagPush) {
    //   doorFlagPush = doorFlag;
    // if (doorFlag) {
    //   Serial.println(",  DOOR Flag : False");
    //   digitalWrite(DOOR_LED, LOW);
    //   if (ledFlag[1] == false) {
    //     ledcWrite(ledPwmCh[1], PWM_ON);
    //   }
    // } else {
    //   Serial.println(",  DOOR Flag : True");
    //   digitalWrite(DOOR_LED, HIGH);
    //   if (ledFlag[1] == false) {
    //     ledcWrite(ledPwmCh[1], 0);
    //   }
    // }
    // s = "DOOR:";
    // s += doorFlag;
    // s += ':';
    // s += doorFlag;
    // comSend(s);
    // } else if (batLiPoFlag != batLiPoFlagPush) {
    //   batLiPoFlagPush = batLiPoFlag;

    // if (batLiPoFlag) {
    //   Serial.println("Bat LiPo Flag : True");
    // } else {
    //   Serial.println("Bat LiPo Flag : False");
    // }

    s = "BAT:";
    s += batLiPoFlag;
    s += ':';
    s += batLiPoFlag;
    // comSend(s);
    // }
    // } else
    // comSend("COM:1");
    loopCount = 0;
  }

  loopCount++;
  comCount++;
  // if (comCount > 60)
  if (comCount > 100)
    rs485Flag = false;
  else
    rs485Flag = true;
  if (comCount > 250)
    comCount = 65;
  // Serial.print("comCount : ");
  // Serial.println(comCount);
}

void doorProcess() {
  // delay(50);
  if (digitalRead(DOOR)) {
    // delay(500);
    doorFlag = true;
    digitalWrite(DOOR_LED, LOW);
    if (ledFlag[1] == false) {
      // for (int i = 0; i < 1000; i = i + 50) {
      //   ledcWrite(ledPwmCh[1], i);
      //   delay(50);
      // }
      ledcWrite(ledPwmCh[1], PWM_ON);
      delay(2000);
    }
  } else {
    // delay(300);
    doorFlag = false;
    digitalWrite(DOOR_LED, HIGH);
    if (ledFlag[1] == false) {
      // for (int i = PWM_ON; i > 0; i = i - 50) {
      //   ledcWrite(ledPwmCh[1], i);
      //   delay(30);
      // }
      ledcWrite(ledPwmCh[1], 0);
      delay(2000);
    }
  }

  Serial.print("Door flag : ");
  Serial.println(doorFlag);
}

uint32_t Wheel1(byte WheelPos, bool neoCh) {
  WheelPos = 255 - WheelPos;

  if (neoCh == 1) {
    if (WheelPos < 85) {
      return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
      WheelPos -= 85;
      return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (neoCh == 2) {
    if (WheelPos < 85) {
      return strip2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
      WheelPos -= 85;
      return strip2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

// 1. 무지개 모드
void rainbow(uint8_t wait) {
  Serial.println("Rainbow...");
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    if (neo1_mode != 0) {
      for (i = neo1Start; i < neo1Total; i++) {
        strip1.setPixelColor(i, Wheel1((i + j) & 255, 1));
      }
    }
    if (neo2_mode != 0) {
      for (i = neo2Start; i < neo2Total; i++) {
        strip2.setPixelColor(i, Wheel1((i + j) & 255, 2));
      }
    }

    // if (neo1_mode != 1)
    //   return;

    if (neo1TypeFlag)
      if (neo1_mode != 0) {
        strip1.show();
        strip1.show();
      }

    if (neo2TypeFlag)
      if (neo2_mode != 0) {
        strip2.show();
        strip2.show();
      }

    delay(wait);
    comProcess();
    if (digitalRead(DOOR) != doorFlag)
      doorProcess();
    if (neoModeChangeFlag)
      return;
  }
}

// 2. 무지개 사이클 모드
void rainbowCycle(uint8_t wait) {
  Serial.println("RainbowCycle...");
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    if (neo1_mode) {
      for (i = neo1Start; i < neo1Total; i++) {
        strip1.setPixelColor(i, Wheel1(((i * 256 / neo1Total) + j) & 255, 1));
      }
    }
    if (neo2_mode != 0) {
      for (i = neo2Start; i < neo2Total; i++) {
        strip2.setPixelColor(i, Wheel1(((i * 256 / neo1Total) + j) & 255, 2));
      }
    }

    // if (neo1_mode != 2)
    //   return;

    if (neo1TypeFlag)
      if (neo1_mode != 0)
        strip1.show();
    if (neo2TypeFlag)
      if (neo2_mode != 0)
        strip2.show();

    delay(wait);
    comProcess();
    if (digitalRead(DOOR) != doorFlag)
      doorProcess();
    if (neoModeChangeFlag)
      return;
  }
}

// 3. 숨쉬기 모드
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {

  int fadeVal = 0, fadeMax = 100;

  for (uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops * 65536;
       firstPixelHue += 256) {
    if (neo1_mode != 0) {
      for (int i = neo1Start; i < neo1Total;
           i++) { // For each pixel in strip...
        uint32_t pixelHue = firstPixelHue + (i * 65536L / neo1Total);
        strip1.setPixelColor(i, strip1.gamma32(strip1.ColorHSV(
                                    pixelHue, 255, 255 * fadeVal / fadeMax)));
        if (Serial2.available() > 0)
          serialEvent2();
      }
    }
    if (neo2_mode != 0) {
      for (int i = neo2Start; i < neo2Total;
           i++) { // For each pixel in strip...
        uint32_t pixelHue = firstPixelHue + (i * 65536L / neo1Total);
        strip2.setPixelColor(i, strip2.gamma32(strip2.ColorHSV(
                                    pixelHue, 255, 255 * fadeVal / fadeMax)));
        if (Serial2.available() > 0)
          serialEvent2();
      }
    }

    // if (neo1_mode != 3)
    //   return;

    if (neo1TypeFlag)
      if (neo1_mode != 0)
        strip1.show();
    if (neo2TypeFlag)
      if (neo2_mode != 0)
        strip2.show();

    delay(wait);
    comProcess();
    // server.handleClient();
    if (firstPixelHue < 65536) { // First loop,
      if (fadeVal < fadeMax)
        fadeVal++;                                              // fade in
    } else if (firstPixelHue >= ((rainbowLoops - 1) * 65536)) { // Last loop,
      if (fadeVal > 0)
        fadeVal--; // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
    if (digitalRead(DOOR) != doorFlag)
      doorProcess();
    if (neoModeChangeFlag)
      return;
  }

  for (int k = 0; k < whiteLoops; k++) {
    for (int j = 0; j < 256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      if (neo1_mode != 0)
        strip1.fill(strip1.Color(0, 0, 0, strip1.gamma8(j)));
      if (neo2_mode != 0)
        strip2.fill(strip2.Color(0, 0, 0, strip2.gamma8(j)));

      // if (neo1_mode != 3)
      //   return;

      if (neo1TypeFlag)
        if (neo1_mode != 0)
          strip1.show();
      if (neo2TypeFlag)
        if (neo2_mode != 0)
          strip2.show();

      if (Serial2.available() > 0)
        serialEvent2();
    }
    delay(50);                       // Pause 1 second
    for (int j = 255; j >= 0; j--) { // Ramp down 255 to 0
      if (neo1_mode != 0)
        strip1.fill(strip1.Color(0, 0, 0, strip1.gamma8(j)));
      if (neo2_mode != 0)
        strip2.fill(strip2.Color(0, 0, 0, strip2.gamma8(j)));

      // if (neo1_mode != 3)
      //   return;

      if (neo1TypeFlag)
        if (neo1_mode != 0)
          strip1.show();
      if (neo2TypeFlag)
        if (neo2_mode != 0)
          strip2.show();

      if (Serial2.available() > 0)
        serialEvent2();
    }
    if (digitalRead(DOOR) != doorFlag)
      doorProcess();
    if (neoModeChangeFlag)
      return;
  }
}

// 4. 컬러 와이프 RED 모드
void colorWipeRed(uint32_t c, uint8_t wait) {
  Serial.println("ColorWipe...");

  for (uint16_t i = neo1Start; i < neo1Total; i++) {
    strip1.setPixelColor(i, c);
    strip2.setPixelColor(i, c);
    if (neo1_mode != 4)
      return;
    if (neo1TypeFlag)
      strip1.show();
    if (neo2TypeFlag)
      strip2.show();
    delay(wait);
    comProcess();
    // server.handleClient();
    // if (Serial2.available() > 0)
    //   serialEvent2();
  }
}

// 5. 컬러 와이프 GREEN 모드
void colorWipeGreen(uint32_t c, uint8_t wait) {
  Serial.println("ColorWipe...");

  for (uint16_t i = neo1Start; i < neo1Total; i++) {
    strip1.setPixelColor(i, c);
    strip2.setPixelColor(i, c);
    if (neo1_mode != 5)
      return;
    if (neo1TypeFlag)
      strip1.show();
    if (neo2TypeFlag)
      strip2.show();
    delay(wait);
    comProcess();
    // server.handleClient();
    // if (Serial2.available() > 0)
    //   serialEvent2();
  }
}

// 6. 컬러 와이프 BLUE 모드
void colorWipeBlue(uint32_t c, uint8_t wait) {
  Serial.println("ColorWipe...");

  for (uint16_t i = neo1Start; i < neo1Total; i++) {
    strip1.setPixelColor(i, c);
    strip2.setPixelColor(i, c);
    if (neo1TypeFlag)
      strip1.show();
    if (neo2TypeFlag)
      strip2.show();
    if (neo1_mode != 6)
      return;
    delay(wait);
    comProcess();
    // server.handleClient();
    // if (Serial2.available() > 0)
    //   serialEvent2();
  }
}

// 7. 극장 체이스 레인보우 모드
void theaterChaseRainbow(uint8_t wait) {
  Serial.println("Theater Chase Rainbow");

  for (int j = 0; j < 256; j++) { // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      if (neo1_mode != 0) {
        for (uint16_t i = neo1Start; i < neo1Total; i = i + 3) {
          strip1.setPixelColor(
              i + q, Wheel1((i + j) % 255, 1)); // turn every third pixel on
        }
      }
      if (neo2_mode != 0) {
        for (uint16_t i = neo2Start; i < neo2Total; i = i + 3) {
          strip2.setPixelColor(
              i + q, Wheel1((i + j) % 255, 2)); // turn every third pixel on
        }
      }

      if (digitalRead(DOOR) != doorFlag)
        doorProcess();
      if (neoModeChangeFlag)
        return;

      // if (neo1_mode != 7)
      //   return;

      if (neo1TypeFlag)
        if (neo1_mode != 0)
          strip1.show();
      if (neo2TypeFlag)
        if (neo2_mode != 0)
          strip2.show();

      delay(wait);
      comProcess();
      // server.handleClient();
      // if (Serial2.available() > 0)
      //   serialEvent2();

      if (neo1_mode != 0) {
        for (uint16_t i = neo1Start; i < neo1Total; i = i + 3) {
          strip1.setPixelColor(i + q, 0); // turn every third pixel off
        }
      }
      if (neo2_mode != 0) {
        for (uint16_t i = neo2Start; i < neo2Total; i = i + 3) {
          strip2.setPixelColor(i + q, 0); // turn every third pixel off
        }
      }
    }
  }
}

// 8. 극장 체이스 컬러모드
void theaterChase8(uint32_t c, uint8_t wait) {
  Serial.println("Theater Chase...");
  for (int j = 0; j < 10; j++) { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = neo1Start; i < neo1Total; i = i + 3) {
        strip1.setPixelColor(i + q, c); // turn every third pixel on
        strip2.setPixelColor(i + q, c); // turn every third pixel on
      }
      if (neo1_mode != 8)
        return;
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
      delay(wait);
      comProcess();
      // server.handleClient();
      // if (Serial2.available() > 0)
      //   serialEvent2();
      for (uint16_t i = neo2Start; i < neo2Total; i = i + 3) {
        strip1.setPixelColor(i + q, 0); // turn every third pixel off
        strip2.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 9. 극장 체이스 컬러모드
void theaterChase9(uint32_t c, uint8_t wait) {
  Serial.println("Theater Chase...");
  for (int j = 0; j < 10; j++) { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = neo1Start; i < neo1Total; i = i + 3) {
        strip1.setPixelColor(i + q, c); // turn every third pixel on
        strip2.setPixelColor(i + q, c); // turn every third pixel on
      }
      if (neo1_mode != 9)
        return;
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
      delay(wait);
      comProcess();
      // server.handleClient();
      // if (Serial2.available() > 0)
      //   serialEvent2();
      for (uint16_t i = neo2Start; i < neo2Total; i = i + 3) {
        strip1.setPixelColor(i + q, 0); // turn every third pixel off
        strip2.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 10. 극장 체이스 컬러모드
void theaterChase10(uint32_t c, uint8_t wait) {
  Serial.println("Theater Chase...");
  for (int j = 0; j < 10; j++) { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = neo1Start; i < neo1Total; i = i + 3) {
        strip1.setPixelColor(i + q, c); // turn every third pixel on
        strip2.setPixelColor(i + q, c); // turn every third pixel on
      }
      if (neo1_mode != 10)
        return;
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
      delay(wait);
      comProcess();
      // server.handleClient();
      // if (Serial2.available() > 0)
      //   serialEvent2();
      for (uint16_t i = neo2Start; i < neo2Total; i = i + 3) {
        strip1.setPixelColor(i + q, 0); // turn every third pixel off
        strip2.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 11. 극장 체이스 BLUE 모드
void theaterChase11(uint32_t c, uint8_t wait) {
  Serial.println("Theater Chase...");
  for (int j = 0; j < 10; j++) { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = neo1Start; i < neo1Total; i = i + 3) {
        strip1.setPixelColor(i + q, c); // turn every third pixel on
        strip2.setPixelColor(i + q, c); // turn every third pixel on
      }
      if (neo1_mode != 11)
        return;
      if (neo1TypeFlag)
        strip1.show();
      if (neo2TypeFlag)
        strip2.show();
      delay(wait);
      comProcess();
      // server.handleClient();
      // if (Serial2.available() > 0)
      //   serialEvent2();
      for (uint16_t i = neo2Start; i < neo2Total; i = i + 3) {
        strip1.setPixelColor(i + q, 0); // turn every third pixel off
        strip2.setPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// 12. 랜덤 페이드 모드
void randomFade() {

  if (random(20) == 1) {
    uint16_t i = random(neo1Total);
    if (redStates[i] < 1 && greenStates[i] < 1 && blueStates[i] < 1) {
      redStates[i] = random(256);
      greenStates[i] = random(256);
      blueStates[i] = random(256);
    }
  }

  for (uint16_t l = 0; l < neo1Total; l++) {
    if (redStates[l] > 1 || greenStates[l] > 1 || blueStates[l] > 1) {
      strip1.setPixelColor(l, redStates[l], greenStates[l], blueStates[l]);
      strip2.setPixelColor(l, redStates[l], greenStates[l], blueStates[l]);

      if (redStates[l] > 1) {
        redStates[l] = redStates[l] * fadeRate;
      } else {
        redStates[l] = 0;
      }

      if (greenStates[l] > 1) {
        greenStates[l] = greenStates[l] * fadeRate;
      } else {
        greenStates[l] = 0;
      }

      if (blueStates[l] > 1) {
        blueStates[l] = blueStates[l] * fadeRate;
      } else {
        blueStates[l] = 0;
      }

    } else {
      strip1.setPixelColor(l, 0, 0, 0);
      strip2.setPixelColor(l, 0, 0, 0);
    }
    if (neo1_mode != 12)
      return;
  }
  if (neo1TypeFlag)
    strip1.show();
  if (neo2TypeFlag)
    strip2.show();
  delay(10);
}

void setup() {

  pinMode(ENABLE, OUTPUT);
  pinMode(LiPo, INPUT_PULLDOWN);
  pinMode(DOOR, INPUT_PULLUP);
  pinMode(NEO_1_RGB, INPUT_PULLUP);
  pinMode(NEO_2_RGB, INPUT_PULLUP);

  pinMode(39, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(TV, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(SWITCH_LED, OUTPUT);
  pinMode(DOOR_LED, OUTPUT);

  digitalWrite(ENABLE, LOW);

  if (digitalRead(LiPo))
    batLiPoFlag = true;
  else
    batLiPoFlag = false;

  ledcSetup(ledPwmCh[1], pwmFreq, resolution);
  ledcSetup(ledPwmCh[2], pwmFreq, resolution);
  ledcSetup(ledPwmCh[3], pwmFreq, resolution);
  ledcSetup(ledPwmCh[4], pwmFreq, resolution);
  ledcSetup(ledPwmCh[5], pwmFreq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledCh[1], ledPwmCh[1]);
  ledcAttachPin(ledCh[2], ledPwmCh[2]);
  ledcAttachPin(ledCh[3], ledPwmCh[3]);
  ledcAttachPin(ledCh[4], ledPwmCh[4]);
  ledcAttachPin(ledCh[5], ledPwmCh[5]);

  EEPROM.begin(250);

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 4, 6);

  strip1.begin();
  strip2.begin();

  digitalWrite(DOOR_LED, HIGH);

  neo1rgbFlag = digitalRead(NEO_1_RGB);
  neo2rgbFlag = digitalRead(NEO_2_RGB);

  // EEPROM Read
  apSSID = EEPROM.readString(AP_SSID_EEPROM);
  apPSWD = EEPROM.readString(AP_PSWD_EEPROM);

  tvAdj = EEPROM.readInt(TV_ADJ_EEPROM);
  // Serial.print("TV_ADJ_EEPROM : ");
  // Serial.println(tvAdj);

  Serial.println("LM Seat Light Controller V2.0........");

  neo1TypeFlag = EEPROM.readBool(NEO_1_FLAG_EEPROM);
  neo2TypeFlag = EEPROM.readBool(NEO_2_FLAG_EEPROM);

  if (EEPROM.readByte(NEO_1_START_EEPROM) > 100) {
    neo1Start = 0;
    EEPROM.writeByte(NEO_1_START_EEPROM, neo1Start);
  } else
    neo1Start = EEPROM.readByte(NEO_1_START_EEPROM);

  if (EEPROM.readByte(NEO_2_START_EEPROM) > 100) {
    neo2Start = 0;
    EEPROM.writeByte(NEO_2_START_EEPROM, neo2Start);
  } else
    neo2Start = EEPROM.readByte(NEO_2_START_EEPROM);

  neo1_r_color = EEPROM.readByte(NEO_1_RED_EEPROM);
  neo1_g_color = EEPROM.readByte(NEO_1_GREEN_EEPROM);
  neo1_b_color = EEPROM.readByte(NEO_1_BLUE_EEPROM);

  neo2_r_color = EEPROM.readByte(NEO_2_RED_EEPROM);
  neo2_g_color = EEPROM.readByte(NEO_2_GREEN_EEPROM);
  neo2_b_color = EEPROM.readByte(NEO_2_BLUE_EEPROM);

  neo1_mode = EEPROM.readByte(NEO_1_MODE);
  neo2_mode = EEPROM.readByte(NEO_2_MODE);

  if (EEPROM.readByte(BG_VALUE_EEPROM) > 100) {
    EEPROM.writeByte(BG_VALUE_EEPROM, 100);
    EEPROM.commit();
  } else
    bright = EEPROM.readByte(BG_VALUE_EEPROM);

  if (EEPROM.readByte(LED_1_PWM_EEPROM) > 100) {
    EEPROM.writeByte(LED_1_PWM_EEPROM, 100);
    EEPROM.commit();
  } else
    pwmPushValue[1] = EEPROM.readByte(LED_1_PWM_EEPROM);

  if (EEPROM.readByte(LED_2_PWM_EEPROM) > 100) {
    EEPROM.writeByte(LED_2_PWM_EEPROM, 100);
    EEPROM.commit();
  } else
    pwmPushValue[2] = EEPROM.readByte(LED_2_PWM_EEPROM);

  if (EEPROM.readByte(LED_3_PWM_EEPROM) > 100) {
    EEPROM.writeByte(LED_3_PWM_EEPROM, 100);
    EEPROM.commit();
  } else
    pwmPushValue[3] = EEPROM.readByte(LED_3_PWM_EEPROM);

  if (EEPROM.readByte(LED_4_PWM_EEPROM) > 100) {
    EEPROM.writeByte(LED_4_PWM_EEPROM, 100);
    EEPROM.commit();
  } else
    pwmPushValue[4] = EEPROM.readByte(LED_4_PWM_EEPROM);

  if (EEPROM.readByte(LED_5_PWM_EEPROM) > 100) {
    EEPROM.writeByte(LED_5_PWM_EEPROM, 100);
    EEPROM.commit();
  } else
    pwmPushValue[5] = EEPROM.readByte(LED_5_PWM_EEPROM);

  ledFlag[1] = EEPROM.readBool(BTN_1_FLAG_EEPROM);
  ledFlag[2] = EEPROM.readBool(BTN_2_FLAG_EEPROM);
  ledFlag[3] = EEPROM.readBool(BTN_3_FLAG_EEPROM);
  ledFlag[4] = EEPROM.readBool(BTN_4_FLAG_EEPROM);
  ledFlag[5] = EEPROM.readBool(BTN_5_FLAG_EEPROM);
  // floorFlag = EEPROM.readBool(BTN_5_FLAG_EEPROM);
  tvFlag = EEPROM.readBool(BTN_6_FLAG_EEPROM);
  btnSelect = EEPROM.readByte(BTN_SELECT_EEPROM);

  Serial.println("");
  Serial.print("LED 1 : ");
  Serial.print(ledFlag[1]);
  Serial.print(", LED 2 : ");
  Serial.print(ledFlag[2]);
  Serial.print(", LED 3 : ");
  Serial.print(ledFlag[3]);
  Serial.print(", LED 4 : ");
  Serial.print(ledFlag[4]);
  Serial.print(", LED 5 : ");
  Serial.print(ledFlag[5]);
  Serial.print(", TV : ");
  Serial.println(tvFlag);

  if (tvFlag)
    digitalWrite(TV, HIGH);

  Serial.print("LED 1 PWM : ");
  Serial.print(pwmPushValue[1]);

  Serial.print(", LED 2 PWM : ");
  Serial.print(pwmPushValue[2]);

  Serial.print(", LED 3 PWM : ");
  Serial.print(pwmPushValue[3]);

  Serial.print(", LED 4 PWM : ");
  Serial.print(pwmPushValue[4]);

  Serial.print(", LED 5 PWM : ");
  Serial.println(pwmPushValue[5]);

  Serial.print("Button Select : ");
  Serial.println(btnSelect);

  if (networkFlag == STATION_MODE) {
    Serial.println("Network Select Station Mode...");

    stSSID.toCharArray(ssidAp, stSSID.length());
    stPSWD.toCharArray(passwordAp, stPSWD.length());

  } else {
    Serial.println("Network Select AP Mode...");

    apSSID.toCharArray(ssidAp, apSSID.length());
    apPSWD.toCharArray(passwordAp, apPSWD.length());
  }

  Serial.println("");
  Serial.print("neo1TypeFlag : ");
  Serial.print(neo1TypeFlag);
  Serial.print(",  neo2TypeFlag : ");
  Serial.println(neo2TypeFlag);

  Serial.print("neo1Start : ");
  Serial.print(neo1Start);
  Serial.print(",  neo2Start : ");
  Serial.println(neo2Start);

  Serial.print("neo1Total : ");
  Serial.print(neo1Total);
  Serial.print(",  neo2Total : ");
  Serial.println(neo2Total);

  Serial.print("neo1_r_color : ");
  Serial.print(neo1_r_color);
  Serial.print(",  neo1_g_color : ");
  Serial.print(neo1_g_color);
  Serial.print(",  neo1_b_color : ");
  Serial.println(neo1_b_color);

  Serial.print("neo2_r_color : ");
  Serial.print(neo2_r_color);
  Serial.print(",  neo2_g_color : ");
  Serial.print(neo2_g_color);
  Serial.print(",  neo2_b_color : ");
  Serial.println(neo2_b_color);

  Serial.print("NEO 1 Mode : ");
  Serial.print(neo1_mode);
  Serial.print(",  NEO 2 Mode : ");
  Serial.println(neo2_mode);

  Serial.print("AP SSID : '");
  Serial.print(ssidAp);
  Serial.print("', AP PSWD : '");
  Serial.print(passwordAp);
  Serial.println("'");

  Serial.println("Setup done.....");
  Serial.println();
  delay(50);

  strip1.clear();
  strip2.clear();
  strip1.show();
  strip2.show();

  for (uint16_t l = 0; l < 12; l++) {
    redStates[l] = 0;
    greenStates[l] = 0;
    blueStates[l] = 0;
  }

  wifiStart();

  if (ledFlag[1])
    ledcWrite(ledPwmCh[1], map(pwmPushValue[1], 1, 100, 1, PWM_ON));

  if (ledFlag[2]) {
    if (neo1TypeFlag) {
      ledcWrite(ledPwmCh[2], PWM_ON);
      strip1.setBrightness(map(pwmPushValue[2], 1, 100, 1, 255));
      if (neo1_mode == 0) {
        Serial.println("NEO 1 Mode is 0....");
        for (uint16_t i = neo1Start; i < neo1Total; i++) {
          if (neo1rgbFlag)
            strip1.setPixelColor(
                i, strip1.Color(neo1_r_color, neo1_g_color, neo1_b_color));
          else
            strip1.setPixelColor(
                i, strip1.Color(neo1_g_color, neo1_b_color, neo1_r_color));
        }
        strip1.show();
        strip1.show();
      }
    } else {
      // ledcWrite(ledPwmCh[2], pwmPushValue[2]);
      ledcWrite(ledPwmCh[2], map(pwmPushValue[2], 1, 100, 1, PWM_ON));
    }
  }

  if (ledFlag[3]) {
    if (neo2TypeFlag) {
      ledcWrite(ledPwmCh[3], PWM_ON);
      strip2.setBrightness(map(pwmPushValue[3], 1, 100, 1, 255));
      if (neo2_mode == 0) {
        Serial.println("NEO 2 Mode is 0....");
        for (uint16_t i = neo2Start; i < neo2Total; i++) {
          if (neo2rgbFlag)
            strip2.setPixelColor(
                i, strip2.Color(neo2_r_color, neo2_g_color, neo2_b_color));
          else
            strip2.setPixelColor(
                i, strip2.Color(neo2_g_color, neo2_b_color, neo2_r_color));
        }
        strip2.show();
        strip2.show();
      }
    } else {
      // ledcWrite(ledPwmCh[3], pwmPushValue[3]);
      ledcWrite(ledPwmCh[3], map(pwmPushValue[3], 1, 100, 1, PWM_ON));
    }
  }

  if (ledFlag[4])
    ledcWrite(ledPwmCh[4], map(pwmPushValue[4], 1, 100, 1, PWM_ON));

  if (ledFlag[5])
    ledcWrite(ledPwmCh[5], map(pwmPushValue[5], 1, 100, 1, PWM_ON));

  switchVolt = analogRead(SWITCH);
  if (switchVolt > 1000)
    switchFlag = true;
  else
    switchFlag = false;

  if (digitalRead(DOOR)) {
    doorFlag = true;
    digitalWrite(DOOR_LED, LOW);
    if (ledFlag[1] == false) {
      for (int i = 0; i < 1000; i = i + 50) {
        ledcWrite(ledPwmCh[1], i);
        delay(50);
      }
      ledcWrite(ledPwmCh[1], PWM_ON);
    }
  } else {
    doorFlag = false;
    digitalWrite(DOOR_LED, HIGH);
    // if (ledFlag[1] == false) {
    //   for (int i = PWM_ON; i > 0; i = i - 50) {
    //     ledcWrite(ledPwmCh[1], i);
    //     delay(30);
    //   }
    //   ledcWrite(ledPwmCh[1], 0);
    // }
  }

  if (digitalRead(LiPo)) {
    batLiPoFlag = true;
    // Serial.println("Bat LiPo Flag : True");
  } else {
    batLiPoFlag = false;
    // Serial.println("Bat LiPo Flag : False");
  }

  Serial.print("NEO 1 RGB Type : ");
  if (neo1rgbFlag)
    Serial.println("RGB");
  else
    Serial.println("GRB");

  Serial.print("NEO 1 RGB Type : ");
  if (neo2rgbFlag)
    Serial.println("RGB");
  else
    Serial.println("GRB");
}

void loop() {

  // server.handleClient();

  if (stableCount > 500) {

    if (neo1_mode == 1 || neo2_mode == 1)
      rainbow(20);

    if (neo1_mode == 2 || neo2_mode == 2)
      rainbowCycle(20);

    if (neo1_mode == 3 || neo2_mode == 3)
      rainbowFade2White(3, 3, 0);

    if (neo1_mode == 7 || neo2_mode == 7)
      theaterChaseRainbow(150);

  } else {
    if (Serial2.available() > 0)
      serialEvent2();
    stableCount++;
    // Serial.print("Stable Count : ");
    // Serial.println(stableCount);
  }

  comProcess();
  neoModeChangeFlag = false;
}