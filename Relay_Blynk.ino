#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLaPabZxUe"
#define BLYNK_DEVICE_NAME "IoT Switch1"
//#define BLYNK_AUTH_TOKEN "H_AgXSlcPAnIvlYLKrnkaUQ5LMXlNlAX"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "H_AgXSlcPAnIvlYLKrnkaUQ5LMXlNlAX";//Paste auth token you copied

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "IOTlab";///Enter your wifi name
char pass[] = "iotlab@p202";// Enter wifi password

BlynkTimer timer;
int relay=13;
BLYNK_WRITE(V0) // Executes when the value of virtual pin 0 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(relay,HIGH);  // Set digital pin 2 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(relay,LOW);  // Set digital pin 2 LOW    
  }
}

void sendSensor()
{}
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.


void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(relay,OUTPUT);
  
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);



  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
