#ifndef DRIVERLCD_H
#define DRIVERLCD_H

#include<Arduino.h>

#include<../BSP.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#define I2C_SDA 25
#define I2C_SCL 26

//LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display


//custom chars
byte E0Char[] = {
  B00000,
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B00100,
  B00000
};

byte E1Char[] = {
  B00000,
  B00100,
  B01100,
  B00100,
  B00100,
  B01110,
  B00000,
  B00000
};

byte upAnimChar1[] = { B00100, B01110, B11011, B10001, B00000, B00000, B00000, B00000 };
byte upAnimChar2[] = { B01110, B11011, B10001, B00000, B00000, B00000, B00000, B00100 };
byte upAnimChar3[] = { B11011, B10001, B00000, B00000, B00000, B00000, B00100, B01110 };
byte upAnimChar4[] = { B10001, B00000, B00000, B00000, B00000, B00100, B01110, B11011 };
byte upAnimChar5[] = { B00000, B00000, B00000, B00000, B00100, B01110, B11011, B10001 };
byte upAnimChar6[] = { B00000, B00000, B00000, B00100, B01110, B11011, B10001, B00000 };
byte upAnimChar7[] = { B00000, B00000, B00100, B01110, B11011, B10001, B00000, B00000 };
byte upAnimChar8[] = { B00000, B00100, B01110, B11011, B10001, B00000, B00000, B00000 };
byte upAnimChar9[] = { B00100, B01110, B11011, B10001, B00000, B00000, B00000, B00000 };

byte downAnimChar1[] = { B00000, B00000, B00000, B00000, B10001, B11011, B01110, B00100 };
byte downAnimChar2[] = { B00100, B00000, B00000, B00000, B00000, B10001, B11011, B01110 };
byte downAnimChar3[] = { B01110, B00100, B00000, B00000, B00000, B00000, B10001, B11011 };
byte downAnimChar4[] = { B11011, B01110, B00100, B00000, B00000, B00000, B00000, B10001 };
byte downAnimChar5[] = { B10001, B11011, B01110, B00100, B00000, B00000, B00000, B00000 };
byte downAnimChar6[] = { B00000, B10001, B11011, B01110, B00100, B00000, B00000, B00000 };
byte downAnimChar7[] = { B00000, B00000, B10001, B11011, B01110, B00100, B00000, B00000 };
byte downAnimChar8[] = { B00000, B00000, B00000, B10001, B11011, B01110, B00100, B00000 };
byte downAnimChar9[] = { B00000, B00000, B00000, B00000, B10001, B11011, B01110, B00100 };



class DriverLCD
{
private:
    
    
public:
    //LiquidCrystal &_lcd;
    LiquidCrystal_I2C &_lcd;

    DriverLCD(/*LiquidCrystal &lcd*/ LiquidCrystal_I2C lcd)
        : _lcd(lcd)
    {}
    ~DriverLCD()
    {}
    
    void init(){
        _lcd.init(I2C_SDA, I2C_SCL); // initialize the _lcd to use user defined I2C pins
        _lcd.backlight();
        _lcd.clear();

        _lcd.createChar(ETAGE_0_CHAR, E0Char);
        _lcd.createChar(ETAGE_1_CHAR, E1Char);
        _lcd.createChar(UP_ANIMATE_1_CHAR, E1Char);
        _lcd.createChar(DOWN_ANIMATE_1_CHAR, E1Char);
    }  

    void setUp();
    void setDown();
    void setEtage0();
    void setEtage1();
};

void DriverLCD::setUp(){
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("ASCENSEUR : ");
  _lcd.setCursor(0, 12);
  _lcd.write(UP_ANIMATE_1_CHAR);

}

void DriverLCD::setDown(){
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("ASCENSEUR : ");
  _lcd.setCursor(0, 12);
  _lcd.write(DOWN_ANIMATE_1_CHAR);

}

void DriverLCD::setEtage0(){
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("ASCENSEUR : ");
  _lcd.setCursor(0, 12);
  _lcd.write(ETAGE_0_CHAR);

}

void DriverLCD::setEtage1(){
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print("ASCENSEUR : ");
  _lcd.setCursor(0, 12);
  _lcd.write(ETAGE_1_CHAR);

}


#endif