  //WIFI
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "C:\Users\Kamil\Documents\domobot\wifi.h";

  //IR
#include <IRremoteESP8266.h>
#include <IRrecv.h>

  //DAPI
#include "C:\Users\Kamil\Documents\domobot\dapi.h";
String DAPI_DEVICE="antresola";

  //LED
const int IRDA = 14;
int LED_CH=0;             //numer kanalu, ktory aktualnie obslugujemy
const int kanaly=8;       //ile jest kanalow (RGBW to 4 kanaly)
const int LED_PIN[]={16, 5, 4, 0, 2, 12, 13, 15};       //1R, 1G, 1B, 1W, 1L, 2R, 2G, 2B, 2W, 2L UWAGA! 3 i 1 to TX i RX. Albo białe światła, albo Serial
//const int kanaly=10;       //ile jest kanalow (RGBW to 4 kanaly)
//const int LED_PIN[]={16, 5, 4, 0, 2, 12, 13, 15, 3, 1};       //1R, 1G, 1B, 1W, 1L, 2R, 2G, 2B, 2W, 2L UWAGA! 3 i 1 to TX i RX. Albo białe światła, albo Serial
int jasnosc[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //jasnosc kanalow/diod
boolean STATE[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //docelowo do usuniecia, bo jasnosc juz trzyma stan diod

int animacja=0;           //numer animacji
int etap=0;               //numer etapu w animacji, bo bez etapow dlugich nie mozna wylaczyc

IRrecv irrecv(IRDA);
decode_results results;

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void dioda_zmien(int dioda, int stan=-1, int update=0){
    //ustawia jasnosc diody, 0-255, -1 oznacza stan przeciwny
  if(stan==-1) analogWrite(LED_PIN[dioda], STATE[dioda]=STATE[dioda]>0?0:75);
  else if(stan==0) analogWrite(LED_PIN[dioda], STATE[dioda]=0);
  else analogWrite(LED_PIN[dioda], STATE[dioda]=1023);

  //update na serwerze
  if(animacja==0 && update>0) //zeby nie dostal pierdolca przy animacjach
  {
    String link=DAPI_URL+"?name="+DAPI_NAME+"&secret="+DAPI_SECRET+"&action=ledUpdate&device="+DAPI_DEVICE+"&data={%22"+dioda+"%22:"+STATE[dioda]+"}";
    Serial.println(link);
    
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(link);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
  }
//    Serial.println(LED_PIN[dioda]);
//    Serial.println(STATE[dioda]);
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
    if(LED_CH<0) LED_CH=kanaly-1;
  } else {
    LED_CH=LED_CH+1;
    if(LED_CH>(kanaly-1)) LED_CH=0;  
  }
  blink(LED_CH);      
  Serial.println(LED_CH);
}

void jasnosc_zmien(int kierunek){
  //zmienia jasnosc kanalu (jesli kierunek 1 to w gore, a 0 to w dol)
  int krok=32;
  if(kierunek==-1){
    jasnosc[LED_CH]=jasnosc[LED_CH]-krok;
    if(jasnosc[LED_CH]<0) jasnosc[LED_CH]=0;
  } else {
    jasnosc[LED_CH]=jasnosc[LED_CH]+krok;
    if(jasnosc[LED_CH]>255) jasnosc[LED_CH]=255;  
  }
  analogWrite(LED_PIN[LED_CH], jasnosc[LED_CH]);    
  Serial.println(jasnosc[LED_CH]);
}

void animacja_zmien(int kierunek){
  //zmienia animacja na sasiedni (jesli kierunek 1 to w gore, 0 to wylacz, a -1 to w dol)
  int max=3;  //ile jest animacji
  
  if(kierunek==0){
    animacja=0;
  } else if(kierunek==-1){
    animacja=animacja-1;
    if(animacja<0) animacja=max;
  } else {
    animacja=animacja+1;
    if(animacja>max) animacja=0;  
  }
  nieanimuj();      
  Serial.println(animacja);
}

void nieanimuj()
{
    //przywraca stan domyslny po animacji
  etap=0;
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
    case 1: //stroboskop
      digitalWrite(LED_PIN[4], HIGH);
      digitalWrite(LED_PIN[5], HIGH);
      delay(10);
      digitalWrite(LED_PIN[4], LOW);
      digitalWrite(LED_PIN[5], LOW);
      delay(150);
      break;
      
    case 2: //policja
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
      
    case 3: //chromoterapia
      int jasnoscR=0;
      int jasnoscG=0;
      int jasnoscB=0;
      int czas=100;
      int kroki=64;
      int mnoznik=256/kroki;

      //ilosc etapow to 3 diody(RGB)*2etapy (+, -)*64 kroki=384 etapy(0-383)

      if(etap<=kroki*1-1)
      {
        //R+BMax Niebieski->Różowy
        jasnoscR=mnoznik*(etap-kroki*0);
        jasnoscB=255;
      }
      else if(etap<=kroki*2-1)
      {
        //B-RMax Różowy->Czerwony
        jasnoscB=mnoznik*(kroki*2-etap-1);
        jasnoscR=255;
      }
      else if(etap<=kroki*3-1)
      {
        //G+RMax Czerwony->Żółty
        jasnoscG=mnoznik*(etap-kroki*2);
        jasnoscR=255;
      }
      else if(etap<=kroki*4-1)
      {
        //R-GMax Żółty->Zielony
        jasnoscR=mnoznik*(kroki*4-etap-1);
        jasnoscG=255;
      }
      else if(etap<=kroki*5-1)
      {
        //B+GMax Zielony->Turkusowy
        jasnoscB=mnoznik*(etap-kroki*4);
        jasnoscG=255;
      }
      else if(etap<=kroki*6-1)
      {
        //G-BMax Turkusowy->Niebieski
        jasnoscG=mnoznik*(kroki*6-etap-1);
        jasnoscB=255;
      }
      else etap=-1;

      if(etap!=-1)
      {
          //jesli normalny etap to swiec diody przez okreslony czas, a jesli nastapilo zerowanie etapu to nie mrugaj nimi, a przeskocz do nastepnego etapu
        analogWrite(LED_PIN[0], jasnoscR);
        analogWrite(LED_PIN[1], jasnoscG);
        analogWrite(LED_PIN[2], jasnoscB);
        delay(czas);
      }

      break;
  } 
//  Serial.println(etap);
  etap++;
}



void setup () {

    //serial
  Serial.begin(115200);

    //wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  Serial.print("Connecting");
  while(WiFi.status()!=WL_CONNECTED) {
 
    delay(1000);
    Serial.print(".");
  }

    //ir
  irrecv.enableIRIn();
//  irrecv.blink13(true);

  for(int i0=0; i0<kanaly; i0++) pinMode(LED_PIN[i0], OUTPUT);

  blink(0); //mrugnij pierwszymi diodami, ze dziala
}
 
void loop()
{
    //ir
  delay(200);
  if(irrecv.decode(&results))
  {
    int wynik=results.value;
    Serial.println(wynik);
    irrecv.resume();
    if(results.value==551505585) dioda_zmien(0, -1, 1);
    if(results.value==551521905) dioda_zmien(1, -1, 1);
    if(results.value==551519865) dioda_zmien(2, -1, 1);
    if(results.value==551536185) dioda_zmien(3, -1, 1);
    if(results.value==551542815) dioda_zmien(4, -1, 1);
    if(results.value==551510175) dioda_zmien(9, -1, 1);
    if(results.value==551537970) kanal_zmien(1);
    if(results.value==551505330) kanal_zmien(0);
    if(results.value==551487480) jasnosc_zmien(1);
    if(results.value==551520120) jasnosc_zmien(-1);

    if(results.value==551489010) animacja=1;
    if(results.value==551509410) animacja_zmien(0);
    if(results.value==551514510) animacja_zmien(1);
    if(results.value==551547150) animacja_zmien(-1);
  }
/*  else if(WiFi.status()==WL_CONNECTED)    //wifi
  {
    HTTPClient http;  //Declare an object of class HTTPClient
    String link=DAPI_URL+"?name="+DAPI_NAME+"&secret="+DAPI_SECRET+"&action=ledGet&device="+DAPI_DEVICE+"&format=semicolon";
//    Serial.println(link);
    
    http.begin(link);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
  
    if(httpCode>0)
    {
      String www=http.getString();   //Get the request response payload
      String ok=getValue(www, ';', 0);
//      String ledmode=getValue(www, ';', 1);
 
      if(ok!="OK")
      {
        Serial.print("Problem: ");
        Serial.println(www);
      }
      else
      {
        for(int i0=0; i0<kanaly; i0++)
        {
            //sterowanie diodami
          String tmp=getValue(www, ';', 1+i0);  //odczytuj kolejne pozycje
//          dioda_zmien(i0, tmp.toInt());
        }
      }
      
      Serial.println(www);                     //Print the response payload
//      Serial.println(ok);                     //Print the response payload
//      Serial.println(pinmode);                     //Print the response payload
//      Serial.println(pinwrite);                     //Print the response payload
     }
   
    http.end();   //Close connection
   
  } 
*/  else if(animacja>0) animuj(animacja);

//  Serial.println(animacja);
//  delay(500);
}
