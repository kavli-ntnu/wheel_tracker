int frameclock_pin = 3;
int sync_pin = 4;
int status_measure;

void setup() {
  // put your setup code here, to run once:
  pinMode(frameclock_pin, INPUT);
  pinMode(sync_pin, OUTPUT);
}

void loop() {
  status_measure = digitalReadFast(frameclock_pin);
   if (status_measure == HIGH){
     digitalWrite(sync_pin, HIGH); 
     delay(50);    
     digitalWrite(sync_pin, LOW); 
   }
}
