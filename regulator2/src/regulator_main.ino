#include <LiquidCrystal.h>
#include "config.h"
LiquidCrystal lcd(DPIN_LCD_RS, DPIN_LCD_ENABLE, DPIN_LCD_DATA1, DPIN_LCD_DATA2, DPIN_LCD_DATA3, DPIN_LCD_DATA4);
#include "types.h"
#include "buttonreader.h"
#include "tempreader.h"
#include "app.h"
#include "screen.h"

TempReader    temp(DPIN_TEMP_SELECT_A, DPIN_TEMP_SELECT_B, APIN_TEMP_SENSOR);
Screen        scr(lcd);
ButtonReader  btns(APIN_LCD_INPUT_BTN);
App           app(btns, temp, scr, DPIN_RELAY_1);

void setup(){
  app.setup();
}

void loop(){
  btns.tick();
  temp.tick();
  app.tick();
  /*
  lcd.setCursor(0, 0);
  lcd.print(millis());
  lcd.print(' ');
  lcd.print(app.last);
  lcd.print(' ');
  */
}

