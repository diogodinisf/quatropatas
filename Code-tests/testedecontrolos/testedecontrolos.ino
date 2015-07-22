const int top = 5;
const int left = 3;
const int right = 4;
const int bot = 6;
const int sel = 8;
const int control = 7;
const int vertical = A0;
const int horizontal = A1;

int valorVertical;
int valorHorizontal;
float piscaV;
float piscaH;
int controlAnt = 0;

void setup()
{
  /*Serial.begin(9600);
  while (!Serial);*/
  
  pinMode(top, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(bot, OUTPUT);
  pinMode(control, OUTPUT);
  pinMode(sel, INPUT_PULLUP);
}

void loop()
{
  /*Serial.print(analogRead(vertical));
  Serial.print("  ");
  Serial.println(analogRead(horizontal));*/

 valorVertical = analogRead(vertical);
 valorHorizontal = analogRead(horizontal);
 
 if ( digitalRead(sel) == LOW )
 {
   controlAnt = !controlAnt;
   
   digitalWrite(control, controlAnt);
   
   while(digitalRead(sel) == LOW);
 }
 
  if ( valorVertical > 521 )
  {
    piscaV = map(valorVertical, 521, 1023, 200, 10);
    
    digitalWrite(top, HIGH);
    delay(piscaV);
    digitalWrite(top, LOW);
    delay(piscaV);
    
  }
  else if ( valorVertical < 519 )
  {
    piscaV = map(valorVertical, 519, 0, 200, 10);
    
    digitalWrite(bot, HIGH);
    delay(piscaV);
    digitalWrite(bot, LOW);
    delay(piscaV);
  }

  if ( valorHorizontal > 518 )
  {
    piscaH = map(valorHorizontal, 518, 1023, 200, 10);
    
    digitalWrite(left, HIGH);
    delay(piscaH);
    digitalWrite(left, LOW);
    delay(piscaH);
    
  }
  else if ( valorHorizontal < 515 )
  {
    piscaH = map(valorHorizontal, 515, 0, 200, 10);
    
    digitalWrite(right, HIGH);
    delay(piscaH);
    digitalWrite(right, LOW);
    delay(piscaH);
  }
  
}
