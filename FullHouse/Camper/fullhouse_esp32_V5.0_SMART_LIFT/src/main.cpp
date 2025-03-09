/*
Full House LIFT Program V5.0

  1. KiCad Main Board 위치
    - E:\CamTrol II\full_house\PCB\Model_Lift_V5.0
  2. SquareLine Studio 위치
    - E:\CamTrol II\full_house\esp32_ST7789V3
  3. SquareLine Studio ui 위치
    - E:\CamTrol II\full_house\esp32_ST7789V3\esp32_ST7789V3\libraries
    - SQL Export 후 ui 폴더를 VSC의 src 폴더에 붙여 사용한다.
  4. Flutter Program 위치
    - E:\CamTrol II\full_house\fullhouse_flutter_v5.0_smart_lift
  5. QR Code 생성은 여기서 해야 함. Size: 150 x 150
    - https://ko.qr-code-generator.com/
  6. LAN Cable 길이
    - 트럭 : 9000 mm   9 미터
    - 칸 : 6000 mm     6 미터

Main Board 에서 SSID, PASS WORD 읽어 오는 방법
    1. Main Board를 FACTORY Mode로 booting 한다..
    2. 리프트의 전원을 투입 한다.
    3. 잠시 후 FACTORY 스위치를 길게 누른다.
    4. 이 때 LED가 빠르게 점멸하면 스위치를 Off 한다.
    5. LED가 간헐적 3 ~ 5회 가량 점멸하면 전원을 재 투입 한다.

Analog Offset 값 자동 적용 방법
    1. FACTORY 스위치를 누르면서 전원을 투입 한다.
    2. LED가 빠르게 점멸 되는것이 확인 되면 FACTORY 스위치를 Off한다.
    3. LED가 간헐적으로 50회 가량 깜박이며 멈추는지 확인 한다.
    4. LED 깜박임이 멈춘 후 FACTORY 스위치를 한번 더 누른다.
    5. 이 때 LED가 2+0회 가량 빠르게 점멸하면 저장 완료임.
    6. 전원 재 투입 후 실행.


 EEPROM Address
    8   : liftMode - bool
    10  : FACTORY SSID - String
    40  : FACTORY PASSWORD - String
    70  : TABLET SSID - String
    100 : TABLET PASSWORD - String
    150 : avrCurrent1 - Float
    160 : avrCurrent2 - Float
    170 : avrCurrent3 - Float
    180 : avrCurrent4 - Float
    210 : compareCurrent1 - Float
    220 : compareCurrent2 - Float
    230 : compareCurrent3 - Float
    240 : compareCurrent4 - Float

    IO08 과 IO40 교체 해야함...
*/

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "ui/ui.h" // this is the ui generated with lvgl / squareline editor
#include <WiFi.h>
#include <HTTPClient.h>
#include <ADS1X15.h>
#include <WebServer.h>
#include <ArduinoJson.h>
// #include <Wire.h>
#include <EEPROM.h>
#include <Nextion.h>

#define AP_MODE true
#define STATION_MODE false

#define ON HIGH
#define OFF LOW

#define ENABLE 35

#define Monitor 13

#define FACTORY 7

// 전원 투입 시 0.5초간 ON 현상 발생...

#define AI_1 4  // ESP32 S3 Analog In 1
#define AI_2 5  // ESP32 S3 Analog In 2
#define AI_3 6  // ESP32 S3 Analog In 3
#define AI_4 19 // ESP32 S3 Analog In 4

#define L1U 15 // LEG 1 UP
#define L1D 16 // LEG 1 DOWN
#define L2U 17 // LEG 2 UP
#define L2D 18 // LEG 2 DOWN

#define L3U 8  // LEG 3 UP
#define L3D 20 // LEG 3 DOWN
#define L4U 39 // LEG 4 UP
#define L4D 40 // LEG 4 DOWN

#define SW_LEFT 14  // Switch Left
#define SW_RIGHT 21 // Switch Right
#define SW_UP 47    // Switch Up
#define SW_DOWN 48  // Switch Down
#define SW_ENTER 45 // Switch Enter

#define SCREEN2 2 // Screen 2
#define SCREEN3 3 // Screen 3
#define SCREEN4 4 // Screen 4
#define SCREEN5 5 // Screen 5
#define SCREEN6 6 // Screen 5

#define BOOT 38

lv_opa_t opaValue = 0;
uint8_t readStatus = 0;

const char *ssidBuffer = "";
const char *pwBuffer = "";

char ssidAp[30] = "리프트 V5.0-073";
char passwordAp[30] = "03240304073";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

unsigned int ads1, ads2, ads3, ads4;
float nowCurrent;
float compareCurrent1;
float compareCurrent2;
float compareCurrent3;
float compareCurrent4;

float current1 = 0.0;
float current2 = 0.0;
float current3 = 0.0;
float current4 = 0.0;
float ads1Offset = 0.0;
float ads2Offset = 0.0;
float ads3Offset = 0.0;
float ads4Offset = 0.0;
float avrCurrent1 = 0.0;
float avrCurrent2 = 0.0;
float avrCurrent3 = 0.0;
float avrCurrent4 = 0.0;

String stationHostName = "CamTrol V3.6";

// TwoWire I2Ctwo = TwoWire(1);
// ADS1115 ads(0x48, &I2Ctwo);
ADS1115 ads(0x48);

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;
String payload;

String EEPROM_SSID; // AP Mode 시 ssid...
String EEPROM_PSWD; // AP Mode 시 password...

String EEPROM_SSID_STA; // Station Mode 시 ssid...
String EEPROM_PSWD_STA; // Station Mode 시 password...

// String EEPROM_SSID_HOTSPOT =
//     "CamTrol V3.5-A-001"; // EEPROM 주소 0부터 SSID를 읽기
// String EEPROM_PSWD_HOTSPOT = "01220201001";

WebServer server(80);
// HardwareSerial lift(2);

NexButton idsave = NexButton(0, 3, "idsave");
NexButton pwsave = NexButton(0, 4, "pwsave");

NexText t2 = NexText(0, 46, "t2");
NexText t3 = NexText(0, 47, "t3");

NexText ssidTxt = NexText(0, 48, "ssidTxt");
NexText pwTxt = NexText(0, 49, "pwTxt");

NexTouch *nex_listen_list[] = {&t2,     &t3,     &ssidTxt, &pwTxt,
                               &idsave, &pwsave, NULL};

// bool liftMode;
bool serialDebug = false;
bool avrFlag = false; // 평균값 Flag
bool networkChangeFlag = false;
bool outLockFlag = true;

bool menu11Flag = false; // Screen 4의 Menu 1 Flag...
bool menu12Flag;         // Screen 4의 Menu 2 Flag (수동/자동 모드)...
bool menu13Flag;         // Screen 4의 Menu 3 Flag (단독/CPU 모드)...
bool menu12FlagPush;     // Screen 4의 Menu 2 Flag (수동/자동 모드)...
bool menu13FlagPush;     // Screen 4의 Menu 3 Flag (단독/CPU 모드)...

bool menu21Flag = false; // Screen 5의 Menu 1 Flag...
bool menu22Flag = false; // Screen 5의 Menu 2 Flag...
bool menu23Flag = false; // Screen 5의 Menu 3 Flag...
bool menu24Flag = false; // Screen 5의 Menu 4 Flag...

bool curSaveFlag = false; // Screen 5의 전류 변경이 있으면 True...

int pageCount = SCREEN2; // 초기 Screen Page 2...
int menu1Count = 0;      // Screen 4의 Menu Count...
int menu2Count = 0;      // Screen 5의 Menu Count...
int menu6Count = 0;      // Screen 6의 Menu Count...
int textAreaCount = 0;
int count = 0;
int dispCount = 0;
int holdTime = 500;
char msg[100];
int wiFiCountValue = 1;
bool wiFiMode = AP_MODE; // true : AP Mode, false : Station Mode...
bool wifiStatus = false; // Monitor LED ON, OFF 용...

bool l1uFlag = false;
bool l1dFlag = false;

bool l2uFlag = false;
bool l2dFlag = false;

bool l3uFlag = false;
bool l3dFlag = false;

bool l4uFlag = false;
bool l4dFlag = false;

bool lauFlag = false;
bool ladFlag = false;

bool currentFlag_1 = false;
bool currentFlag_2 = false;
bool currentFlag_3 = false;
bool currentFlag_4 = false;

bool curChgFlag_1 = false;
bool curChgFlag_2 = false;
bool curChgFlag_3 = false;
bool curChgFlag_4 = false;

String s;
String eventString;

char buffer[100] = {0};

// 1.47" 172x320 ST7789V
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance; // SPIバスのインスタンス
  // lgfx::Light_PWM _light_instance;
  // lgfx::Touch_CST816S _touch_instance;
  // lgfx::Touch_GT911 _touch_instance;

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST; // 使用するSPIを選択  ESP32-S2,C3 : SPI2_HOST or
                                // SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      cfg.spi_mode = 3;         // 2 SPI通信モードを設定 (0 ~ 3)
      cfg.freq_write = 80000000; // 送信時のSPIクロック (最大80MHz,
      // 80MHzを整数で割った値に丸められます)
      cfg.freq_read = 20000000; // 受信時のSPIクロック
      cfg.spi_3wire = false; // 受信をMOSIピンで行う場合はtrueを設定
      cfg.use_lock = true; // トランザクションロックを使用する場合はtrueを設定
      cfg.dma_channel = SPI_DMA_CH_AUTO; // 使用するDMAチャンネルを設定
                                         // (0=DMA不使用 / 1=1ch / 2=ch /
                                         // SPI_DMA_CH_AUTO=自動設定)
      cfg.pin_sclk = 11;                 // SPIのSCLKピン番号を設定
      cfg.pin_mosi = 12;                 // SPIのMOSIピン番号を設定
      cfg.pin_miso = -1; // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc = 9;    // SPIのD/Cピン番号を設定  (-1 = disable)
      _bus_instance.config(cfg); // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {
      auto cfg =
          _panel_instance.config(); // 表示パネル設定用の構造体を取得します。
      cfg.pin_cs = 10; // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst = 1; // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy = -1; // BUSYが接続されているピン番号 (-1 = disable)
      cfg.panel_width = 172;  // 172, 240 実際に表示可能な幅
      cfg.panel_height = 320; // 320, 280 実際に表示可能な高さ
      cfg.offset_x = 34;      // パネルのX方向オフセット量
      cfg.offset_y = 0;       // パネルのY方向オフセット量
      cfg.offset_rotation = 0; // 回転方向の値のオフセット 0~7 (4~7は上下反転)
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
      _panel_instance.config(cfg);
    }
    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};

void ledMonitor(uint8_t rpt) {
  for (uint8_t i = 0; i < rpt; i++) {
    digitalWrite(Monitor, OFF);
    delay(50);
    digitalWrite(Monitor, ON);
    delay(30);
  }
}

void lv_print(String msg) {
  textAreaCount++;
  if (textAreaCount > 30) { // Text 가 너무 많으면 화면 Clear...
    lv_textarea_set_text(ui_TextArea1, "");
    textAreaCount = 0;
  }
  char _text[50];
  msg.toCharArray(_text, sizeof(_text));
  lv_textarea_add_text(ui_TextArea1, _text);
  lv_textarea_add_char(ui_TextArea1, '\n');
}

void t2PushCallback(void *ptr) { // Station SSID Read...
  ssidTxt.setText("  ");
  // ssidTxt.setText(ssidAp);
  EEPROM_SSID_STA = EEPROM.readString(70);
  EEPROM_SSID_STA.trim();
  EEPROM_SSID_STA.toCharArray(msg, EEPROM_SSID_STA.length() + 1);
  ssidTxt.setText(msg);
}

void t3PushCallback(void *ptr) { // Station PassWord Read...
  pwTxt.setText("  ");
  // pwTxt.setText(passwordAp);
  EEPROM_PSWD_STA = EEPROM.readString(100);
  EEPROM_PSWD_STA.trim();
  EEPROM_PSWD_STA.toCharArray(msg, EEPROM_PSWD_STA.length() + 1);
  pwTxt.setText(msg);
}

void idSavePushCallback(void *ptr) {
  char ch;
  String receive = "";
  Serial.println();
  Serial.println(F("Nextion ID Save Button Push... : "));
  while (Serial.available() > 0) {
    ch = Serial.read();
    if (ch != '~')
      receive += ch;
    else {
      ch = Serial.read();
      ch = Serial.read();
      Serial.print(ch);
      break;
    }
  }
  Serial.print(F(" byte Receive : "));
  Serial.println(receive);
  ssidBuffer = receive.c_str();
  // if (wiFiMode == AP_MODE)
  //   EEPROM.writeString(10, ssidBuffer); // FACTORY SSID
  // else
  EEPROM_SSID_STA = ssidBuffer;
  EEPROM.writeString(70, EEPROM_SSID_STA); // TABLET SSID
  EEPROM.commit();
  s = "\n다음과 같이 변경되었습니다.";
  lv_print(s);
  s = "SSID: ";
  s += EEPROM_SSID_STA;
  lv_print(s);
  ledMonitor(5);
}

void pwSavePushCallback(void *ptr) {
  char ch;
  String receive = "";
  Serial.println();
  Serial.println(F("Nextion Password Save Button Push..."));
  while (Serial.available() > 0) {
    ch = Serial.read();
    if (ch != '~')
      receive += ch;
    else {
      ch = Serial.read();
      ch = Serial.read();
      Serial.print(ch);
      break;
    }
  }
  Serial.print(F(" byte Receive : "));
  Serial.println(receive);
  pwBuffer = receive.c_str();
  // if (wiFiMode == AP_MODE)
  //   EEPROM.writeString(40, pwBuffer); // FACTORY PASSWORD
  // else
  EEPROM_PSWD_STA = pwBuffer;
  EEPROM.writeString(100, EEPROM_PSWD_STA); // TABLET PASSWORD
  EEPROM.commit();
  s = "\n다음과 같이 변경되었습니다.";
  lv_print(s);
  s = "PSWD: ";
  s += EEPROM_PSWD_STA;
  lv_print(s);
  ledMonitor(5);
}

void wifiConnect() {
  EEPROM_SSID.toCharArray(ssidAp, EEPROM_SSID.length() + 1);
  EEPROM_PSWD.toCharArray(passwordAp, EEPROM_PSWD.length() + 1);

  Serial.println();
  Serial.println();
  Serial.println(F("Full House Camping Car Version V3.6..."));
  Serial.println();
  Serial.println(F("ESP32 Program Source by   "
                   "'E:\\CamTrol_V3.6\\Full_house\\Fullhouse_esp32_V3.6'"));
  Serial.println(
      F("Nextion Program Source by "
        "'E:\\CamTrol_V3.6\\Full_house\\CamTrol_esp32_V3.6_Nextion'"));
  Serial.println(F("Flutter Program Source by "
                   "'E:\\CamTrol_V3.6\\Full_house\\Fullhouse_flutter_v3.6'"));
  Serial.println();
  Serial.println(F("KiCad Main board Source by  'C:\\Users\\Lee "
                   "JunHo\\Desktop\\업무\\CamTrol Final\\CamTrol "
                   "V3.6\\mini\\12ch\\FullHouse-Main Board'"));
  Serial.println(F("KiCad Input board Source by 'C:\\Users\\Lee "
                   "JunHo\\Desktop\\업무\\CamTrol Final\\CamTrol "
                   "V3.6\\mini\\12ch\\FullHouse-Input Board'"));
  Serial.println(F("KiCad Cover board Source by 'C:\\Users\\Lee "
                   "JunHo\\Desktop\\업무\\CamTrol Final\\CamTrol "
                   "V3.6\\mini\\12ch\\FullHouse-Cover Board'"));
  Serial.println();
  Serial.println(F("Made by 'CamTrol LTD,.' Lee JunHo...  2022,05,29"));
  Serial.println();
  Serial.println();
  Serial.print(F("SSID : "));
  Serial.print(ssidAp);
  Serial.print(F(",  P/W : "));
  Serial.println(passwordAp);

  Serial.println();

  Serial.println("");
  Serial.println(F("Connected"));
  Serial.println("");
  Serial.print(F("Connected to WiFi network with IP Address: "));
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.println("");
}

void handleRequest() {
  wiFiCountValue = 0;
  outLockFlag = false;

  s = "";
  s = s + "{" + "\"serialDebug\":" + serialDebug +
      ", " + // Factory Mode 이면 Serial debug true...
      "\"L1U_Flag\":" + l1uFlag + ", " +  // Lift 1 up Bool...
      "\"L1D_Flag\":" + l1dFlag + ", " +  // Lift 1 down Bool...
      "\"L2U_Flag\":" + l2uFlag + ", " +  // Lift 2 up Bool...
      "\"L2D_Flag\":" + l2dFlag + ", " +  // Lift 2 down Bool...
      "\"L3U_Flag\":" + l3uFlag + ", " +  // Lift 3 up Bool...
      "\"L3D_Flag\":" + l3dFlag + ", " +  // Lift 3 down Bool...
      "\"L4U_Flag\":" + l4uFlag + ", " +  // Lift 4 up Bool...
      "\"L4D_Flag\":" + l4dFlag + ", " +  // Lift 4 down Bool...
      "\"LAU_Flag\":" + lauFlag + ", " +  // Lift all up Bool...
      "\"LAD_Flag\":" + ladFlag + ", " +  // Lift all down Bool...
      "\"CURRENT1\":" + current1 + ", " + // Lift all down float...
      "\"CURRENT2\":" + current2 + ", " + // Lift all down float...
      "\"CURRENT3\":" + current3 + ", " + // Lift all down float...
      "\"CURRENT4\":" + current4 +        // Lift all down float...
      "}";

  server.send(200, "text/plain", s);

  for (int i = 0; i < 5; i++) { // WiFi 수신이 되었음을 알림...
    digitalWrite(Monitor, !i);
    delay(1);
  }
  digitalWrite(Monitor, ON);
}

void handleJson() {
  wiFiCountValue = 0;
  outLockFlag = false;

  server.arg("j").toCharArray(msg, 2000);
  if (serialDebug) {
    Serial.println();
    Serial.println(F("handleJson()..."));
    Serial.println(msg);
  }
  deserializeJson(doc, msg);
  root = doc.as<JsonObject>();

  String division = root["division"];
  String ssidParse = root["ssid"];
  String passwordParse = root["password"];
  int chanel = root["Chanel"];

  if (division == "L1U") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L1U, ON);
      l1uFlag = true;
      lv_print("리프트 1             상승...");
      delay(holdTime);
    } else {
      digitalWrite(L1U, OFF);
      l1uFlag = false;
      lv_print("리프트 1             정지...");
    }
  } else if (division == "L1D") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L1D, ON);
      l1dFlag = true;
      lv_print("리프트 1             하강...");
    } else {
      digitalWrite(L1D, OFF);
      l1dFlag = false;
      lv_print("리프트 1             정지...");
    }
    if (currentFlag_1)
      currentFlag_1 = false;
  } else if (division == "L2U") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L2U, ON);
      l2uFlag = true;
      lv_print("리프트     2         상승...");
      delay(holdTime);
    } else {
      digitalWrite(L2U, OFF);
      l2uFlag = false;
      lv_print("리프트     2         정지...");
    }
  } else if (division == "L2D") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L2D, ON);
      l2dFlag = true;
      lv_print("리프트     2         하강...");
    } else {
      digitalWrite(L2D, OFF);
      l2dFlag = false;
      lv_print("리프트     2         정지...");
    }
    if (currentFlag_2)
      currentFlag_2 = false;
  } else if (division == "L3U") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L3U, ON);
      l3uFlag = true;
      lv_print("리프트         3     상승...");
      delay(holdTime);
    } else {
      digitalWrite(L3U, OFF);
      l3uFlag = false;
      lv_print("리프트         3     정지...");
    }
  } else if (division == "L3D") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L3D, ON);
      l3dFlag = true;
      lv_print("리프트         3     하강...");
    } else {
      digitalWrite(L3D, OFF);
      l3dFlag = false;
      lv_print("리프트         3     정지...");
    }
    if (currentFlag_3)
      currentFlag_3 = false;
  } else if (division == "L4U") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L4U, ON);
      l4uFlag = true;
      lv_print("리프트             4 상승...");
      delay(holdTime);
    } else {
      digitalWrite(L4U, OFF);
      l4uFlag = false;
      lv_print("리프트             4 정지...");
    }
  } else if (division == "L4D") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      digitalWrite(L4D, ON);
      l4dFlag = true;
      lv_print("리프트             4 하강...");
    } else {
      digitalWrite(L4D, OFF);
      l4dFlag = false;
      lv_print("리프트             4 정지...");
    }
    if (currentFlag_4)
      currentFlag_4 = false;
  } else if (division == "LAU") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      if (l1dFlag || l2dFlag || l3dFlag || l4dFlag) {
        digitalWrite(L1D, OFF);
        digitalWrite(L2D, OFF);
        digitalWrite(L3D, OFF);
        digitalWrite(L4D, OFF);
        l1dFlag = false;
        l2dFlag = false;
        l3dFlag = false;
        l4dFlag = false;
        delay(200);
      }

      digitalWrite(L1U, ON);
      digitalWrite(L2U, ON);
      digitalWrite(L3U, ON);
      digitalWrite(L4U, ON);
      lauFlag = true;
      l1uFlag = true;
      l2uFlag = true;
      l3uFlag = true;
      l4uFlag = true;
      lv_print("리프트 1 2 3 4 상승...");
      delay(holdTime);
    } else {
      digitalWrite(L1U, OFF);
      digitalWrite(L2U, OFF);
      digitalWrite(L3U, OFF);
      digitalWrite(L4U, OFF);
      lauFlag = false;
      l1uFlag = false;
      l2uFlag = false;
      l3uFlag = false;
      l4uFlag = false;
      lv_print("리프트 1 2 3 4 정지...");
    }
  } else if (division == "LAD") // A type Sensor High 값 수신 처리...
  {
    if (chanel) {
      if (l1uFlag || l2uFlag || l3uFlag || l4uFlag) {
        digitalWrite(L1U, OFF);
        digitalWrite(L2U, OFF);
        digitalWrite(L3U, OFF);
        digitalWrite(L4U, OFF);
        l1uFlag = false;
        l2uFlag = false;
        l3uFlag = false;
        l4uFlag = false;
        delay(200);
      }

      digitalWrite(L1D, ON);
      digitalWrite(L2D, ON);
      digitalWrite(L3D, ON);
      digitalWrite(L4D, ON);
      ladFlag = true;
      l1dFlag = true;
      l2dFlag = true;
      l3dFlag = true;
      l4dFlag = true;
      lv_print("리프트 1 2 3 4 하강...");
    } else {
      digitalWrite(L1D, OFF);
      digitalWrite(L2D, OFF);
      digitalWrite(L3D, OFF);
      digitalWrite(L4D, OFF);
      ladFlag = false;
      l1dFlag = false;
      l2dFlag = false;
      l3dFlag = false;
      l4dFlag = false;
      lv_print("리프트 1 2 3 4 정지...");
    }
    if (currentFlag_1)
      currentFlag_1 = false;
    if (currentFlag_2)
      currentFlag_2 = false;
    if (currentFlag_3)
      currentFlag_3 = false;
    if (currentFlag_4)
      currentFlag_4 = false;
  } else if (division == "wifiSend") {
    String output = "";
    StaticJsonDocument<100> doc;

    if (chanel == 0) { // EEPROM의 SSID 와 PASSWORD 를 전송 한다...

      doc["ssid"] = EEPROM.readString(70); // EEPROM 주소 10부터 SSID를 읽기
      doc["password"] =
          EEPROM.readString(100); // EEPROM 주소 30부터 저장된 비밀번호 읽기
      EEPROM_SSID_STA.trim();
      EEPROM_PSWD_STA.trim();
      serializeJson(doc, output);

      if (serialDebug) {
        Serial.println(F("handleJson()..."));
        Serial.print(F("JSON object Convert : "));
        Serial.println(output);
        Serial.println();
        Serial.print(F("'wifiSend' Receive from AT80X..."));
      }
      s = output;
      server.send(200, "text/plain", s);
    } else if (chanel == 1) { // 전달 받은 SSID 와 PASSWORD 를 표시한다...

      if (serialDebug) {
        ssidBuffer = ssidParse.c_str();
        EEPROM.writeString(70, ssidBuffer); // TABLET SSID EEPROM 저장...
        EEPROM.commit();

        pwBuffer = passwordParse.c_str();
        EEPROM.writeString(100,
                           passwordParse); // TABLET PASSWORD EEPROM 저장...
        EEPROM.commit();
      }

      // if (serialDebug) {
      //   Serial.println(F("handleJson()..."));
      //   Serial.print(F("ssid from Factory : "));
      //   Serial.println(EEPROM.readString(10));
      //   Serial.print(F("password from Factory : "));
      //   Serial.println(EEPROM.readString(40));

      //   Serial.println();
      //   Serial.print(F("ssid from AT80X : "));
      //   Serial.println(EEPROM.readString(70));
      //   Serial.print(F("password from AT80X : "));
      //   Serial.println(EEPROM.readString(100));
      // }
      ledMonitor(10);
      delay(100);
      ledMonitor(10);
    }
  }

  s = "";
  s = s + "{\"result\":" + "\"Receive OK\"}";
  server.send(200, "text/plain", s);
  // m.dataChange = true;

  if (serialDebug) {
    Serial.println();
    Serial.println(F("handleJson()..."));
    Serial.print(F("Sends s : "));
    Serial.print(s);
    Serial.println();
  }
} // handleJson()

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  wiFiCountValue = 0;
  outLockFlag = false;

  server.send(200, "text/plain", "");
  if (serialDebug) {
    Serial.println(F("handleRoot()..."));
  }

  for (int i = 0; i < 5; i++) { // WiFi 수신이 되었음을 알림...
    digitalWrite(Monitor, !i);
    delay(1);
  }
  digitalWrite(Monitor, ON);
}

void lv_print_current(int select) {
  String msg;
  char _text[10];

  if (select == 1) { // Screen 2의 기준 전류값 표시...
    msg = '(';
    msg += String(compareCurrent1, 1);
    msg += "A)";
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refDisp1, _text);

    msg = '(';
    msg += String(compareCurrent2, 1);
    msg += "A)";
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refDisp2, _text);

    msg = '(';
    msg += String(compareCurrent3, 1);
    msg += "A)";
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refDisp3, _text);

    msg = '(';
    msg += String(compareCurrent4, 1);
    msg += "A)";
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refDisp4, _text);

  } else if (select == 2) { // Screen 5의 변경 기준 전류값 표시...
    msg = String(compareCurrent1, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refValue1, _text);
    msg = String(compareCurrent2, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refValue2, _text);
    msg = String(compareCurrent3, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refValue3, _text);
    msg = String(compareCurrent4, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_refValue4, _text);
  } else if (select == 3) { // Screen 2의 현재 전류 표시...
    msg = String(current1, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_liftValue1, _text);
    msg = String(current2, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_liftValue2, _text);
    msg = String(current3, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_liftValue3, _text);
    msg = String(current4, 1);
    msg.toCharArray(_text, sizeof(_text));
    lv_label_set_text(ui_liftValue4, _text);
  }
}

void screenLoad(int pageCount) {
  switch (pageCount) {
  case 2:
    lv_disp_load_scr(ui_Screen2);
    break;
  case 3:
    lv_disp_load_scr(ui_Screen3);
    break;
  case 4:
    lv_disp_load_scr(ui_Screen4);
    break;
  case 5:
    lv_disp_load_scr(ui_Screen5);
    break;
  case 6:
    lv_disp_load_scr(ui_Screen6);
    break;
  default:
    break;
  }
}

// Screen 4의 Flag 상태 Load...
void menu1Load(int menu1Count) {

  lv_obj_set_style_outline_width(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Button1, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(ui_Button2, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  switch (menu1Count) {
  case 1: // Screen 4의 리프트 초기화 실행 Panel이면 실행...
    lv_obj_set_style_outline_width(ui_Panel1, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel1, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    break;
  case 2: // Screen 4의 수동/자동 모드 선택 Panel이면 실행...

    lv_obj_set_style_outline_width(ui_Panel2, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel2, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    break;
  case 3: // Screen 4의 단독/CPU 모드 선택 Panel이면 실행...

    lv_obj_set_style_outline_width(ui_Panel3, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel3, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 4: // Screen 4의 확인 버튼 Panel이면 실행...
    lv_obj_set_style_outline_width(ui_Button1, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button1, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 5: // Screen 4의 취소 버튼 Panel이면 실행...
    lv_obj_set_style_outline_width(ui_Button2, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button2, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  default:
    break;
  }
}

void menu2Load(int menu2Count) {
  lv_obj_set_style_outline_width(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Panel8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Panel8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Button3, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Button4, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  switch (menu2Count) {
  case 1:
    lv_obj_set_style_outline_width(ui_Panel5, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel5, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 2:
    lv_obj_set_style_outline_width(ui_Panel6, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel6, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 3:
    lv_obj_set_style_outline_width(ui_Panel7, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel7, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 4:
    lv_obj_set_style_outline_width(ui_Panel8, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Panel8, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 5:
    lv_obj_set_style_outline_width(ui_Button3, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button3, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 6:
    lv_obj_set_style_outline_width(ui_Button4, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button4, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  default:
    break;
  }
}

void menu6Load(int menuCount) {
  lv_obj_set_style_outline_width(ui_Button5, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Button6, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_outline_width(ui_Button7, 0,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(ui_Button7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  switch (menuCount) {
  case 1:
    lv_obj_set_style_outline_width(ui_Button5, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button5, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 2:
    lv_obj_set_style_outline_width(ui_Button6, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button6, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 3:
    lv_obj_set_style_outline_width(ui_Button7, 2,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_Button7, 4,
                                 LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  default:
    break;
  }
}

void avrSave() {
  delay(500);
  if (!digitalRead(FACTORY)) {
    delay(50);
  }
  EEPROM.writeFloat(150, avrCurrent1);
  EEPROM.writeFloat(160, avrCurrent2);
  EEPROM.writeFloat(170, avrCurrent3);
  EEPROM.writeFloat(180, avrCurrent4);
  EEPROM.commit();
  ledMonitor(20);
  Serial.println();
  Serial.println(F("평균값 EEPROM 저장 완료...."));
  lv_print("평균값 EEPROM 저장 완료....");
  lv_print("전원을 OFF -> ON 하여 주세요...");
  delay(1000);
  ads1Offset = avrCurrent1;
  ads2Offset = avrCurrent2;
  ads3Offset = avrCurrent3;
  ads4Offset = avrCurrent4;
  avrFlag = false;
}

void avrExecution() { // 리프트의 무부하시 평균 전류값 산출...
  String msg;
  // float avrCurrent1 = 0.0;
  // float avrCurrent2 = 0.0;
  // float avrCurrent3 = 0.0;
  // float avrCurrent4 = 0.0;
  lv_textarea_set_text(ui_TextArea1, " ");

  for (int i = 0; i < 50; i++) {
    lv_timer_handler();
    // current1 = abs((ads.readADC(0) * 0.000394300) * 5);
    // current2 = abs((ads.readADC(1) * 0.000394300) * 5);
    // current3 = abs((ads.readADC(2) * 0.000394300) * 5);
    // current4 = abs((ads.readADC(3) * 0.000394300) * 5);
    current1 = abs((ads.readADC(0) * 0.000390625) * 5);
    current2 = abs((ads.readADC(1) * 0.000390625) * 5);
    current3 = abs((ads.readADC(2) * 0.000390625) * 5);
    current4 = abs((ads.readADC(3) * 0.000390625) * 5);

    avrCurrent1 += current1;
    avrCurrent2 += current2;
    avrCurrent3 += current3;
    avrCurrent4 += current4;

    Serial.println();
    Serial.print(F("평균값 : "));
    Serial.print(i + 1);
    Serial.println(F("-----------------"));
    Serial.print(F("ads1Offset = "));
    Serial.print(current1);
    Serial.println(";");

    Serial.print(F("ads2Offset = "));
    Serial.print(current2);
    Serial.println(";");

    Serial.print(F("ads3Offset = "));
    Serial.print(current3);
    Serial.println(";");

    Serial.print(F("ads4Offset = "));
    Serial.print(current4);
    Serial.println(";");

    msg = "평균값 : ";
    msg += i + 1;
    msg += " -----------------";
    lv_print(msg);
    delay(100);

    ledMonitor(2);
  }

  avrCurrent1 = (avrCurrent1 / 50);
  avrCurrent2 = (avrCurrent2 / 50);
  avrCurrent3 = (avrCurrent3 / 50);
  avrCurrent4 = (avrCurrent4 / 50);

  Serial.println();
  Serial.print(F("avrCurrent1 = "));
  Serial.print(avrCurrent1);
  Serial.println(";");

  Serial.print(F("avrCurrent2 = "));
  Serial.print(avrCurrent2);
  Serial.println(";");

  Serial.print(F("avrCurrent3 = "));
  Serial.print(avrCurrent3);
  Serial.println(";");

  Serial.print(F("avrCurrent4 = "));
  Serial.print(avrCurrent4);
  Serial.println(";");

  msg = F("1번 평균값 : ");
  msg += String(avrCurrent1, 1);
  msg += F(", 3번 평균값 : ");
  msg += String(avrCurrent3, 1);
  lv_print(msg);
  msg = F("2번 평균값 : ");
  msg += String(avrCurrent2, 1);
  msg += F(", 4번 평균값 : ");
  msg += String(avrCurrent4, 1);
  lv_print(msg);

  avrFlag = true;
}

void buttonInput() {
  if (!digitalRead(SW_LEFT)) {
    // lv_textarea_add_text(ui_TextArea1, "왼쪽 스위치 눌림...\n");
    if (!curChgFlag_1 && !curChgFlag_2 && !curChgFlag_3 && !curChgFlag_4) {
      while (!digitalRead(SW_LEFT))
        ;
      if (pageCount == SCREEN4) {
        menu11Flag = false;
        lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
        menu12FlagPush = menu12Flag;
        menu13FlagPush = menu13Flag;
        if (menu12FlagPush) {
          lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
          lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
        } else {
          lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
          lv_obj_add_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
        }
        if (menu13FlagPush) {
          lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
        } else
          lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
      }
      if (pageCount == SCREEN2)
        pageCount = SCREEN5;
      else
        --pageCount;
      screenLoad(pageCount);
    }
  }
  if (!digitalRead(SW_RIGHT)) {
    // lv_textarea_add_text(ui_TextArea1, "오른쪽 스위치 눌림...\n");
    if (pageCount == SCREEN4) {
      menu11Flag = false;
      lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
      menu12FlagPush = menu12Flag;
      menu13FlagPush = menu13Flag;
      if (menu12FlagPush) {
        lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
        lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
      } else {
        lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
        lv_obj_add_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
      }
      if (menu13FlagPush) {
        lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
      } else
        lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
    }
    if (!curChgFlag_1 && !curChgFlag_2 && !curChgFlag_3 && !curChgFlag_4) {
      while (!digitalRead(SW_RIGHT))
        ;
      if (pageCount == SCREEN5)
        pageCount = SCREEN2;
      else
        ++pageCount;
      screenLoad(pageCount);
    }
  }
  if (!digitalRead(SW_UP)) {
    // lv_textarea_add_text(ui_TextArea1, "위쪽 스위치 눌림...\n");
    while (!digitalRead(SW_UP))
      ;

    if (pageCount == SCREEN3) {
      opaValue -= 10;
      if (opaValue <= 0)
        opaValue = 250;
      lv_obj_set_style_img_recolor_opa(ui_Image2, opaValue,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (!curChgFlag_1 && !curChgFlag_2 && !curChgFlag_3 && !curChgFlag_4) {
      if (pageCount == SCREEN4) {
        if (menu1Count == 1 || menu1Count == 0)
          menu1Count = 5;
        else
          --menu1Count;
        menu1Load(menu1Count);
        Serial.print(F("pageCount: "));
        Serial.print(pageCount);
        Serial.print(F(", menu1Count: "));
        Serial.println(menu1Count);
      } else if (pageCount == SCREEN5) {
        if (menu2Count == 1 || menu2Count == 0)
          menu2Count = 6;
        else
          --menu2Count;
        menu2Load(menu2Count);
        Serial.print(F("pageCount: "));
        Serial.print(pageCount);
        Serial.print(F(", menu2Count: "));
        Serial.println(menu2Count);
      }
    } else { // 전류값을 변경 중 이면 실행...
      curSaveFlag = true;
      if (curChgFlag_1) {
        compareCurrent1 += 0.1;
        if (compareCurrent1 >= 9.9)
          compareCurrent1 = 1.0;
      }
      if (curChgFlag_2) {
        compareCurrent2 += 0.1;
        if (compareCurrent2 >= 9.9)
          compareCurrent2 = 1.0;
      }
      if (curChgFlag_3) {
        compareCurrent3 += 0.1;
        if (compareCurrent3 >= 9.9)
          compareCurrent3 = 1.0;
      }
      if (curChgFlag_4) {
        compareCurrent4 += 0.1;
        if (compareCurrent4 >= 9.9)
          compareCurrent4 = 1.0;
      }
      lv_print_current(2);
    }
    if (pageCount == SCREEN6) {
      if (menu6Count == 0)
        menu6Count = 3;
      else
        --menu6Count;
      menu6Load(menu6Count);
    }
  }

  if (!digitalRead(SW_DOWN)) {
    // lv_textarea_add_text(ui_TextArea1, "아래쪽 스위치 눌림...\n");
    while (!digitalRead(SW_DOWN))
      ;

    if (pageCount == SCREEN3) {
      opaValue += 10;
      if (opaValue >= 250)
        opaValue = 0;
      lv_obj_set_style_img_recolor_opa(ui_Image2, opaValue,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (!curChgFlag_1 && !curChgFlag_2 && !curChgFlag_3 && !curChgFlag_4) {
      if (pageCount == SCREEN4) {
        if (menu1Count == 5 || menu1Count == 0)
          menu1Count = 1;
        else
          ++menu1Count;
        menu1Load(menu1Count);
        Serial.print(F("pageCount: "));
        Serial.print(pageCount);
        Serial.print(F(", menu1Count: "));
        Serial.println(menu1Count);
      } else if (pageCount == SCREEN5) {
        if (menu2Count == 6 || menu2Count == 0)
          menu2Count = 1;
        else
          ++menu2Count;
        menu2Load(menu2Count);
        Serial.print(F("pageCount: "));
        Serial.print(pageCount);
        Serial.print(F(", menu2Count: "));
        Serial.println(menu2Count);
      }
    } else { // 전류값을 변경 중 이면 실행...
      curSaveFlag = true;
      if (curChgFlag_1) {
        compareCurrent1 -= 0.1;
        if (compareCurrent1 <= 0.0)
          compareCurrent1 = 1.0;
      }
      if (curChgFlag_2) {
        compareCurrent2 -= 0.1;
        if (compareCurrent2 <= 0.0)
          compareCurrent2 = 1.0;
      }
      if (curChgFlag_3) {
        compareCurrent3 -= 0.1;
        if (compareCurrent3 <= 0.0)
          compareCurrent3 = 1.0;
      }
      if (curChgFlag_4) {
        compareCurrent4 -= 0.1;
        if (compareCurrent4 <= 0.0)
          compareCurrent4 = 1.0;
      }
      lv_print_current(2);
    }
    if (pageCount == SCREEN6) {
      if (menu6Count == 4)
        menu6Count = 0;
      ++menu6Count;
      menu6Load(menu6Count);
    }
  }
  if (!digitalRead(SW_ENTER)) {
    // lv_textarea_add_text(ui_TextArea1, "엔터 스위치 눌림...\n");
    while (!digitalRead(SW_ENTER))
      ;
    if (pageCount == SCREEN2 && avrFlag) {
      avrFlag = false;
      avrSave(); // 무부하 평균 전류값 저장...
    }
    if (pageCount == SCREEN4) {
      switch (menu1Count) {
      case 1: // Screen 4의 리프트 최기화 Panel...
        if (!menu11Flag) {
          menu11Flag = true;
          lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
        } else {
          menu11Flag = false;
          lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
        }
        break;
      case 2: // Screen 4의 수동, 자동 모드 Toggle Panel...
        if (!menu12FlagPush) {
          menu12FlagPush = true;
          lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
          lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
        } else {
          lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
          lv_obj_add_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
          menu12FlagPush = false;
        }
        Serial.print(F("Manual/Auto Mode : "));
        Serial.println(menu12FlagPush);

        break;
      case 3: // Screen 4의 단독, CPU 모드 Toggle Panel...
        if (!menu13FlagPush) {
          menu13FlagPush = true;
          lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
        } else {
          menu13FlagPush = false;
          lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
        }
        break;
      case 4: // Screen 4의 확인 버튼에서 엔터를 누르면 실행...
        pageCount = SCREEN2;
        screenLoad(SCREEN2);
        if (lauFlag || l1uFlag || l2uFlag || l3uFlag || l4uFlag)
          lv_print(F("리프트가 동작중 입니다..."));
        else {
          Serial.print(F("변경된 내용을 저장합니다."));
          lv_print(F("변경된 내용을 저장 했습니다."));
          lv_print(F("리프트 재 기동 후 적용됩니다."));
          lv_timer_handler();
          delay(3500);
          if ((menu12Flag != menu12FlagPush) ||
              (menu13Flag != menu13FlagPush)) {
            if (menu12Flag != menu12FlagPush) {
              menu12Flag = menu12FlagPush;
              EEPROM.writeBool(8, menu12FlagPush ? 1 : 0);
              EEPROM.commit();
            } else if (menu13Flag != menu13FlagPush) {
              menu13Flag = menu13FlagPush;
              EEPROM.writeBool(9, menu13FlagPush ? 1 : 0);
              EEPROM.commit();
            }
            ESP.restart();
          }
          if (menu11Flag) {
            menu11Flag = false;
            lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
            avrExecution();
            lv_print("저장 : 스위치 또는 엔터키 누름.");
          }
        }

        break;
      case 5: // 취소 버튼에서 엔터를 누르면 실행...
        if (pageCount == SCREEN4) {
          menu12FlagPush = menu12Flag;
          menu13FlagPush = menu13Flag;
          if (menu12FlagPush) {
            lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
            lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
          } else {
            lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
            lv_obj_add_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
          }
          if (menu13FlagPush) {
            lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
          } else
            lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
          menu11Flag = false;
          lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
        }
        pageCount = SCREEN2;
        screenLoad(SCREEN2);
        break;
      default:
        break;
      }
    } else if (pageCount == SCREEN5) {
      switch (menu2Count) {
      case 1:
        if (!curChgFlag_1) {
          curChgFlag_1 = true;
          lv_obj_set_style_outline_width(ui_refValue1, 2,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue1, 4,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);

        } else {
          curChgFlag_1 = false;
          lv_obj_set_style_outline_width(ui_refValue1, 0,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue1, 0,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
      case 2:
        if (!curChgFlag_2) {
          curChgFlag_2 = true;
          lv_obj_set_style_outline_width(ui_refValue2, 2,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue2, 4,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
          curChgFlag_2 = false;
          lv_obj_set_style_outline_width(ui_refValue2, 0,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue2, 0,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
      case 3:
        if (!curChgFlag_3) {
          curChgFlag_3 = true;
          lv_obj_set_style_outline_width(ui_refValue3, 2,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue3, 4,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
          curChgFlag_3 = false;
          lv_obj_set_style_outline_width(ui_refValue3, 0,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue3, 0,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
      case 4:
        if (!curChgFlag_4) {
          curChgFlag_4 = true;
          lv_obj_set_style_outline_width(ui_refValue4, 2,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue4, 4,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
          curChgFlag_4 = false;
          lv_obj_set_style_outline_width(ui_refValue4, 0,
                                         LV_PART_MAIN | LV_STATE_DEFAULT);
          lv_obj_set_style_outline_pad(ui_refValue4, 0,
                                       LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
      case 5:
        pageCount = SCREEN2;
        screenLoad(SCREEN2);
        lv_print_current(1);

        if (curSaveFlag) {
          EEPROM.writeFloat(210, compareCurrent1);
          EEPROM.writeFloat(220, compareCurrent2);
          EEPROM.writeFloat(230, compareCurrent3);
          EEPROM.writeFloat(240, compareCurrent4);
          EEPROM.commit();
          curSaveFlag = false;
          lv_print("변경된 전류값이 저장 되었습니다.");
        } else
          lv_print("변경된 전류값이 없습니다.");
        break;
      case 6:
        pageCount = SCREEN2;
        screenLoad(SCREEN2);
        break;
      default:
        break;
      }
    } else if (pageCount == SCREEN2 && !digitalRead(SW_DOWN)) {
      Serial.print("Text area pageCount : ");
      Serial.println(pageCount);
      lv_textarea_set_text(ui_TextArea1, " ");
    } else if (pageCount == SCREEN6) {
      if (menu6Count == 1) {
        Serial.println("연결을 눌렀습니다.");
        lv_label_set_text(ui_Label27, "CamTrol Factory 로 연결중 입니다.");
        String s = "";
        char _text[30];
        int loopCount = 0;
        WiFi.mode(WIFI_MODE_STA);
        WiFi.begin("CamTrol Factory",
                   "1234567890"); // Access Point mode begin...
        while (WiFi.status() != WL_CONNECTED) {
          delay(5);
          lv_timer_handler();
          // buttonInput();
          loopCount++;
          if (loopCount > 300) {
            s += '.';
            Serial.print(".");
            s.toCharArray(_text, sizeof(_text));
            lv_label_set_text(ui_LabelConnect, _text);
            loopCount = 0;
            // ledMonitor(2);
          }
          // s = "";
          // lv_label_set_text(ui_LabelConnect, s);
        }
        lv_label_set_text(ui_Label27, "CamTrol Factory 로 연결되었습니다.");
        menu6Count++;
        menu6Load(menu6Count);
      } else if (menu6Count == 2) {
        Serial.println("읽기를 눌렀습니다.");

        WiFiClient client;
        HTTPClient http;
        http.begin(client, "http://192.168.4.1/NetWork");
        int httpResponseCode = http.GET();
        String payload = "--";
        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          payload = http.getString();
        } else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
        Serial.print("Pay Load: ");
        Serial.println(payload);
        // Free resources
        http.end();

        // int split = payload.indexOf(","); // 첫 번째 콤마 위치
        // int length = payload.length();    // 문자열 길이

        String ssid =
            payload.substring(0, payload.indexOf(",")); // 첫 번째 토큰 (0, 3)
        String pswd = payload.substring(
            payload.indexOf(",") + 1, payload.length()); // 세 번째 토큰(8, 10)

        EEPROM_SSID_STA = ssid;
        EEPROM_PSWD_STA = pswd;
        char _text[50];
        ssid.toCharArray(_text, sizeof(_text));
        lv_label_set_text(ui_LabelConnect, _text);
        pswd.toCharArray(_text, sizeof(_text));
        lv_label_set_text(ui_LabelPW, _text);

        Serial.print("SSID: ");
        Serial.print(ssid);
        Serial.print(", PSWD: ");
        Serial.println(pswd);

      } else if (menu6Count == 3) {
        EEPROM.writeString(70, EEPROM_SSID_STA);
        EEPROM.commit();
        EEPROM.writeString(100, EEPROM_PSWD_STA);
        EEPROM.commit();
        lv_label_set_text(ui_Label22, "- 네트워크 저장됨 -");
        Serial.println("저장을 눌렀습니다.");
      }
    }
  }
}

void wifiStart() {

  String msg;

  if (wiFiMode == AP_MODE) { // SOLO 또는 단독 Mode 이면 실행...

    Serial.println();
    Serial.print(F("EEPROM SSID Factory : "));
    Serial.println(ssidAp);
    Serial.print(F("EEPROM PASSWORD Factory : "));
    Serial.println(passwordAp);
    msg = "SSID: ";
    msg += ssidAp;
    lv_print(msg);
    msg = "PSWD: ";
    msg += passwordAp;
    lv_print(msg);

    WiFi.mode(WIFI_MODE_AP);
    // Serial.println(F("setupWifi()..."));
    Serial.println(F("Access Point(WiFi) Mode Start..."));
    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAP(ssidAp, passwordAp); // Access Point mode begin...
    WiFi.softAPConfig(local_ip, gateway, subnet);
    Serial.println();
    Serial.print(F("ESP32 IP as soft AP : "));
    Serial.println(WiFi.softAPIP());
    // msg = "Address : ";
    // msg += WiFi.softAPIP().toString();
    // lv_print(msg);
  } else { // CPU Mode (Station Mode) 이면 실행...
    int loopCount = 0;
    // int ledFlag = 0;
    Serial.println(F("stationConnect()..."));
    Serial.println(F("Connecting to Station(Hot Spot)..."));
    Serial.print(F("WiFi Status : "));
    Serial.println(WiFi.status());
    msg = "SSID: ";
    msg += EEPROM_SSID_STA;
    lv_print(msg);
    msg = "PSWD: ";
    msg += EEPROM_PSWD_STA;
    lv_print(msg);

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(EEPROM_SSID_STA, EEPROM_PSWD_STA);
    Serial.println(F("Station(WiFi) Mode Start..."));

    while (WiFi.status() != WL_CONNECTED) {
      delay(5);
      lv_timer_handler();
      buttonInput();
      nexLoop(nex_listen_list);
      loopCount++;
      if (loopCount > 300) {
        Serial.print(".");
        lv_textarea_add_char(ui_TextArea1, '.');
        loopCount = 0;
        ledMonitor(2);
      }
    }
    digitalWrite(Monitor, ON);

    Serial.println(F("stationConnect()..."));
    Serial.println(F("Station(Hot Spot) Connected OK..."));
    Serial.print(F("WiFi Status : "));
    Serial.println(WiFi.status());
    lv_print("\n정상연결 되었습니다.");
  }
}

void wIfIConnection() {
  nexLoop(nex_listen_list);

  if (wiFiCountValue > 1) {

    // Mobile에서 주기적인 통신이 없으면 깜박임...
    digitalWrite(Monitor, !wifiStatus);
    wifiStatus = !wifiStatus;

    Serial.println();
    Serial.println(F("wIfIConnection()..."));
    Serial.print(F("WiFi status LED : "));
    Serial.println(wifiStatus);
  }
  // }

  if (wiFiCountValue > 160) { // 5분 20초 마다 실행...
    wiFiCountValue = 1;
    outLockFlag = true;
  }

  if (outLockFlag) {
    digitalWrite(L1U, OFF);
    digitalWrite(L1D, OFF);
    digitalWrite(L2U, OFF);
    digitalWrite(L2D, OFF);

    digitalWrite(L3U, OFF);
    digitalWrite(L3D, OFF);
    digitalWrite(L4U, OFF);
    digitalWrite(L4D, OFF);

    l1uFlag = false;
    l1dFlag = false;
    l2uFlag = false;
    l2dFlag = false;

    l3uFlag = false;
    l3dFlag = false;
    l4uFlag = false;
    l4dFlag = false;

    lauFlag = false;
    ladFlag = false;
  }

  // Serial.println(F("wifiConnection()..."));
  // Serial.print(F("WiFi Count Value : "));
  // Serial.print(wiFiCountValue);
  // Serial.print(F(", OutLockFlag : "));
  // Serial.println(outLockFlag ? "True" : "False");

  nexLoop(nex_listen_list);

  if (!digitalRead(FACTORY)) {
    Serial.println();
    Serial.print(F("네트워크 변경..."));
    delay(1000);
    EEPROM_SSID = "CamTrol Factory";
    EEPROM_PSWD = "1234567890";
    EEPROM_SSID.toCharArray(ssidAp, EEPROM_SSID.length() + 1);
    EEPROM_PSWD.toCharArray(passwordAp, EEPROM_PSWD.length() + 1);
    delay(2000);
    Serial.println();
    Serial.print(F("SSID : "));
    Serial.print(ssidAp);
    Serial.print(F(",  P/W : "));
    Serial.println(passwordAp);
    WiFi.begin(ssidAp, passwordAp);
    ledMonitor(20);
    delay(3000);
    networkChangeFlag = true;
  }

  Serial.println();
  Serial.println();
  Serial.print(F("WiFi Status : "));
  Serial.println(WiFi.status());
  Serial.print(F("current1 : "));
  Serial.println(current1);
  Serial.print(F("current2 : "));
  Serial.println(current2);
  Serial.print(F("current3 : "));
  Serial.println(current3);
  Serial.print(F("current4 : "));
  Serial.println(current4);
  Serial.println();

  wiFiCountValue++;

  s = "";
  s = s + "{" + "\"LiftFlag\":" + true + ", " + "\"serialDebug\":" +
      serialDebug + ", " + // Factory Mode 이면 Serial debug true...
      "\"L1U_Flag\":" + l1uFlag + ", " + // Lift 1 up Bool...
      "\"L1D_Flag\":" + l1dFlag + ", " + // Lift 1 down Bool...
      "\"L2U_Flag\":" + l2uFlag + ", " + // Lift 2 up Bool...
      "\"L2D_Flag\":" + l2dFlag + ", " + // Lift 2 down Bool...
      "\"L3U_Flag\":" + l3uFlag + ", " + // Lift 3 up Bool...
      "\"L3D_Flag\":" + l3dFlag + ", " + // Lift 3 down Bool...
      "\"L4U_Flag\":" + l4uFlag + ", " + // Lift 4 up Bool...
      "\"L4D_Flag\":" + l4dFlag + ", " + // Lift 4 down Bool...
      "\"LAU_Flag\":" + lauFlag + ", " + // Lift all up Bool...
      "\"LAD_Flag\":" + ladFlag +        // Lift all down Bool...
      // "\"CURRENT1\":" + current1 + ", " +   // Lift all down float...
      // "\"CURRENT2\":" + current2 + ", " +   // Lift all down float...
      // "\"CURRENT3\":" + current3 + ", " +   // Lift all down float...
      // "\"CURRENT4\":" + current4 +          // Lift all down float...
      "}";

  if (networkChangeFlag)
    ledMonitor(5); // Monitor LED 5회 점멸...
  else
    ledMonitor(2); // Monitor LED 2회 점멸...
}

static LGFX tft; // LGFXのインスタンスを作成。

static const uint16_t screenWidth = 320;  // 172, 240
static const uint16_t screenHeight = 172; // 320, 280

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

void serialEvent2() {

  eventString = Serial2.readStringUntil('\n');

  // char _text[10];
  // int index1 = eventString.indexOf(':');
  // int index2 = eventString.length();
  // String _division = eventString.substring(0, index1);
  // String _flag = eventString.substring(index1 + 1, index2);
  // bool _value;
  // _flag.toCharArray(_text, sizeof(_text));

  // _value = (_text[0] == '1') ? true : false;

  Serial.println("콘트롤러 수신 OK...!");
  Serial.print("eventString: ");
  Serial.println(eventString);

  // Serial.print("division: ");
  // Serial.print(_division);
  // Serial.print(", ValueChar[0]: ");
  // Serial.print(_text[0]);
  // Serial.print(", ValueChar[1]: ");
  // Serial.println(_text[1]);

  char _convText[100];

  eventString.toCharArray(_convText, sizeof(_convText));
  deserializeJson(doc, _convText);

  String _division = doc["division"];
  bool _value = doc["Value"];

  if (_division == "L1U") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L1U, ON);
      l1uFlag = true;
      lv_print("리프트 1             상승...");
      delay(holdTime);
    } else {
      digitalWrite(L1U, OFF);
      l1uFlag = false;
      lv_print("리프트 1             정지...");
    }
  } else if (_division == "L1D") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L1D, ON);
      l1dFlag = true;
      lv_print("리프트 1             하강...");
    } else {
      digitalWrite(L1D, OFF);
      l1dFlag = false;
      lv_print("리프트 1             정지...");
    }
    if (currentFlag_1)
      currentFlag_1 = false;
  } else if (_division == "L2U") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L2U, ON);
      l2uFlag = true;
      lv_print("리프트     2         상승...");
      delay(holdTime);
    } else {
      digitalWrite(L2U, OFF);
      l2uFlag = false;
      lv_print("리프트     2         정지...");
    }
  } else if (_division == "L2D") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L2D, ON);
      l2dFlag = true;
      lv_print("리프트     2         하강...");
    } else {
      digitalWrite(L2D, OFF);
      l2dFlag = false;
      lv_print("리프트     2         정지...");
    }
    if (currentFlag_2)
      currentFlag_2 = false;
  } else if (_division == "L3U") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L3U, ON);
      l3uFlag = true;
      lv_print("리프트         3     상승...");
      delay(holdTime);
    } else {
      digitalWrite(L3U, OFF);
      l3uFlag = false;
      lv_print("리프트         3     정지...");
    }
  } else if (_division == "L3D") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L3D, ON);
      l3dFlag = true;
      lv_print("리프트         3     하강...");
    } else {
      digitalWrite(L3D, OFF);
      l3dFlag = false;
      lv_print("리프트         3     정지...");
    }
    if (currentFlag_3)
      currentFlag_3 = false;
  } else if (_division == "L4U") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L4U, ON);
      l4uFlag = true;
      lv_print("리프트             4 상승...");
      delay(holdTime);
    } else {
      digitalWrite(L4U, OFF);
      l4uFlag = false;
      lv_print("리프트             4 정지...");
    }
  } else if (_division == "L4D") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      digitalWrite(L4D, ON);
      l4dFlag = true;
      lv_print("리프트             4 하강...");
    } else {
      digitalWrite(L4D, OFF);
      l4dFlag = false;
      lv_print("리프트             4 정지...");
    }
    if (currentFlag_4)
      currentFlag_4 = false;
  } else if (_division == "LAU") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      if (l1dFlag || l2dFlag || l3dFlag || l4dFlag) {
        digitalWrite(L1D, OFF);
        digitalWrite(L2D, OFF);
        digitalWrite(L3D, OFF);
        digitalWrite(L4D, OFF);
        l1dFlag = false;
        l2dFlag = false;
        l3dFlag = false;
        l4dFlag = false;
        delay(200);
      }

      digitalWrite(L1U, ON);
      digitalWrite(L2U, ON);
      digitalWrite(L3U, ON);
      digitalWrite(L4U, ON);
      lauFlag = true;
      l1uFlag = true;
      l2uFlag = true;
      l3uFlag = true;
      l4uFlag = true;
      lv_print("리프트 1 2 3 4 상승...");
      delay(holdTime);
    } else {
      digitalWrite(L1U, OFF);
      digitalWrite(L2U, OFF);
      digitalWrite(L3U, OFF);
      digitalWrite(L4U, OFF);
      lauFlag = false;
      l1uFlag = false;
      l2uFlag = false;
      l3uFlag = false;
      l4uFlag = false;
      lv_print("리프트 1 2 3 4 정지...");
    }
  } else if (_division == "LAD") // A type Sensor High 값 수신 처리...
  {
    if (_value) {
      if (l1uFlag || l2uFlag || l3uFlag || l4uFlag) {
        digitalWrite(L1U, OFF);
        digitalWrite(L2U, OFF);
        digitalWrite(L3U, OFF);
        digitalWrite(L4U, OFF);
        l1uFlag = false;
        l2uFlag = false;
        l3uFlag = false;
        l4uFlag = false;
        delay(200);
      }
      digitalWrite(L1D, ON);
      digitalWrite(L2D, ON);
      digitalWrite(L3D, ON);
      digitalWrite(L4D, ON);
      ladFlag = true;
      l1dFlag = true;
      l2dFlag = true;
      l3dFlag = true;
      l4dFlag = true;
      lv_print("리프트 1 2 3 4 하강...");
    } else {
      digitalWrite(L1D, OFF);
      digitalWrite(L2D, OFF);
      digitalWrite(L3D, OFF);
      digitalWrite(L4D, OFF);
      ladFlag = false;
      l1dFlag = false;
      l2dFlag = false;
      l3dFlag = false;
      l4dFlag = false;
      lv_print("리프트 1 2 3 4 정지...");
    }
    if (currentFlag_1)
      currentFlag_1 = false;
    if (currentFlag_2)
      currentFlag_2 = false;
    if (currentFlag_3)
      currentFlag_3 = false;
    if (currentFlag_4)
      currentFlag_4 = false;
  }

  eventString = "";
}

void setup() {

  pinMode(FACTORY, INPUT_PULLUP);
  // pinMode(WIFI, INPUT);
  pinMode(SW_LEFT, INPUT_PULLUP);  // AI-1
  pinMode(SW_RIGHT, INPUT_PULLUP); // AI-2
  pinMode(SW_UP, INPUT_PULLUP);    // AI-3
  pinMode(SW_DOWN, INPUT_PULLUP);  // AI-4
  pinMode(SW_ENTER, INPUT_PULLUP); // AI-4

  pinMode(BOOT, INPUT);

  pinMode(ENABLE, OUTPUT);
  pinMode(41, OUTPUT);

  pinMode(AI_1, INPUT); // Analog In 1
  pinMode(AI_2, INPUT); // Analog In 2
  pinMode(AI_3, INPUT); // Analog In 3
  pinMode(AI_4, INPUT); // Analog In 4

  pinMode(L1U, OUTPUT); // LEG 1 UP
  pinMode(L1D, OUTPUT); // LEG 1 DOWN
  pinMode(L2U, OUTPUT); // LEG 2 UP
  pinMode(L2D, OUTPUT); // LEG 2 DOWN

  pinMode(L3U, OUTPUT); // LEG 3 UP
  pinMode(L3D, OUTPUT); // LEG 3 DOWN
  pinMode(L4U, OUTPUT); // LEG 4 UP
  pinMode(L4D, OUTPUT); // LEG 4 DOWN

  digitalWrite(41, LOW);

  digitalWrite(L1U, LOW);
  digitalWrite(L1D, LOW);
  digitalWrite(L2U, LOW);
  digitalWrite(L2D, LOW);

  digitalWrite(L3U, LOW);
  digitalWrite(L3D, LOW);
  digitalWrite(L4U, LOW);
  digitalWrite(L4D, LOW);

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
  lv_timer_handler();

  pinMode(Monitor, OUTPUT);

  // digitalWrite(L1U, HIGH);
  // digitalWrite(L1D, HIGH);
  // digitalWrite(L2U, HIGH);
  // digitalWrite(L2D, HIGH);

  // digitalWrite(L3U, HIGH);
  // digitalWrite(L3D, HIGH);
  // digitalWrite(L4U, HIGH);
  // digitalWrite(L4D, HIGH);

  digitalWrite(ENABLE, LOW); // RS485 수신대기모드...

  EEPROM.begin(250);
  Wire.begin(42, 2);
  Serial.begin(115200);
  // lift.begin(19200, SERIAL_8N1, 36, 37);
  Serial2.begin(19200, SERIAL_8N1, 36, 37);
  delay(100);

  // lv_label_set_text(ui_Label1, ssidAp);
  // lv_label_set_text(ui_Label2, passwordAp);

  while (ads.begin() != true) {
    Serial.println(F("Could not find ADS1115 Check wiring"));
    lv_print("아나로그 회로 점검 요망...");
    delay(100);
    if (count > 5)
      break;
    count++;
  }
  Serial.println("ADS1115 found");
  lv_print("아나로그 회로 정상...");

  if (EEPROM.readBool(8)) { // Screen 4의 수동/자동 모드 Flag...
    menu12Flag = true;
    lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED);
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED);
    lv_obj_add_flag(ui_Panel3, LV_OBJ_FLAG_HIDDEN);
    menu12Flag = false;
  }

  /* Screen 4의 수동/자동 모드 Flag 와
     Screen 4의 단독/CPU 모드 Flag 모두 True 이면...  */
  if (EEPROM.readBool(9)) {
    menu13Flag = true;
    lv_obj_add_state(ui_Switch3, LV_STATE_CHECKED);
  } else {
    menu13Flag = false;
    lv_obj_clear_state(ui_Switch3, LV_STATE_CHECKED);
  }

  menu12FlagPush = menu12Flag;
  menu13FlagPush = menu13Flag;

  if (menu12Flag) {    // Screen 4의 자동모드 선택이면 실행...
    if (!menu13Flag) { // Screen 4의 수동모드 선택이면 실행...
      wiFiMode = AP_MODE;
      lv_print("자동-단독 모드로 리프트 시작...");
    } else { // Screen 4의 CPU모드 선택이면 실행...
      wiFiMode = STATION_MODE;
      lv_print("자동-CPU 모드로 리프트 시작...");
    }
  } else { // Screen 4의 수동모드 선택이면 실행...
    if (digitalRead(BOOT) == true) {
      wiFiMode = AP_MODE;
      lv_print("수동-단독모드로 리프트 시작...");
    } else {
      wiFiMode = STATION_MODE;
      lv_print("수동-CPU모드로 리프트 시작...");
    }
  }

  String LVGL_Arduino = "LVGL Version! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("리프트 모듈 시작...");
  lv_print("리프트 모듈 시작...");

  t2.attachPush(t2PushCallback, &t2);
  t3.attachPush(t3PushCallback, &t3);

  idsave.attachPush(idSavePushCallback, &idsave);
  pwsave.attachPush(pwSavePushCallback, &pwsave);

  if (!digitalRead(FACTORY) &&
      !menu12Flag) { // 수동모드 이고 Switch ON 이면 실행...
    delay(50);
    while (!digitalRead(FACTORY))
      ;
    serialDebug = true;
    ledMonitor(5);
    delay(50);
    ledMonitor(5);
    delay(50);
    ledMonitor(5);
  }

  if (EEPROM.readFloat(210) > 1.0 && 10.0 > EEPROM.readFloat(210))
    compareCurrent1 = EEPROM.readFloat(210);
  else {
    compareCurrent1 = 3.0; // 최초 전류값...
    EEPROM.writeFloat(210, compareCurrent1);
    EEPROM.commit();
  }

  if (EEPROM.readFloat(220) > 1.0 && 10.0 > EEPROM.readFloat(220))
    compareCurrent2 = EEPROM.readFloat(220);
  else {
    compareCurrent2 = 3.0; // 최초 전류값...
    EEPROM.writeFloat(220, compareCurrent2);
    EEPROM.commit();
  }

  if (EEPROM.readFloat(230) > 1.0 && 10.0 > EEPROM.readFloat(230))
    compareCurrent3 = EEPROM.readFloat(230);
  else {
    compareCurrent3 = 3.0; // 최초 전류값...
    EEPROM.writeFloat(230, compareCurrent3);
    EEPROM.commit();
  }

  if (EEPROM.readFloat(240) > 1.0 && 10.0 > EEPROM.readFloat(240))
    compareCurrent4 = EEPROM.readFloat(240);
  else {
    compareCurrent4 = 3.0; // 최초 전류값...
    EEPROM.writeFloat(240, compareCurrent4);
    EEPROM.commit();
  }

  digitalWrite(L1U, LOW);
  digitalWrite(L1D, LOW);
  digitalWrite(L2U, LOW);
  digitalWrite(L2D, LOW);

  digitalWrite(L3U, LOW);
  digitalWrite(L3D, LOW);
  digitalWrite(L4U, LOW);
  digitalWrite(L4D, LOW);

  if (serialDebug) { // Factory Mode일때 실행...
    String _msg;
    _msg = "CamTrol Factory";
    _msg.toCharArray(ssidAp, _msg.length() + 1);
    _msg = "1234567890";
    _msg.toCharArray(passwordAp, _msg.length() + 1);
    // ssidAp = "CamTrol Factory";
    // passwordAp = "1234567890";
  } else {                              // Factory Mode가 아니면 실행...
    ads1Offset = EEPROM.readFloat(150); // 무부하시 평균 전류값 1채널
    ads2Offset = EEPROM.readFloat(160); // 무부하시 평균 전류값 1채널
    ads3Offset = EEPROM.readFloat(170); // 무부하시 평균 전류값 2채널
    ads4Offset = EEPROM.readFloat(180); // 무부하시 평균 전류값 2채널

    if (wiFiMode == STATION_MODE) {            // Station Mode Start...
      EEPROM_SSID_STA = EEPROM.readString(70); // EEPROM 주소 70부터 SSID를 읽기
      EEPROM_PSWD_STA =
          EEPROM.readString(100); // EEPROM 주소 100부터 저장된 비밀번호 읽기
      EEPROM_SSID_STA.trim();
      EEPROM_PSWD_STA.trim();
      if (EEPROM_SSID_STA.length() > 30 || EEPROM_PSWD_STA.length() > 30) {
        EEPROM_SSID_STA = "CamTrol-V5.0-001";
        EEPROM_PSWD_STA = "1234567890";
      }
      EEPROM_SSID_STA.toCharArray(ssidAp, EEPROM_SSID_STA.length() + 1);
      EEPROM_PSWD_STA.toCharArray(passwordAp, EEPROM_PSWD_STA.length() + 1);
    }
  }

  // Serial.print("EEPROM_SSID Length:");
  // Serial.println(EEPROM_SSID.length());
  // Serial.print("EEPROM_PSWD Length:");
  // Serial.println(EEPROM_PSWD.length());

  Serial.print("WiFi Mode: ");
  Serial.println(wiFiMode);

  s = lv_label_get_text(ui_Label1);
  s.toCharArray(ssidAp, s.length() + 1);
  Serial.print("SSID : ");
  Serial.println(s);

  s = lv_label_get_text(ui_Label2);
  s.toCharArray(passwordAp, s.length() + 1);
  Serial.print("PSWD : ");
  Serial.println(s);

  wifiStart();

  server.on("/root", handleRoot);
  server.on("/json", handleJson);
  server.on("/Request", handleRequest);

  server.begin();

  // if (wiFiMode == STATION_MODE) {
  //   Serial.println();
  //   Serial.print("ads1Offset = ");
  //   Serial.print(ads1Offset);
  //   Serial.println(";");

  //   Serial.print("ads2Offset = ");
  //   Serial.print(ads2Offset);
  //   Serial.println(";");

  //   Serial.print("ads3Offset = ");
  //   Serial.print(ads3Offset);
  //   Serial.println(";");

  //   Serial.print("ads4Offset = ");
  //   Serial.print(ads4Offset);
  //   Serial.println(";");
  // }

  // s = "";
  // s = s + "{" + "\"ssid\":" + EEPROM_SSID_HOTSPOT + ", " + "\"password\":" +
  //     EEPROM_PSWD_HOTSPOT + "}";

  // String output = "";
  // StaticJsonDocument<100> doc;
  // doc["ssid"] = EEPROM_SSID;
  // doc["password"] = EEPROM_PSWD;
  // serializeJson(doc, output);

  lv_print_current(1); // Screen 2에 표시...
  lv_print_current(2); // Screen 3에 표시...

  ledMonitor(5);

  String convStr;
  String testName = "김현숙";
  int testAge = 58;
  float testSize = 165.7;
  bool testSex = false;

  char _text[100];
  char _convText[100];

  JsonObject root = doc.to<JsonObject>();
  root["name"] = testName;
  root["age"] = testAge;
  root["size"] = testSize;
  root["sex"] = testSex;
  serializeJson(root, _text);
  convStr = _text;

  convStr.toCharArray(_convText, sizeof(_convText));
  deserializeJson(doc, _convText);

  String jsonName = doc["name"];
  int jsonAge = doc["age"];
  float jsonSize = doc["size"];
  bool jsonSex = doc["sex"];

  Serial.print("convStr: ");
  Serial.println(convStr);
  // Serial.println(json);

  Serial.print("이름: ");
  Serial.print(jsonName);
  Serial.print(", 나이: ");
  Serial.print(jsonAge);
  Serial.print(", 키: ");
  Serial.print(jsonSize);
  Serial.print(", 성별: ");
  Serial.println(jsonSex);

  // delay(100);
  digitalWrite(41, HIGH);

} // setup()

void loop() {

  unsigned long currentMillis = millis();

  server.handleClient();
  lv_timer_handler(); /* let the GUI do its work */

  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if (wiFiMode == STATION_MODE) // Station Mode 이고 연결이 끊기면 실행...
  {
    if ((WiFi.status() != WL_CONNECTED) &&
        (currentMillis - previousMillis >= interval)) {
      Serial.print(millis());
      Serial.println(F("Reconnecting to WiFi."));
      WiFi.disconnect();
      WiFi.reconnect();
      previousMillis = currentMillis;
    }
  }

  nexLoop(nex_listen_list);

  ads1 = ads.readADC(0);
  ads2 = ads.readADC(1);
  ads3 = ads.readADC(2);
  ads4 = ads.readADC(3);

  // float ads1Offset = 0.0;
  // float ads2Offset = 0.0;
  // float ads3Offset = 0.0;
  // float ads4Offset = 0.0;

  // bool offsetAdjust = true;
  // bool offsetAdjust = false;

  // if (serialDebug)
  // { // offset 검출용...
  //     ads1Offset = 0.0;
  //     ads2Offset = 0.0;
  //     ads3Offset = 0.0;
  //     ads4Offset = 0.0;
  // }
  // else
  // { // offset 적용...
  //     ads1Offset = 26.10;
  //     ads2Offset = 26.17;
  //     ads3Offset = 26.13;
  //     ads4Offset = 26.15;
  // }

  // Serial 에서 Copy...

  // ads1 : 13389,  current1 : 26.15
  // ads2 : 13325,  current2 : 26.03
  // ads3 : 13347,  current3 : 26.07
  // ads4 : 13281,  current4 : 25.94

  int adDelayTime = 100;

  dispCount++;
  if (dispCount > 5) {
    nowCurrent = abs(((ads1 * 0.000396850) * 5) - ads1Offset);
    if (nowCurrent > 1.0 && l1uFlag)
      current1 = nowCurrent;
    else if (nowCurrent < 1.0 && !l1uFlag)
      current1 = 0;

    nowCurrent = abs(((ads2 * 0.000396850) * 5) - ads2Offset);
    if (nowCurrent > 1.0 && l2uFlag)
      current2 = nowCurrent;
    else if (nowCurrent < 1.0 && !l2uFlag)
      current2 = 0;

    nowCurrent = abs(((ads3 * 0.000396850) * 5) - ads3Offset);
    if (nowCurrent > 1.0 && l3uFlag)
      current3 = nowCurrent;
    else if (nowCurrent < 1.0 && !l3uFlag)
      current3 = 0;

    nowCurrent = abs(((ads4 * 0.000396850) * 5) - ads4Offset);
    if (nowCurrent > 1.0 && l4uFlag)
      current4 = nowCurrent;
    else if (nowCurrent < 1.0 && !l4uFlag)
      current4 = 0;

    dispCount = 0;
    lv_print_current(3);
    // Serial.print("current1 : ");
    // Serial.print(current1);
    // Serial.print(", current2 : ");
    // Serial.print(current2);
    // Serial.print(", current3 : ");
    // Serial.print(current3);
    // Serial.print(", current4 : ");
    // Serial.println(current4);
  }

  if (currentFlag_1 == false && current1 > compareCurrent1 && l1uFlag == true) {
    digitalWrite(L1U, OFF);
    currentFlag_1 = true;
    l1uFlag = false;
    lv_print("리프트 1             지면 정지...");
  }

  if (currentFlag_2 == false && current2 > compareCurrent2 && l2uFlag == true) {
    digitalWrite(L2U, OFF);
    currentFlag_2 = true;
    l2uFlag = false;
    lv_print("리프트     2         지면 정지...");
  }

  if (currentFlag_3 == false && current3 > compareCurrent3 && l3uFlag == true) {
    digitalWrite(L3U, OFF);
    currentFlag_3 = true;
    l3uFlag = false;
    lv_print("리프트         3     지면 정지...");
  }

  if (currentFlag_4 == false && current4 > compareCurrent4 && l4uFlag == true) {
    digitalWrite(L4U, OFF);
    currentFlag_4 = true;
    l4uFlag = false;
    lv_print("리프트             4 지면 정지...");
  }

  if (serialDebug && !avrFlag)
    avrExecution();

  if (serialDebug && !digitalRead(FACTORY))
    avrSave();

  delay(5);

  buttonInput();
}
