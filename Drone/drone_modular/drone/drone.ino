/* Quatro-Patas -> teste de sensores e motores*/

/*******************************************
 *****             INCLUDE             *****
 *******************************************/ 
 
#include <Servo.h>
#include "config.h"

// I use a lots of code of MPU6050_DMP6 example from MPU6050 library!

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

/***************************************************
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 **************************************************/

/*******************************************
 *****               VARS              *****
 *******************************************/ 
// tempo de ciclo
long time = 0;

// Transmission
int incomingByte;

// Battery
int const battery = A3; // pin for battery voltage

// Motors
Servo motorFL; // front-left motor
Servo motorBL; // back-left motor
Servo motorFR; // front-right motor
Servo motorBR; // back-right motor

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//// for magnetomer
//#define HMC5883L_DEFAULT_ADDRESS    0x1E
//#define HMC5883L_RA_DATAX_H         0x03
//#define HMC5883L_RA_DATAZ_H         0x05
//#define HMC5883L_RA_DATAY_H         0x07

//int16_t mx, my, mz;     //To store magnetometer readings
//float heading;          // Simple magnetic heading. (NOT COMPENSATED FOR PITCH AND ROLL)
//int16_t gyro[3];        //To store gyro's measures

/*******************************************
 *****   INTERRUPT DETECTION ROUTINE   *****
 *******************************************/ 

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady() 
{
  mpuInterrupt = true;
}

/*******************************************
 *****             HEADERS             *****
 *******************************************/
void arm(void);
void setSpeed(int, float *);
void getSensorValue(void);
void initializeSensor(void);
void stabilizationSensor(void);

/*******************************************
 *****          INITIAL SETUP          *****
 *******************************************/
 
void setup() 
{
    // initialize motors
  Serial.begin(115200);
  motorFL.attach(11);
  motorFR.attach(10);
  motorBR.attach(9);
  motorBL.attach(6);
  arm();  
  
  initializeSensor();
}

/*******************************************
 *****        MAIN PROGRAM LOOP        *****
 *******************************************/

void loop() 
{
  // if programming failed, don't try to do anything
  //if (!dmpReady) return;
  
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    // other program behavior stuff here
    if ( Serial.available() > 0 )
    {
      incomingByte = Serial.read();    
      
      if ( incomingByte == 'S' )
      {
        while ( Serial.available() <= 0 )
          ;
        
        incomingByte = Serial.parseInt();
        
        setSpeed(incomingByte, ypr);
      }
    }
    // if you are really paranoid you can frequently test in between other
    // stuff to see if mpuInterrupt is true, and if so, "break;" from the
    // while() loop to immediately process the MPU data
  }
  
  getSensorValue();
  
  // battery
  //Serial.println(analogRead(battery)); 
  
  //Serial.println(millis() - time);
  //time = millis();
  
 
 // Serial.print(millis() - time);
 // time = millis();
  
  
}
 
