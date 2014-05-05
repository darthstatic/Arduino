#include <Wire.h>

const int controlPin = 3;
const int buttonPin = 12;
int i = 0;
int buttonState = 0;
int remoteSet = 0;
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
  remoteSet = 1;
}
void loop() {
  // put your main code here, to run repeatedly: 
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
     if (c < 255) {
       c = c + 1;
     } else {
       c = 255;
     }
     analogWrite(controlPin, c);
     remoteSet = 0;
  } else {
    if (remoteSet == 0) {
      if (c > 0) {
        c = c - 1;
      } else {
        c = 0;
      }
      analogWrite(controlPin, c);
      remoteSet = 0;
    }
  }
}
