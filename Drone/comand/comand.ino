int const pot = A0;
int speed = 0;
long time = 0;
int count = 0;
int aux = 0;

void setup()
{
  //Serial.begin(9600);
  Serial1.begin(115200);
  delay(2000);  
}

void loop()
{
  speed = map(analogRead(pot), 0 ,1023, 139, 40);
  
  if ( speed < 55 )
  {
    speed = 45;
  }
  
  Serial1.print('S'); 
  Serial1.print(speed);
  
  /*if(count >= 1000) {
    Serial.print((millis() - time) / 1000.);
    Serial.print("  :::  ");*/
    Serial.println(speed);/*
    time = millis();
    count = 0;
  } else {
    count += 1;
  }*/
  
  
  /*if ((aux = Serial1.read()) != -1)
  {
    Serial.println(aux);
  }*/
}
