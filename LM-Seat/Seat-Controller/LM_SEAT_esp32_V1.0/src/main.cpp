/* 2024년 7월 30

LM Seat V1.0 시트 콘트롤 프로그램
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
// #include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define STATION_MODE true
#define AP_MODE false

#define ENABLE 39 // RS485 Enable...

// #define PWM_ON 255
#define PWM_ON 1023
#define ON HIGH
#define OFF LOW

#define SW1_LEFT 12
#define SW1_RIGHT 45

#define SW2_LEFT 11
#define SW2_RIGHT 48

// #define SW3_LEFT 35
#define SW3_LEFT 13
#define SW3_RIGHT 10

#define H_SET 21
#define H_M1 47
#define H_M2 14

#define HEATER1 7
#define HEATER2 8

#define H1_L1 15
#define H1_L2 16
#define H1_L3 17
#define H1_L4 18
#define H2_L1 19
#define H2_L2 9
#define H2_L3 3
#define H2_L4 20

#define AP_SSID_EEPROM 20
#define AP_PSWD_EEPROM 70

// EEPROM Address...
#define NETWORK_EEPROM 2 // bool

unsigned long previousMillis = 0;

uint8_t comCount = 0;
uint8_t h1_LED, h2_LED, h3_LED, h1_LED_Push, h2_LED_Push, h3_LED_Push = 0;

char _label_text[30];

char ssidAp[30] = "LM V1.1-SB-001";
char passwordAp[30] = "17240731001";
char msg[100];

String apSSID, apPSWD, stSSID, stPSWD, hotSpotIP;

bool networkFlag = false;     // Network Select Switch...
bool hotSpotFlag = false;     // Station Connect Flag...
bool wifiStartFlag = false;   // WiFi Start Flag...
bool wifiConnectFlag = false; // WiFi Connect Flag...
bool apModeStartFlag = false; // Station Connect Flag...
bool rs485Flag = false;       // RS485 Connect Flag...

bool keyPressed = false;
bool remoteFlag = false; // Remote ...
bool wifiFlag = false;   // Analog, WiFi 구분...

bool ledFlag[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

bool ledAllFlag = false;

bool invertorFlag = false;
bool tvFlag = false;

bool neoFlag = true;
bool neoRandomFlag = true;

void wifiStart();

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload, s;

WebServer server(80);

void comSend(String sendData) {
  digitalWrite(ENABLE, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(ENABLE, LOW);
}

void delayTime(int wait) {

  for (int i = 0; i < (wait / 50); i++) {
    delay(55);
    server.handleClient();
    Serial.print("delayTime : ");
    Serial.println(i);
  }
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

  if (chanel == "BTN1_LEFT") {
    digitalWrite(SW1_RIGHT, LOW);
    digitalWrite(SW1_LEFT, HIGH);
  } else if (chanel == "BTN1_RIGHT") {
    digitalWrite(SW1_LEFT, LOW);
    digitalWrite(SW1_RIGHT, HIGH);
  } else if (chanel == "BTN1_STOP") {
    digitalWrite(SW1_LEFT, LOW);
    digitalWrite(SW1_RIGHT, LOW);
  } else if (chanel == "BTN2_UP") {
    digitalWrite(SW3_RIGHT, LOW);
    digitalWrite(SW3_LEFT, HIGH);
  } else if (chanel == "BTN2_DOWN") {
    digitalWrite(SW3_LEFT, LOW);
    digitalWrite(SW3_RIGHT, HIGH);
  } else if (chanel == "BTN2_STOP") {
    digitalWrite(SW3_LEFT, LOW);
    digitalWrite(SW3_RIGHT, LOW);
  } else if (chanel == "BTN3_LEFT") {
    digitalWrite(SW2_LEFT, LOW);
    digitalWrite(SW2_RIGHT, HIGH);
  } else if (chanel == "BTN3_RIGHT") {
    digitalWrite(SW2_RIGHT, LOW);
    digitalWrite(SW2_LEFT, HIGH);
  } else if (chanel == "BTN3_STOP") {
    digitalWrite(SW2_LEFT, LOW);
    digitalWrite(SW2_RIGHT, LOW);
  } else if (chanel == "CALL1") {
    digitalWrite(H_M1, HIGH);
    delay(300);
    digitalWrite(H_M1, LOW);
  } else if (chanel == "CALL2") {
    digitalWrite(H_M2, HIGH);
    delay(300);
    digitalWrite(H_M2, LOW);
  } else if (chanel == "MEMORY1") {
    digitalWrite(H_SET, HIGH);
    delay(300);
    server.handleClient();
    digitalWrite(H_SET, LOW);
    delay(300);
    server.handleClient();
    digitalWrite(H_M1, HIGH);
    delay(300);
    digitalWrite(H_M1, LOW);
  } else if (chanel == "MEMORY2") {
    digitalWrite(H_SET, HIGH);
    delay(300);
    server.handleClient();
    digitalWrite(H_SET, LOW);
    delay(300);
    server.handleClient();
    digitalWrite(H_M2, HIGH);
    delay(300);
    digitalWrite(H_M2, LOW);
  } else if (chanel == "HEAT1") {
    digitalWrite(HEATER1, HIGH);
    delayTime(300);
    digitalWrite(HEATER1, LOW);
    if (h1_LED_Push == 0)
      h1_LED_Push = 7;
    else if (h1_LED_Push == 7)
      h1_LED_Push = 5;
    else if (h1_LED_Push == 5)
      h1_LED_Push = 1;
    else if (h1_LED_Push == 1)
      h1_LED_Push = 0;
  } else if (chanel == "HEAT2") {
    digitalWrite(HEATER2, HIGH);
    delayTime(300);
    digitalWrite(HEATER2, LOW);
    if (h2_LED_Push == 0)
      h2_LED_Push = 7;
    else if (h2_LED_Push == 7)
      h2_LED_Push = 5;
    else if (h2_LED_Push == 5)
      h2_LED_Push = 1;
    else if (h2_LED_Push == 1)
      h2_LED_Push = 0;
  } else if (chanel == "AP_SSID") { // Access Point Mode
    Serial.print("AP_SSID String Length : ");
    Serial.print(str2.length());
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
  } else if (chanel == "COM") {
    if (!rs485Flag) {

      wifiStartFlag = false; // WiFi 통신 해제...

      // if (networkFlag == AP_MODE)
      //   WiFi.softAPdisconnect(); // 통신이 계속 되고 있으면 WiFi 종료...
      // else
      //   WiFi.disconnect();

      rs485Flag = true;
    }
    // uint16_t aux_read = analogRead(13);
    // Serial.print("ESP ADC Value : ");
    // Serial.println(aux_read);
    // Serial.print("ESP ADC Volt : ");
    // Serial.println(aux_read * (3.2 / 4095.0));
    // Serial.print("ADC CON Volt : ");
    // aux_v = aux_read * 0.00744109136;
    // Serial.println(aux_v);
    // s = "TV_VOLT:";
    // s += String(aux_v, 1);
    // s += " V";
    // comSend(s); // 1초 마다 SC01-PLUS측으로 회신...
    comSend("READY:1"); // 1초 마다 SC01-PLUS측으로 회신...
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
  int chanel = root["Chanel"];
  int value = root["Value"];

  if (division == "BTN_1_LEFT" || division == "BTN_1_RIGHT") {
    wifiFlag = true;
    remoteFlag = true;
    if (chanel == 1) {
      if (division == "BTN_1_LEFT") {
        digitalWrite(SW1_RIGHT, LOW);
        digitalWrite(SW1_LEFT, HIGH);
        comSend("BTN1_LEFT:1");
      } else if (division == "BTN_1_RIGHT") {
        digitalWrite(SW1_LEFT, LOW);
        digitalWrite(SW1_RIGHT, HIGH);
        comSend("BTN1_RIGHT:1");
      }
    } else {
      digitalWrite(SW1_RIGHT, LOW);
      digitalWrite(SW1_LEFT, LOW);
      comSend("BTN1_STOP:1");
    }
    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "BTN_2_UP" || division == "BTN_2_DOWN") {
    wifiFlag = true;
    remoteFlag = true;
    if (chanel == 1) {
      if (division == "BTN_2_UP") {
        digitalWrite(SW3_RIGHT, LOW);
        digitalWrite(SW3_LEFT, HIGH);
        comSend("BTN2_UP:1");
      } else if (division == "BTN_2_DOWN") {
        digitalWrite(SW3_LEFT, LOW);
        digitalWrite(SW3_RIGHT, HIGH);
        comSend("BTN2_DOWN:1");
      }
    } else {
      digitalWrite(SW3_RIGHT, LOW);
      digitalWrite(SW3_LEFT, LOW);
      comSend("BTN2_STOP:1");
    }
    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "BTN_3_LEFT" || division == "BTN_3_RIGHT") {
    wifiFlag = true;
    remoteFlag = true;
    if (chanel == 1) {
      if (division == "BTN_3_LEFT") {
        digitalWrite(SW2_LEFT, LOW);
        digitalWrite(SW2_RIGHT, HIGH);
        comSend("BTN3_LEFT:1");
      } else if (division == "BTN_3_RIGHT") {
        digitalWrite(SW2_RIGHT, LOW);
        digitalWrite(SW2_LEFT, HIGH);
        comSend("BTN3_RIGHT:1");
      }
    } else {
      digitalWrite(SW2_RIGHT, LOW);
      digitalWrite(SW2_LEFT, LOW);
      comSend("BTN3_STOP:1");
    }
    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "SEAT_HEATER1") {
    digitalWrite(HEATER1, HIGH);
    delay(200);
    digitalWrite(HEATER1, LOW);
    if (h1_LED_Push == 0)
      h1_LED_Push = 7;
    else if (h1_LED_Push == 7)
      h1_LED_Push = 5;
    else if (h1_LED_Push == 5)
      h1_LED_Push = 1;
    else if (h1_LED_Push == 1)
      h1_LED_Push = 0;
  } else if (division == "SEAT_HEATER2") {
    digitalWrite(HEATER2, HIGH);
    delay(200);
    digitalWrite(HEATER2, LOW);
    if (h2_LED_Push == 0)
      h2_LED_Push = 7;
    else if (h2_LED_Push == 7)
      h2_LED_Push = 5;
    else if (h2_LED_Push == 5)
      h2_LED_Push = 1;
    else if (h2_LED_Push == 1)
      h2_LED_Push = 0;
  } else if (division == "CALL1") {
    digitalWrite(H_M1, HIGH);
    delay(300);
    digitalWrite(H_M1, LOW);
  } else if (division == "CALL2") {
    digitalWrite(H_M2, HIGH);
    delay(300);
    digitalWrite(H_M2, LOW);
  } else if (division == "MEMORY1") {
    digitalWrite(H_SET, HIGH);
    delay(300);
    digitalWrite(H_SET, LOW);
    delay(300);
    digitalWrite(H_M1, HIGH);
    delay(300);
    digitalWrite(H_M1, LOW);
  } else if (division == "MEMORY2") {
    digitalWrite(H_SET, HIGH);
    delay(300);
    digitalWrite(H_SET, LOW);
    delay(300);
    digitalWrite(H_M2, HIGH);
    delay(300);
    digitalWrite(H_M2, LOW);
  } else if (division == "QR_OK") {
    Serial.println("QR_OK Received...");
    comSend("QR_OK:1");
    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  }

  server.send(200, "text/plain", s);

} // handleJson()

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  server.send(200, "text/plain", "");
}

void handleRequest() {

  s = "";
  s = s + "{" + "\"H1_LED\":" + h1_LED + ", " + "\"H2_LED\":" + h2_LED + ", " +
      "\"H3_LED\":" + h3_LED + "}";

  server.send(200, "text/plain", s);
  comCount = 0;

} // handleRequest()

void wifiStart() { // WiFi Mode Start...

  String msg;

  wifiStartFlag = true; // WiFi 통신 시작...
  rs485Flag = false;

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
      Serial.print("SSID: ");
      Serial.print(ssidAp);
      Serial.print(", PSWD: ");
      Serial.println(passwordAp);
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

void setup() {

  pinMode(ENABLE, OUTPUT);

  pinMode(13, INPUT);

  pinMode(SW1_LEFT, OUTPUT);
  pinMode(SW1_RIGHT, OUTPUT);
  pinMode(SW2_LEFT, OUTPUT);
  pinMode(SW2_RIGHT, OUTPUT);
  pinMode(SW3_LEFT, OUTPUT);
  pinMode(SW3_RIGHT, OUTPUT);

  pinMode(H_SET, OUTPUT);
  pinMode(H_M1, OUTPUT);
  pinMode(H_M2, OUTPUT);

  pinMode(HEATER1, OUTPUT);
  pinMode(HEATER2, OUTPUT);

  pinMode(H1_L1, INPUT_PULLUP);
  pinMode(H1_L2, INPUT_PULLUP);
  pinMode(H1_L3, INPUT_PULLUP);
  // pinMode(H1_L4, OUTPUT);
  pinMode(H2_L1, INPUT_PULLUP);
  pinMode(H2_L2, INPUT_PULLUP);
  pinMode(H2_L3, INPUT_PULLUP);

  digitalWrite(ENABLE, LOW); // RS485 수신 대기...

  digitalWrite(SW1_LEFT, LOW);
  digitalWrite(SW1_RIGHT, LOW);
  digitalWrite(SW2_LEFT, LOW);
  digitalWrite(SW2_RIGHT, LOW);
  digitalWrite(SW3_LEFT, LOW);
  digitalWrite(SW3_RIGHT, LOW);

  digitalWrite(H_SET, LOW);
  digitalWrite(H_M1, LOW);
  digitalWrite(H_M2, LOW);

  digitalWrite(HEATER1, LOW);
  digitalWrite(H1_L4, HIGH);
  digitalWrite(HEATER2, LOW);
  digitalWrite(H2_L4, HIGH);

  EEPROM.begin(250);

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 37, 38);

  apSSID = EEPROM.readString(AP_SSID_EEPROM);
  apPSWD = EEPROM.readString(AP_PSWD_EEPROM);

  Serial.println("Setup done.....");

  apSSID.toCharArray(ssidAp, apSSID.length());
  apPSWD.toCharArray(passwordAp, apPSWD.length());

  wifiStart();
}

void loop() {

  server.handleClient();

  // if (digitalRead(H1_L1) && digitalRead(H1_L2) && digitalRead(H1_L3))
  //   h1_LED_Push = 0;
  // else if (!digitalRead(H1_L1) && !digitalRead(H1_L2) && !digitalRead(H1_L3))
  //   h1_LED_Push = 7;
  // else if (digitalRead(H1_L1) && !digitalRead(H1_L2) && !digitalRead(H1_L3))
  //   h1_LED_Push = 5;
  // else if (digitalRead(H1_L1) && digitalRead(H1_L2) && !digitalRead(H1_L3))
  //   h1_LED_Push = 1;

  // if (digitalRead(H2_L1) && digitalRead(H2_L2) && digitalRead(H2_L3))
  //   h2_LED_Push = 0;
  // else if (!digitalRead(H2_L1) && !digitalRead(H2_L2) && !digitalRead(H2_L3))
  //   h2_LED_Push = 7;
  // else if (digitalRead(H2_L1) && !digitalRead(H2_L2) && !digitalRead(H2_L3))
  //   h2_LED_Push = 5;
  // else if (digitalRead(H2_L1) && digitalRead(H2_L2) && !digitalRead(H2_L3))
  //   h2_LED_Push = 1;

  if (h1_LED != h1_LED_Push) {
    h1_LED = h1_LED_Push;
    s = "H1_LED:";
    s += h1_LED;
    comSend(s);
    Serial.print("H1_L1 : ");
    Serial.print(digitalRead(H1_L1));
    Serial.print(", H1_L2 : ");
    Serial.print(digitalRead(H1_L2));
    Serial.print(", H1_L3 : ");
    Serial.print(digitalRead(H1_L3));
    Serial.print(", H1_LED_Push : ");
    Serial.println(h1_LED_Push);
  }

  if (h2_LED != h2_LED_Push) {
    h2_LED = h2_LED_Push;
    s = "H2_LED:";
    s += h2_LED;
    comSend(s);
    Serial.print("H2_L1 : ");
    Serial.print(digitalRead(H2_L1));
    Serial.print(", H2_L2 : ");
    Serial.print(digitalRead(H2_L2));
    Serial.print(", H2_L3 : ");
    Serial.print(digitalRead(H2_L3));
    Serial.print(", H2_LED_Push : ");
    Serial.println(h2_LED_Push);
  }

  if (millis() - previousMillis >= 1400) {
    comSend("COM:1");
    //   Serial.print("H1_L1 : ");
    //   Serial.print(digitalRead(H1_L1));
    //   Serial.print(", H1_L2 : ");
    //   Serial.print(digitalRead(H1_L2));
    //   Serial.print(", H1_L3 : ");
    //   Serial.print(digitalRead(H1_L3));
    //   Serial.print(", H1_LED_Push : ");
    //   Serial.println(h1_LED_Push);
    //   Serial.print("H2_L1 : ");
    //   Serial.print(digitalRead(H2_L1));
    //   Serial.print(", H2_L2 : ");
    //   Serial.print(digitalRead(H2_L2));
    //   Serial.print(", H2_L3 : ");
    //   Serial.print(digitalRead(H2_L3));
    //   Serial.print(", H2_LED_Push : ");
    //   Serial.println(h2_LED_Push);
    //   Serial.println();

    previousMillis = millis();
  }
  delay(5);
}