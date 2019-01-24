// Teensy 3.5 control of behavioral wheel
#include <Encoder.h>

const int motor_enable_pin = 10;
const int button_pin = 12;
const int led = 13;
const int interrupter_pin = 9;

const int frameclock_pin = 15;
const int sync_pin = 14;
const int sync_interrupter_pin = 3;

elapsedMillis measure_timer; // measure frameclock

elapsedMillis interval_timer; // for regular intervals
unsigned int interval_ = 20;  // time in ms between serial events 

elapsedMillis interval_timer_motor;  // for regular intervals
unsigned int interval_motor_ = 2000; // Keep pin on motor controler high for this long (roughly the time the motor is turning)

bool measurement = false;
bool start_clock = true;
bool motor_enable = false;
bool interrupted = false;
bool interrupt_saved = true;

long interruptedPosition;
long newPosition;

int button_state;
int interrupter_state;

Encoder myEnc(0,1);

void setup() {
 pinMode(motor_enable_pin, OUTPUT);
 pinMode(led, OUTPUT);
 pinMode(sync_pin, OUTPUT);
 pinMode(sync_interrupter_pin, OUTPUT);
 
 pinMode(button_pin, INPUT);
 pinMode(interrupter_pin, INPUT); // has pulldown attached
 pinMode(frameclock_pin, INPUT_PULLDOWN); 
 attachInterrupt(frameclock_pin, clock_detected, RISING); // Microscope frame clock
 attachInterrupt(interrupter_pin, interrupt_detected, RISING); // Beam interrupter

 Serial.begin(250000);
}

void loop() {
  newPosition = myEnc.read();
  
  // MOTOR ACTION
  button_state = digitalReadFast(button_pin);
  if(button_state == HIGH){  
     motor_enable = true;
     interval_timer_motor = 0;
  } 
  if ((interval_timer_motor <= interval_motor_) && (motor_enable == true)){
    digitalWriteFast(led, HIGH); 
    digitalWriteFast(motor_enable_pin, HIGH); 
    } else {
    motor_enable = false;
    digitalWriteFast(led, LOW); 
    digitalWriteFast(motor_enable_pin, LOW);       
  }
  
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
      if (motor_enable == true) {
        Serial.println("1");
      } else {
        Serial.println("0");
      }
      
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




