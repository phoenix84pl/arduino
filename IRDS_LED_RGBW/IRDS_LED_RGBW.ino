#include <IRremote.h>

const int IRDA = 12;
int LED_CH=0;             //numer kanalu (kanal to tasma z led z oddzielnym zarzadzaniem
const int LED_PIN[]={5, 6, 10, 7, 3, 4, 13};       //1R, 1G, 1B, 1W, 2W, 3W, 4R, 4G, 4B, 4W (stan docelowy na 4 kanaly)
const int CH_START[]={0, 4, 5, 6};              //na ktorej diodzie z LED_PIN zaczyna sie ktory kanal
boolean STATE[]={LOW, LOW, LOW, LOW, LOW, LOW};

int animacja=0;

IRrecv irrecv(IRDA);
decode_results results;

void dioda_zmien(int dioda){
    //przelacza diode na stan przeciwny
  digitalWrite(LED_PIN[dioda], STATE[dioda]=!STATE[dioda]);
  Serial.println(LED_PIN[dioda]);
  Serial.println(STATE[dioda]);
//   analogWrite(LED_PIN[dioda], 255);
}

void blink(int dioda){
  digitalWrite(LED_PIN[dioda], LOW);
  delay(50);
  digitalWrite(LED_PIN[dioda], HIGH);
  delay(50);
  digitalWrite(LED_PIN[dioda], STATE[dioda]);
}

void kanal_zmien(int kierunek){
  //zmienia kanal na sasiedni (jesli kierunek 1 to w gore, a 0 to w dol)
  if(kierunek==0){
    LED_CH=LED_CH-1;
    if(LED_CH<0) LED_CH=(sizeof(CH_START)/2)-1;
  } else {
    LED_CH=LED_CH+1;
    if(LED_CH>((sizeof(CH_START)/2)-1)) LED_CH=0;  
  }
  blink(CH_START[LED_CH]);      
  Serial.println(LED_CH);
}

void animacja_zmien(int kierunek){
  //zmienia animacja na sasiedni (jesli kierunek 1 to w gore, 0 to wylacz, a -1 to w dol)
  int max=3;  //ile jest animacji
  
  if(kierunek==0){
    animacja=0;
  } else if(kierunek==-1){
    animacja=animacja-1;
    if(animacja<0) LED_CH=max;
  } else {
    animacja=animacja+1;
    if(animacja>max) animacja=0;  
  }
  nieanimuj();      
  Serial.println(animacja);
}

void nieanimuj()
{
  digitalWrite(LED_PIN[0], STATE[0]);
  digitalWrite(LED_PIN[1], STATE[1]);
  digitalWrite(LED_PIN[2], STATE[2]);
  digitalWrite(LED_PIN[3], STATE[3]);
  digitalWrite(LED_PIN[4], STATE[4]);
  digitalWrite(LED_PIN[5], STATE[5]);
  digitalWrite(LED_PIN[6], STATE[6]);
}

void animuj(int program){
  //funkcja robi jakas animacje
  
//  Serial.println("Animacja");

  switch(program){
    case 3:
      digitalWrite(LED_PIN[4], HIGH);
      digitalWrite(LED_PIN[5], HIGH);
      delay(10);
      digitalWrite(LED_PIN[4], LOW);
      digitalWrite(LED_PIN[5], LOW);
      delay(150);
      break;
      
    case 2:
      digitalWrite(LED_PIN[0], HIGH);
      digitalWrite(LED_PIN[1], LOW);
      digitalWrite(LED_PIN[2], LOW);
      digitalWrite(LED_PIN[3], LOW);
      delay(100);
      digitalWrite(LED_PIN[0], LOW);
      digitalWrite(LED_PIN[1], LOW);
      digitalWrite(LED_PIN[2], HIGH);
      digitalWrite(LED_PIN[3], LOW);
      delay(100);
      break;
      
    case 1:
      int i0=0;
      int jasnosc=0;
      int czas=100;
      int kroki=64;
      int mnoznik=256/kroki;

      for(i0=kroki; i0>0; i0--){
        //R-
        jasnosc=mnoznik*i0-1;
        analogWrite(LED_PIN[0], jasnosc);
//        Serial.println(jasnosc);
        delay(czas);
      }
      for(i0=0; i0<kroki; i0++){
        //B+
        jasnosc=mnoznik*i0;
        analogWrite(LED_PIN[2], jasnosc);
//        Serial.println(jasnosc);
        delay(czas);
      }
      for(i0=kroki; i0>0; i0--){
        //G-
        jasnosc=mnoznik*i0-1;
        analogWrite(LED_PIN[1], jasnosc);
//        Serial.println(jasnosc);
        delay(czas);
      }
      for(i0=0; i0<kroki; i0++){
        //R+
        jasnosc=mnoznik*i0;
        analogWrite(LED_PIN[0], jasnosc);
//        Serial.println(jasnosc);
        delay(czas);
      }
      for(i0=kroki; i0>0; i0--){
        //B-
        jasnosc=mnoznik*i0-1;
        analogWrite(LED_PIN[2], jasnosc);
//        Serial.println(jasnosc);
        delay(czas);
      }
      for(i0=0; i0<kroki; i0++){
        //G+
        jasnosc=mnoznik*i0;
        analogWrite(LED_PIN[1], jasnosc);
//        Serial.println(jasnosc);
        delay(czas);
      }
      break;
  } 
}

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  pinMode(LED_PIN[0], OUTPUT);
  pinMode(LED_PIN[1], OUTPUT);
  pinMode(LED_PIN[2], OUTPUT);
  pinMode(LED_PIN[3], OUTPUT);
  pinMode(LED_PIN[4], OUTPUT);
  pinMode(LED_PIN[5], OUTPUT);

  blink(0); //mrugnij pierwszymi diodami, ze dziala
}

void loop(){
  if(irrecv.decode(&results)){
    Serial.println(results.value);
    irrecv.resume();
    if(results.value==551505585) dioda_zmien(0);
    if(results.value==551521905) dioda_zmien(1);
    if(results.value==551519865) dioda_zmien(2);
    if(results.value==551536185) dioda_zmien(3);
    if(results.value==551542815) dioda_zmien(4);
    if(results.value==551510175) dioda_zmien(5);
    if(results.value==551537970) kanal_zmien(1);
    if(results.value==551505330) kanal_zmien(0);
    if(results.value==551489010) animacja=1;
    if(results.value==551509410) animacja_zmien(0);
    if(results.value==551514510) animacja_zmien(1);
    if(results.value==551547150) animacja_zmien(-1);
  }
//  Serial.println(animacja);
  if(animacja>0) animuj(animacja);
}

