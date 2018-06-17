#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Phoenix";
const char* password = "stworzonka";

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

void setup () {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting");
  while(WiFi.status()!=WL_CONNECTED) {
 
    delay(1000);
    Serial.print(".");
  }
}
 
void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  { //Check WiFi connection status
   
    HTTPClient http;  //Declare an object of class HTTPClient
  
    http.begin("http://domobot.monettosa.nstrefa.pl/www/ESP8266.php");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
  
    if (httpCode > 0)
    {
      String www=http.getString();   //Get the request response payload
      String ok=getValue(www, ';', 0);
      String pinmode=getValue(www, ';', 1);
      String pinwrite=getValue(www, ';', 2);
 
      if(ok!="OK")
      {
        Serial.print("Problem: ");
        Serial.println(www);
      }
      else
      {
        for(int i0=0; i0<=4; i0++)
        {
          pinMode(i0, bitRead(pinmode.toInt(), i0));
          digitalWrite(i0, bitRead(pinwrite.toInt(), i0));
        }
      }
      
      Serial.println(www);                     //Print the response payload
      Serial.println(ok);                     //Print the response payload
      Serial.println(pinmode);                     //Print the response payload
      Serial.println(pinwrite);                     //Print the response payload
     }
   
    http.end();   //Close connection
   
  } 
  delay(1000);    //Send a request every 3 seconds
 
}
