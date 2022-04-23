#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "D-Link";                   // wifi ssid
const char* password =  "atiwifi++";         // wifi password
const char* mqttServer = "192.168.1.9";    // IP adress Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "mayna";      // if you don't have MQTT Username, no need input
const char* mqttPassword = "mayna";  // if you don't have MQTT Password, no need input
const char* client_ID = "client";  // if you don't have MQTT Password, no need input
#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);  // on indique la broche et le type de capteur
#define Led 2
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {

  Serial.begin(115200);
  Serial.println("DHT11 sensor!");
  //call begin to start sensor
dht.begin();
  pinMode(Led, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);

    }
  }

//  client.publish("esp8266", "Hello Raspberry Pi");
//  client.subscribe("esp8266");

}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}

void loop() {
   digitalWrite(Led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(Led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
//get value of humidity 
float h = dht.readHumidity();
//get value of temp abd  Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Check if any reads failed cad ==0 ou negative with isnan and exit early (to try again).
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
// print the result to Terminal
Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
//we delay a little bit for next read
delay(2000);
  
    client.publish("humid", String(h).c_str());
    client.publish("tempF", String(t).c_str());
    
    client.publish("esp8266", "Hello Raspberry Pi");
    client.subscribe("esp8266");
    delay(300);
  client.loop();
}
