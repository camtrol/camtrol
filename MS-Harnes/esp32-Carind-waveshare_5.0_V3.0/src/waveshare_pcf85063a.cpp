/*****************************************************************************
* | File      	:   waveshare_pcf85063a.c
* | Author      :   Waveshare team
* | Function    :   Waveshare PCF85063A driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-02-02
* | Info        :   Basic version
*
******************************************************************************/
#include "waveshare_pcf85063a.h"

static uint8_t decToBcd(UBYTE val);
static int bcdToDec(uint8_t val);

// Month strings
const unsigned char MonthStr[12][4] = {"Jan", "Feb", "Mar", "Apr",
                                       "May", "Jun", "Jul", "Aug",
                                       "Sep", "Oct", "Nov", "Dec"};

// Write a byte to the specified register
void DEV_I2C_Write_Byte(uint8_t addr, uint8_t reg, uint8_t Value) {
  Wire.beginTransmission(addr);
  delay(3);
  Wire.write(reg);
  delay(3);
  Wire.write(Value);
  delay(3);
  Wire.endTransmission();
  delay(3);
}

// Write multiple bytes to the specified address
void DEV_I2C_Write_nByte(uint8_t addr, uint8_t *pData, uint32_t Len) {
  Wire.beginTransmission(addr);
  Wire.write(pData, Len);
  Wire.endTransmission();
}

// Read a byte from the specified register
void DEV_I2C_Read_Byte(uint8_t addr, uint8_t reg, uint8_t *data) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, (uint8_t)1);
  *data = Wire.read();
}

// Read multiple bytes from the specified address
void DEV_I2C_Read_nByte(uint8_t addr, uint8_t reg, uint8_t *pData,
                        uint32_t Len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, Len);

  for (uint8_t i = 0; i < Len; i++) {
    pData[i] = Wire.read();
  }
}

/******************************************************************************
function:	PCF85063A initialized
parameter:

Info: Initiate Normal Mode, RTC Run, NO reset, No correction, 24hr format,
Internal load capacitance 12.5pf
******************************************************************************/
void PCF85063A_Init() {
  uint8_t Value = RTC_CTRL_1_DEFAULT | RTC_CTRL_1_CAP_SEL; // Set control value
  Wire.begin(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO, I2C_MASTER_FREQ_HZ); //
  // Initialize I2C
  DEV_I2C_Write_Byte(PCF85063A_ADDRESS, RTC_CTRL_1_ADDR,
                     Value); // Write control value to RTC
}

/******************************************************************************
function:	Software reset PCF85063A
parameter:
Info:
******************************************************************************/
void PCF85063A_Reset() {
  uint8_t Value = RTC_CTRL_1_DEFAULT | RTC_CTRL_1_CAP_SEL |
                  RTC_CTRL_1_SR; // Set reset value
  DEV_I2C_Write_Byte(PCF85063A_ADDRESS, RTC_CTRL_1_ADDR,
                     Value); // Write reset value to RTC
}

/******************************************************************************
function:	Set RTC time
parameter:
                        time: Time structure
Info:
******************************************************************************/
void PCF85063A_Set_Time(datetime_t time) {
  uint8_t buf[4] = {RTC_SECOND_ADDR, decToBcd(time.sec), decToBcd(time.min),
                    decToBcd(time.hour)};         // Prepare time data
  DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 4); // Write time data to RTC
}

/******************************************************************************
function:	Set RTC date
parameter:
                        date: Date structure
Info:
******************************************************************************/
void PCF85063A_Set_Date(datetime_t date) {
  uint8_t buf[5] = {RTC_DAY_ADDR, decToBcd(date.day), decToBcd(date.dotw),
                    decToBcd(date.month), decToBcd(date.year - YEAR_OFFSET)};
  DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 5);
}

/******************************************************************************
function:	Set RTC time and date
parameter:
                        time: Time and date structure
Info:
******************************************************************************/
void PCF85063A_Set_All(datetime_t time) {
  uint8_t buf[8] = {RTC_SECOND_ADDR,      decToBcd(time.sec),
                    decToBcd(time.min),   decToBcd(time.hour),
                    decToBcd(time.day),   decToBcd(time.dotw),
                    decToBcd(time.month), decToBcd(time.year - YEAR_OFFSET)};
  DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 8);
}

/******************************************************************************
function:	Read current RTC time and date
parameter:
                        time: Time and date structure
Info:
******************************************************************************/
void PCF85063A_Read_now(datetime_t *time) {
  uint8_t bufss[7] = {0};
  DEV_I2C_Read_nByte(PCF85063A_ADDRESS, RTC_SECOND_ADDR, bufss, 7);
  time->sec = bcdToDec(bufss[0] & 0x7F);
  time->min = bcdToDec(bufss[1] & 0x7F);
  time->hour = bcdToDec(bufss[2] & 0x3F);
  time->day = bcdToDec(bufss[3] & 0x3F);
  time->dotw = bcdToDec(bufss[4] & 0x07);
  time->month = bcdToDec(bufss[5] & 0x1F);
  time->year = bcdToDec(bufss[6]) + YEAR_OFFSET;
}

/******************************************************************************
function:	Enable Alarm and Clear Alarm flag
parameter:
Info:		Default enable alarm interrupt
******************************************************************************/
void PCF85063A_Enable_Alarm() {
  uint8_t Value = RTC_CTRL_2_DEFAULT | RTC_CTRL_2_AIE;
  Value &= ~RTC_CTRL_2_AF;
  DEV_I2C_Write_Byte(PCF85063A_ADDRESS, RTC_CTRL_2_ADDR, Value);
}

/******************************************************************************
function:	Get Alarm flag
parameter:
Info:		  Read the alarm flag
******************************************************************************/
uint8_t PCF85063A_Get_Alarm_Flag() {
  uint8_t Value = 0;
  DEV_I2C_Read_Byte(PCF85063A_ADDRESS, RTC_CTRL_2_ADDR, &Value);
  Value &= RTC_CTRL_2_AF | RTC_CTRL_2_AIE;
  return Value;
}

/******************************************************************************
function:	Set Alarm time
parameter:
Info:		  Set the alarm time
******************************************************************************/
void PCF85063A_Set_Alarm(datetime_t time) {
  uint8_t buf[6] = {
      RTC_SECOND_ALARM,
      (uint8_t)(decToBcd(time.sec) & (~RTC_ALARM)),
      (uint8_t)(decToBcd(time.min) & (~RTC_ALARM)),
      (uint8_t)(decToBcd(time.hour) & (~RTC_ALARM)),
      RTC_ALARM, // Disable day
      RTC_ALARM  // Disable weekday
  };
  DEV_I2C_Write_nByte(PCF85063A_ADDRESS, buf, 6);
}

/******************************************************************************
function:	Read the set alarm time
parameter:
Info:
******************************************************************************/
void PCF85063A_Read_Alarm(datetime_t *time) {
  uint8_t bufss[6] = {0};
  DEV_I2C_Read_nByte(PCF85063A_ADDRESS, RTC_SECOND_ALARM, bufss, 6);
  time->sec = bcdToDec(bufss[0] & 0x7F);
  time->min = bcdToDec(bufss[1] & 0x7F);
  time->hour = bcdToDec(bufss[2] & 0x3F);
  time->day = bcdToDec(bufss[3] & 0x3F);
  time->dotw = bcdToDec(bufss[4] & 0x07);
}

/******************************************************************************
function:	Convert normal decimal numbers to binary coded decimal
parameter:
Info:
******************************************************************************/
static uint8_t decToBcd(UBYTE val) {
  return (uint8_t)((val / 10 * 16) + (val % 10));
}

/******************************************************************************
function:	Convert binary coded decimal to normal decimal numbers
parameter:
Info:
******************************************************************************/
static int bcdToDec(uint8_t val) { return (int)((val / 16 * 10) + (val % 16)); }

/******************************************************************************
function:	Convert time to string
parameter:
                        datetime_str: Buffer to store converted string
                        time: Time and date structure
Info:
******************************************************************************/
void datetime_to_str(char *datetime_str, datetime_t time) {
  sprintf(datetime_str, " %d.%d.%d  %d %d:%d:%d ", time.year, time.month,
          time.day, time.dotw, time.hour, time.min, time.sec);
}
