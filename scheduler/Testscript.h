#ifndef TESTSCRIPT_H
#define TESTSCRIPT_H

#include <Arduino.h>
#include <pgmspace.h>
#include <Preferences.h>
#include <cstdlib>
#include "Scheduler.h"

//const PROGMEM char print_script[] = "Serial.println(\"Hello, world!\");\n\nfor (int i = 0; i < 5; i++) {\n    Serial.println(i);\n    delay(1000);\n}\n";

/*
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile int counter = 0;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  counter++;
  portEXIT_CRITICAL_ISR(&timerMux);

  void execute_task1(void* arg) {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  
  while (true) {
    portENTER_CRITICAL(&timerMux);
    int count = counter;
    counter = 0;
    portEXIT_CRITICAL(&timerMux);

    for (int i = 0; i < count; i++) {
      Serial.println("Hello World");
      delay(10);
    }
  }
}
*/

void execute_task(void* arg) {
  unsigned long startTime = millis();
  while (true) {
    if (millis() - startTime >= 1000) {
        Serial.println("Hello World");
        startTime = millis();
    }
  }
}

// State machine
Preferences preferences;

void execute_task2(void* arg) {
    preferences.begin("execute_task2", false);
    unsigned int state = preferences.getUInt("state", 0);
    unsigned long lastTime = preferences.getULong("lastTime", 0);
    switch (state) {
        case 0: //Idle
            if ((millis() - lastTime)>= 1000 ) {
                lastTime = millis();
                state = 1;
            }
            break;
        case 1: //Running
            Serial.println("Hello World...");
            state = 0;
            break;
    }
    preferences.putUInt("state", state);
    preferences.putULong("lastTime", lastTime);
}

void pythonScript(void* arg){
    system("python pyTest.py");
}

void handleInterrupt() {
  interruptFlag = true;
  Serial.print("Interrupt");
}

#endif