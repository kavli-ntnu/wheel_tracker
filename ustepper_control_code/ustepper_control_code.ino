#include <uStepper.h>

#define MAXACCELERATION 100         // Max acceleration = 100 Steps/s^2
#define MAXVELOCITY 500             // Max velocity = 500 steps/s

const int motor_enable_pin = 2;
uStepper stepper(MAXACCELERATION, MAXVELOCITY);

void setup() {
  pinMode(motor_enable_pin, INPUT);
  stepper.hardStop(SOFT);
}

void loop() {
  int motor_run = digitalRead(motor_enable_pin);
  
  if (motor_run == HIGH) {
      if(!stepper.getMotorState())
        {
         stepper.moveSteps(3000, CW, SOFT); // move 3000 steps and stop without blocking stepper
        }
   }
}
