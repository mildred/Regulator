#include <LiquidCrystal.h>
#include <OneWire.h>
#include "config.h"
LiquidCrystal lcd(DPIN_LCD_RS, DPIN_LCD_ENABLE, DPIN_LCD_DATA1, DPIN_LCD_DATA2, DPIN_LCD_DATA3, DPIN_LCD_DATA4);
#include "types.h"
#include "buttonreader.h"
#include "tempreader.h"
#include "app.h"
#include "screen.h"
#include "screens.h"

OneWire       oneWire(DPIN_ONEWIRE);
TempReader    temp(oneWire);
Screen        scr(lcd);
ButtonReader  btns(APIN_LCD_INPUT_BTN);
App           app(temp, DPIN_RELAY_1);
Screens       ui(btns, app, scr);

void setup(){
  app.setup();
}

void loop(){
  btns.tick();
  temp.tick();
  app.tick();
  ui.tick();
  /*
  lcd.setCursor(0, 0);
  lcd.print(millis());
  lcd.print(' ');
  lcd.print(app.last);
  lcd.print(' ');
  */
}

