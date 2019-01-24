/* 
 Rotary encoder code

 Sends values "newPosition" over serial, regardlesss of frameclock
 
 */
#include <Encoder.h>
elapsedMillis interval_timer; // for regular intervals
elapsedMillis measure_timer; // measure frameclock
unsigned int interval_ = 25; // time in ms between serial events 

int sync_pin = 4;

Encoder myEnc(1,0);

void setup() {
  Serial.begin(250000);
  pinMode(sync_pin, OUTPUT);
}

void loop() {
  long newPosition = myEnc.read();
  
  if (interval_timer >= interval_){
    interval_timer = 0;
    digitalWrite(sync_pin, HIGH); 
    Serial.println(newPosition);
    delay(5);
    digitalWrite(sync_pin, LOW); 
    }  


}
