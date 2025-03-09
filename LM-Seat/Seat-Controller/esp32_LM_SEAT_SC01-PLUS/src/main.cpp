//*********************************************************************************************************/
// 오토홈스 V6.0
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
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <EEPROM.h>

#define RTS 2 // RS485 Enable

#define ON 1
#define OFF 0

#define CHECKED 3
#define UNCHECKED 2

#define LOGO_CHANGE_EEPROM 4 // bool

void comSend(String sendData);

StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

String payload, s, ssidHotSpot, pswdHotSpot, hotSpotIP;

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
}

void button1Left(lv_event_t *e) {
  Serial.println("Button 1 Gesture Left...");
  logoChangeCounter = 0;
  logoCounter = 0;

  btn_1_stop = false;
  btn_1_right = false;
  btn_1_left = true;
  comSend("BTN1_LEFT:1");
}

void button1Right(lv_event_t *e) {
  Serial.println("Button 1 Gesture Right...");
  logoChangeCounter = 0;
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
  logoChangeCounter = 0;
  logoCounter = 0;

  btn_3_stop = false;
  btn_3_left = false;
  btn_3_right = true;
  comSend("BTN3_RIGHT:1");
}

void button2Up(lv_event_t *e) {
  Serial.println("Button 2 Gesture right...");
  logoChangeCounter = 0;
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
  logoChangeCounter = 0;
  logoCounter = 0;

  btn_2_stop = false;
  btn_2_right = false;
  btn_2_left = true;
  comSend("BTN2_DOWN:1");
}

void button3Down(lv_event_t *e) {
  Serial.println("Button 3 Gesture right...");
  logoChangeCounter = 0;
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
  logoCounter = 0;

  comSend("HEAT1:1");
}

void heater2_click(lv_event_t *e) {
  Serial.println("Heater 2 Click...");
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

void panelMessageClick(lv_event_t *e) { Serial.print("PanelMessage Click..."); }

void comSend(String sendData) {
  digitalWrite(RTS, HIGH);
  Serial2.println(sendData);
  Serial2.flush();

  digitalWrite(RTS, LOW);
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

void setup() {
  pinMode(RTS, OUTPUT); // RS485 Enable Pin

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial2.begin(115200, SERIAL_8N1, 1, 42);

  Serial.flush();
  Serial2.flush();

  digitalWrite(RTS, LOW);

  EEPROM.begin(250);

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  logoChangeFlag = EEPROM.readBool(LOGO_CHANGE_EEPROM);

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

  // lv_obj_add_flag(ui_PanelMessage,
  //                 LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST);

  tft.setBrightness(255);

  // 열선의 Lamp 1, 2, 3 Back ground Off...
  lv_obj_set_style_bg_opa(ui_ContainerL1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_ContainerL2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_ContainerL3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_ContainerR1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_ContainerR2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_ContainerR3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void loop() {

  lv_timer_handler(); /* let the GUI do its work */

  // if (!btn_flag)
  //   lv_timer_handler(); /* let the GUI do its work */

  if (loopCount > 180) { // 약 1초 마다 실실행...
    loopCount = 0;
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
