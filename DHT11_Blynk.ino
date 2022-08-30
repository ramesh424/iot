#define BLYNK_PRINT Serial
//#define BLYNK_TEMPLATE_ID "**************"      //Paste your credentials here
//#define BLYNK_DEVICE_NAME "*****************************"  //Paste your credentials here
#define BLYNK_TEMPLATE_ID "TMPLFqKs3KF2"
#define BLYNK_DEVICE_NAME "Automatic Plant Watering System"
//#define BLYNK_AUTH_TOKEN "OiDo8pEbjFrThq0qf0w0L6BpNvK4dN_u"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "OiDo8pEbjFrThq0qf0w0L6BpNvK4dN_u";//Paste auth token you copied

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "IOTlab";///Enter your wifi name
char pass[] = "iotlab@p202";// Enter wifi password

#define DHTPIN 27         // What digital pin we're connected to select yours accordingly

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
int moistpin=32;
BLYNK_WRITE(V3)
{
  int moistvalue = param.asInt();
  digitalWrite(15,moistvalue);
  }
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  int moistvalue = analogRead(moistpin);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, h); // select your virtual pins accordingly
  Blynk.virtualWrite(V0, t); // select your virtual pins accordingly
    Blynk.virtualWrite(V2, moistvalue); // select your virtual pins accordingly
  
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(15,OUTPUT);
delay(1000);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
