#include "painlessMesh.h"
#include <Arduino_JSON.h>
#include "DHT.h"

//MESH Details
#define MESH_SSID "ESP32 Remote Lab:"
#define MESH_PASSWORD "1234567890"
#define MESH_PORT 5555                 //default port

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//String to send to other nodes with sensor readings
String readings;

Scheduler userScheduler;              //to control your personal task
painlessMesh mesh;

//User stub
void sendMessage();                   //Prototype so PlatformIO doesn't complaint
String getReadings();                 //Prototype for sending sensor readings

//Create Tasks: to send messages and get readings;
Task taskSendMessage(TASK_SECOND * 5 , TASK_FOREVER, &sendMessage);

String gerReadings() {
  JSONVar jsonReadings;
  jsonReadings["temperature"] = dht.readTemperature();
  jsonReadings["humidity"] = dht.readHumidity();
  return readings;
}

void sendMessage () {
  String msg = getReadings();
  mesh.sendBroadcast(msg);
}

//Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  JSONVar myObject = JSON.parse (msg.c_str());
  double mq2 = myObject["mq2"];
  Serial.print("mq2: ");
  Serial.println(mq2);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);

  //mesh.setDebugMsgTypes (ERROR | MESH+STATUS | CONNECTION | SYNC | COMMUNICATION)
  mesh.setDebugMsgTypes ( ERROR | STARTUP );

  mesh.init( MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  //it will run the user scheduler as well
  mesh.update();
}
