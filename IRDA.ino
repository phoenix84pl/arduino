int IRPin = 7;   // TSOP2236 podlaczony do pinu 7
int ledPin = 10;  // LED na pinie 10
boolean lastButton = HIGH;
boolean currentButton = HIGH; 
boolean ledOn = false;



void setup () {
pinMode (IRPin, INPUT);
pinMode (ledPin, OUTPUT);
}

boolean debounce(boolean last) 
{
  boolean current = digitalRead(IRPin);
  if (last != current)
  {
    delay (5);
  current = digitalRead(IRPin);
  }
  return current;
}
void loop () {
  currentButton = debounce(lastButton);
if (lastButton ==HIGH && currentButton ==LOW)  //digitalRead(IRPin) == HIGH && lastButton == LOW)
{
  ledOn = !ledOn;
}
lastButton = currentButton;
digitalWrite(ledPin, ledOn);
}
