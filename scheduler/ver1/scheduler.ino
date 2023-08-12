#include <WiFi.h>
#include <WiFiUdp.h>
#include <stdlib.h>
#include <Arduino.h>
#include <pgmspace.h>
#include "Scheduler.h"
#include "Testscript.h"

const char* ssid = "BITLAB GUEST";
const char* password = "BITLAB123";

WiFiUDP udp;
const int espPort = 1234;

Scheduler scheduler(10); //set max task

void taskCallback(void* arg) {
  int taskNumber = *(static_cast<int*>(arg));
  Serial.print("Task ");
  Serial.print(taskNumber);
  Serial.println(" executed");
}

void add(int taskNumber) {  
  int* taskPtr = new int(taskNumber);
  if (scheduler.add(taskNumber*1000, taskCallback, taskPtr)) {
    Serial.print("Task delay="); 
    Serial.print(taskNumber*1000);
    //Serial.print(taskNumber);
    Serial.println(" added");
  } else {
    Serial.println("Unable to add task");
    delete taskPtr;
  }
}

//remove index
void remove(int taskNumber) {
  if (scheduler.remove(taskNumber)) {
    Serial.print("Task List Index ");
    Serial.print(taskNumber);
    Serial.println(" removed");
  }
}

void removeAll() {
  scheduler.removeAll();
  Serial.print("Removed All Task");
}

void enable(int taskIndex) {
  scheduler.enable(taskIndex);
  Serial.print("Enabled Index ");
  Serial.println(taskIndex);
}

void disable(int taskIndex) {
  scheduler.disable(taskIndex);
  Serial.print("Disabled Index");
  Serial.println(taskIndex);
}

void enableAll() {
  scheduler.enableAll();
  Serial.println("Enabled All");
}

void disableAll() {
  scheduler.disableAll();
  Serial.println("Disabled All");
}

void instantFire(int taskIndex) {
  scheduler.instantFireOne(taskIndex);
  Serial.print("Set Index ");
  Serial.print(taskIndex);
  Serial.println(" To Instant Fire");
}

void instantNoFire(int taskIndex) {
  scheduler.instantNoFireOne(taskIndex);
  Serial.print("Stop Index ");
  Serial.print(taskIndex);
  Serial.println(" To Instant Fire");
}

void instantFireAll(){
  scheduler.instantFireAll();
  Serial.println("Instant Fire All");
}

void instantNoFireAll(){
  scheduler.instantNoFireAll();
  Serial.println("Stop All From Instant Firing");
}
//======================================================

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected. IP:");
  Serial.println(WiFi.localIP());

  udp.begin(espPort);
  Serial.print("Listening on port");
  Serial.println(espPort);
  scheduler.add(0, execute_task, NULL, false);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    String command = "";
    for (int i = 0; i < packetSize; i++) {
      char byte = udp.read();
      if (byte == '?') {
        break;
      }
      command += byte;
    }
    Serial.println("Received command: " + command);

    int space_pos = command.indexOf(' ');
    if (space_pos != -1) {
      String cmd_type = command.substring(0, space_pos);
      String cmd_arg = command.substring(space_pos + 1);
      int task_number = cmd_arg.toInt();

      if (cmd_type == "add") {
        add(task_number);
      } else if (cmd_type == "remove") {
        remove(task_number);
      } else if (cmd_type == "enable") {
        enable(task_number);
      } else if (cmd_type == "disable") {
        disable(task_number);
      } else if (cmd_type == "enable_all") {
        enableAll();
      } else if (cmd_type == "disable_all") {
        disableAll();
      } else if (cmd_type == "instant_fire") {
        instantFire(task_number);
      } else if (cmd_type == "instant_fire_all") {
        instantFireAll();
      } else if (cmd_type == "instant_no_fire") {
        instantNoFire(task_number);
      } else if (cmd_type == "instant_no_fire_all") {
        instantNoFireAll();
      } else if (cmd_type == "remove_all") {
        removeAll();
      
      } else {
        Serial.println("Invalid command");
      }
    }
  }
  scheduler.update();
}