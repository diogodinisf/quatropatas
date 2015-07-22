// this uses the Arduino servo library included with version 0012 

// caution, this code sweeps the motor up to maximum speed !
// make sure the motor is mounted securily before running.

#include <Servo.h> 

Servo motorFE, motorTE, motorFD, motorTD;

void arm(){
  // arm the speed controller, modify as necessary for your ESC  
  setSpeed(10);
  delay(5000);
  setSpeed(0); 
  delay(2000); //delay 2 second,  some speed controllers may need longer
}

void setSpeed(int speed){
  // speed is from 0 to 100 where 0 is off and 100 is maximum speed
  //the following maps speed values of 0-100 to angles from 0-180,
  // some speed controllers may need different values, see the ESC instructions
  int angle = map(speed, 0, 100, 0, 180);
  motorTD.write(angle);    
  motorTE.write(angle);
}

void setup()
{
  //motorFE.attach(11);
  //motorFD.attach(6);
  motorTE.attach(10);
  motorTD.attach(9);
  arm();  
}


void loop()
{
  int speed;
  
  speed = 40;
  setSpeed(speed);
  delay(10000);
/*
  // sweep up from 0 to to maximum speed in 20 seconds
  for(speed = 0; speed <= 40; speed += 5) {
    setSpeed(speed);
    delay(5000);
  }
  // sweep back down to 0 speed.
  for(speed = 35; speed > 0; speed -= 5) {
    setSpeed(speed);
    delay(5000);
  }*/
  setSpeed(0);  
  delay(15000); // stop the motor for 5 seconds
}
