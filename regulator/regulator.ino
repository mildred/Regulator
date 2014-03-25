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
#include "buttonreader.h"

/*-----( Declare objects )-----*/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //These are the pins used on this shield
//Buttons b;
Screen  scr(lcd);
App     app(scr);
ButtonReader btns(&app, 0, 50);


class Task : public AdOSTask2<256> {
public:
  Task() {
  }
  
  void loop() {
    lcd.setCursor(0,1);
    lcd.print(millis());
    ados()->Sleep(500);
    //delay(500);
  }

};
//Task t;

unsigned char threshold;

unsigned char get_threshold(){
  return EEPROM.read(0);
}

void set_threshold(char n){
  EEPROM.write(0, n);
}

int lcd_key = 0;;
int adc_key_prev = 0;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  lcd.begin(16, 2);              // start the lcd object
  
  //lcd.setCursor(0,0);
  //lcd.print("Push A Button!"); 
  
  //lcd.setCursor(10,1);
  //lcd.print("A="); // For display of A0 Analog values from button push
  
  threshold = get_threshold();
}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  ADOS->Start();
  //lcd.setCursor(0,0);
  //lcd.print(millis()); 
  return;
  
  
  /*int btn_pressed = b.read_LCD_buttons();
  app.update(btn_pressed);
  delay(200);
  return;*/
  
  #if 0
  
  lcd.setCursor(7,1);            // move cursor to second line "1" and 7 spaces over
  lcd.print(millis()/1000);      // display seconds elapsed since power-up

  int adc_key_in;
  adc_key_prev = lcd_key ;       // Looking for changes
  lcd_key = b.read_LCD_buttons(adc_key_in);  // read the buttons

  if (adc_key_prev != lcd_key)
  {
    lcd.setCursor(12,1); 
    lcd.print("    ");         // Blank, display returned Analog value of button
    lcd.setCursor(12,1); 
    lcd.print(adc_key_in); 
  }

  lcd.setCursor(0,1);            // move to the begining of the second line

  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
  case btnRIGHT:
    {
      lcd.print("RIGHT ");
      break;
    }
  case btnLEFT:
    {
      lcd.print("LEFT   ");
      break;
    }
  case btnUP:
    {
      lcd.print("UP    ");
      threshold += 1;
      delay(200);
      break;
    }
  case btnDOWN:
    {
      lcd.print("DOWN  ");
      threshold -= 1;
      delay(200);
      break;
    }
  case btnSELECT:
    {
      lcd.print("SELECT");
      break;
    }
  case btnNONE:
    {
      lcd.print("NONE  ");
      break;
    }
  }/* --(end switch )-- */

  lcd.setCursor(0, 0);
  lcd.print("    ");
  lcd.setCursor(0, 0);
  lcd.print((int) threshold);
  set_threshold(threshold);
  
  #endif
}/* --(end main loop )-- */

