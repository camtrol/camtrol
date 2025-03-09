/********

  풀하우스 캠핑카 메인콘트롤러 V6.0
  WaveShare 7" Touch와 연동...

  1. KiCad Main Board 위치
    - E:\CamTrol II\CamTrol\PCB_V5.0\CamTrol_V5.0\CamTrol-S-Cover_V5.0
  2. SquareLine Studio 위치
    - E:\CamTrol II\full_house\esp32_ST7789V3_Touch
  3. SquareLine Studio ui 위치
    - E:\CamTrol II\full_house\esp32_ST7789V3_Touch\SquareLine_ST7789V\libraries
    - SQL Export 후 ui 폴더를 VSC의 src 폴더에 붙여 사용한다.
  4. Flutter Program 위치
    - E:\CamTrol II\full_house\fullhouse_flutter_v5.0
  5. LED 휴즈
    - 1: 10A, 2: 10A, 3: 10A, 4: 10A, 5: 10A, 6: 10A,
  6. OUT 휴즈
    - 1: 10A, 2: 15A, 3: 10A, 4: 10A, 5: 10A, 6: 20A,
  7. 연료, 청수, 오수 센서 type
    - 0~190
  8. 청수, 오수 퇴수밸브 Type
    - -COM
  9. 센서 티칭 방법
    - A, B type Sensor 선택 후 청수센서 Port에서 실시 한다.
  10. QR Code 생성은 여기서 해야 함. Size: 150 x 150
    - https://ko.qr-code-generator.com/
  11. LAN Cable 길이
    - 트럭 : 8000 mm   8 미터
    -   칸 : 6000 mm   6 미터


********/

/*  EEPROM Address
      0 : bool 연료료센서 type
      1 : bool 청수센서 type
      2 : bool 청수센서 type
      3 : bool sensorValueType
      5 : float Volt Adjust
     10 : string SSID
     40 : string PASSWORD
     70 : bool LED 1 POE(Power On Exe)
     71 : bool LED 2 POE(Power On Exe)
     72 : bool LED 3 POE(Power On Exe)
     73 : bool LED 4 POE(Power On Exe)
     74 : bool LED 5 POE(Power On Exe)
     75 : bool LED 6 POE(Power On Exe)
     80 : bool OUT 1 POE(Power On Exe)
     81 : bool OUT 2 POE(Power On Exe)
     82 : bool OUT 3 POE(Power On Exe)
     83 : bool OUT 4 POE(Power On Exe)
     84 : bool OUT 5 POE(Power On Exe)
     85 : bool OUT 6 POE(Power On Exe)
     90 : Volt Offset float 값
    100 : string PSWD_HOST
    200 : int A type Sensor LOW 값
    210 : int A type Sensor HIGH 값
    220 : int B type Sensor LOW 값
    230 : int B type Sensor HIGH 값
*/

#include "Arduino.h"
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "ui/ui.h"
#include <WiFi.h>
// #include <PCF8574.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <ADS1X15.h>
#include <EEPROM.h>
#include <Adafruit_PWMServoDriver.h>

#define PWM_ON 4095
#define ON HIGH
#define OFF LOW

#define ENABLE 5 // RS485 true: Send, false: Receive

// #define HEATER 1 // (ESP32의 1번 사용시 RST-Wait Mode 진입 않됨) 무시동
// 히터...
#define HEATER 47       //  무시동 히터...
#define CLEAN_VALVE 35  //  CLEAN VALVE...
#define WASTE_VALVE 45  //  WASTE VALVE...
#define INVERTOR 48     //  INVERTOR...
#define LIFT_FLAG 16    //  리프트 On, Off 상태 Input...
#define LIFT_POWER 14   //  리프트 전원 출력...
#define LIGHT_SWITCH 13 //  라이트 스위치 입력...

#define HT_FAN 11     // HEATER FAN Pin...
#define HT_MOTOR_1 7  // HEAT MOTOR 1 Pin...
#define HT_MOTOR_2 15 // HEAT MOTOR 2 Pin...

#define VOLT_ADJ_EEPROM 5
#define AP_SSID_EEPROM 10
#define AP_PSWD_EEPROM 40

#define NTC 12

uint8_t readStatus = 0;

// NTC 관련 Parameter
float R1 = 10000.0;
float Beta = 3950.0;
float To = 298.15;
float Ro = 10000.0;
float Rt, T, Tf, adc, Voltage, Vout = 0.0;

float voltAdjust;

const char *ssidBuffer = "";
const char *pwBuffer = "";

char _label_text[20];

char ssidAp[30] = "CamTrol V5.0-T-066";
char passwordAp[30] = "03240304066";

unsigned long previousMillis = 0;

TwoWire I2Ctwo = TwoWire(1);
ADS1115 ads_1(0x48, &I2Ctwo);
Adafruit_PWMServoDriver ledPwm = Adafruit_PWMServoDriver(0x40);

WebServer server(80);
HardwareSerial lift(2);

StaticJsonDocument<500> doc;
DeserializationError error;
JsonObject root;
// JsonObject liftRoot;
String payload;
String apSSID, apPSWD;
String ch[20]; // Display 용 Buffer...

float temp, humi;

// const int freq = 240; // LED PWM 주파수 설정...
const int freq = 240; // LED PWM 주파수 설정...
// const int freq = 2500; // LED PWM 주파수 설정...
const int motorFreq = 8000; // PWM 주파수 설정...
// const int motorFreq = 20000; // PWM 주파수 설정...
const int resolution = 8;

int16_t aux_adc, clean_adc, waste_adc,
    fuel_adc; // we read from the ADC, we have a sixteen bit integer as a result

const int motorCh[4] = {0, 1, 2, 3};
const int ledCh[7] = {0, 1, 2, 3, 4, 5, 6};
const byte outCh[7] = {0, 36, 37, 38, 39, 40, 41};
int pwmPush[7] = {0, 0, 0, 0, 0, 0, 0};
int pwm = 0;
int lvPrintCount = 0;

int screenNumber;

// Serial Monitoring 시 선택...
bool qrFlag = false;
bool eepLedFlag[7];
bool eepOutFlag[2];

bool serialDebug = false;

bool extSwFlag = false;
bool liftFlag = true; // true: 리프트 꺼짐, false: 리프트 꺼짐

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

float current1 = 0.0;
float current2 = 0.0;
float current3 = 0.0;
float current4 = 0.0;

typedef struct struct_message {
  byte dataChange = 0; // 0: No Change, 1: LED Change, 2: OUT Change
  bool ledAllFlag = true;
  bool fuelType = false;
  bool cleanType = false;
  bool wasteType = false;
  float aux_v = 0.0, main_v = 0.0;
  int fuel = 50;
  int clean = 50;
  int waste = 50;
  float temperature = 22.2;
  float setTemp = 25.0;
  bool motorFlag[4] = {false, false, false, false};
  int motorValue[4] = {100, 100, 100, 100};
  bool ledFlag[7] = {false, false, false, false, false, false, false};
  int ledValue[7] = {0, 100, 100, 100, 100, 100, 0};
  int sp3Value = 100;
  int sp4Value = 100;
  bool outOnOff[7] = {false, false, false, false, false, false, false};
  bool invertorFlag = false;
  bool clnValveFlag = false;
  bool wstValveFlag = false;
  bool sleepFlag = false; // true 이면 Sleep Mode...
} struct_message;
struct_message m;
struct_message incomingReading;

// int screenNumber;

int count = 5;
char msg[100];
char liftMsg[100];
// bool bB0 = false, bChange = false;
// int color = 864;
// int cleanPercent;
// int wastePercent;
int wiFiCountValue = 1;
int liftCountValue = 1;
// bool stationFlag = true;
// bool wiFiSelect = true;  // true : AP Mode, false : Station Mode...
// bool wifiStatus = false; // Monitor LED ON, OFF 용...
// bool powerFlag = true;   // Monitor LED ON, OFF 용...
// bool chFlag = true;      // Main Board가 12Ch 이면 True...
// bool relayFlag = true;   // 릴레이 스위치가 Main 이면 True...
// bool lightFlag = false; // 외부 취침 스위치 Flag...

// AZ1117 5.0 : 4.95V ~ 5.1V 까지 정상 동작됨...
// int cleanLow = 6165, cleanHigh = 12000; //0 ~ 190
// int wasteLow = 13456, wasteHigh = 7700; //240 ~ 33

int aLow = 0, aHigh = 0; // 0 ~ 190
int bLow = 0, bHigh = 0; // 240 ~ 33

String s;

// char buffer[100] = {0};

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
      cfg.pin_sclk = 8;                  // SPIのSCLKピン番号を設定
      cfg.pin_mosi = 19;                 // SPIのMOSIピン番号を設定
      cfg.pin_miso = -1; // SPIのMISOピン番号を設定 (-1 = disable)
      cfg.pin_dc = 17;   // SPIのD/Cピン番号を設定  (-1 = disable)
      _bus_instance.config(cfg); // 設定値をバスに反映します。
      _panel_instance.setBus(&_bus_instance); // バスをパネルにセットします。
    }

    {
      auto cfg =
          _panel_instance.config(); // 表示パネル設定用の構造体を取得します。
      cfg.pin_cs = 18; // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst = 20; // RSTが接続されているピン番号  (-1 = disable)
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

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.y_min = 0;
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;
      // I2C connection
      // cfg.i2c_port = I2C_NUM_0;
      cfg.pin_sda = 46;
      cfg.pin_scl = 3;
      cfg.pin_int = 10;
      cfg.pin_rst = 9;
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

void networkScreenLoaded(lv_event_t *e) {}

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
  int value;
  int pwm;

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
    // ledcWrite(ledPwmCh[1], pwm);
    pwmPush[1] = pwm;
    ledPwm.setPWM(ledCh[1], 0, pwmPush[1]);
    m.ledValue[1] = value;
    if (value != 0) {
      m.ledFlag[1] = true;
    } else {
      m.ledFlag[1] = false;
      m.ledAllFlag = false;
    }
    s = "조명 1";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED2") {
    // ledcWrite(ledPwmCh[1], pwm);
    pwmPush[2] = pwm;
    ledPwm.setPWM(ledCh[2], 0, pwmPush[2]);
    m.ledValue[2] = value;
    if (value != 0) {
      m.ledFlag[2] = true;
    } else {
      m.ledFlag[2] = false;
      m.ledAllFlag = false;
    }
    s = "조명 2";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED3") {
    // ledcWrite(ledPwmCh[1], pwm);
    pwmPush[3] = pwm;
    ledPwm.setPWM(ledCh[3], 0, pwmPush[3]);
    m.ledValue[3] = value;
    if (value != 0) {
      m.ledFlag[3] = true;
    } else {
      m.ledFlag[3] = false;
      m.ledAllFlag = false;
    }
    s = "조명 3";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED4") {
    // ledcWrite(ledPwmCh[1], pwm);
    pwmPush[4] = pwm;
    ledPwm.setPWM(ledCh[4], 0, pwmPush[4]);
    m.ledValue[4] = value;
    if (value != 0) {
      m.ledFlag[4] = true;
    } else {
      m.ledFlag[4] = false;
      m.ledAllFlag = false;
    }
    s = "조명 4";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED5") {
    // ledcWrite(ledPwmCh[1], pwm);
    pwmPush[5] = pwm;
    ledPwm.setPWM(ledCh[5], 0, pwmPush[5]);
    m.ledValue[5] = value;
    if (value != 0) {
      m.ledFlag[5] = true;
    } else {
      m.ledFlag[5] = false;
      m.ledAllFlag = false;
    }
    s = "조명 5";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED6") {
    // ledcWrite(ledPwmCh[1], pwm);
    pwmPush[6] = pwm;
    ledPwm.setPWM(ledCh[6], 0, pwmPush[6]);
    m.ledValue[6] = value;
    if (value != 0) {
      m.ledFlag[6] = true;
    } else {
      m.ledFlag[6] = false;
      m.ledAllFlag = false;
    }
    s = "조명 6";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "LED_ALL") {
    pwmPush[1] = pwm;
    pwmPush[2] = pwm;
    pwmPush[3] = pwm;
    pwmPush[4] = pwm;
    pwmPush[5] = pwm;
    // pwmPush[6] = pwm;
    ledPwm.setPWM(ledCh[1], 0, pwmPush[1]);
    ledPwm.setPWM(ledCh[2], 0, pwmPush[2]);
    ledPwm.setPWM(ledCh[3], 0, pwmPush[3]);
    ledPwm.setPWM(ledCh[4], 0, pwmPush[4]);
    ledPwm.setPWM(ledCh[5], 0, pwmPush[5]);
    ledPwm.setPWM(ledCh[6], 0, pwmPush[6]);
    digitalWrite(outCh[1], LOW);
    m.outOnOff[1] = false;

    m.ledValue[1] = value;
    m.ledValue[2] = value;
    m.ledValue[3] = value;
    m.ledValue[4] = value;
    m.ledValue[5] = value;
    m.ledValue[6] = value;
    if (value != 0) {
      m.ledFlag[1] = true;
      m.ledFlag[2] = true;
      m.ledFlag[3] = true;
      m.ledFlag[4] = true;
      m.ledFlag[5] = true;
      // m.ledFlag[6] = true;
      m.ledAllFlag = true;
    } else {
      m.ledFlag[1] = false;
      m.ledFlag[2] = false;
      m.ledFlag[3] = false;
      m.ledFlag[4] = false;
      m.ledFlag[5] = false;
      m.ledFlag[6] = false;
      m.ledAllFlag = false;
    }

    s = "조명 ALL";
    s += " : ";
    s += value;
    s += "...";
    lv_print(s);
  } else if (chanel == "OUT1") {
    value == 1 ? digitalWrite(outCh[1], HIGH) : digitalWrite(outCh[1], LOW);
    if (value) {
      m.outOnOff[1] = true;
      lv_print("현관등 켜짐...");
    } else {
      m.outOnOff[1] = false;
      lv_print("현관등    꺼짐...");
    }
  } else if (chanel == "OUT2") {
    if (value) {
      m.outOnOff[2] = true;
      lv_print("물펌프 켜짐...");
      digitalWrite(outCh[2], ON);
      delay(100);
      digitalWrite(outCh[2], OFF);
      delay(100);
      digitalWrite(outCh[2], HIGH);
    } else {
      digitalWrite(outCh[2], LOW);
      m.outOnOff[2] = false;
      lv_print("물펌프    꺼짐...");
    }
  } else if (chanel == "OUT3") {
    value == 1 ? digitalWrite(outCh[3], HIGH) : digitalWrite(outCh[3], LOW);
    if (value) {
      m.outOnOff[3] = true;
      lv_print("맥스팬 켜짐...");
    } else {
      m.outOnOff[3] = false;
      lv_print("맥스팬    꺼짐...");
    }
  } else if (chanel == "OUT4") {
    value == 1 ? digitalWrite(outCh[4], HIGH) : digitalWrite(outCh[4], LOW);
    if (value) {
      m.outOnOff[4] = true;
      lv_print("전기실 팬 켜짐...");
    } else {
      m.outOnOff[4] = false;
      lv_print("전기실 팬    꺼짐...");
    }
  } else if (chanel == "OUT6") {
    value == 1 ? digitalWrite(outCh[6], HIGH) : digitalWrite(outCh[6], LOW);
    if (value) {
      m.outOnOff[6] = true;
      lv_print("냉장고 켜짐...");
    } else {
      m.outOnOff[6] = false;
      lv_print("냉장고    꺼짐...");
    }
  } else if (chanel == "clnValve") {
    if (value == 1) {
      m.clnValveFlag = true;
      digitalWrite(CLEAN_VALVE, ON);
    } else {
      m.clnValveFlag = false;
      digitalWrite(CLEAN_VALVE, OFF);
    }
  } else if (chanel == "wstValve") {
    if (value == 1) {
      m.wstValveFlag = true;
      digitalWrite(WASTE_VALVE, ON);
    } else {
      m.wstValveFlag = false;
      digitalWrite(WASTE_VALVE, OFF);
    }
  } else if (chanel == "INVERTOR") {
    m.dataChange = 2;
    if (value == 1) {
      m.invertorFlag = true;
      digitalWrite(INVERTOR, ON);
    } else {
      m.invertorFlag = false;
      digitalWrite(INVERTOR, OFF);
    }
  } else if (chanel == "BOILER") {
    m.dataChange = 2;

    if (value) { // 베바스토 ON 조건...
      m.outOnOff[5] = true;
    } else { // 베바스토 OFF 조건...
      m.outOnOff[5] = false;
    }

    digitalWrite(HEATER, ON);
    delay(600);
    digitalWrite(HEATER, OFF);

    m.outOnOff[5] = value ? true : false;
  } else if (chanel == "HEATER") {
    m.dataChange = 2;

    if (value != 0) {
      m.motorFlag[1] = true;
      m.motorValue[1] = value;
      pwm = map(value, 20, 100, 20, 255);
      ledcWrite(motorCh[1], pwm);
    } else {
      m.motorFlag[1] = false;
      ledcWrite(motorCh[1], 0);
      // pwm = 0;
    }
  } else if (chanel == "BUNKER") {
    if (value == 0) {
      ledcWrite(motorCh[2], 0);
      m.motorFlag[2] = false;
    } else {
      ledcWrite(motorCh[2], pwm);
      m.motorFlag[2] = true;
    }
  } else if (chanel == "LIVING") {
    m.dataChange = 2;

    if (value != 0) {
      m.motorFlag[3] = true;
      m.motorValue[3] = value;
      pwm = map(value, 20, 100, 20, 255);
      ledcWrite(motorCh[3], pwm);
    } else {
      m.motorFlag[3] = false;
      ledcWrite(motorCh[3], 0);
      // pwm = 0;
    }
  } else if (chanel == "REQUEST") {
    Serial.println("Request OK...");

    s = "VOLT";
    s += ':';
    s += String(m.aux_v, 1);
    s += " V";
    comSend(s);
    delay(5);

    if (m.temperature > -50.0) {
      s = "TEMP";
      s += ':';
      s += String(m.temperature, 1);
      s += " °C";
      comSend(s);
    } else
      comSend("TEMP:--.- °C");
    delay(5);

    if (m.clean != 255) {
      s = "CLN_VAL:";
      s += m.clean;
      comSend(s);
    } else {
      comSend("CLN_VAL:---");
    }
    delay(5);

    if (m.waste != 255) {
      s = "WST_VAL:";
      s += m.waste;
      comSend(s);
    } else {
      comSend("WST_VAL:---");
    }
    delay(5);

  } else if (chanel == "SET_TEMP") {
    m.setTemp = value / 10.0;
    Serial.print("m.setTemp : ");
    Serial.println(m.setTemp);
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
  }
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

void funCall(int event) {
  Serial.print("lv_event Function call : ");
  Serial.println(event);
  Serial.print("lv_obj_get_state : ");
  // Serial.println(lv_obj_get_state(ui_Switch4));
  Serial.print("lv_label_get_text : ");
  // Serial.println(lv_label_get_text(ui_clnLowValue));
}

void panelVolt(lv_event_t *e) {
  s = String(voltAdjust, 2);
  s.toCharArray(_label_text, sizeof(_label_text));
  lv_label_set_text(ui_volt, _label_text);
}

void screen1Loaded(lv_event_t *e) {
  screenNumber = 1;
  funCall(11);
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
  // lv_obj_set_style_opa(ui_QRImage, rollValue, 0);
  // _ui_opacity_set(ui_QRImage, rollValue);
  lv_obj_set_style_img_recolor(ui_QRImage, lv_color_hex(0x000000),
                               LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_img_recolor_opa(ui_QRImage, rollValue,
                                   LV_PART_MAIN | LV_STATE_DEFAULT);
  // _ui_opacity_set(ui_QRImage, rollValue);
  Serial.print("Slider 값: ");
  Serial.println(rollValue);
  Serial.print("Event Code: ");
  Serial.println(code);
}

void screen4Loaded(lv_event_t *e) {

  EEPROM.readBool(0) ? lv_obj_add_state(ui_SwitchFuel, LV_STATE_CHECKED)
                     : lv_obj_clear_state(ui_SwitchFuel, LV_STATE_CHECKED);
  EEPROM.readBool(1) ? lv_obj_add_state(ui_SwitchClean, LV_STATE_CHECKED)
                     : lv_obj_clear_state(ui_SwitchClean, LV_STATE_CHECKED);
  EEPROM.readBool(2) ? lv_obj_add_state(ui_SwitchWaste, LV_STATE_CHECKED)
                     : lv_obj_clear_state(ui_SwitchWaste, LV_STATE_CHECKED);

  screenNumber = 4;
  funCall(44);
}

void screen5LoadedExe() {
  String _value;
  char _ui_value[10];

  Serial.print("screen5 EEPROM 200: ");
  Serial.print(EEPROM.readInt(200));
  Serial.print(", EEPROM 220: ");
  Serial.print(EEPROM.readInt(220));
  Serial.print(", EEPROM 210: ");
  Serial.print(EEPROM.readInt(210));
  Serial.print(", EEPROM 230: ");
  Serial.println(EEPROM.readInt(230));

  if (EEPROM.readBool(3)) {
    lv_obj_add_state(ui_switchSensorType, LV_STATE_CHECKED);
    _value = String(EEPROM.readInt(200));
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorLowValue, _ui_value);
    _value = String(EEPROM.readInt(210));
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorHighValue, _ui_value);
  } else {
    lv_obj_clear_state(ui_switchSensorType, LV_STATE_CHECKED);
    _value = String(EEPROM.readInt(220));
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorLowValue, _ui_value);
    _value = String(EEPROM.readInt(230));
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorHighValue, _ui_value);
  }
  screenNumber = 5;
  funCall(55);
}

void screen5Loaded(lv_event_t *e) { screen5LoadedExe(); }

void screen6Loaded(lv_event_t *e) {
  EEPROM.readBool(70) ? lv_obj_add_state(ui_ledSwitch1, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_ledSwitch1, LV_STATE_CHECKED);
  EEPROM.readBool(71) ? lv_obj_add_state(ui_ledSwitch2, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_ledSwitch2, LV_STATE_CHECKED);
  EEPROM.readBool(72) ? lv_obj_add_state(ui_ledSwitch3, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_ledSwitch3, LV_STATE_CHECKED);
  EEPROM.readBool(73) ? lv_obj_add_state(ui_ledSwitch4, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_ledSwitch4, LV_STATE_CHECKED);
  EEPROM.readBool(74) ? lv_obj_add_state(ui_ledSwitch5, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_ledSwitch5, LV_STATE_CHECKED);
  EEPROM.readBool(75) ? lv_obj_add_state(ui_ledSwitch6, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_ledSwitch6, LV_STATE_CHECKED);

  EEPROM.readBool(80) ? lv_obj_add_state(ui_outSwitch1, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_outSwitch1, LV_STATE_CHECKED);
  EEPROM.readBool(81) ? lv_obj_add_state(ui_outSwitch2, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_outSwitch2, LV_STATE_CHECKED);
  EEPROM.readBool(82) ? lv_obj_add_state(ui_outSwitch3, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_outSwitch3, LV_STATE_CHECKED);
  EEPROM.readBool(83) ? lv_obj_add_state(ui_outSwitch4, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_outSwitch4, LV_STATE_CHECKED);
  EEPROM.readBool(84) ? lv_obj_add_state(ui_outSwitch5, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_outSwitch5, LV_STATE_CHECKED);
  EEPROM.readBool(85) ? lv_obj_add_state(ui_outSwitch6, LV_STATE_CHECKED)
                      : lv_obj_clear_state(ui_outSwitch6, LV_STATE_CHECKED);

  screenNumber = 6;
  funCall(66);
}

void sensorValueSelect(lv_event_t *e) {
  EEPROM.readBool(3) ? EEPROM.writeBool(3, false) : EEPROM.writeBool(3, true);
  EEPROM.commit();

  screen5LoadedExe();
  // screenNumber = 5;
  // funCall(55);
}

void sensorValueSet(lv_event_t *e) {
  String _value;
  char _ui_value[10];
  if (lv_obj_get_state(ui_switchSensorType)) {
    _value = EEPROM.readInt(200);
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorLowValue, _ui_value);
    _value = EEPROM.readInt(210);
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorLowValue, _ui_value);
  } else {
    _value = EEPROM.readInt(220);
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorLowValue, _ui_value);
    _value = EEPROM.readInt(230);
    _value.toCharArray(_ui_value, sizeof(_ui_value));
    lv_label_set_text(ui_sensorLowValue, _ui_value);
  }
  EEPROM.commit();
}

// Screen 4 의 저장 버튼...
void btnSensorSelectSave(lv_event_t *e) {
  EEPROM.writeBool(0, lv_obj_get_state(ui_SwitchFuel) ? true : false);
  EEPROM.writeBool(1, lv_obj_get_state(ui_SwitchClean) ? true : false);
  EEPROM.writeBool(2, lv_obj_get_state(ui_SwitchWaste) ? true : false);
  EEPROM.commit();

  lv_print("다음과 같이 설정 되었습니다.");
  s = "연료센서: ";
  EEPROM.readBool(0) ? s += "0~190" : s += "240~33";
  lv_print(s);
  s = "청수센서: ";
  EEPROM.readBool(1) ? s += "0~190" : s += "240~33";
  lv_print(s);
  s = "오수센서: ";
  EEPROM.readBool(2) ? s += "0~190" : s += "240~33";
  lv_print(s);

  // if (EEPROM.readBool(0))
  //   s += "0~190";
  // else
  //   s += "240~33";
  // lv_print(s);

  // s = "청수센서: ";
  // if (EEPROM.readBool(1))
  //   s += "0~190";
  // else
  //   s += "240~33";
  // lv_print(s);

  // s = "오수센서: ";
  // if (EEPROM.readBool(2))
  //   s += "0~190";
  // else
  //   s += "240~33";
  // lv_print(s);
}

// Screen 5 의 하한 값 저장 버튼...
void SensorLowSave(lv_event_t *e) {
  lv_label_set_text(ui_sensorLowValue, lv_label_get_text(ui_sensorNowValue));
}

// Screen 5 의 상한 값 저장 버튼...
void SensorHighSave(lv_event_t *e) {
  lv_label_set_text(ui_sensorHighValue, lv_label_get_text(ui_sensorNowValue));
}

// Screen 5 의 센서값 저장 버튼...
void btnSensorValueSave(lv_event_t *e) {
  String _value;

  _value = lv_label_get_text(ui_sensorLowValue);
  if (lv_obj_get_state(ui_switchSensorType))
    EEPROM.writeInt(200, _value.toInt());
  else
    EEPROM.writeInt(220, _value.toInt());

  _value = lv_label_get_text(ui_sensorHighValue);
  if (lv_obj_get_state(ui_switchSensorType))
    EEPROM.writeInt(210, _value.toInt());
  else
    EEPROM.writeInt(230, _value.toInt());

  EEPROM.commit();

  Serial.print("EEPROM 200: ");
  Serial.print(EEPROM.readInt(200));
  Serial.print(", EEPROM 220: ");
  Serial.print(EEPROM.readInt(220));
  Serial.print(", EEPROM 210: ");
  Serial.print(EEPROM.readInt(210));
  Serial.print(", EEPROM 230: ");
  Serial.println(EEPROM.readInt(230));
}

// Screen 6 의 POE 저장 버튼...
void btnPowerOnExeSave(lv_event_t *e) {
  if (lv_obj_get_state(ui_ledSwitch1) != EEPROM.readBool(70))
    EEPROM.writeBool(70, lv_obj_get_state(ui_ledSwitch1));
  if (lv_obj_get_state(ui_ledSwitch2) != EEPROM.readBool(71))
    EEPROM.writeBool(71, lv_obj_get_state(ui_ledSwitch2));
  if (lv_obj_get_state(ui_ledSwitch3) != EEPROM.readBool(72))
    EEPROM.writeBool(72, lv_obj_get_state(ui_ledSwitch3));
  if (lv_obj_get_state(ui_ledSwitch4) != EEPROM.readBool(73))
    EEPROM.writeBool(73, lv_obj_get_state(ui_ledSwitch4));
  if (lv_obj_get_state(ui_ledSwitch5) != EEPROM.readBool(74))
    EEPROM.writeBool(74, lv_obj_get_state(ui_ledSwitch5));
  if (lv_obj_get_state(ui_ledSwitch6) != EEPROM.readBool(75))
    EEPROM.writeBool(75, lv_obj_get_state(ui_ledSwitch6));

  if (lv_obj_get_state(ui_outSwitch1) != EEPROM.readBool(80))
    EEPROM.writeBool(80, lv_obj_get_state(ui_outSwitch1));
  if (lv_obj_get_state(ui_outSwitch2) != EEPROM.readBool(81))
    EEPROM.writeBool(81, lv_obj_get_state(ui_outSwitch2));
  if (lv_obj_get_state(ui_outSwitch3) != EEPROM.readBool(82))
    EEPROM.writeBool(82, lv_obj_get_state(ui_outSwitch3));
  if (lv_obj_get_state(ui_outSwitch4) != EEPROM.readBool(83))
    EEPROM.writeBool(83, lv_obj_get_state(ui_outSwitch4));
  if (lv_obj_get_state(ui_outSwitch5) != EEPROM.readBool(84))
    EEPROM.writeBool(84, lv_obj_get_state(ui_outSwitch5));
  if (lv_obj_get_state(ui_outSwitch6) != EEPROM.readBool(85))
    EEPROM.writeBool(85, lv_obj_get_state(ui_outSwitch6));
  EEPROM.commit();

  lv_print("전원공급 시 자동 On 합니다.");
  s = "LED:";
  if (EEPROM.readBool(70))
    s += " 1";
  if (EEPROM.readBool(71))
    s += ", 2";
  if (EEPROM.readBool(72))
    s += ", 3";
  if (EEPROM.readBool(73))
    s += ", 4";
  if (EEPROM.readBool(74))
    s += ", 5";
  if (EEPROM.readBool(75))
    s += ", 6";
  lv_print(s);
  s = "OUT:";
  if (EEPROM.readBool(80))
    s += " 1";
  if (EEPROM.readBool(81))
    s += ", 2";
  if (EEPROM.readBool(82))
    s += ", 3";
  if (EEPROM.readBool(83))
    s += ", 4";
  if (EEPROM.readBool(84))
    s += ", 5";
  if (EEPROM.readBool(85))
    s += ", 6";
  lv_print(s);
}

void keyPressedOn() {
  int pwmCount;

  if (!extSwFlag) { // 조명 On...
    lv_print("전체등 켜짐...");

    m.ledAllFlag = true;
    pwmCount = 0;
    m.sleepFlag = false;

    s = "SLEEP:1:";
    s += m.sleepFlag;
    comSend(s);

    lv_timer_handler();

    while (pwmCount < 4096) {
      for (int i = 1; i < 6; i++) {
        ledPwm.setPWM(ledCh[i], 0, pwmCount);
        m.ledFlag[i] = true;
      }
      pwmCount += 120;
      delay(50);
    }
  } else { // All LED Off...
    lv_print("전체등      꺼짐...");

    m.ledAllFlag = false;
    m.sleepFlag = true;
    s = "SLEEP:1:";
    s += m.sleepFlag;
    comSend(s);

    pwmCount = 4095;
    digitalWrite(outCh[1], LOW);
    m.outOnOff[1] = false;

    lv_timer_handler();

    for (int i = 1; i < 7; i++) {
      if (m.ledFlag[i] == true)
        pwmPush[i] = map(m.ledValue[i], 1, 100, 3, PWM_ON);
    }

    while (pwmCount > 0) {
      for (int i = 1; i < 7; i++) {
        // if (pwmCount < 0)
        //   pwmPush[i] = 0;
        if (m.ledFlag[i]) {
          ledPwm.setPWM(ledCh[i], 0, pwmPush[i]);
          pwmPush[i] -= 120;
        }
      }
      pwmCount -= 120;
      delay(30);
    }

    for (int i = 1; i < 7; i++) {
      // if (eepLedFlag[i]) {
      if (m.ledFlag[i]) {
        ledPwm.setPWM(ledCh[i], 0, 0);
        m.ledFlag[i] = false;
      }
    }
  }
}

void waterLevel() {
  fuel_adc = ads_1.readADC(1);
  clean_adc = ads_1.readADC(2);
  waste_adc = ads_1.readADC(3);

  // 연료샌서 처리...
  if (m.fuelType == true) // A Type Sensor 0 ~ 190
  {
    if (((aHigh - aLow) / 2) + (aHigh - aLow) > fuel_adc)
      m.fuel =
          map(fuel_adc, aLow, ((aHigh - aLow) / 2) + (aHigh - aLow), 0, 30);
    else
      m.fuel =
          map(fuel_adc, ((aHigh - aLow) / 2) + (aHigh - aLow), aHigh, 31, 100);
  } else // B Type Sensor 240 ~ 33
  {
    if (((bLow - bHigh) / 2) + (bLow - bHigh) + 1000 < fuel_adc)
      m.fuel = map(fuel_adc, bLow, ((bLow - bHigh) / 2) + (bLow - bHigh) + 1000,
                   0, 50);
    else
      m.fuel = map(fuel_adc, ((bLow - bHigh) / 2) + (bLow - bHigh) + 1000,
                   bHigh, 51, 100);
  }

  // 청수샌서 처리...
  if (m.cleanType == true) // A Type Sensor 0 ~ 190
  {
    if (((aHigh - aLow) / 2) + (aHigh - aLow) > clean_adc)
      m.clean =
          map(clean_adc, aLow, ((aHigh - aLow) / 2) + (aHigh - aLow), 0, 30);
    else
      m.clean =
          map(clean_adc, ((aHigh - aLow) / 2) + (aHigh - aLow), aHigh, 31, 100);
  } else // B Type Sensor 240 ~ 33
  {
    if (((bLow - bHigh) / 2) + (bLow - bHigh) + 1000 < clean_adc)
      m.clean = map(clean_adc, bLow,
                    ((bLow - bHigh) / 2) + (bLow - bHigh) + 1000, 0, 50);
    else
      m.clean = map(clean_adc, ((bLow - bHigh) / 2) + (bLow - bHigh) + 1000,
                    bHigh, 51, 100);
  }

  // 오수샌서 처리...
  if (m.wasteType == true) // A Type Sensor 0 ~ 190
  {
    if (((aHigh - aLow) / 2) + (aHigh - aLow) > waste_adc)
      m.waste =
          map(waste_adc, aLow, ((aHigh - aLow) / 2) + (aHigh - aLow), 0, 30);
    else
      m.waste =
          map(waste_adc, ((aHigh - aLow) / 2) + (aHigh - aLow), aHigh, 31, 100);
  } else // B Type Sensor 240 ~ 33
  {
    if (((bLow - bHigh) / 2) + (bLow - bHigh) + 1000 < waste_adc)
      m.waste = map(waste_adc, bLow,
                    ((bLow - bHigh) / 2) + (bLow - bHigh) + 1000, 0, 50);
    else
      m.waste = map(waste_adc, ((bLow - bHigh) / 2) + (bLow - bHigh) + 1000,
                    bHigh, 51, 100);
  }

  if (m.fuel > 97) // 97 보다 크면 100%
    m.fuel = 100;
  else if (m.fuel < 5) // 5 보다 작으면 0%
    m.fuel = 0;

  if (m.clean > 97) // 97 보다 크면 100%
    m.clean = 100;
  else if (m.clean < 5) // 5 보다 작으면 0%
    m.clean = 0;

  if (m.waste > 97) // 97 보다 크면 100%
    m.waste = 100;
  else if (m.waste < 5) // 5 보다 작으면 0%
    m.waste = 0;

  if (20000 < fuel_adc) // 연료 Sensor가 꼽혀 있지 않으면 255
    m.fuel = 255;

  if (20000 < clean_adc) // 청수 Sensor가 꼽혀 있지 않으면 255
    m.clean = 255;

  if (20000 < waste_adc) // 오수 Sensor가 꼽혀 있지 않으면 255
    m.waste = 255;

  Serial.println();
  Serial.print("Fuel ADC: ");
  Serial.print(fuel_adc);
  Serial.print(", Clean ADC: ");
  Serial.print(clean_adc);
  Serial.print(",  Waste ADC: ");
  Serial.print(waste_adc);

  Serial.println();
  Serial.print("m.fuel: ");
  Serial.print(m.fuel);
  Serial.print(", m.clean: ");
  Serial.print(m.clean);
  Serial.print(", m.waste: ");
  Serial.println(m.waste);
}

void nowVolt() {

  aux_adc = ads_1.readADC(0);
  delay(5);
  m.temperature = analogRead(NTC);
  Voltage = (aux_adc * 0.0001875) * 5;
  m.aux_v = round(Voltage * 10) / 10;
  m.aux_v += voltAdjust;

  adc = analogRead(NTC);

  Vout = adc * 3.3 / 4096;
  Rt = R1 * Vout / (3.3 - Vout);

  T = 1 / (1 / To + log(Rt / Ro) / Beta); // 켈빈 온도
  m.temperature = (T - 273.15) + 20.0;    // 섭씨 온도
  m.temperature = round(m.temperature * 10) / 10;

  Serial.print(m.temperature);
  Serial.println(" 도");
}

// void webSocketReceive(uint8_t *payload) {
//   Serial.printf("수신 Payload : %s\n", payload);
//   error = deserializeJson(doc, payload);
//   root = doc.as<JsonObject>();

//   if (error) {
//     Serial.println();
//     Serial.print(F("deserializeJson() failed: "));
//     Serial.println(error.c_str());
//     return;
//   } else {

//     if (doc["LiftFlag"] == 1)
//       liftCountValue = 0;

//     if (doc["NetworkChangeFlag"] == 1) {
//       String ssid;
//       String pw;

//       ssid = EEPROM.readString(10); // EEPROM 주소 10부터 SSID를 읽기
//       pw = EEPROM.readString(40); // EEPROM 주소 30부터 저장된 비밀번호 읽기
//       ssid.trim();
//       pw.trim();
//       s = "";
//       s = s + "{" + "\"ssid\":" + "\"" + ssid + "\"" + ", " + "\"pw\":" +
//       "\"" +
//           pw + "\"" + "}";
//     }

//     if (doc["L1U_Flag"] == 1)
//       l1uFlag = true;
//     else
//       l1uFlag = false;

//     if (doc["L1D_Flag"] == 1)
//       l1dFlag = true;
//     else
//       l1dFlag = false;

//     if (doc["L2U_Flag"] == 1)
//       l2uFlag = true;
//     else
//       l2uFlag = false;

//     if (doc["L2D_Flag"] == 1)
//       l2dFlag = true;
//     else
//       l2dFlag = false;

//     if (doc["L3U_Flag"] == 1)
//       l3uFlag = true;
//     else
//       l3uFlag = false;

//     if (doc["L3D_Flag"] == 1)
//       l3dFlag = true;
//     else
//       l3dFlag = false;

//     if (doc["L4U_Flag"] == 1)
//       l4uFlag = true;
//     else
//       l4uFlag = false;

//     if (doc["L4D_Flag"] == 1)
//       l4dFlag = true;
//     else
//       l4dFlag = false;

//     if (doc["LAU_Flag"] == 1)
//       lauFlag = true;
//     else
//       lauFlag = false;

//     if (doc["LAD_Flag"] == 1)
//       ladFlag = true;
//     else
//       ladFlag = false;
//   }
// }

void serverSend() {

  int setTemp = m.setTemp * 10;

  s = "";
  s = s + "{" + "\"LiftFlag\":" + liftFlag +
      ", " + // 릴레이 스위치가 Main 이면 True...
      "\"aux_v\":" + m.aux_v + ", " + "\"Clean\":" + m.clean + ", " +
      "\"Waste\":" + m.waste + ", " + "\"Temperature\":" + m.temperature +
      ", " + "\"Fuel\":" + m.fuel + ", " + "\"ledValue0\":" + m.ledValue[0] +
      ", " + "\"ledValue1\":" + m.ledValue[1] + ", " + "\"ledValue2\":" +
      m.ledValue[2] + ", " + "\"ledValue3\":" + m.ledValue[3] + ", " +
      "\"ledValue4\":" + m.ledValue[4] + ", " + "\"ledValue5\":" +
      m.ledValue[5] + ", " + "\"ledValue6\":" + m.ledValue[6] + ", " +
      "\"outOnOff1\":" + m.outOnOff[1] + ", " + "\"outOnOff2\":" +
      m.outOnOff[2] + ", " + "\"outOnOff3\":" + m.outOnOff[3] + ", " +
      "\"outOnOff4\":" + m.outOnOff[4] + ", " + "\"outOnOff5\":" +
      m.outOnOff[5] + ", " + "\"outOnOff6\":" + m.outOnOff[6] + ", " +
      "\"invertor\":" + m.invertorFlag + ", " + "\"clnValve\":" +
      m.clnValveFlag + ", " + "\"wstValve\":" + m.wstValveFlag + ", " +
      "\"MT1Flag\":" + m.motorFlag[1] + ", " +   // Heater Fan Flag...
      "\"MT2Flag\":" + m.motorFlag[2] + ", " +   // Motor 1 Flag...
      "\"MT3Flag\":" + m.motorFlag[3] + ", " +   // Motor 2 Flag...
      "\"MT1Value\":" + m.motorValue[1] + ", " + // Heater Fan Value...
      "\"MT2Value\":" + m.motorValue[2] + ", " + // Motor 1 Value...
      "\"MT3Value\":" + m.motorValue[3] + ", " + // Motor 2 Value...
      "\"ledAllFlag\":" + m.ledAllFlag + ", " + "\"sleep\":" + m.sleepFlag +
      ", " + "\"ledFlag0\":" + m.ledFlag[0] + ", " + "\"ledFlag1\":" +
      m.ledFlag[1] + ", " + "\"ledFlag2\":" + m.ledFlag[2] + ", " +
      "\"ledFlag3\":" + m.ledFlag[3] + ", " + "\"ledFlag4\":" + m.ledFlag[4] +
      ", " + "\"ledFlag5\":" + m.ledFlag[5] + ", " + "\"ledFlag6\":" +
      m.ledFlag[6] + ", " + "\"L1U_Flag\":" + l1uFlag +
      ", " +                             // Lift 1 up Bool...
      "\"L1D_Flag\":" + l1dFlag + ", " + // Lift 1 down Bool...
      "\"L2U_Flag\":" + l2uFlag + ", " + // Lift 2 up Bool...
      "\"L2D_Flag\":" + l2dFlag + ", " + // Lift 2 down Bool...
      "\"L3U_Flag\":" + l3uFlag + ", " + // Lift 3 up Bool...
      "\"L3D_Flag\":" + l3dFlag + ", " + // Lift 3 down Bool...
      "\"L4U_Flag\":" + l4uFlag + ", " + // Lift 4 up Bool...
      "\"L4D_Flag\":" + l4dFlag + ", " + // Lift 4 down Bool...
      "\"LAU_Flag\":" + lauFlag + ", " + // Lift all up Bool...
      "\"LAD_Flag\":" + ladFlag +        // Lift all down Bool...
      "}";

  server.send(200, "text/plain", s);
}

void handleJson() {
  char _lvChar[30];
  wiFiCountValue = 0;

  pwm = 0;
  server.arg("j").toCharArray(msg, 2000);
  Serial.println(F("handleJson()..."));
  Serial.println(msg);
  deserializeJson(doc, msg);
  root = doc.as<JsonObject>();
  // liftRoot = doc.as<JsonObject>();

  String division = root["division"];
  String name = root["name"];
  String ssidParse = root["ssid"];
  String passwordParse = root["password"];
  int chanel = root["Chanel"];
  int value = root["Value"];

  String msg;

  s = name;
  s += " :  ";

  if (division == "LED" || division == "MOTOR") {
    if (value == 0)
      s += "      꺼짐...";
    else
      s += value;
  } else if (division == "OUT" || division == "INVERTOR") {
    s += value ? "켜짐..." : "      꺼짐...";
  } else if (division == "clnValve" || division == "wstValve") {
    s += value ? "열림..." : "      닫힘...";
  } else if (division == "sleep") {
    s += (chanel == 1) ? "시작..." : "      해제...";
  } else if (division == "ALL" || division == "LIFT")
    s += (value == 1) ? "켜짐..." : "      커짐...";

  lv_print(s);

  if (division == "L1U" || division == "L1D" || division == "L2U" ||
      division == "L2D" || division == "L3U" || division == "L3D" ||
      division == "L4U" || division == "L4D" || division == "LAU" ||
      division == "LAD") { // {"division":"L1U","Chanel":1,"Value":1}
    s = "";
    s = s + "{" + "\"division\":" + "\"" + division + "\"" + ", " + //
        "\"Chanel\":" + chanel + ", " +                             //
        "\"Value\":" + value + "}";
  }

  if (division == "LED") {
    msg = division;

    switch (chanel) {
    case 1:
      msg += " 1";
      s = "LED1:";
      break;
    case 2:
      msg += " 2";
      s = "LED2:";
      break;
    case 3:
      msg += " 3";
      s = "LED3:";
      break;
    case 4:
      msg += " 4";
      s = "LED4:";
      break;
    case 5:
      msg += " 5";
      s = "LED5:";
      break;
    case 6:
      msg += " 6";
      s = "LED6:";
      break;
    default:
      break;
    }

    m.dataChange = 1;
    pwm = map(value, 1, 100, 3, 4095);
    if (pwm <= 2) // 2 이하이면 0 으로 처리...
      pwm = 0;

    // m.ledValue[chanel] = value;

    if (value == 0) // LED OFF...
    {
      m.ledFlag[chanel] = false;

      if (!m.ledFlag[1] && !m.ledFlag[2] && !m.ledFlag[3] && !m.ledFlag[4] &&
          !m.ledFlag[5])
        m.ledAllFlag = false;
    } else // LED ON...
    {
      m.ledFlag[chanel] = true;

      if (m.ledFlag[1] && m.ledFlag[2] && m.ledFlag[3] && m.ledFlag[4] &&
          m.ledFlag[5])
        m.ledAllFlag = true;
    }
    s += value;
    s += ':';
    s += m.ledFlag[chanel];
    comSend(s);

    m.ledValue[chanel] = value;
    pwmPush[chanel] = pwm;
    ledPwm.setPWM(ledCh[chanel], 0, pwmPush[chanel]);

  } else if (division == "SET_TEMP") {
    m.setTemp = chanel / 10.0;
    Serial.println();
    Serial.print("Conv Temp : ");
    Serial.println(m.setTemp);
    // msg = "Conv Temp : ";
    msg += m.setTemp;
    lv_print(msg);
    s = "SET_TEMP:";
    s += m.setTemp;
    comSend(s);
  } else if (division == "MOTOR") {
    String msg = "MOTOR";

    if (value != 0) {
      m.motorValue[chanel] = value;
      if (chanel == 1)
        pwm = map(value, 20, 100, 20, 255);
      else
        pwm = map(value, 20, 100, 100, 255);
    }

    switch (chanel) {
    case 1: // 히터팬 이면...
      if (!value) {
        ledcWrite(motorCh[chanel], 0);
        m.motorFlag[chanel] = false;
        comSend("HEATER:0");
      } else {
        if (m.temperature <= m.setTemp) // 현재온도 보다 설정온도가 낮으면...
        {
          ledcWrite(motorCh[chanel], pwm);
          msg = "Heater Fan ON : ";
          msg += String(value);
        } else // 현재온도 보다 설정온도가 높으면...
        {
          ledcWrite(motorCh[chanel], 0);
        }
        m.motorFlag[chanel] = true;
        s = "HEATER:";
        s += value;
        comSend(s);
      }
      break;
    case 2: // 벙커 이면...
      if (!value) {
        ledcWrite(motorCh[chanel], 0);
        m.motorFlag[chanel] = false;
      } else {
        // ledcWrite(motorCh[chanel], pwm);
        ledcWrite(motorCh[chanel], 255);
        m.motorValue[chanel] = 255;
        m.motorFlag[chanel] = true;
        msg = "Heating 1 ON : ";
        msg += String(value);
      }
      s = "BUNKER:";
      s += m.motorFlag[chanel];
      comSend(s);
      break;
    case 3: // 거실 이면...
      if (!value) {
        ledcWrite(motorCh[chanel], 0);
        m.motorFlag[chanel] = false;
        comSend("LIVING:0");
      } else {
        ledcWrite(motorCh[chanel], pwm);
        m.motorFlag[chanel] = true;
        msg = "Heating 2 ON : ";
        msg += String(value);
        s = "LIVING:";
        s += value;
        comSend(s);
      }
      break;
    default:
      break;
    }
  } else if (division == "ALL" || division == "ALLSW") {
    int pwmCount = 0;

    // FullHouse 만 해당...
    digitalWrite(outCh[1], OFF);
    m.outOnOff[1] = false;

    // if (chanel == true) { // All LED On...
    if (value == 1) { // All LED On...
      pwmCount = 0;

      if (division == "ALL") {
        comSend("LED_ALL:1:1");
        if (m.sleepFlag)
          m.sleepFlag = false;

        while (pwmCount < 4095) {
          for (int i = 1; i < 6; i++) {
            if (pwmPush[i] < pwmCount)
              ledPwm.setPWM(ledCh[i], 0, pwmCount);
          }
          pwmCount += 120;
          delay(50);
        }
        pwmPush[1] = 4095;
        pwmPush[2] = 4095;
        pwmPush[3] = 4095;
        pwmPush[4] = 4095;
        pwmPush[5] = 4095;

        for (int i = 1; i < 6; i++) {
          ledPwm.setPWM(ledCh[i], 0, 4095);
          m.ledValue[i] = 100;
          m.ledFlag[i] = true;
        }
      } else if (division = "ALLSW") {
        for (int i = 1; i < 6; i++) {
          ledPwm.setPWM(ledCh[i], 0, 4095);
          m.ledFlag[i] = true;
        }
      }
      m.ledAllFlag = true;
    } else { // All LED Off...
      comSend("LED_ALL:0:0");
      pwmCount = 4095;
      for (int i = 1; i < 7; i++) {
        if (m.ledFlag[i] == true) {
          pwmPush[i] = map(m.ledValue[i], 1, 100, 3, 4095);
        } else
          pwmPush[i] = 0;
      }
      if (division == "ALL" && m.sleepFlag == false) {
        while (pwmCount > 0) {
          for (int i = 1; i < 7; i++) {
            if (pwmCount < 0)
              pwmPush[i] = 0;
            if (m.ledFlag[i] == true) {
              ledPwm.setPWM(ledCh[i], 0, pwmPush[i]);
              pwmPush[i] -= 120;
            }
          }
          pwmCount -= 120;
          delay(30);
        }
      } else if (division == "ALLSW" && m.sleepFlag == false) {
        for (int i = 1; i < 7; i++) {
          ledPwm.setPWM(ledCh[i], 0, 0);
        }
      }

      for (int i = 1; i < 7; i++) {
        ledPwm.setPWM(ledCh[i], 0, 0);
        m.ledFlag[i] = false;
      }
      m.ledAllFlag = false;
    }

    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "OUT") {
    String msg = "OUT ";
    switch (chanel) {
    case 1:
      msg += "1 : ";
      break;
    case 2:
      msg += "2 : ";
      break;
    case 3:
      msg += "3 : ";
      break;
    case 4:
      msg += "4 : ";
      break;
    case 5:
      msg += "5 : ";
      break;
    case 6:
      msg += "6 : ";
      break;
    default:
      break;
    }
    // msg += value;
    // lv_print(msg);

    if (value) {
      msg += "ON...";
      if (chanel == 2) { // 과 돌입 전류 예방...
        delay(100);
        digitalWrite(outCh[chanel], OFF);
        delay(100);
        digitalWrite(outCh[chanel], value);
      } else if (chanel == 5) { // 히터 출력은 500ms 후 Off
        // 베바스토 ON 조건...
        if (m.outOnOff[chanel] != value) {
          digitalWrite(HEATER, ON);
          delay(600);
          digitalWrite(HEATER, OFF);
        }
      } else
        digitalWrite(outCh[chanel], value);
    } else {
      msg += "OFF...";
      if (chanel == 5) { // 히터 출력은 500ms 후 Off
        // 베바스토 OFF 조건...
        if (m.outOnOff[chanel] != value) {
          digitalWrite(HEATER, ON);
          delay(600);
          digitalWrite(HEATER, OFF);
        }
      } else {
        digitalWrite(outCh[chanel], value);
      }
    }

    m.dataChange = 2;
    m.outOnOff[chanel] = value ? true : false;

    s = "OUT:";
    s += chanel;
    s += ':';
    s += m.outOnOff[chanel];
    comSend(s);

    s = "";
    s = s + "{\"result\":" + "\"Receive OK\"}";
  } else if (division == "INVERTOR") {
    m.dataChange = 2;
    if (value == 1) {
      m.invertorFlag = true;
      digitalWrite(INVERTOR, ON);
    } else {
      m.invertorFlag = false;
      digitalWrite(INVERTOR, OFF);
    }
    s = "INV:";
    s += m.invertorFlag;
    comSend(s);
    Serial.println(F("handleJson()..."));
    Serial.println(F("Invertor Process..."));
  } else if (division == "LIFT") {
    // m.dataChange = 2;
    if (value == 1)
      // pcf8574.digitalWrite(LIFT_POWER, ON);
      digitalWrite(LIFT_POWER, ON);
    else
      // pcf8574.digitalWrite(LIFT_POWER, OFF);
      digitalWrite(LIFT_POWER, OFF);

    Serial.println(F("handleJson()..."));
    Serial.println(F("LIFT Process..."));
    // s = "";
    // s = s + "{\"result\":" + "\"Receive OK\"}";
  }
  // else if (division == "clnSensor") {
  //   bool clntype = root["cleanType"];
  //   if (clntype == 0) // Clean Sensor Type : 240 ~ 33
  //   {
  //     EEPROM.write(0, false);
  //     m.cleanType = false;
  //     Serial.println(F("handleJson()..."));
  //     Serial.println(F("Clean Sensor Type : false"));
  //   } else // Clean Sensor Type : 0 ~ 190
  //   {
  //     EEPROM.write(0, true);
  //     m.cleanType = true;
  //     Serial.println(F("handleJson()..."));
  //     Serial.println(F("Clean Sensor Type : true"));
  //   }
  //   EEPROM.commit();

  //   Serial.println(F("handleJson()..."));
  //   Serial.print(F("EEPROM Clean Sensor : "));
  //   Serial.println(EEPROM.read(0));

  // }
  // else if (division == "wstSensor") {
  //   bool clntype = root["wasteType"];
  //   if (clntype == 0) // Waste Sensor Type : 240 ~ 33
  //   {
  //     EEPROM.write(1, false);
  //     m.wasteType = false;
  //     Serial.println(F("handleJson()..."));
  //     Serial.println(F("Waste Sensor Type : false"));
  //   } else // Waste Sensor Type : 0 ~ 190
  //   {
  //     EEPROM.write(1, true);
  //     m.wasteType = true;
  //     Serial.println(F("handleJson()..."));
  //     Serial.println(F("Waste Sensor Type : true"));
  //   }
  //   EEPROM.commit();
  //   Serial.println(F("handleJson()..."));
  //   Serial.print(F("EEPROM Waste Sensor : "));
  //   Serial.println(EEPROM.read(1));

  // }
  // else if (division == "read") {
  //   s = "";
  //   s = s + "{" + "\"cleanType\":" + m.cleanType + ", " + "\"wasteType\":"
  //   +
  //       m.wasteType + "}";
  // }
  // else if (division == "wifiSend") {
  //   String output = "";
  //   StaticJsonDocument<100> doc;

  //   if (chanel == 0) { // EEPROM의 SSID 와 PASSWORD 를 전송 한다...

  //     serializeJson(doc, output);

  //     Serial.println(F("handleJson()..."));
  //     Serial.print(F("JSON object Convert : "));
  //     Serial.println(output);
  //     Serial.println();
  //     Serial.print(F("'wifiSend' Receive from AT80X..."));
  //     s = output;
  //   } else if (chanel == 1) { // 전달 받은 SSID 와 PASSWORD 를 표시한다...

  //     String msg = "ID ";

  //     ssidBuffer = ssidParse.c_str();

  //     msg += ssidBuffer;

  //     ssidBuffer = passwordParse.c_str();
  //     msg = "PW ";
  //     msg += ssidBuffer;

  //     s = "";
  //     s = s + "{\"result\":" + "\"Receive OK\"}";
  //   }
  // }
  else if (division == "clnValve") {
    if (value == 1) {
      m.clnValveFlag = true;
      digitalWrite(CLEAN_VALVE, ON);
    } else {
      m.clnValveFlag = false;
      digitalWrite(CLEAN_VALVE, OFF);
    }
    s = "CLEAN:";
    s += m.clnValveFlag;
    comSend(s);
  } else if (division == "wstValve") {
    if (value == 1) {
      m.wstValveFlag = true;
      digitalWrite(WASTE_VALVE, ON);
    } else {
      m.wstValveFlag = false;
      digitalWrite(WASTE_VALVE, OFF);
    }
    s = "WASTE:";
    s += m.wstValveFlag;
    comSend(s);
  } else if (division == "sleep") {
    if (chanel == 1) {
      ledPwm.setPWM(ledCh[1], 0, OFF);
      ledPwm.setPWM(ledCh[2], 0, OFF);
      ledPwm.setPWM(ledCh[3], 0, OFF);
      ledPwm.setPWM(ledCh[4], 0, OFF);
      ledPwm.setPWM(ledCh[5], 0, OFF);
      ledPwm.setPWM(ledCh[6], 0, OFF);
      digitalWrite(outCh[1], OFF);
      m.ledAllFlag = false;
      m.ledFlag[1] = false;
      m.ledFlag[2] = false;
      m.ledFlag[3] = false;
      m.ledFlag[4] = false;
      m.ledFlag[5] = false;
      m.ledFlag[6] = false;
      m.outOnOff[1] = false;
      m.sleepFlag = true;
    } else {
      m.sleepFlag = false;
    }
    s = "SLEEP:0:";
    s += m.sleepFlag;
    comSend(s);
  } else if (division == "LAU") {
    (value == 1) ? lauFlag = true : lauFlag = false;
    Serial.println("Lift All Up...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = lauFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
    if (lauFlag) {
      l1uFlag = true;
      l2uFlag = true;
      l3uFlag = true;
      l4uFlag = true;
      l1dFlag = false;
      l2dFlag = false;
      l3dFlag = false;
      l4dFlag = false;
    } else {
      l1uFlag = false;
      l2uFlag = false;
      l3uFlag = false;
      l4uFlag = false;
      l1dFlag = false;
      l2dFlag = false;
      l3dFlag = false;
      l4dFlag = false;
    }
  } else if (division == "LAD") {
    (value == 1) ? ladFlag = true : ladFlag = false;
    Serial.println("Lift All Down...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = ladFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
    if (ladFlag) {
      l1dFlag = true;
      l2dFlag = true;
      l3dFlag = true;
      l4dFlag = true;
      l1uFlag = false;
      l2uFlag = false;
      l3uFlag = false;
      l4uFlag = false;
    } else {
      l1dFlag = false;
      l2dFlag = false;
      l3dFlag = false;
      l4dFlag = false;
      l1uFlag = false;
      l2uFlag = false;
      l3uFlag = false;
      l4uFlag = false;
    }
  } else if (division == "L1U") {
    (value == 1) ? l1uFlag = true : l1uFlag = false;
    Serial.println("Lift 1 Up...");
    // s = division + ':' + l1uFlag;

    // liftRoot["division"] = division;
    // liftRoot["value"] = l1uFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L1D") {
    (value == 1) ? l1dFlag = true : l1dFlag = false;
    Serial.println("Lift 1 Down...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l1dFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L2U") {
    (value == 1) ? l2uFlag = true : l2uFlag = false;
    Serial.println("Lift 2 Up...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l2uFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L2D") {
    (value == 1) ? l2dFlag = true : l2dFlag = false;
    Serial.println("Lift 2 Down...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l2dFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L3U") {
    (value == 1) ? l3uFlag = true : l3uFlag = false;
    Serial.println("Lift 3 Up...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l3uFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L3D") {
    (value == 1) ? l3dFlag = true : l3dFlag = false;
    Serial.println("Lift 3 Down...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l3dFlag;
    serializeJson(root, liftMsg);

    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L4U") {
    (value == 1) ? l4uFlag = true : l4uFlag = false;
    Serial.println("Lift 4 Up...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l4uFlag;
    serializeJson(root, liftMsg);

    s = liftMsg;

    // Serial2.println(s);
    comSend(s);
  } else if (division == "L4D") {
    (value == 1) ? l4dFlag = true : l4dFlag = false;
    Serial.println("Lift 4 Down...");

    // liftRoot["division"] = division;
    // liftRoot["value"] = l4dFlag;
    serializeJson(root, liftMsg);
    s = liftMsg;

    // s = division + ':' + l4dFlag;
    // Serial2.println(s);
    comSend(s);
  }

  serverSend();
  // server.send(200, "text/plain", s);

  Serial.println();
  Serial.println(F("handleJson()..."));
  Serial.print(F("Clean Type : "));
  Serial.print(m.cleanType);
  Serial.print(F(", Waste Type : "));
  Serial.println(m.wasteType);
  Serial.print(F("Sends s : "));
  Serial.print(s);
}

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  wiFiCountValue = 0;
  server.send(200, "text/plain", "");
  Serial.println(F("handleRoot()..."));
}

void handleRequest() {

  wiFiCountValue = 0;
  waterLevel();
  nowVolt();

  serverSend();

  // if (m.motorFlag[1]) // 히터팬이 True 이면...
  // {
  //   if (m.temperature <= m.setTemp) // 현재온도 보다 설정온도가 낮으면...
  //   {
  //     ledcWrite(motorCh[1], map(m.motorValue[1], 10, 100, 20, 1023));
  //   } else // 현재온도 보다 설정온도가 높으면...
  //   {
  //     ledcWrite(motorCh[1], 0);
  //   }
  // }

  if (!qrFlag) {
    comSend("QR_OK:1");
    qrFlag = true;
  }
}

void handleNetWork() {
  s = ssidAp;
  s += ',';
  s += passwordAp;
  // server.send(200, "text/plain", "SSID:CamTrol V5.0-T-078,PSWD:03240304078");
  server.send(200, "text/plain", s);
  Serial.println("Handle NetWork Receive...");
}

void stationConnect() {
  String msg = ".";
  Serial.println(F("stationConnect()..."));
  Serial.println(F("Connecting to Station(Hot Spot)..."));
  Serial.print(F("WiFi Status : "));
  Serial.println(WiFi.status());
  // }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    msg += ".";
    if (msg == "......................")
      msg = ".";
    Serial.print(".");
    delay(1000);
    msg += ".";
    if (msg == "......................")
      msg = ".";
  }
  Serial.println(F("stationConnect()..."));
  Serial.println(F("Station(Hot Spot) Connected OK..."));
  Serial.print(F("WiFi Status : "));
  Serial.println(WiFi.status());
}

void wifiStart() {
  String msg;

  WiFi.mode(WIFI_MODE_AP);

  Serial.println(F("setupWifi()..."));
  Serial.println(F("Access Point(WiFi) Mode Start..."));

  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAP(ssidAp, passwordAp); // Access Point mode begin...
  WiFi.softAPConfig(local_ip, gateway, subnet);

  Serial.println();
  Serial.print(F("ESP32 IP as soft AP : "));
  Serial.println(WiFi.softAPIP());
  lv_print("WiFi 시작...");
  lv_textarea_add_text(ui_TextArea1, ssidAp);
  lv_textarea_add_char(ui_TextArea1, '\n');
  lv_label_set_text(ui_SSIDLabel, ssidAp);
  lv_label_set_text(ui_PWLabel, passwordAp);
}

void factoryProccess(lv_event_t *e) {
  WiFi.softAP("CamTrol Factory", "1234567890");
  Serial.println("Start to CamTrol Factory...");
}

void setup() {
  pinMode(ENABLE, OUTPUT); // RS485 handle Pin...
  // pinMode(0, INPUT_PULLUP); // PCF8574 Interrupt Pin...
  // pinMode(INTERRUPTED_PIN, INPUT);
  pinMode(NTC, INPUT);

  pinMode(HT_MOTOR_1, OUTPUT);
  pinMode(HT_MOTOR_2, OUTPUT);

  pinMode(outCh[1], OUTPUT);
  pinMode(outCh[2], OUTPUT);
  pinMode(outCh[3], OUTPUT);
  pinMode(outCh[4], OUTPUT);
  pinMode(outCh[5], OUTPUT);
  pinMode(outCh[6], OUTPUT);

  pinMode(HEATER, OUTPUT);
  pinMode(INVERTOR, OUTPUT);
  pinMode(CLEAN_VALVE, OUTPUT);
  pinMode(WASTE_VALVE, OUTPUT);
  pinMode(LIFT_FLAG, INPUT_PULLUP);
  pinMode(LIFT_POWER, OUTPUT);
  pinMode(LIGHT_SWITCH, INPUT_PULLUP);

  ledcSetup(motorCh[1], 8000, 10);
  ledcSetup(motorCh[2], motorFreq, resolution);
  ledcSetup(motorCh[3], motorFreq, resolution);

  ledcAttachPin(HT_FAN, motorCh[1]);
  ledcAttachPin(HT_MOTOR_1, motorCh[2]);
  ledcAttachPin(HT_MOTOR_2, motorCh[3]);

  digitalWrite(ENABLE, LOW); // RS 수신

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

  I2Ctwo.begin(42, 2);
  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1, 4, 6); // 리프트는 19200

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

  ledPwm.begin();
  ledPwm.setPWMFreq(freq);
  // lv_print("출력 회로 정상...");
  // Serial.println(F("PCA9685 회로 정상..."));

  ledPwm.setPWM(0, 0, 0);
  ledPwm.setPWM(1, 0, 0);
  ledPwm.setPWM(2, 0, 0);
  ledPwm.setPWM(3, 0, 0);
  ledPwm.setPWM(4, 0, 0);
  ledPwm.setPWM(5, 0, 0);
  ledPwm.setPWM(6, 0, 0);

  aLow = EEPROM.readInt(200);  // A type Sensor LOW 값
  aHigh = EEPROM.readInt(210); // A type Sensor HIGH 값
  bLow = EEPROM.readInt(220);  // B type Sensor LOW 값
  bHigh = EEPROM.readInt(230); // B type Sensor HIGH 값

  // Serial.print("aLow: ");
  // Serial.print(aLow);
  // Serial.print(", aHigh: ");
  // Serial.print(aHigh);
  // Serial.print(", bLow: ");
  // Serial.print(bLow);
  // Serial.print(", bHigh: ");
  // Serial.println(bHigh);

  delay(100);

  // 연료 센서 Type 결정...
  if (EEPROM.read(0) == 0) {
    m.fuelType = false;
  } else {
    m.fuelType = true;
  }

  // 청수 센서 Type 결정...
  if (EEPROM.read(1) == 0) {
    m.cleanType = false;
  } else {
    m.cleanType = true;
  }

  // 폐수 센서 Type 결정...
  if (EEPROM.read(2) == 0) {
    m.wasteType = false;
  } else {
    m.wasteType = true;
  }

  // LED Power On Exe 처리...
  if (EEPROM.readBool(70)) {
    m.ledFlag[1] = true;
    eepLedFlag[1] = true;
  } else {
    m.ledFlag[1] = false;
    eepLedFlag[1] = false;
  }
  if (EEPROM.readBool(71)) {
    m.ledFlag[2] = true;
    eepLedFlag[2] = true;
  } else {
    m.ledFlag[2] = false;
    eepLedFlag[2] = false;
  }
  if (EEPROM.readBool(72)) {
    m.ledFlag[3] = true;
    eepLedFlag[3] = true;
  } else {
    m.ledFlag[3] = false;
    eepLedFlag[3] = false;
  }
  if (EEPROM.readBool(73)) {
    m.ledFlag[4] = true;
    eepLedFlag[4] = true;
  } else {
    m.ledFlag[4] = false;
    eepLedFlag[4] = false;
  }
  if (EEPROM.readBool(74)) {
    m.ledFlag[5] = true;
    eepLedFlag[5] = true;
  } else {
    m.ledFlag[5] = false;
    eepLedFlag[5] = false;
  }
  if (EEPROM.readBool(75)) {
    m.ledFlag[6] = true;
    eepLedFlag[6] = true;
  } else {
    m.ledFlag[6] = false;
    eepLedFlag[6] = false;
  }

  if (EEPROM.readBool(80)) {
    m.outOnOff[1] = true;
    eepOutFlag[1] = true;
  } else {
    m.outOnOff[1] = false;
    eepOutFlag[1] = false;
  }

  voltAdjust = EEPROM.readFloat(VOLT_ADJ_EEPROM);
  if (isnan(voltAdjust)) {
    voltAdjust = 0.0;
    EEPROM.writeFloat(VOLT_ADJ_EEPROM, 0.0);
    EEPROM.commit();
  }

  // OUT Power On Exe 처리...
  // m.outOnOff[1] = EEPROM.readBool(80);
  m.outOnOff[2] = EEPROM.readBool(81);
  m.outOnOff[3] = EEPROM.readBool(82);
  m.outOnOff[4] = EEPROM.readBool(83);
  m.outOnOff[5] = EEPROM.readBool(84);
  m.outOnOff[6] = EEPROM.readBool(85);

  digitalWrite(outCh[1], m.outOnOff[1]);
  digitalWrite(outCh[2], m.outOnOff[2]);
  digitalWrite(outCh[3], m.outOnOff[3]);
  digitalWrite(outCh[4], m.outOnOff[4]);
  digitalWrite(outCh[5], m.outOnOff[5]);
  digitalWrite(outCh[6], m.outOnOff[6]);

  Serial.print("ledFlag: ");
  Serial.print(m.ledFlag[1]);
  Serial.print(", ");
  Serial.print(m.ledFlag[2]);
  Serial.print(", ");
  Serial.print(m.ledFlag[3]);
  Serial.print(", ");
  Serial.print(m.ledFlag[4]);
  Serial.print(", ");
  Serial.print(m.ledFlag[5]);
  Serial.print(", ");
  Serial.println(m.ledFlag[6]);

  // 전원 투입 시 ON 처리...
  int bright = 0;
  while (bright < 4095) {
    if (m.ledFlag[1])
      ledPwm.setPWM(ledCh[1], 0, bright);
    if (m.ledFlag[2])
      ledPwm.setPWM(ledCh[2], 0, bright);
    if (m.ledFlag[3])
      ledPwm.setPWM(ledCh[3], 0, bright);
    if (m.ledFlag[4])
      ledPwm.setPWM(ledCh[4], 0, bright);
    if (m.ledFlag[5])
      ledPwm.setPWM(ledCh[5], 0, bright);
    if (m.ledFlag[6])
      ledPwm.setPWM(ledCh[6], 0, bright);
    delay(30);
    bright += 120;
  }

  if (m.ledFlag[1]) {
    ledPwm.setPWM(ledCh[1], 0, 4095);
    pwmPush[1] = 4095;
  }
  if (m.ledFlag[2]) {
    ledPwm.setPWM(ledCh[2], 0, 4095);
    pwmPush[2] = 4095;
  }
  if (m.ledFlag[3]) {
    ledPwm.setPWM(ledCh[3], 0, 4095);
    pwmPush[3] = 4095;
  }
  if (m.ledFlag[4]) {
    ledPwm.setPWM(ledCh[4], 0, 4095);
    pwmPush[4] = 4095;
  }
  if (m.ledFlag[5]) {
    ledPwm.setPWM(ledCh[5], 0, 4095);
    pwmPush[5] = 4095;
  }
  if (m.ledFlag[6]) {
    ledPwm.setPWM(ledCh[6], 0, 4095);
    pwmPush[6] = 4095;
  }

  s = EEPROM.readString(AP_SSID_EEPROM);
  s.toCharArray(_label_text, s.length());
  lv_label_set_text(ui_LabelSSID, _label_text);
  s.toCharArray(ssidAp, s.length());
  Serial.print("ssidAp : ");
  Serial.print(s);

  s = EEPROM.readString(AP_PSWD_EEPROM);
  s.toCharArray(_label_text, s.length());
  lv_label_set_text(ui_LabelPSWD, _label_text);
  s.toCharArray(passwordAp, s.length());
  Serial.print(", passwordAp : ");
  Serial.println(s);

  // Serial.print("SSID : ");
  // Serial.println(s);

  // s = lv_label_get_text(ui_PWLabel);

  // Serial.print("PSWD : ");
  // Serial.println(s);

  wifiStart();

  // Serial.println();
  // Serial.println(F("Full House Camping Car Version V5.0..."));
  // Serial.println();
  // Serial.println(F("ESP32 Program Source by   "
  //                  "'E:\\CamTrol_V3.6\\Full_house\\Fullhouse_esp32_V3.6'"));
  // Serial.println(
  //     F("Nextion Program Source by "
  //       "'E:\\CamTrol_V3.6\\Full_house\\CamTrol_esp32_V3.6_Nextion'"));
  // Serial.println(F("Flutter Program Source by "
  //                  "'E:\\CamTrol_V3.6\\Full_house\\Fullhouse_flutter_v3.6'"));
  // Serial.println();
  // Serial.println(F("KiCad Main board Source by  'C:\\Users\\Lee "
  //                  "JunHo\\Desktop\\업무\\CamTrol Final\\CamTrol "
  //                  "V3.6\\mini\\12ch\\FullHouse-Main Board'"));
  // Serial.println(F("KiCad Input board Source by 'C:\\Users\\Lee "
  //                  "JunHo\\Desktop\\업무\\CamTrol Final\\CamTrol "
  //                  "V3.6\\mini\\12ch\\FullHouse-Input Board'"));
  // Serial.println(F("KiCad Cover board Source by 'C:\\Users\\Lee "
  //                  "JunHo\\Desktop\\업무\\CamTrol Final\\CamTrol "
  //                  "V3.6\\mini\\12ch\\FullHouse-Cover Board'"));
  // Serial.println();
  // Serial.println(F("Make by 'CamTrol LTD,.' Lee JunHo...  2022,05,29"));
  // Serial.println();
  // Serial.println();
  // Serial.print(F("clean Sensor Type : "));
  // Serial.print(m.cleanType);
  // Serial.print(F(", waste Sensor Type : "));
  // Serial.print(m.wasteType);

  waterLevel();
  nowVolt();

  String output = "";
  StaticJsonDocument<100> doc;
  serializeJson(doc, output);

  Serial.println();
  String LVGL_Arduino = "풀하우스캠핑카 콘트롤러! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch() + '\n';

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");
  lv_print("풀하우스캠핑카 콘트롤러 시작...");

  Serial.println("Setup done.....");
  Serial.println();

  server.on("/root", handleRoot);
  server.on("/json", handleJson);
  server.on("/Request", handleRequest);
  server.on("/NetWork", handleNetWork);

  server.begin();

  Serial.print("Volt eeprom Float Value:");
  Serial.println(voltAdjust);

  // lv_obj_add_flag(ui_QRImage, LV_OBJ_FLAG_HIDDEN);
  // lv_obj_add_flag(ui_Roller2, LV_OBJ_FLAG_HIDDEN);

} // setup()

void loop() {
  unsigned long currentMillis = millis();

  lv_timer_handler(); /* let the GUI do its work */
  server.handleClient();

  if (millis() - previousMillis >= 1500) {
    // char _label_text[20];
    previousMillis = millis();

    waterLevel();
    nowVolt();
    // if (pcf8574.digitalRead(LIFT_FLAG) == true)
    if (digitalRead(LIFT_FLAG) == true)
      liftFlag = false;
    else
      liftFlag = true;

    if (screenNumber == 1) {
      s = "전압: ";
      s += String(m.aux_v, 1);
      s += 'V';
      s.toCharArray(_label_text, sizeof(_label_text));
      lv_label_set_text(ui_LabelVolt, _label_text);

      s = "연료: ";
      if (m.fuel == 255)
        s += "--";
      else
        s += m.fuel;
      s += '%';
      s.toCharArray(_label_text, sizeof(_label_text));
      lv_label_set_text(ui_LabelFuel, _label_text);

      s = "청수: ";
      if (m.clean == 255)
        s += "--";
      else
        s += m.clean;
      s += '%';
      s.toCharArray(_label_text, sizeof(_label_text));
      lv_label_set_text(ui_LabelClean, _label_text);

      s = "오수: ";
      if (m.waste == 255)
        s += "--";
      else
        s += m.waste;
      s += '%';
      s.toCharArray(_label_text, sizeof(_label_text));
      lv_label_set_text(ui_LabelWaste, _label_text);
    } else if (screenNumber == 5) {
      String _value;
      char _ui_value[10];

      _value = String(clean_adc);
      _value.toCharArray(_ui_value, sizeof(_ui_value));
      lv_label_set_text(ui_sensorNowValue, _ui_value);
    }
    if (m.motorFlag[1]) // 히터팬이 True 이면...
    {
      if (m.temperature <=
          m.setTemp + 0.5) // 현재온도 보다 설정온도가 낮으면...
      {
        ledcWrite(motorCh[1], map(m.motorValue[1], 10, 100, 20, 1023));
      } else if (m.temperature >=
                 m.setTemp - 0.5) // 현재온도 보다 설정온도가 높으면...
      {
        ledcWrite(motorCh[1], 0);
      }
    }
  }

  if (!digitalRead(LIGHT_SWITCH)) {
    delay(50);
    if (!digitalRead(LIGHT_SWITCH)) {
      if (extSwFlag) // 조명 On...
        extSwFlag = false;
      else // 조명 Off...
        extSwFlag = true;
      keyPressedOn();
    }
  }

  delay(5);
}