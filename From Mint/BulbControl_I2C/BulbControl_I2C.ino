#include <Wire.h>

const int controlPin = 3;
const int buttonPin = 12;
int buttonState = 0;
int c = 0;
void setup() {
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  // put your setup code here, to run once:
  pinMode(controlPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}
void receiveEvent(int brightness) {
  c = Wire.read();
  analogWrite(controlPin, c);
  Serial.println(c);
}
void loop() {
  // put your main code here, to run repeatedly: 
  delay(100);
}
