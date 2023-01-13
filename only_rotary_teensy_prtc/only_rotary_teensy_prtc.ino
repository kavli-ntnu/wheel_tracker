// Teensy 3.5 control of behavioral wheel
// This here only includes the rotary encoder

// This is a modified version that not only keeps track of position / interrupt data
// but also exports the internal clock via serial 
// Whenever a pseudo random timing code (TTL) is detected (PRTC), 
// a timestamp is sent via serial 


#include <Encoder.h>

const int led = 13;
const int interrupter_pin = 9;
const int sync_pin = 15;


elapsedMillis measure_timer; // internal (measurement) clock
elapsedMillis interval_timer; // for regular intervals
elapsedMillis led_on_timer; // for on time LED after sync detection
unsigned int interval_ = 20;  // time in ms between serial events 
unsigned int led_on_ = 20; // time the LED stays on after a sync signal is detected

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
 pinMode(sync_pin, INPUT_PULLDOWN);
 attachInterrupt(sync_pin, sync_detected, RISING); // pseudo random timing code input
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
      measure_timer = 0; // Reset (global) timer
      start_clock = false;
    }
    
    if (interval_timer >= interval_){
      interval_timer -= interval_;
      digitalWriteFast(sync_pin, HIGH);
      // Serial print: Position_Interruped_InterruptedPosition_Motor
      Serial.print(measure_timer);
      Serial.print("_");
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
    
     // ... this is still inside "if (measurement == true){ " !
     if(led_on_timer > led_on_){
      digitalWriteFast(led, LOW);
     }
  } 
}

void sync_detected()
// A PRTC TTL is detected on sync input 
{
 Serial.println(measure_timer);
 Serial.send_now();
 led_on_timer = 0; // Reset LED on timer
 digitalWriteFast(led, HIGH);
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
