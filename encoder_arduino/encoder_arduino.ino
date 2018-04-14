/* 
 Rotary encoder code
 */

#include <Encoder.h>
elapsedMillis timer1;
int interval = 25; // time in ms between serial events 
int LED = 13;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(1,0);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(250000);
  Serial.println("Basic Encoder Test:");
  pinMode(LED,OUTPUT);
}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  if (timer1 >= interval) {
      timer1 = timer1 - interval;
      digitalWriteFast(LED,HIGH);
      Serial.println(newPosition);
      digitalWriteFast(LED,LOW);
    }
 
}
