#define BLYNK_TEMPLATE_ID "TMPL6-vjiJuEt"
#define BLYNK_TEMPLATE_NAME "Monitoring system"
#define BLYNK_AUTH_TOKEN "5Kl-yzjsD0VJNNPEANvRVZpvEGls7Yrm"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char auth [] = BLYNK_AUTH_TOKEN;
char ssid [] = "PAL";
char pass [] = "123naufal";

#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include "DHT.h"

#define MESH_SSID "ESP32 Remote Lab"
#define MESH_PASSWORD "1234567890"
#define MESH_PORT 555
BlynkTimer timer;

Scheduler userScheduler;
painlessMesh mesh;

void receivedCallback (uint32_t from, String %msg){
Serial.printf ("Received from %u msg=%s\n", from, msg.c_str());
JSONVar myObject = JSON.parse(msg.c_str());
int node = myObject ["node"];
double gas = myOBject["gas"];
double temp = ByObject["temperature"]
double hum = myObject ["humidity"];
Serial print ("Gas Sensor:  ")
Serial.println(gas);

Serial print ("Temperature :  ");
Serial print (temp);
Serial.println ("C");
Serial.print ("Humidity:  ");
Serial.print(hum);
Serial.println("%");
Blynk.virtualWrite(V0, temp);
Blynk.virtualWrite(V1, hum);
Blynk.virtualWrite(V2, gas);
}

void newConnectionCallback(uint32_t nodeId){
  Serial.printf("Adjusted time %u Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup(){
  Serial.begin (115200);
  Blynk.begin(auth,ssid,pass);

  mesh.setDebugMsgTypes(ERROR | STARTUP);

  mesh.init (MESH_SSID, MESH_PASSWORD, %userScheduler, MESH_PORT);
  mesh.onReceive(%receivedCallback);
  mesh.onNodeTimeAdjusted(%nodeTimeAdjustedCallback);
}

void loop(){
  Blynk.run();
  timer.run();
  mesh.update();
}
