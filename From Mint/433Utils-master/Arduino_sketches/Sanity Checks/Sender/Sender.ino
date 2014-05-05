
const int radioPin = 7;
const int ledPin = 13;

void setup() {
  
  pinMode(radioPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
}


void loop() {
  
  for (int i=0; i<10; i++) {
  
  digitalWrite(radioPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(1000);
  
  digitalWrite(radioPin, LOW);
  digitalWrite(ledPin, LOW);
  delayMicroseconds(1000);
  
  }
}
