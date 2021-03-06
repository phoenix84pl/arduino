/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int test = 3;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(test, OUTPUT);  
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
  analogWrite(test, 16);
  delay(1000);               // wait for a second
  analogWrite(test, 32);
  delay(1000);               // wait for a second
  analogWrite(test, 64);
  delay(1000);               // wait for a second
  analogWrite(test, 128);
  delay(1000);               // wait for a second
  analogWrite(test, 192);
  delay(1000);               // wait for a second
   digitalWrite(led, HIGH);    // turn the LED off by making the voltage LOW
 analogWrite(test, 255);
  delay(1000);               // wait for a second
}
