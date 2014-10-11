/* YourDuino.com Example Software Sketch
 TEST LCD Display with Pushbuttons
 Based on code by Mark Bramwell and debouncing by peterstrobl
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "buttons.h"
#include "screen.h"
#include "app.h"
#include "settings.h"

/*-----( Declare objects )-----*/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //These are the pins used on this shield
Settings      settings;
Screen        scr(lcd);
App           app(scr, settings);
ButtonReader  btns(&app, 0, 50);

unsigned char threshold;

unsigned char get_threshold(){
  return EEPROM.read(0);
}

void set_threshold(char n){
  EEPROM.write(0, n);
}

void setup()
{
  threshold = get_threshold();
}

void loop()
{
  ADOS->Start();
  return;
}

