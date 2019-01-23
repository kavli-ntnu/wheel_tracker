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
unsigned int interval_motor_ = 1000; // Keep pin on motor controler high for this long

bool measurement = false;
bool start_clock = true;
bool motor_enable = false;
bool interrupted = false;
bool interrupt_saved = true;
int status_measure;

long interruptedPosition;

Encoder myEnc(0,1);

void setup() {
 pinMode(motor_enable_pin, OUTPUT);
 pinMode(led, OUTPUT);
 pinMode(sync_pin, OUTPUT);
 pinMode(sync_interrupter_pin, OUTPUT);
 
 pinMode(button_pin, INPUT);
 pinMode(interrupter_pin, INPUT);
 pinMode(frameclock_pin, INPUT);


 Serial.begin(250000);
}

void loop() {
  long newPosition = myEnc.read();
  
  status_measure = digitalReadFast(frameclock_pin);
  int button_state = digitalReadFast(button_pin);
  int interrupter_state = digitalReadFast(interrupter_pin);

  // MOTOR ACTION
  if(button_state == HIGH){  
     motor_enable = true;
     interval_timer_motor = 0;
  } 
  if ((interval_timer_motor <= interval_motor_) && (motor_enable == true)){
    digitalWrite(led, HIGH); 
    digitalWrite(motor_enable_pin, HIGH); 
    } else {
    motor_enable = false;
    digitalWrite(led, LOW); 
    digitalWrite(motor_enable_pin, LOW);       
  }

  // CHECK INTERRUPT
  if(interrupter_state == HIGH){  
    interrupted = true;
    if(interrupt_saved == true){
      interruptedPosition = newPosition; // save current position at interrupt
      interrupt_saved = false; // make sure only first occurence is saved
    }
  } 

  // ROTARY ENCODER
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
      // Check if beam was interrupted in between
      if (interrupted == true){
        digitalWrite(sync_interrupter_pin, HIGH);
      }

      // Serial print: Position_Interruped_InterruptedPosition_Motor
      Serial.print(newPosition);
      Serial.print("_");
      if (interrupted == true){
        Serial.print("1_");
        Serial.print(interruptedPosition);
        Serial.print("_");
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
      
      interrupt_saved = true;      
      interrupted = false;
      digitalWrite(sync_pin, LOW);
      digitalWrite(sync_interrupter_pin, LOW);
      }
    }

  if(measure_timer > 1000){
    measurement = false;
    start_clock = true;
  }
}
