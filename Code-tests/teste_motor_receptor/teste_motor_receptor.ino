#include <Servo.h>

int incomingByte;
Servo motorFE, motorTE, motorFD, motorTD;

void arm(void);
void setSpeed(int);

void setup(void)
{
  Serial.begin(9600);
  motorFE.attach(11);
  motorFD.attach(10);
  motorTD.attach(9);
  motorTE.attach(6);
  arm();
}

void loop(void)
{
  if ( Serial.available() > 0 )
  {
    incomingByte = Serial.read();
      
    
    if ( incomingByte == 'M' )
    {
      
      while ( Serial.available() <= 0 )
          ;
      
      incomingByte = Serial.parseInt();
      //Serial.println(incomingByte);
        setSpeed(incomingByte);
    }
  }
}

void arm(void)
{
  setSpeed(40);
  delay(5000);
  
}

void setSpeed(int incomingByte)
{
  motorTD.write(incomingByte);
  motorTE.write(incomingByte);
  motorFD.write(incomingByte);
  motorFE.write(incomingByte);
}
