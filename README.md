# Wheel tracker

Documentation for behavioral wheel used for head fixed in vivo recordings. It enables precise recordings of wheel movement and enforced motion via a stepper motor. The motor is coupled to the main axle via a one-way clutch. This way the animal can always outrun the (slower) motor movement, but is blocked from running backwards. Two microcontrollers are used, one for recording the wheel motion, one as stepper motor driver. 

![alt text][logo]

- One way clutch: HF0612 KULELAGER AS, Molde, NO (post@kulelager24.no)
- Bearing: HK 0608 KULELAGER AS, Molde, NO 
- Shaft coupler 6 mm inner diameter (similar to [this][shaft coupler])
- GT2 timing belt [pulley][pulley] (this one has to be enlarged to fit bearings)
- GT2 timing [belt][belt]

#### Main assembly
The wheel is made from black foam and has a radius of 85mm and a width of 70mm. Larger diameters can be chosen if needed. 
- Customized aluminium parts: [STEP][step files] files | [Drawings][drawings]
- Thorlabs posts (minimum height for radius 85 mm) - RS2.5P/M - Ø25.0 mm Pedestal Pillar Post, M6 Taps, L = 65 mm
- Thorlabs breadboard (e.g. MB3030/M)

### Rotary encoder and photo interrupter (microcontroller 1)
A YUMO Rotary Encoder (model E6B2-CWZ3E, see [datasheet](mouser_datasheet_YUMO.pdf)) connected to a [teensy 3.5][Teensy homepage] is used to digitize the wheel motion. A t-slot [photo interruper][t-slot] is used to record 360 degree turns. 
- Arduino [code][teensy code] teensy3.5
- [Teensy encoder library][encoder library]

Recording starts upon rising edge detection on `frameclock_pin` and stops when there was no signal on this pin for > 500 ms (adjustable, see code). This event and the detection of a beam break (photo interrupter) are detected as hardware interrupts on the teensy and sent over to the recording computer via USB serial (see section *Recorder* below) in 20 ms intervals (`interval_`). Before and after data is sent via serial the `sync pin` is switched, which can be used to record when communication happened (the delay between measurement of wheel position and initiation of data transfer is assumed to be negligible). This sync pin is connected to an event recorder (femtonics digital in or else). 
The teensy also registers user button presses (`button_pin`) and sends out a HIGH on `motor_enable_pin` to microcontroller 2 (see below) over a certain interval (`interval_motor_`). During this time a *1* will be sent over for entry *Motor* (see serial data below).

### Motor (microcontroller 2)
For motor control a NEMA17 stepper motor is used, controlled by [uStepper][ustepper homepage] board, running a acceleration/deceleration protocol over defined number of steps. The stepper motor driver gets triggered by the Teensy 3.5 when a button is pressed. 
- Arduino [code][ustepper code] uStepper
- [Product PDF ustepper][pdf uStepper]
- [Documentation page][documentation ustepper] for uStepper

### Serial
Data is recorded via USB connection to the teensy (see [save_tracking.py][python code wheel]). It arrives in the form `"Position_Interruped_InterruptedPosition_Motor"`, where `Position` is the integer rotary encoder position, `Interrupted` is either 0 or 1 for when a beam break was detected, `InterruptedPosition` is the integer position at the time when the beam was interrupted, `Motor` is either 0 or 1 for the times when the user pressed the button.

### Camera
For behavioral recording this setup is paired with a fast camera that tracks the animal's movements on the wheel. The camera sends a HIGH during exposure which can be logged with an event recorder as for the teensy pulses (see above).
 - Camera [acA2000-165umNIR - Basler ace][basler camera]
 - Lens [VS-1614H1N][lens]
 - GP I/O [cable][cable basler]
 
 
[logo]: wheel_cad.jpg
[shaft coupler]: https://www.geartech.no/produkter/transmission/akselkoblinger/fast-kobling/
[pulley]: https://www.adafruit.com/product/1254
[belt]: https://www.adafruit.com/product/1254

[step files]: /step_files_wheel
[drawings]: drawings_wheel.pdf

[ustepper homepage]: http://www.ustepper.com/index/
[Teensy homepage]: https://www.pjrc.com/store/teensy35.html
[t-slot]: https://www.adafruit.com/product/3985

[encoder library]: https://www.pjrc.com/teensy/td_libs_Encoder.html

[teensy code]: /motor_control_teensy/motor_control_teensy.ino


[ustepper code]: /ustepper_control_code/ustepper_control_code.ino
[documentation ustepper]: http://ustepper.com/docs/html/index.html
[pdf ustepper]: /ustepper_control_code/product_sheet_revB.pdf

[python code wheel]: /wheel_tracker/save_tracking.py

[basler camera]: https://www.baslerweb.com/en/products/cameras/area-scan-cameras/ace/aca2000-165umnir/
[lens]: https://vst.co.jp/en/vs-h1-series/
[cable basler]: https://www.baslerweb.com/de/produkte/vision-komponenten/kabel/gp-i-o-cable-6p-open-10-m/