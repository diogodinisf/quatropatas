#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int const pot = A0;

void setup()
{
  Serial1.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Bem vindo ao");
  lcd.setCursor(0,1);
  lcd.print("teste do 4patas!");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Velocidade: ");
}

void loop()
{
  int speed, potVal = analogRead(pot);
  
  speed = map(potVal, 0, 1023, 0, 100);
 
  lcd.setCursor(12, 0);
  lcd.print(speed);
  Serial1.print('M');
  Serial1.print(speed);
  delay(1);
}
