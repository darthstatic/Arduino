#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(8,10);
const uint64_t pipes[2] =  { 0xABCDABCD71LL, 0xABCDABCD81LL };
const int controlPin = 9;

void setup() {
  pinMode(controlPin, OUTPUT);
  Serial.begin(57600);
  printf_begin();
  printf("ROLE: LED Board\n\r");
  radio.begin();
  radio.setAutoAck(1);
  radio.enableAckPayload();
  radio.setRetries(0,15);
  radio.setPayloadSize(2);
  pinMode(controlPin, OUTPUT);
  radio.openReadingPipe(1,pipes[1]);
  radio.openWritingPipe(pipes[0]);
  radio.startListening();
  radio.powerUp();
  radio.printDetails();
}
 
void loop() {
    // Dump the payloads until we've gotten everything
    byte pipeNo, gotByte;                          // Declare variables for the pipe and the byte received
    while( radio.available(&pipeNo)){              // Read all available payloads
      delay(100);
      radio.read( &gotByte, 1 );
      radio.writeAckPayload(pipeNo,&gotByte, 1 );  // This can be commented out to send empty payloads.
      printf("Sent response %d \n\r", gotByte);
      analogWrite( controlPin, gotByte);
      gotByte=0;
    }
    /*if ( radio.available() ) {
    delay(100);
      bool done = false;
      while (!done) {
        done = radio.read(&gotByte, sizeof(2));
        printf("Got this: %i\r\n", gotByte);
        delay(1);
      }
    }*/
}
