/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI:
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(SCK, MISO, 5, 7, 6);

int const pot = A0;

int velocity;

void setup()   {
  Serial1.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  display.clearDisplay();   // clears the screen and buffer

  // text display tests
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0,0);
  display.println("Bem vindo ao  comando do    quatro patas!");
//  display.setTextColor(WHITE, BLACK); // 'inverted' text
//  display.println(3.141592);
//  display.setTextSize(2);
//  display.setTextColor(BLACK);
//  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);
}


void loop() 
{
  velocity = map(analogRead(pot), 0 ,1023, 139, 40);
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Velocidade:");
  display.println(map(velocity, 40, 139, 0, 99));
  
  if ( velocity < 55 )
  {
    velocity = 40;
  }
  
  Serial1.print('S');
  Serial1.print(velocity);
  //delay(1);
  
  //display.setCursor(0,10);
  //display.print("Bateria:");
  //display.println(map(Serial1.parseInt(), 0, 1023, 0, 12.9));
  display.display();
}
