
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

#include "Arduino.h"
#include "ui/ui.h" // this is the ui generated with lvgl / squareline editor
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define TFT_BL 2

#define RTS 11 // RS485 Enable

#define ON 1
#define OFF 0

#define CHECKED 3
#define UNCHECKED 2

#define SLAVE_MODE false
#define MASTER_MODE true

#define STATION_MODE true
#define AP_MODE false

#define MASTER_SLAVE_EEPROM 2 // bool
#define LOGO_CHANGE_EEPROM 4  // bool

#define SSID_HOTSPOT_EEPROM 110 // HOT Spot SSID String
#define PSWD_HOTSPOT_EEPROM 160 // HOT Spot PSWD String

void eventSec();
void wifiStart();
void comSend(String sendData);

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, ssidHotSpot, pswdHotSpot, hotSpotIP;

// WebServer server(80);

bool logoFlag = true;
bool dataReceiveFlag = false; // 최초 Data 수신 Check...
bool comInitFlag = false;
bool masterModeFlag = false;
bool networkFlag = false;
bool networkChangeFlag = false;
bool hotSpotIpSetFlag = false;

bool btn_1_stop = false;
bool btn_1_left = false;
bool btn_1_right = false;
bool btn_2_stop = false;
bool btn_2_left = false;
bool btn_2_right = false;
bool btn_3_stop = false;
bool btn_3_left = false;
bool btn_3_right = false;

bool btn_flag = false;

bool logoChangeFlag;

char ssidAp[30] = "엘엠 V1.0-LM6-001";
char passwordAp[30] = "17240719001";

char msg[100];
char label_text[50];

uint8_t loopCount = 0;
uint8_t readStatus = 0;
uint8_t h1_LED, h2_LED = 0;
uint8_t bgValue = 255;
uint8_t bgValue_push = 255;

uint16_t lvState;
uint16_t logoCounter = 0;

byte logoChangeCounter = 0;

int wifiCount = 10;
int countValue = 0;
int rs485ComCount = 1000;

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

// ESP32-8048S050 IPS LCD 800x480
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */,
    GFX_NOT_DEFINED /* SDA */, 40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */,
    42 /* PCLK */, 45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */,
    14 /* R4 */, 5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */,
    4 /* G5 */, 8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
    );

// ESP32-8048S050 IPS LCD 800x480
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus, 800 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */,
    4 /* hsync_pulse_width */, 8 /* hsync_back_porch */, 480 /* height */,
    0 /* vsync_polarity */, 8 /* vsync_front_porch */,
    4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 14000000 /* prefer_speed */,
    true /* auto_flush */);

#endif /* !defined(DISPLAY_DEV_KIT) */

#include "touch.h"

void logoChangeClick(lv_event_t *e) {
  logoChangeCounter++;
  Serial.println("Logo Clicked...");
  Serial.print("Logo Counter : ");
  Serial.println(logoChangeCounter);
  if (logoChangeCounter > 10) {
    if (logoChangeFlag) {
      logoChangeFlag = false;
      lv_disp_load_scr(ui_ScreenLogo);
    } else {
      logoChangeFlag = true;
      lv_disp_load_scr(ui_ScreenLogo2);
    }

    logoChangeCounter = 0;
    EEPROM.writeBool(LOGO_CHANGE_EEPROM, logoChangeFlag);
    EEPROM.commit();
  }
  delay(100);
}

void logoClick(lv_event_t *e) {
  Serial.println("Logo Long click...");

  // logoFlag = true;
  ledcWrite(0, 50);
  if (logoChangeFlag)
    lv_disp_load_scr(ui_ScreenLogo2);
  else
    lv_disp_load_scr(ui_ScreenLogo);
  // logoCounter = 0;
}

void logoFlagReset(lv_event_t *e) {
  Serial.println("Logo click...");

  lv_disp_load_scr(ui_ScreenHome);
  ledcWrite(0, bgValue_push);
  logoCounter = 0;
  comSend("READY:1");
}

void logoReset() {
  Serial.println("Logo Reset...");

  lv_disp_load_scr(ui_ScreenHome);
  ledcWrite(0, bgValue_push);
  logoFlag = true;
  logoCounter = 0;
}

void bright_save(lv_event_t *e) {
  Serial.println("Bright Save...");
  bgValue = bgValue_push;
}

void bright_cancel(lv_event_t *e) {
  Serial.println("Bright Save...");
  ledcWrite(0, bgValue);
}

void BGBright(lv_event_t *e) { // 화면 밝기 조절...
  bgValue_push = lv_slider_get_value(ui_Slider3);
  Serial.println("Bright Slider... ");
  Serial.println(bgValue_push);
  if (bgValue_push < 10)
    ledcWrite(0, 2); // 제일 낮은 밝기임...
  else if (bgValue_push > 250)
    ledcWrite(0, 255);
  else
    ledcWrite(0, bgValue_push);
  bgValue = bgValue_push;
}

void button1Left(lv_event_t *e) {
  Serial.println("Button 1 Gesture Left...");
  logoChangeCounter++;
  logoCounter = 0;

  btn_1_stop = false;
  btn_1_right = false;
  btn_1_left = true;
  comSend("BTN1_LEFT:1");
}

void button1Right(lv_event_t *e) {
  Serial.println("Button 1 Gesture Right...");
  logoChangeCounter++;
  logoCounter = 0;

  btn_1_stop = false;
  btn_1_left = false;
  btn_1_right = true;
  comSend("BTN1_RIGHT:1");
}

void Button1Lost(lv_event_t *e) {
  Serial.println("Button 1 Click...");

  btn_1_left = false;
  btn_1_right = false;
  btn_1_stop = true;
  comSend("BTN1_STOP:1");
}

void button2Down(lv_event_t *e) {
  Serial.println("Button 2 Gesture left...");
  logoCounter = 0;
  logoChangeCounter++;
  btn_3_stop = false;
  btn_3_left = false;
  btn_3_right = true;
  comSend("BTN3_RIGHT:1");
}

void button2Up(lv_event_t *e) {
  Serial.println("Button 2 Gesture right...");
  logoChangeCounter++;
  logoCounter = 0;

  btn_3_stop = false;
  btn_3_right = false;
  btn_3_left = true;
  comSend("BTN3_LEFT:1");
}

void Button2Lost(lv_event_t *e) {
  Serial.println("Button 2 Click...");
  logoCounter = 0;

  btn_3_left = false;
  btn_3_right = false;
  btn_3_stop = true;
  comSend("BTN3_STOP:1");
}

void button3Up(lv_event_t *e) {
  Serial.println("Button 3 Gesture left...");
  logoChangeCounter++;
  logoCounter = 0;

  btn_2_stop = false;
  btn_2_right = false;
  btn_2_left = true;
  comSend("BTN2_DOWN:1");
}

void button3Down(lv_event_t *e) {
  Serial.println("Button 3 Gesture right...");
  logoChangeCounter++;
  logoCounter = 0;

  // btn_3_stop = false;
  // btn_3_left = false;
  // btn_3_right = true;
  // comSend("BTN3_RIGHT:1");

  btn_2_stop = false;
  btn_2_left = false;
  btn_2_right = true;
  comSend("BTN2_UP:1");
}

void Button3Lost(lv_event_t *e) {
  Serial.println("Button 3 Click...");
  logoCounter = 0;

  // btn_3_left = false;
  // btn_3_right = false;
  // btn_3_stop = true;
  // comSend("BTN3_STOP:1");

  btn_2_left = false;
  btn_2_right = false;
  btn_2_stop = true;
  comSend("BTN2_STOP:1");
}

void heater1_click(lv_event_t *e) {
  Serial.println("Heater 1 Click...");
  logoChangeCounter++;
  logoCounter = 0;

  comSend("HEAT1:1");
}

void heater2_click(lv_event_t *e) {
  Serial.println("Heater 2 Click...");
  logoChangeCounter++;
  logoCounter = 0;

  comSend("HEAT2:1");
}

void memoryCall(lv_event_t *e) {
  Serial.println("Memory Call Display...");
  logoCounter = 0;

  lv_obj_clear_state(ui_ButtonSet, LV_STATE_CHECKED);
  // s = "호출";
  // s.toCharArray(label_text, s.length() + 1);
  // lv_label_set_text(ui_Label2, label_text);
}

// Panel1, ButtonSet
void memory_save(lv_event_t *e) {
  logoCounter = 0;
  Serial.print("Memory Save Button : ");
  Serial.println(lv_obj_get_state(ui_ButtonSet));
  Serial.print("Memory Save Button : ");
  Serial.println(lv_obj_get_state(ui_ButtonSet));
}

// Panel1, ButtonSave1
void memory_1(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Memory 1 Button...");

  Serial.print("Button Set Status : ");
  Serial.println(lv_obj_get_state(ui_ButtonSet));

  if (lv_obj_get_state(ui_ButtonSet)) {
    comSend("MEMORY1:1");
  } else {
    comSend("CALL1:1");
  }
  lv_obj_add_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_state(ui_ButtonSet, LV_STATE_CHECKED);
}

// Panel1, ButtonSave2
void memory_2(lv_event_t *e) {
  logoCounter = 0;
  Serial.println("Memory 2 Button...");

  Serial.print("Button Set Status : ");
  Serial.println(lv_obj_get_state(ui_ButtonSet));

  if (lv_obj_get_state(ui_ButtonSet)) {
    comSend("MEMORY2:1");
  } else {
    comSend("CALL2:1");
  }
  lv_obj_add_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_state(ui_ButtonSet, LV_STATE_CHECKED);
}

void handleRoot() { // Main 화면이 아닐 때 Dummy 통신...
  Serial.println("handleRoot...");
  // server.send(200, "text/plain", "");
}

void handleJson() {
  Serial.println("handleJson...");

  // server.arg("j").toCharArray(msg, 2000);
  Serial.println();
  Serial.println(F("handleJson()..."));
  Serial.println(msg);
  deserializeJson(doc, msg);
  root = doc.as<JsonObject>();

  String division = root["division"];
  String ssidParse = root["ssid"];
  String passwordParse = root["password"];
  // String _ledName = root["LED_NAME"];
  String nowTime = root["NOW_TIME"];
  int chanel = root["Chanel"];
  int value = root["Value"];

  if (division == "QR_OK") {
    lv_disp_load_scr(ui_ScreenHome);
  }

  s = "";
  s = s + "{\"result\":" + "\"Receive OK\"}";
  // server.send(200, "text/plain", s);

} // handleJson()

void handleRequest() {

  wifiCount = 0;

  Serial.println("handleRequest...");

  s = "";
  s = s + "{" + "\"BTN_1_LEFT\":" + btn_1_left + ", " + "\"BTN_1_RIGHT\":" +
      btn_1_right + ", " + "\"BTN_1_STOP\":" + btn_1_stop + ", " +
      "\"BTN_2_LEFT\":" + btn_2_left + ", " + "\"BTN_2_RIGHT\":" + btn_2_right +
      ", " + "\"BTN_2_STOP\":" + btn_2_stop + ", " + "\"BTN_3_LEFT\":" +
      btn_3_left + ", " + "\"BTN_3_RIGHT\":" + btn_3_right + ", " +
      "\"BTN_3_STOP\":" + btn_3_stop + ", " + "\"H1_LED\":" + h1_LED + ", " +
      "\"H2_LED\":" + h2_LED + "}";

  // server.send(200, "text/plain", s);
  Serial.print("s : ");
  Serial.println(s);

} // handleRequest()

void ScreenHomeLoaded(lv_event_t *e) { Serial.println("ScreenHomeLoaded..."); }

void wifiClick(lv_event_t *e) {
  Serial.println("WiFi Click...");
  if (networkFlag == STATION_MODE) {
    hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
    lv_label_set_text(ui_LabelMessage, label_text);
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
  }
}

void masterModeSelect(lv_event_t *e) {

  Serial.println("MasterModeSelect... ");

  if (lv_obj_get_state(ui_Switch1) == CHECKED) {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, MASTER_MODE);
    masterModeFlag = MASTER_MODE;
    Serial.println("Master Mode Selected...");

    lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSL, LV_OBJ_FLAG_HIDDEN);

    if (networkFlag == AP_MODE) {
      lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    }

  } else {
    EEPROM.writeBool(MASTER_SLAVE_EEPROM, SLAVE_MODE);
    masterModeFlag = SLAVE_MODE;
    Serial.println("Slave Mode Selected...");

    lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_LabelSL, LV_OBJ_FLAG_HIDDEN);
  }
  EEPROM.commit();
}

void panelMessageClick(lv_event_t *e) { Serial.print("PanelMessage Click..."); }

void comSend(String sendData) {
  digitalWrite(RTS, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(RTS, LOW);
}

void brightQR(lv_event_t *e) {
  uint8_t opa = lv_slider_get_value(ui_Slider2);
  Serial.print("brightQR... ");
  Serial.println(opa);
  lv_obj_set_style_opa(ui_ImageQR, opa, 0);
}

void serialEvent2() {
  Serial.println("serialEvent2...");
  // comInitFlag = true;
  // rs485ComCount = 0;

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

  if (_division == "READY") {
    if (!logoFlag)
      logoReset();
  } else if (_division == "BTN1_LEFT" || _division == "BTN1_RIGHT") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    lv_obj_add_state(ui_Button1, LV_STATE_CHECKED); /// States
  } else if (_division == "BTN1_STOP") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED); /// States
  } else if (_division == "BTN2_UP" || _division == "BTN2_DOWN") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    lv_obj_add_state(ui_Button3, LV_STATE_CHECKED); /// States
  } else if (_division == "BTN2_STOP") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED); /// States
  } else if (_division == "BTN3_LEFT" || _division == "BTN3_RIGHT") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    lv_obj_add_state(ui_Button2, LV_STATE_CHECKED); /// States
  } else if (_division == "BTN3_STOP") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED); /// States
  } else if (_division == "H1_LED") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    h1_LED = _value.toInt();
    Serial.print("H1_LED : ");
    Serial.println(_value);
    switch (h1_LED) {
    case 0:
      lv_obj_set_style_bg_opa(ui_ContainerL1, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL2, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL3, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 7:
      lv_obj_set_style_bg_opa(ui_ContainerL1, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL2, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL3, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 5:
      lv_obj_set_style_bg_opa(ui_ContainerL1, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL2, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL3, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 1:
      lv_obj_set_style_bg_opa(ui_ContainerL1, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL2, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerL3, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;

    default:
      break;
    }
  } else if (_division == "H2_LED") {
    if (!logoFlag)
      logoReset();
    comInitFlag = true;
    rs485ComCount = 0;
    h2_LED = _value.toInt();
    Serial.print("H2_LED : ");
    Serial.println(_value);
    switch (h2_LED) {
    case 0:
      lv_obj_set_style_bg_opa(ui_ContainerR1, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR2, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR3, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 7:
      lv_obj_set_style_bg_opa(ui_ContainerR1, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR2, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR3, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 5:
      lv_obj_set_style_bg_opa(ui_ContainerR1, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR2, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR3, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    case 1:
      lv_obj_set_style_bg_opa(ui_ContainerR1, 255,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR2, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_ContainerR3, 0,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
      break;
    default:
      break;
    }
  } else if (_division == "BTN1") {
    if (!logoFlag)
      logoReset();
    Serial.print("Button Flag : ");
    Serial.print(_value);
    Serial.println("...");
    if (_value == "1") {
      btn_flag = true;
      lv_obj_add_state(ui_Button1, LV_STATE_CHECKED);
    } else {
      btn_flag = false;
      lv_obj_clear_state(ui_Button1, LV_STATE_CHECKED);
    }
  } else if (_division == "BTN2") {
    if (!logoFlag)
      logoReset();
    if (_value == "1") {
      btn_flag = true;
      lv_obj_add_state(ui_Button2, LV_STATE_CHECKED);
    } else {
      btn_flag = false;
      lv_obj_clear_state(ui_Button2, LV_STATE_CHECKED);
    }
  } else if (_division == "BTN3") {
    if (!logoFlag)
      logoReset();
    if (_value == "1") {
      btn_flag = true;
      lv_obj_add_state(ui_Button3, LV_STATE_CHECKED);
    } else {
      btn_flag = false;
      lv_obj_clear_state(ui_Button3, LV_STATE_CHECKED);
    }
  } else if (_division == "QR_OK") {
    lv_disp_load_scr(ui_ScreenHome);
  } else if (_division == "COM") {
    comInitFlag = true;
    rs485ComCount = 0;
  }
}

void wifiStart() { // WiFi Mode Start...
  String msg;

  if (networkFlag == AP_MODE) {
    // WiFi.mode(WIFI_MODE_AP);

    Serial.println(F("setupWifi()..."));
    Serial.println(F("Access Point(WiFi) Mode Start..."));

    // IPAddress local_ip(192, 168, 4, 1);
    // IPAddress gateway(192, 168, 4, 1);
    // IPAddress subnet(255, 255, 255, 0);
    // WiFi.softAP(ssidAp, passwordAp); // Access Point mode begin...
    // WiFi.softAPConfig(local_ip, gateway, subnet);

    Serial.println();
    Serial.print("SSID: ");
    Serial.print(ssidAp);
    Serial.print(", PSWD: ");
    Serial.println(passwordAp);
    Serial.print(F("ESP32 IP as soft AP : "));
    // Serial.println(WiFi.softAPIP());
  } else {
    Serial.println(F("setupWifi()..."));
    Serial.println(F("Hot Spot Mode Start..."));
    Serial.println();
    Serial.print("Hot Spot SSID: ");
    Serial.print(ssidHotSpot);
    Serial.print(", Hot Spot PSWD: ");
    Serial.println(pswdHotSpot);

    // WiFi.mode(WIFI_STA);
    // WiFi.disconnect();
    delay(100);

    // WiFi.begin(ssidHotSpot, pswdHotSpot);
    Serial.print("Connecting to WiFi ..");
    // lv_obj_clear_flag(ui_ImageHotSpot,
    //                   LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    // s = "핫스팟 연결중 입니다...";
    // s.toCharArray(label_text, s.length() + 1);
    lv_label_set_text(ui_LabelMessage, label_text);
    lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);

    for (int i = 1; i < 10; i++) {
      // s = i;
      // s.toCharArray(label_text, s.length() + 1);
      // lv_label_set_text(ui_LabelSecCount, label_text);
      // if (WiFi.status() == WL_CONNECTED)
      //   break;
      lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      lv_timer_handler();
      delay(900);
      lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
      lv_timer_handler();
      delay(100);

      Serial.print('.');
    }

    // if (WiFi.status() == WL_CONNECTED) {
    //   hotSpotIP = WiFi.localIP().toString();
    //   hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
    //   lv_label_set_text(ui_LabelMessage, label_text);
    //   Serial.println();
    //   Serial.print("HotSpotIP : ");
    //   Serial.println(hotSpotIP);
    // } else {
    //   s = "핫스팟 네트워크 정보 확인...";
    //   s.toCharArray(label_text, s.length() + 1);
    //   lv_label_set_text(ui_LabelMessage, label_text);
    // }
    // lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);

    // Serial.println();
    // Serial.println("\nConnected to the WiFi network");
    // Serial.print("Local ESP32 IP: ");
    // Serial.println(WiFi.localIP());
    // Serial.print("Subnet Mask: ");
    // Serial.println(WiFi.subnetMask());
    // Serial.print("Gateway IP: ");
    // Serial.println(WiFi.gatewayIP());
    // Serial.print("DNS 1: ");
    // Serial.println(WiFi.dnsIP(0));
    // Serial.print("DNS 2: ");
    // Serial.println(WiFi.dnsIP(1));
  }
}

void eventSec() {
  Serial.println("1 Sec Event...");

  if (countValue++ > 1) {

    if (wifiCount > 2) { // WiFi 가 연결 안되어 있으면...
      lv_obj_add_flag(ui_ImageWiFi,
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
}

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

      // Serial.print("Data x ");
      // Serial.println(touch_last_x);

      // Serial.print("Data y ");
      // Serial.println(touch_last_y);
    } else if (touch_released()) {
      data->state = LV_INDEV_STATE_REL;
    }
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  pinMode(RTS, OUTPUT); // RS485 Enable Pin

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial2.begin(115200, SERIAL_8N1, 12, 13);

  Serial.flush();
  Serial2.flush();

  digitalWrite(RTS, LOW);

  EEPROM.begin(250);

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  // EEPROM Read Process...
  ssidHotSpot = EEPROM.readString(SSID_HOTSPOT_EEPROM);
  pswdHotSpot = EEPROM.readString(PSWD_HOTSPOT_EEPROM);

  masterModeFlag = EEPROM.readBool(MASTER_SLAVE_EEPROM);
  logoChangeFlag = EEPROM.readBool(LOGO_CHANGE_EEPROM);

  // Init Display
  lv_init();
  gfx->begin();
  touch_init();

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  ledcSetup(0, 240, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, 255);
#endif

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
    Serial.println("LVGL disp_draw_buf allocate failed!");
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

    ui_init();

    lv_obj_add_flag(ui_PanelMessage,
                    LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);

    // 열선의 Lamp 1, 2, 3 Back ground Off...
    lv_obj_set_style_bg_opa(ui_ContainerL1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerL2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerL3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerR1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerR2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ContainerR3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (networkFlag) // true : Tethering Mode, false : WiFi
    {                // 핫스팟 Mode...
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

      s = "와이파이 비밀번호:";
      s.toCharArray(label_text, s.length() + 1);
      lv_label_set_text(ui_Label24, label_text);
    }

    if (masterModeFlag) // true : aster Mode, false : Slave Mode
    {
      lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED);
      lv_obj_clear_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelSL, LV_OBJ_FLAG_HIDDEN);

      if (networkFlag == AP_MODE) {
        lv_obj_clear_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
      }
    } else {
      lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
      lv_obj_add_flag(ui_ImageQR, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label23, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelSSID, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Label24, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_LabelPSWD, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_Slider2, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(ui_ImageWiFi, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(ui_LabelSL, LV_OBJ_FLAG_HIDDEN);
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

    comSend("COM1:1");
    delay(10);

    s = "AP_SSID:";
    s += ssidAp;
    comSend(s);
    delay(10);

    s = "AP_PSWD:";
    s += passwordAp;
    comSend(s);
    delay(10);

    Serial.println("Setup done");

    comSend("REQUEST:1");
    Serial2.flush();
  }
}

void loop() {

  // 최초 Data 수신이 false 면 재 요청...
  if (!dataReceiveFlag) {
    comSend("REQUEST:1");
    dataReceiveFlag = true;
  }

  if (!btn_flag)
    lv_timer_handler(); /* let the GUI do its work */
  // server.handleClient();

  if (loopCount > 180) { // 약 1초 마다 실실행...

    // if (networkChangeFlag) {
    //   WiFi.disconnect();
    //   wifiStart();
    //   networkChangeFlag = false;
    // }

    // if ((networkFlag == STATION_MODE) && (WiFi.status() != WL_CONNECTED)) {
    //   hotSpotIpSetFlag = false;
    //   WiFi.begin(ssidHotSpot, pswdHotSpot);
    //   Serial.println("Reconnecting to WiFi...");
    // } else if (!hotSpotIpSetFlag && (WiFi.status() == WL_CONNECTED)) {
    //   hotSpotIpSetFlag = true;
    //   hotSpotIP = WiFi.localIP().toString();
    //   hotSpotIP.toCharArray(label_text, hotSpotIP.length() + 1);
    //   lv_label_set_text(ui_LabelMessage, label_text);
    //   Serial.print("Loop HotSpotIP : ");
    //   Serial.println(hotSpotIP);
    //   lv_obj_clear_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
    //   lv_timer_handler();
    //   delay(2000);
    //   lv_obj_add_flag(ui_PanelMessage, LV_OBJ_FLAG_HIDDEN);
    // }
    loopCount = 0;
    // if (masterModeFlag == MASTER_MODE)
    //   comSend("COM:1");
  }

  if (rs485ComCount == 0) {
    lv_obj_clear_flag(ui_ImageNetwork,
                      LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
  }
  if (rs485ComCount > 300) {
    lv_obj_add_flag(ui_ImageNetwork,
                    LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);
    rs485ComCount = 10;
    comInitFlag = false;
  }

  if (logoCounter > 15000) {
    ledcWrite(0, 50);
    if (logoChangeFlag)
      lv_disp_load_scr(ui_ScreenLogo2);
    else
      lv_disp_load_scr(ui_ScreenLogo);
    logoFlag = false;
    if (logoCounter > 60000)
      logoCounter = 16000;
  }

  delay(5);
  loopCount++;
  logoCounter++;
  rs485ComCount++;
}
