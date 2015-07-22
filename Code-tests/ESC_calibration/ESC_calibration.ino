#include <Servo.h>

Servo rotor;
int value = 0;
int value_old = 0;

void setup() 
{
  rotor.attach(11);
  Serial.begin(9600);

}

void loop() 
{
  value = Serial.parseInt();  
  
  if (value != value_old && value != 0 )
  {
    Serial.println(value);
    rotor.write(value);
    value_old = value;
  }
}
