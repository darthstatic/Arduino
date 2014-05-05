



#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
RF24 radio(9,10);
const int role_pin = 2;
const uint64_t pipes[2] =  { 
  0xABCDABCD71LL, 0x544d52687CLL };
typedef enum { 
  role_remote = 1, role_led } 
role_e;
const char* role_friendly_name[] = { 
  "invalid", "Remote", "LED Board"};
role_e role;

const int controlPin = 3;
int i = 0;
int c = 0;

void setup() {
  pinMode(controlPin, OUTPUT);
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin

  if ( digitalRead(role_pin) ) {
    role = role_led;
  } 
  else {
    role = role_remote;
  }
  Serial.begin(57600);
  printf_begin();
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  radio.begin();
  radio.setAutoAck(1);
  radio.enableAckPayload();
  radio.setRetries(0,15);
  radio.setPayloadSize(2);
  if ( role == role_led ) {
    pinMode(controlPin, OUTPUT);
    radio.openReadingPipe(1,pipes[1]);
    radio.openWritingPipe(pipes[0]);
    radio.startListening();
  } 
  else {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
    radio.startListening();
  }
  radio.powerUp();
  radio.printDetails();
}

void loop() {
  if ( role == role_remote )
  {
    byte gotByte;
    radio.stopListening();

    for(int something=5;something<=255;something = something + 5) {
      printf("This is integer %i with size %i\r\n",something, sizeof(something));
      unsigned long time = micros();
      if ( radio.write(&something,2) ) {
        if(!radio.available()) {
          printf("Got blank response. delay: %lu ms\n\r",micros()-time);
        }
        else{
          while(radio.available()){
            radio.read(&gotByte,2);
            printf("Got response %d, round-trip delay: %lu ms\n\r",gotByte,micros()-time);
          }
        }
      } 
      else { 
        printf("Sending failed.\r\n"); 
      }


      delay(2000);

    }

  }

  //
  // LED role.  Receive the state of all buttons, and reflect that in the LEDs
  //

  if ( role == role_led )
  {
    // if there is data ready
    /*if ( radio.available() )
     {*/

    // Dump the payloads until we've gotten everything
    byte pipeNo, gotByte;                          // Declare variables for the pipe and the byte received
    while( radio.available(&pipeNo)){              // Read all available payloads
      printf("Into the loop...\r\n");
      radio.read( &gotByte, 2 );                  
      // Since this is a call-response. Respond directly with an ack payload.
      // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio.writeAckPayload(pipeNo,&gotByte, 2 );  // This can be commented out to send empty payloads.
      printf("Sent response %d \n\r", gotByte);
      analogWrite( controlPin, gotByte);
    }
    /*bool done = false;
     int brightness;
     while (!done)
     {
     done = radio.read( &brightness, sizeof(brightness) );
     printf("Data read was %i\r\n", brightness );
     printf("Length was %i\r\n", sizeof(brightness));
     analogWrite( controlPin, brightness );
     delay(20);*/

  }
}





