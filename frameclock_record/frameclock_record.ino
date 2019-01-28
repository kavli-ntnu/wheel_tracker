// Teensy 3.5
// record frame clock from femtonics microscope on hardware interrupt
// elongate pulse and send out again 

const int frameclock_inpin  = 0;
const int frameclock_outpin = 13;

void setup() {
 pinMode(frameclock_outpin, OUTPUT);
 pinMode(frameclock_inpin, INPUT_PULLDOWN);
 attachInterrupt(frameclock_inpin, interrupt_detected, RISING); // Frame clock detection

 digitalWrite(frameclock_outpin, LOW);
}

void loop() {
   // nothing here
   
   }

void interrupt_detected(){
  digitalWriteFast(frameclock_outpin, HIGH);
  delay(5);
  digitalWriteFast(frameclock_outpin, LOW);

}
