/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

int period_on = 0;
int period_off = 0;
bool is_on = false;
int time_last_changed = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     

  period_on = 1000;
  period_off = 500;
  is_on = false;
  time_last_changed = millis();
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned long now = millis();
  unsigned long period = now - time_last_changed;
  if( is_on ){
    if( period >= period_on ){
      is_on = false;
      time_last_changed = now;
      digitalWrite( led, LOW );
    }
  }
  else{
    if( period >= period_off ){
      is_on = true;
      time_last_changed = now;
      digitalWrite( led, HIGH );
    }
  }
}
