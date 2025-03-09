/* 2024년 10월 31I{일

카인드 캠핑카 V2.0 프로그램
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

#include "Arduino.h"
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

uint16_t ledPwm[6] = {100, 100, 100, 100, 100, 100};
uint16_t aux_read;

unsigned long previousMillis = 0;

uint8_t comCount = 0;
uint16_t globalCount = 0;
uint16_t chanelCount = 1;
int16_t aux_adc, main_adc;

char ssidAp[30] = "카인드 V2.0-CD-001";
char passwordAp[30] = "18250101001";
char msg[100];

String apSSID, apPSWD, stSSID, stPSWD, hotSpotIP;

bool ledComFlag = false; // LED COM Flag

bool networkFlag = false;     // Network Select Switch...
bool hotSpotFlag = false;     // Station Connect Flag...
bool wifiStartFlag = false;   // WiFi Start Flag...
bool wifiConnectFlag = false; // WiFi Connect Flag...
bool apModeStartFlag = false; // Station Connect Flag...
bool rs485Flag = false;       // RS485 Connect Flag...
bool remoteFlag = false;      // Remote ...

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
// bool trunkDoorFlagPush = false;

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload, s;

WebServer server(80);

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
  if (value != 0) {
    if (trunkDoorFlag)
      // 카니발 모드 일 때...
      pwm = map(value, 1, 100, 1, PWM_ON);
    else
      // 스타리아 모드일 때...
      pwm = PWM_ON;
  }

  if (chanel == "LED1") {
    ledcWrite(ledPwmCh[1], pwm);
    ledPwm[1] = value;
    if (value != 0) {
      ledFlag[1] = true;
    } else {
      ledFlag[1] = false;
      ledAllFlag = false;
    }

  } else if (chanel == "LED2") {

    ledcWrite(ledPwmCh[2], pwm);

    ledPwm[2] = value;
    if (value != 0) {
      ledFlag[2] = true;
    } else {
      ledFlag[2] = false;
      ledAllFlag = false;
    }
  } else if (chanel == "LED3") {
    ledcWrite(ledPwmCh[3], pwm);
    ledPwm[3] = value;
    if (value != 0) {
      ledFlag[3] = true;
    } else {
      ledFlag[3] = false;
      ledAllFlag = false;
    }
    // if (trunkDoorFlag) {
    //   ledPwm[3] = 100;
    //   ledFlag[3] = false;
    //   digitalWrite(TRUNK_DOOR, HIGH);
    //   delay(500);
    //   digitalWrite(TRUNK_DOOR, LOW);
    //   s = "LED3:";
    //   s += 0;
    //   s += ':';
    //   s += 0;
    //   comSend(s);
    // } else {
    //   ledcWrite(ledPwmCh[3], pwm);
    //   ledPwm[3] = value;
    //   if (value != 0) {
    //     ledFlag[3] = true;
    //   } else {
    //     ledFlag[3] = false;
    //     ledAllFlag = false;
    //   }
    // }
  } else if (chanel == "LED4") {
    ledcWrite(ledPwmCh[4], pwm);
    ledPwm[4] = value;
    if (value != 0) {
      ledFlag[4] = true;
    } else {
      ledFlag[4] = false;
      ledAllFlag = false;
    }
  } else if (chanel == "PUMP") { // 물펌프
    if (trunkDoorFlag) {
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
    }
  } else if (chanel == "CIGAR") { // 시거잭
    value == 1 ? digitalWrite(CIGAR, HIGH) : digitalWrite(CIGAR, LOW);
    if (value) {
      cigarFlag = true;
    } else {
      cigarFlag = false;
    }
  } else if (chanel == "FRIGE") { // 냉장고
    value == 1 ? digitalWrite(FRIGE, HIGH) : digitalWrite(FRIGE, LOW);
    if (value) {
      frigeFlag = true;
    } else {
      frigeFlag = false;
    }
  } else if (chanel == "TV") { // TV
    value == 1 ? digitalWrite(TV, HIGH) : digitalWrite(TV, LOW);
    if (value) {
      tvFlag = true;
    } else {
      tvFlag = false;
    }
  } else if (chanel == "INV") { // 인버터

    value == 1 ? digitalWrite(INVERTOR, HIGH) : digitalWrite(INVERTOR, LOW);

    if (value) {
      invertorFlag = true;
    } else {
      invertorFlag = false;
    }
  } else if (chanel == "SLEEP") {
    if (value) {
      // ledcWrite(ledPwmCh[1], 0);
      // ledcWrite(ledPwmCh[2], 0);
      // ledcWrite(ledPwmCh[3], 0);
      // ledcWrite(ledPwmCh[4], 0);
      // ledFlag[1] = false;
      // ledFlag[2] = false;
      // ledFlag[3] = false;
      // ledFlag[4] = false;
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
    // s = "LED_ALL:";
    // s += (ledAllFlag != 0) ? 100 : 0;
    // s += ':';
    // s += ledAllFlag;
    // comSend(s);
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
  String ledName = root["LED_NAME"];
  String nowTime = root["NOW_TIME"];
  int chanel = root["Chanel"];
  int value = root["Value"];

  if (division == "LED") {
    wifiFlag = true;
    remoteFlag = true;

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
    wifiFlag = true;
    remoteFlag = true;
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
    wifiFlag = true;
    remoteFlag = true;
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
    wifiFlag = true;
    remoteFlag = true;
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
    wifiFlag = true;
    remoteFlag = true;
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
    wifiFlag = true;
    remoteFlag = true;
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
    wifiFlag = true;
    remoteFlag = true;
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
    wifiFlag = true;
    remoteFlag = true;
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
  comCount = 0;

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

void dipSW_read() {

  if (digitalRead(SWITCH_3)) {
    companyFlag = false; // 카인드 모드
    Serial.println("CarInd 모드");
  } else {
    companyFlag = true; // MS 모드
    Serial.println("MS 모드");
  }

  if (digitalRead(SWITCH_4)) {
    trunkDoorFlag = false; // 트렁크 스타리아 모드
    Serial.println("스타리아 모드");
  } else {
    trunkDoorFlag = true; // 트렁크 카니발 모드
    Serial.println("카니발 모드");
  }

  // if (trunkDoorFlagPush != trunkDoorFlag) {
  //   trunkDoorFlagPush = trunkDoorFlag;
  //   s = "TRUNK_MODE:";
  //   s += trunkDoorFlag;
  //   s += ':';
  //   s += trunkDoorFlag;
  //   comSend("COM:1");
  //   delay(30);
  //   comSend(s);
  // }
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

  ledDisp(10, 30);
  dipSW_read();

  delay(1000);
  comSend("COM:1");
  delay(10);
  comSend("REQ_NET:1");

  wifiStart();
  Serial.println("WiFi Started....");
}

void loop() {

  server.handleClient();

  if (millis() - previousMillis >= 2000) {
    Serial.print("WiFi.status() : ");
    Serial.println(WiFi.status());

    Serial.print("WifiConnectFlag : ");
    Serial.println(wifiStartFlag ? "WiFi Start..." : "WiFi Stop...");

    Serial.print("Communication Count : ");
    Serial.println(comCount);

    // if (3 < comCount++) {
    //   if (!wifiStartFlag) {
    //     wifiStart();
    //     Serial.println("WiFi Started....");
    //   }
    //   if (comCount > 1000)
    //     comCount = 10;
    // }

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
      // for (int i = 1; i < 10; i++) {
      //   digitalWrite(COM_LED, LOW);
      //   delay(30);
      //   digitalWrite(COM_LED, HIGH);
      //   delay(30);
      // }
    }

    dipSW_read();
  }

  delay(5);
  globalCount++;
  // if (trunkDoorFlag && pumpFlag) {
  //   pumpFlag = false;
  //   s = "PUMP:";
  //   s += pumpFlag;
  //   s += ':';
  //   s += pumpFlag;
  //   comSend(s);
  // }
}
