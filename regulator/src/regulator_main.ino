/* YourDuino.com Example Software Sketch
 TEST LCD Display with Pushbuttons
 Based on code by Mark Bramwell and debouncing by peterstrobl
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <LiquidCrystal.h>
#include <EEPROM.h>
//#include "buttons.h"
#include "screen.h"
#include "app.h"
#include "settings.h"
#include "tempreader.h"
#include "relay.h"

// Pin Allocation:
// A0:  LCD Keypad Shield: button input
// A1:  Temperature Sensors: analog input
// D0:
// D1:
// D2:  Temperature Sensors: A line for channel selection
// D3:  Temperature Sensors: B line for channel selection
// D4:  LCD Keypad Shield: LCD d4 data line
// D5:  LCD Keypad Shield: LCD d5 data line
// D6:  LCD Keypad Shield: LCD d6 data line
// D7:  LCD Keypad Shield: LCD d7 data line
// D8:  LCD Keypad Shield: LCD RS Pin (Data or Signal Display Selection)
// D9:  LCD Keypad Shield: LCD Enable Pin
// D10: LCD Keypad Shield: Backlight control
// D11: Relay Board: relay #1 (ballon bypass)
// D12
// D13
// D??: Relay Board: relay #1
// D??: Relay Board: relay #2
// D??: Relay Board: relay #3
// D??: Relay Board: relay #4

#define APIN_LCD_INPUT_BTN  0
#define APIN_TEMP_SENSOR    1
#define APIN_2              2
#define APIN_3              3
#define APIN_4              4
#define APIN_5              5
#define DPIN_0              0
#define DPIN_1              1
#define DPIN_TEMP_SELECT_A  2
#define DPIN_TEMP_SELECT_B  3
#define DPIN_LCD_DATA1      4
#define DPIN_LCD_DATA2      5
#define DPIN_LCD_DATA3      6
#define DPIN_LCD_DATA4      7
#define DPIN_LCD_RS         8
#define DPIN_LCD_ENABLE     9
#define DPIN_LCD_BACKLIGHT 10
#define DPIN_RELAY_1       11
#define DPIN_12            12
#define DPIN_13            13

#define TEMP_NUM_CHANNELS        2
#define TEMP_CAL_VAL1_PRECISION  0.317460317
#define TEMP_CAL_CAL2_ZERO      -93.23232323


/*-----( Declare objects )-----*/
LiquidCrystal lcd(DPIN_LCD_RS, DPIN_LCD_ENABLE, DPIN_LCD_DATA1, DPIN_LCD_DATA2, DPIN_LCD_DATA3, DPIN_LCD_DATA4);
Settings      settings;
Screen        scr(lcd);
TempReader    temp(DPIN_TEMP_SELECT_A, DPIN_TEMP_SELECT_B, APIN_TEMP_SENSOR, TEMP_NUM_CHANNELS);
Relay         relay_ballon_bypass(DPIN_RELAY_1);
App           app(scr, settings, relay_ballon_bypass, &temp.temp_event);
ButtonReader  btns(&app, 0, 50 /* ms between reads */);


unsigned char threshold;

unsigned char get_threshold(){
  return EEPROM.read(0);
}

void set_threshold(char n){
  EEPROM.write(0, n);
}

void setup()
{
  temp.rtd.calibration(0, TEMP_CAL_VAL1_PRECISION, TEMP_CAL_CAL2_ZERO);
  temp.rtd.calibration(1, TEMP_CAL_VAL1_PRECISION, TEMP_CAL_CAL2_ZERO);
  temp.rtd.calibration(2, TEMP_CAL_VAL1_PRECISION, TEMP_CAL_CAL2_ZERO);
  temp.rtd.calibration(3, TEMP_CAL_VAL1_PRECISION, TEMP_CAL_CAL2_ZERO);
  
  threshold = get_threshold();
}

void loop()
{
  ADOS->Start();
  return;
}

