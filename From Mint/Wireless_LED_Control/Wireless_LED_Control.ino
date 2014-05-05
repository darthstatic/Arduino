//#include <Wire.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Starts RF radio on pins 9 and 10 (CE and CSN)
RF24 radio(9,10);

// Ground role pin for transmitter, leave open for receiver
const int role_pin = A4;


const uint64_t pipe = 0xE8E8F0F0E1LL;

// role shit
typedef enum { 
  role_remote = 1, role_led } 
role_e;
const char* role_friendly_name[] = { 
  "invalid", "Remote", "LED Board"};
role_e role;

const int controlPin = 3;
const int buttonPin = 4;
int i = 0;
int buttonState = 0;
int remoteSet = 0;
int c = 0;

void setup() {
  //Wire.begin(4);
  //Wire.onReceive(receiveEvent);
  //pinMode(buttonPin, INPUT);
  Serial.begin(57600);
  printf_begin();
  // set up the role pin
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin
  radio.begin();
  radio.setRetries(15,2);
  radio.setPayloadSize(4);
  
  // read the address pin, establish our role

  // Assigns variables to the 'role' enum, by name
  if ( digitalRead(role_pin) ) {
    role = role_led;
    radio.openReadingPipe(1,pipe);
  } 
  else {
    role = role_remote;
    radio.openWritingPipe(pipe);
  }

  printf("ROLE: %s\n\r",role_friendly_name[role]);

  if ( role == role_led ) {
    pinMode(controlPin, OUTPUT); 
  }

  radio.startListening(); 
  radio.printDetails();
}

void loop() {
  if ( role == role_remote )
  {
    // Get the current state of buttons, and
    // Test if the current state is different from the last state we sent
    radio.stopListening();
    if ( Serial.available() ) {
      //c = atoi(&RFin_bytes[sizeof(RFin_bytes)]);
      //delay(500);
      //int f = Serial.parseInt();
      int c = constrain(Serial.parseInt(), 0, 255);
      printf("Current c is %i\r\n", c);

      if ( c ) {
        printf("Sending LED brightness %i ", c);
        bool ok = radio.write( &c, sizeof(c) );
        if (ok)
          printf("ok\n\r");
        else
          printf("failed\n\r");
      }
      /*
      for(int fuck=0;fuck<=255;fuck = fuck + 10) {
        bool ok = radio.write( &fuck, 3 );
        if (ok)
          printf("writing %i ok\r\n", fuck);
        else
          printf("writing %i failed\r\n", fuck);
        delay(500);
      }*/
      delay(1000);
    }
    // Try again in a short while
    
  }

  //
  // LED role.  Receive the state of all buttons, and reflect that in the LEDs
  //

  if ( role == role_led )
  {
    radio.startListening();
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      bool done = false;
      int16_t brightness;
      while (!done)
      {
        done = radio.read( &brightness, 3 );
        printf("Data read was %i\r\n", brightness );
        analogWrite( controlPin, brightness );
        delay(20);
      }
    radio.stopListening();
    radio.startListening();
    }
  }
}



