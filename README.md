# Wheel tracker

Documentation for behavioral wheel. 

![alt text][logo]


Uses a YUMO Rotary Encoder (model E6B2-CWZ3E, see [datasheet](mouser_datasheet_YUMO.pdf)) and [teensy 3.5][Teensy homepage] with [encoder library][encoder library]. 




For motor control a standard NEMA17 stepper motor is used, controlled by [uStepper][ustepper homepage] board, running a acceleration/deceleration protocol over defined number of steps. The stepper motor driver gets triggered by the Teensy 3.5 when a button is pressed. 
- Arduino [code][ustepper code] uStepper


http://ustepper.com/docs/html/index.html

[logo]: wheel_cad.png
[ustepper homepage]: http://www.ustepper.com/index/
[Teensy homepage]: https://www.pjrc.com/store/teensy35.html
[encoder library]: https://www.pjrc.com/teensy/td_libs_Encoder.html
[ustepper code]: /ustepper_control_code