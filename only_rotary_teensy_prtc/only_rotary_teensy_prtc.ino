// Teensy 3.5 control of behavioral wheel
// This here only includes the rotary encoder

// This is a modified version that not only keeps track of position / interrupt data,
// but also exports the internal clock via serial. 
// Whenever a pseudo random timing code (TTL) is detected (PRTC), 
// a timestamp is sent via serial 


#include <Encoder.h>

const int led = 13;
const int interrupter_pin = 9; // Connects to the beam interrupt for full rotation measurement
const int sync_pin = 15; // Input pin for PRTC TTLs

elapsedMillis measure_timer; // internal (measurement) clock
elapsedMillis interval_timer; // for regular intervals
elapsedMillis interrupt_timer; // for keeping track of sync interrupts
elapsedMillis led_on_timer; // for on time LED after sync detection
unsigned int interval_ = 20;  // time in ms between serial events 
unsigned int led_on_ = 10; // time in ms the LED stays on after a sync signal is detected
unsigned int debounce_ = 10; // debouncing time in ms for sync inputs

bool measurement = false;
bool start_clock = true;
bool interrupted = false;
bool interrupt_saved = true;

long interruptedPosition;
long newPosition;


Encoder myEnc(0,1);

void setup() {
 pinMode(led, OUTPUT);
 
 // Define two input / interrupt routines
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
      Serial.print(measure_timer);
      Serial.print("_");
      Serial.print(newPosition);
      Serial.print("_");
      if (interrupted == true){
        Serial.print("1_");
        Serial.println(interruptedPosition);
        interrupt_saved = true;      
        interrupted = false;
      } else {
        Serial.print("0_");
        Serial.println(interruptedPosition);

      }
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
   // Let's debounce the interrupts a bit 
   // I am not expecting the cleanest sync pulses here and we might 
   // overload the serial bus with writes if the pulses are too noisy...
   if(interrupt_timer > debounce_){
      Serial.flush();
      Serial.println(measure_timer);
      Serial.send_now(); // Teensy specific 
      digitalWriteFast(led, HIGH);
      led_on_timer = 0; // Reset LED on timer
      interrupt_timer = 0;
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
