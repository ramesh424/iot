#include "DHTesp.h"
#include <Wire.h>
#include "SSD1306.h"
#define DHTpin 27
DHTesp dht;
SSD1306 display(0x3c,21,22);
String myString;
int count;

void setup() {
  // put your setup code here, to run once:
  display.init();
  display.setFont(ArialMT_Plain_24);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity(%)\tTemperature(C)");

  dht.setup(DHTpin,DHTesp::DHT11);
  

}

void loop() {
  delay(2000);
  float h = dht.getHumidity();
  float t = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(h);
  Serial.print("\t");
  Serial.println(t);  
display.clear();

myString = "H :" + String(h);
display.drawString(0,0,myString);
display.display();
myString = "T :" + String(t);
display.drawString(0,24,myString);
display.display();
delay(1000);

}
