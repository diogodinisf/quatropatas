byte incomingByte,pot;

void setup() {

// start serial port at 19200 bps
Serial.begin(19200);

pinMode (5, OUTPUT);  // set led pin 5 to output
delay(1000);
}

void loop() {

 if (Serial.available()) {   // is there any information available on serial port ?
incomingByte = Serial.read(); // assign bytes to the variable ‘incomingByte’
Serial.print(int(incomingByte));

if ((int(incomingByte) == 254)) {
pot = Serial.read();
Serial.print("Pot = ");
Serial.print(int(pot)); //print the values of pot on serial monitor
}
}
analogWrite (5, pot); //pwm led according to pot values
}
