//ubidots 
#include <WiFi.h>
#include <PubSubClient.h>


#include <Wire.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C



#define WIFISSID "WEP" // Put your WifiSSID here
#define PASSWORD "wep@p015" // Put your wifi password here
#define TOKEN "BBFF-nNchyAXUJLve69VahV5IGpB9e65aVA" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "zabeen" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
                                           //it should be a random and unique ascii string and different from all other devices
 
/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "altitute" // Assing the variable label
#define VARIABLE_LABEL1 "temperature"
#define VARIABLE_LABEL2 "pressure"
//#define VARIABLE_LABEL3 "temperature"

#define DEVICE_LABEL "esp32" // Assig the device label

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char topic1[150];
char topic2[150];
// Space to store values to send
char str_a[10];
char str_p[10]; 
char str_t[10];
WiFiClient ubidots;
PubSubClient client(ubidots);
 
void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}



#define SEALEVELPRESSURE_HPA (1013.25)
float tempC,tempF,atPressure,altitude,humidity;
void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

WiFi.begin(WIFISSID, PASSWORD);
Serial.println();
  Serial.print("Waiting for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback); 


}

void loop() {
 if (!client.connected()) {
    reconnect();
  }
    printValues();
    delay(2000);


    
}
void printValues(){
 
  Serial.print("Temperature = ");
  tempC = bmp.readTemperature();
//Serial.print(bme.readTemperature());
Serial.print(tempC);
Serial.println(" deg C");


Serial.print("Temperature = ");
Serial.print(1.8*tempC+32);
Serial.println(" deg F");

Serial.print("Pressure = ");
atPressure=bmp.readPressure()/100.0F;
Serial.print(atPressure);
Serial.println(" hPa");


Serial.print("Altitude = ");
altitude=bmp.readAltitude(SEALEVELPRESSURE_HPA);
Serial.print(altitude);
Serial.println(" m");
Serial.println();

sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  
 
   
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(altitude, 4, 2, str_a);
  
 // DHT dht(DHTPIN, DHTTYPE);
 //  float h = dht.readHumidity();
  //float t = tempC;
  dtostrf(atPressure,4,2,str_p);
  dtostrf(tempC,4,2,str_t);
// publishing altitute
  sprintf(payload, "%s {\"value\": %s}}", payload, str_a); // Adds the value
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
// Temperature and Humidity 

sprintf(topic1, "%s", ""); // Cleans the topic content
  sprintf(topic1, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload content
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL1); // Adds the variable label
  sprintf(payload, "%s {\"value\": %s", payload,  str_t); // Adds the value
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
  client.publish(topic1, payload);
 // pressure 
  sprintf(topic2, "%s", ""); // Cleans the topic content
  sprintf(topic2, "%s%s", "/v1.6/devices/", DEVICE_LABEL);

  sprintf(payload, "%s", ""); // Cleans the payload content
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL2); // Adds the variable label
  sprintf(payload, "%s {\"value\": %s", payload,  str_p); // Adds the value
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets

  client.publish(topic2, payload);
  
  client.loop();
  delay(500);


}
