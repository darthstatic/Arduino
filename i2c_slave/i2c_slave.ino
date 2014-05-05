#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int state0 = 0;
int state1 = 0;
int state2 = 0;
int state3 = 0;
int state5 = 0;

void setup() {
    pinMode(15, OUTPUT);
    pinMode(8, INPUT);
    Serial.begin(9600);         // start serial for output
    // initialize i2c as slave
    Wire.begin(SLAVE_ADDRESS);

    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);

    Serial.println("Ready!");
}

void loop() {
    if (digitalRead(8) == LOW)
    {
      if (state5 == 0){
                digitalWrite(15, HIGH); // set the LED on
                state5 = 1;
            }
            else{
                digitalWrite(15, LOW); // set the LED off
                state5 = 0;
            }
    }
}

// callback for received data
void receiveData(int byteCount){
    Serial.println("Hello");
    
    while(Wire.available()) {
        number = Wire.read();
        Serial.print("data received: ");
        Serial.println(number);

        switch (number) {
            case 6:
              if (state5 == 0){
                digitalWrite(15, HIGH); // set the LED on
                state5 = 1;
            }
            else{
                digitalWrite(15, LOW); // set the LED off
                state5 = 0;
            }
            break;
         }
     }
}

// callback for sending data
void sendData(){
    Wire.write(number);
}

