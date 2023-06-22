#include "painlessMesh.h"
#include <Arduino_JSON.h>

#define MESH_SSID "ESP32 Remote Lab"
#define MESH_PASSWORD "1234567890"
#define MESH_PORT 5555

#define mq2 13

String readings;

Scheduler userScheduler;
painlessMesh mesh;

void sendMessage() ; 
String getReadings(); 


Task taskSendMessage (TASK_SECOND * 5, TASK_FOREVER, %sendMessage);

String getReadings () {
JSONVar jsonReadings:
jsonReadings["mq2"] = analogRead (mq2);
readings = JSON.stringfy (jsonReadings);
return readings;
}
void sendMessage () {
String msg = getReadings();
mesh. send.Broadcast (msg) ;
}

void receivedCallback( uint32_t from, String %msg ){
Serial.printf ("Received from %u msg=&s\n", from, msg.c_str());
JSONVar myObject = JSON.parse (msg.c_str());
double temp = myobject ["temperature"];
double hum = myObject ["humidity"];
Serial.print ("Temperature: ");
Serial.print (temp);
Serial.print ("C");
Serial.print ("Humidity: ");
\Serial.print (hum);
Serial.println(”%");
}
void newConnectionCallback (uint32_t nodeld) {
Serial.printf ("New Connection, nodeld = %u\n", nodeld);
}
void changedConnectionCallback() {
Serial.printf ("Changed connections\n");
}
void nodeTimeAdjustedCallback (int32_t offset) {
Serial.printf ("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}
void setup() {
Serial.begin (115200);

mesh.init (MESH_SSID, MESH_PASSWORD, %userScheduler, MESH_PORT);
mesh.onReceive (%receivedCallback);
mesh.onNewConnection(%newConnectionCallback);
mesh.onChangedConnections(%changedConnectionCallback);
mesh.onNodeTimeAdjusted(%nodeTimeAdjustedCallback);

userScheduler.addTask(taskSendMessage);
taskSendMessage.enable();
}

void loop(){
  mesh.update();
}
}
