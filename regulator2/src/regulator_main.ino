#if 1

#include <LiquidCrystal.h>
#include <OneWire.h>
#include "config.h"
LiquidCrystal lcd(DPIN_LCD_RS, DPIN_LCD_ENABLE, DPIN_LCD_DATA1, DPIN_LCD_DATA2, DPIN_LCD_DATA3, DPIN_LCD_DATA4);
#include "types.h"
#include "buttonreader.h"
#include "tempreader.h"
#include "app.h"
#include "screen.h"

OneWire       oneWire(DPIN_ONEWIRE);
TempReader    temp(oneWire);
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

#else

#include <OneWire.h>
#include <LiquidCrystal.h>
#include "config.h"

#define private public
#include "tempreader.h"
LiquidCrystal lcd(DPIN_LCD_RS, DPIN_LCD_ENABLE, DPIN_LCD_DATA1, DPIN_LCD_DATA2, DPIN_LCD_DATA3, DPIN_LCD_DATA4);

int DS18S20_Pin = DPIN_ONEWIRE; //DS18S20 Signal pin on digital 7 by rahulmitra
//Temperature chip i/o
OneWire ds(DPIN_ONEWIRE); // on digital pin 7 by rahulmitra

void setup(void) {
Serial.begin(9600);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Rahul Mitra");
}

void loop(void) {
  float temperature = getTemp();
  Serial.println(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Temp : ");
  lcd.print(temperature);
  lcd.print(" *C");
  delay(100); //just here to slow down the output so it is easier to read

}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  TempReader temp(ds);

  byte data[12];
 #if 0
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1001;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1002;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1003;
  }
#else
#define addr temp.devices[0]
  temp.setup();
#endif

#if 0
  ds.reset();
  ds.select(addr);
  ds.write(0x44,0); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
#else
  return temp.getTemperature(addr);
#endif
}
 
 #endif
