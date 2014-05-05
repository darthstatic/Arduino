const int radioPin = 8;
const int ledPin = 13;

void setup() {
  
  pinMode(radioPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
}


void loop() {
  
  digitalWrite(ledPin, digitalRead(radioPin));
  
  
}
