/* 
 Rotary encoder code
 */

#include <Encoder.h>
elapsedMillis timer1;
int interval = 20; // minimum time in ms between serial events 
int frame_clock = 3; // Frame clock pin from 2P setup 
int NI_out = 4; // National Instruments hardware out
int status_scope;

Encoder myEnc(1,0);

void setup() {
  Serial.begin(250000);
  pinMode(frame_clock,INPUT_PULLDOWN);
  pinMode(NI_out,OUTPUT);
  digitalWrite(NI_out,LOW);

}

long oldPosition  = -999;

void loop() {
  long newPosition = myEnc.read();
  status_scope = digitalReadFast(frame_clock);
  if (timer1 >= interval) {
      timer1 = timer1 - interval;
      if (status_scope == 1) {
        digitalWriteFast(NI_out,HIGH);
        Serial.println(newPosition);
        delay(2);
        digitalWriteFast(NI_out,LOW);
        delay(2);
       }
    }
}
