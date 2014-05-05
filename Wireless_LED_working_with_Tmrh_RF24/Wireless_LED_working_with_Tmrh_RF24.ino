#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(8,10);
                                                                           // Topology
const uint64_t addresses[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  

const int controlPin = 9;
void setup(){

  Serial.begin(57600);
  pinMode(controlPin, OUTPUT);
  // Setup and configure radio
  printf_begin();
  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(addresses[0]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1,addresses[1]);      // Open a reading pipe on address 0, pipe 1
  radio.startListening();                 // Start listening
  radio.powerUp();
  printf("Role: LED Board\n");
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {

    byte pipeNo, gotByte;                          // Declare variables for the pipe and the byte received
    while( radio.available(&pipeNo)){              // Read all available payloads
      radio.read( &gotByte, 1 );                   
                                                   // Since this is a call-response. Respond directly with an ack payload.
                                                   // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio.writeAckPayload(pipeNo,&gotByte, 1 );  // This can be commented out to send empty payloads.
      printf("Sent response %d \n\r", gotByte);  
      analogWrite(controlPin,gotByte);
    }
 }
 

