// Teensy 3.5 control of behavioral wheel
// This here only includes the rotary encoder
// Default for miniscope setup

#include <Encoder.h>

const int led = 13;
const int interrupter_pin = 9;

const int frameclock_pin = 16;
const int sync_pin = 15;

elapsedMillis measure_timer; // measure frameclock

elapsedMillis interval_timer; // for regular intervals
unsigned int interval_ = 20;  // time in ms between serial events 

bool measurement = false;
bool start_clock = true;
bool interrupted = false;
bool interrupt_saved = true;

long interruptedPosition;
long newPosition;

int interrupter_state;

Encoder myEnc(0,1);

void setup() {
 pinMode(led, OUTPUT);
 pinMode(sync_pin, OUTPUT);
 
 pinMode(interrupter_pin, INPUT_PULLUP); // either this or attach external pullup
 pinMode(frameclock_pin, INPUT_PULLDOWN); 
 attachInterrupt(frameclock_pin, clock_detected, RISING); // Microscope frame clock
 attachInterrupt(interrupter_pin, interrupt_detected, RISING); // Beam interrupter

 Serial.begin(250000);
}

void loop() {
  newPosition = myEnc.read();

  // ROTARY ENCODER
  if (measurement == true){
   
    if (start_clock == true){ // set back timer once
      interval_timer = interval_; 
      start_clock = false;
    }
    
    if (interval_timer >= interval_){
      interval_timer = interval_timer-interval_;
      digitalWriteFast(sync_pin, HIGH);
     
      // Serial print: Position_Interruped_InterruptedPosition_Motor
      Serial.print(newPosition);
      Serial.print("_");
      if (interrupted == true){
        Serial.print("1_");
        Serial.print(interruptedPosition);
        Serial.print("_");
        interrupt_saved = true;      
        interrupted = false;
      } else {
        Serial.print("0_");
        Serial.print(interruptedPosition);
        Serial.print("_");
      }

      Serial.println("0"); // This is reserved for motor
         
      delay(5);
      
      digitalWriteFast(sync_pin, LOW);
      }
    }

  if(measure_timer > 500){
    measurement = false;
    start_clock = true;
  }
}

void clock_detected()
{
  measurement = true;
  measure_timer = 0;
}

void interrupt_detected()
{
  interrupted = true;
  if(interrupt_saved == true){
      interruptedPosition = myEnc.read(); // save current position at interrupt
      interrupt_saved = false;            // make sure only first occurence is saved
  }
}




