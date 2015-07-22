void arm(void)
{
  float set[3] = {0,0,0};
  
  setSpeed(MIN_VALUE, set);
  delay(5000);  
}

void setSpeed(int value, float *ypr)
{
  motorBR.write(value - STABILIZER_VALUE*ypr[1] + STABILIZER_VALUE*ypr[2]);
  motorBL.write(value - STABILIZER_VALUE*ypr[1] - STABILIZER_VALUE*ypr[2]);
  motorFR.write(value + STABILIZER_VALUE*ypr[1] + STABILIZER_VALUE*ypr[2]);
  motorFL.write(value + STABILIZER_VALUE*ypr[1] - STABILIZER_VALUE*ypr[2]);
}
