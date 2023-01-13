// Teensy 3.5 control of behavioral wheel
// This here only includes the rotary encoder

// This is a modified version that triggers serial 
// writing only when the serial connection is actually established 
// i.e. when the python recorder script is running

#include <Encoder.h>

const int led = 13;
const int interrupter_pin = 9;
const int sync_pin = 15;

elapsedMillis interval_timer; // for regular intervals
unsigned int interval_ = 20;  // time in ms between serial events 

bool measurement = false;
bool start_clock = true;
bool interrupted = false;
bool interrupt_saved = true;
bool led_state = LOW;

long interruptedPosition;
long newPosition;

int interrupter_state;

Encoder myEnc(0,1);

void setup() {
 pinMode(led, OUTPUT);
 pinMode(sync_pin, OUTPUT);
 
 pinMode(interrupter_pin, INPUT); // either this or attach external pullup
 attachInterrupt(interrupter_pin, interrupt_detected, RISING); // Beam interrupter

 Serial.begin(500000);
}

void loop() {
  newPosition = myEnc.read();

  // Check Serial DTR (serial connection used?)
  if (Serial.dtr()) {
    measurement = true;
  } else {
    measurement = false;
    start_clock = true;
    digitalWriteFast(led, LOW);

  }

  // ROTARY ENCODER
  if (measurement == true){   
    if (start_clock == true){ // set back timer once
      interval_timer = interval_; 
      start_clock = false;
    }
    
    if (interval_timer >= interval_){
      interval_timer -= interval_;
      digitalWriteFast(sync_pin, HIGH);
      led_state = !led_state;
      digitalWriteFast(led, led_state);
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
         
      delay(5); // Sync pin is HIGH for > 5 ms (serial print takes some time too)
      
      digitalWriteFast(sync_pin, LOW);
      }
    } 
}


void interrupt_detected()
// A full rotation of the wheel
{
  interrupted = true;
  if(interrupt_saved == true){
      interruptedPosition = myEnc.read(); // save current position at interrupt
      interrupt_saved = false;            // make sure only first occurence is saved
  }
}
