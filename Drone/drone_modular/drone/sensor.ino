void getSensorValue()
{
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
  {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if ( mpuIntStatus & 0x02 ) 
  {
    // wait for correct available data length, should be a VERY short wait
    while ( fifoCount < packetSize ) 
    {
      fifoCount = mpu.getFIFOCount();
    }
    
    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
    
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    
    //Serial.print("yaw: ");
    //Serial.print(ypr[0]);
    //Serial.print(" pitch: ");
    //Serial.print(ypr[1]);
    //Serial.print(" roll: ");
    //Serial.println(ypr[2]);
    
    // display initial world-frame acceleration, adjusted to remove gravity
    // and rotated based on known orientation from quaternion
//    mpu.dmpGetQuaternion(&q, fifoBuffer);
//    mpu.dmpGetAccel(&aa, fifoBuffer);
//    mpu.dmpGetGravity(&gravity, &q);
//    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
//    mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
    
    ////Read magnetometer measures
    //mx=mpu.getExternalSensorWord(0);
    //my=mpu.getExternalSensorWord(2);
    //mz=mpu.getExternalSensorWord(4);
    //            
    //heading = atan2(my, mx);
    //if(heading < 0) heading += 2 * M_PI;
  }
}

void initializeSensor()
{  
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
  // initialize device
  mpu.initialize();
  
  // verify connection
  mpu.testConnection();
  
  // wait for ready
  
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();
  
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
  //mpu.setXAccelOffset(0);
  //mpu.setYAccelOffset(0);
  
  ///***** Magnetometer configuration *****/
  //
  //mpu.setI2CMasterModeEnabled(0);
  //mpu.setI2CBypassEnabled(1);
  //  
  //Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
  //Wire.write(0x02); 
  //Wire.write(0x00);  // Set continuous mode
  //Wire.endTransmission();
  //delay(5);
  //  
  //Wire.beginTransmission(HMC5883L_DEFAULT_ADDRESS);
  //Wire.write(0x00);
  //Wire.write(B00011000);  // 75Hz
  //Wire.endTransmission();
  //delay(5);
  //  
  //mpu.setI2CBypassEnabled(0);
  // 
  //// X axis word
  //mpu.setSlaveAddress(0, HMC5883L_DEFAULT_ADDRESS | 0x80); // 0x80 turns 7th bit ON, according to datasheet, 7th bit controls Read/Write direction
  //mpu.setSlaveRegister(0, HMC5883L_RA_DATAX_H);
  //mpu.setSlaveEnabled(0, true);
  //mpu.setSlaveWordByteSwap(0, false);
  //mpu.setSlaveWriteMode(0, false);
  //mpu.setSlaveWordGroupOffset(0, false);
  //mpu.setSlaveDataLength(0, 2);
  //  
  //// Y axis word
  //mpu.setSlaveAddress(1, HMC5883L_DEFAULT_ADDRESS | 0x80);
  //mpu.setSlaveRegister(1, HMC5883L_RA_DATAY_H);
  //mpu.setSlaveEnabled(1, true);
  //mpu.setSlaveWordByteSwap(1, false);
  //mpu.setSlaveWriteMode(1, false);
  //mpu.setSlaveWordGroupOffset(1, false);
  //mpu.setSlaveDataLength(1, 2);
  // 
  //// Z axis word
  //mpu.setSlaveAddress(2, HMC5883L_DEFAULT_ADDRESS | 0x80);
  //mpu.setSlaveRegister(2, HMC5883L_RA_DATAZ_H);
  //mpu.setSlaveEnabled(2, true);
  //mpu.setSlaveWordByteSwap(2, false);
  //mpu.setSlaveWriteMode(2, false);
  //mpu.setSlaveWordGroupOffset(2, false);
  //mpu.setSlaveDataLength(2, 2);
  //  
  //mpu.setI2CMasterModeEnabled(1);
  //
  ///**************************************/
  
  // make sure it worked (returns 0 if so)
  if ( devStatus == 0 ) 
  {
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);
    
    // enable Arduino interrupt detection
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    dmpReady = true;
    
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else 
  {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
  }
  
  stabilizationSensor();
}

void stabilizationSensor()
{
  float yprp[3] = {-1000,-1000,-1000}; // valores anteriores de yaw, pitch, roll
  float testValue;
  
  while ( testValue > 1 )
  {
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }
      
    getSensorValue();
    
    testValue = yprp[0] - ypr[0];
    testValue = testValue < 0 ? testValue:-testValue; // absolute value
  }
}
