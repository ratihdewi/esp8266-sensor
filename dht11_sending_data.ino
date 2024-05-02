#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <MQ2.h>
//#include <UniversalTelegramBot.h>
//#include <WiFiClientSecure.h>

#define DHTPIN11 D3
#define DHTPIN22 D2     

#define DHTTYPE11 DHT11   // DHT 11
#define DHTTYPE22 DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
const char* ssid = "Yuhu";
const char* password = "Kampret123";

//const char* ssid = "Kost Simprug 43";  
//const char* password = "Zubair28";

//interval simpan data ke db
int period = 30000; 
unsigned long time_now = 0;

//telegram
//#define BOTtoken "6441116256:AAEvZS0KSHGuazbvO2dFymyOzejraIX4MYs"
//#define idChat "6441116256"
//
//WiFiClientSecure client;
//UniversalTelegramBot bot(BOTtoken, client);

DHT dht11(DHTPIN11, DHTTYPE11);
DHT dht22(DHTPIN22, DHTTYPE22)

//sensor mq
int pinAout = A0;
int lpg_gas, co_gas, smoke_gas;
MQ2 mq2(pinAout);

float sensorValue;
float ppm;

//buzzer
#define buzzer D7

void setup() {
  Serial.begin(9600);
  //Serial.println(F("DHTxx test!"));
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    int i=0;
    while(WiFi.status() != WL_CONNECTED){ 
      Serial.print(".");
      delay(1000);     
    } 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  pinMode(buzzer, OUTPUT);
  delay(2000); 
}

void loop() { 
    sensor();

    if(millis() >= time_now + period){
    time_now += period;
    Sendingdata();
  }
  // double kelembaban = dht.readHumidity();
  // double suhu = dht.readTemperature();

  // double kelembaban22 = dht.readHumidity();
  // double suhu22 = dht.readTemperature();

  // // Check if any reads failed and exit early (to try again).
  // if (isnan(kelembaban) || isnan(suhu)) {
  //   Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }

  // sensorValue = analogRead(pinAout);
  // ppm = 10.938 * exp(1.7742 * (sensorValue * 5.0 / 4095));

  // if(ppm<0){
  //   ppm = 0;
  // }

  // String print_ppm = "Methane PPM: "; print_ppm += String(ppm);
  
  // Serial.print(F("Temperature:"));
  // Serial.print(suhu);
  // Serial.print(F("°C   Humidity:"));
  // Serial.print(kelembaban);
  // Serial.println(F("%"));
  // Serial.println(ppm);
  // Serial.println(print_ppm);


  /*if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    StaticJsonDocument<200> doc;
    String url, nodemcuData; 
    
    //ip webserver
    url ="http://10.4.171.66/arduino/api/index.php";
    
    doc["suhu"] = String(suhu);
    doc["kelembaban"] = String(kelembaban);

    doc["suhu22"] = String(suhu22);
    doc["kelembaban22"] = String(kelembaban22);
    
    doc["ppm"] = String(ppm);

    http.begin(client,url);
    http.addHeader("Content-Type", "application/json");
    
    serializeJson(doc, nodemcuData);
    Serial.print("POST data >> ");
    Serial.println(nodemcuData); 
  
    int httpCode = http.POST(nodemcuData);//Send the request
    String payload;  
    if (httpCode > 0) { //Check the returning code    
        payload = http.getString();   //Get the request response payload
        payload.trim();
        if( payload.length() > 0 ){
           Serial.println(payload + "\n");
        }
    }
    
    http.end();   //Close connection
  }else{
    Serial.print("Not connected to wifi ");Serial.println(ssid);
  }*/

  // if(suhu >26 && kelembaban < 100 && ppm > 46){
//    bot.sendChatAction(idChat, "Sedang mengetik...");
    // digitalWrite(buzzer, HIGH);
    // delayMicroseconds(10);

//    String t = "Intensitas suhu : ";
//    t += int(suhu);
//    t += " *C\n";
//    t += "Suhu maksimal gaes\n";
//    bot.sendMessage(idChat, t, "");
    
  //   Serial.print("Mengirim data sensor ke telegram");
  // }
  // else {
  //   digitalWrite(buzzer, LOW);
  //   delayMicroseconds(10);
  // }
  // delay(10000); //interval 60s
}

void sensor()
{
  double kelembaban = dht11.readHumidity();
  double suhu = dht11.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  sensorValue = analogRead(pinAout);
  ppm = 10.938 * exp(1.7742 * (sensorValue * 5.0 / 4095));

  if(ppm<0){
    ppm = 0;
  }

  String print_ppm = "Methane PPM: "; print_ppm += String(ppm);
  
  Serial.print(F("Temperature:"));
  Serial.print(suhu);
  Serial.print(F("°C   Humidity:"));
  Serial.print(kelembaban);
  Serial.println(F("%"));
  Serial.println(ppm);
  Serial.println(print_ppm);

  if(suhu >26 && kelembaban < 100 && ppm > 46)
  {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(10);
  } else {
    digitalWrite(buzzer, LOW);
    delayMicroseconds(10);
  }

  delay(10000); //interval 60s
}

void sesnsor2()
{
  double kelembaban22 = dht22.readHumidity();
  double suhu22 = dht22.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(kelembaban22) || isnan(suhu22)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  sensorValue = analogRead(pinAout);
  ppm = 10.938 * exp(1.7742 * (sensorValue * 5.0 / 4095));

  if(ppm<0){
    ppm = 0;
  }

  String print_ppm = "Methane PPM: "; print_ppm += String(ppm);
  
  Serial.print(F("Temperature dht22:"));
  Serial.print(suhu22);
  Serial.print(F("°C   Humidity dht22:"));
  Serial.print(kelembaban22);
  Serial.println(F("%"));
  Serial.println(ppm);
  Serial.println(print_ppm);

  if(suhu22 >26 && kelembaban22 < 100 && ppm > 46)
  {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(10);
  } else {
    digitalWrite(buzzer, LOW);
    delayMicroseconds(10);
  }

  delay(10000); //interval 60s
}

void Sendingdata()
{
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    StaticJsonDocument<200> doc;
    String url, nodemcuData; 
    
    //ip webserver
    url ="http://10.4.171.66/arduino/api/index.php";
    
    doc["suhu"] = String(suhu);
    doc["kelembaban"] = String(kelembaban);

    doc["suhu22"] = String(suhu22);
    doc["kelembaban22"] = String(kelembaban22);
    
    doc["ppm"] = String(ppm);

    http.begin(client,url);
    http.addHeader("Content-Type", "application/json");
    
    serializeJson(doc, nodemcuData);
    Serial.print("POST data >> ");
    Serial.println(nodemcuData); 
  
    int httpCode = http.POST(nodemcuData);//Send the request
    String payload;  
    if (httpCode > 0) { //Check the returning code    
        payload = http.getString();   //Get the request response payload
        payload.trim();
        if( payload.length() > 0 ){
           Serial.println(payload + "\n");
        }
    }
    
    http.end();   //Close connection
  }else{
    Serial.print("Not connected to wifi ");Serial.println(ssid);
  }
}