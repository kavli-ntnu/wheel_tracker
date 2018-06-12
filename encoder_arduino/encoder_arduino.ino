/* 
 Rotary encoder code

 Sends values "newPosition" over serial. 
 
 */
#include <Encoder.h>
elapsedMillis interval_timer; // for regular intervals
elapsedMillis measure_timer; // measure frameclock
unsigned int interval_ = 25; // time in ms between serial events 

int frameclock_pin = 3;
int sync_pin = 4;

bool measurement = false;
bool start_clock = true;
int sync_state = LOW;
int status_measure;

Encoder myEnc(1,0);

void setup() {
  Serial.begin(250000);
  pinMode(frameclock_pin, INPUT);
  pinMode(sync_pin, OUTPUT);
}

void loop() {
   long newPosition = myEnc.read();
   status_measure = digitalReadFast(frameclock_pin);
   if (status_measure == HIGH){
      measurement = true;
      if (start_clock == true){
        interval_timer = interval_;
        start_clock = false;
      }
      measure_timer = 0;
   } 
  
  if (measurement == true){
    if (interval_timer >= interval_){
      interval_timer = interval_timer-interval_;
      digitalWrite(sync_pin, HIGH); 
      Serial.println(newPosition);
      delay(5);
      digitalWrite(sync_pin, LOW); 
      }  
    }

  if(measure_timer > 1000){
    measurement = false;
    start_clock = true;
  }
}
