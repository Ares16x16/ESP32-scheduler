#include <Arduino.h>
#include "Scheduler.h"

Scheduler::Scheduler(unsigned int maxTasks) {
  _maxTasks = maxTasks;
  _numTasks = 0;

  // Allocate memory
  _period = new unsigned long[maxTasks];
  _callback = new TaskCallback[maxTasks];
  // Set all to nullptr
  for (unsigned int i = 0; i < _maxTasks; i++){_callback[i]=nullptr;}
  _arg = new void*[maxTasks];
  _lastTime = new unsigned long[maxTasks];
  _fireImmediately = new bool[maxTasks];
}

Scheduler::~Scheduler() {
  // Free memory 
  delete[] _period;
  delete[] _callback;
  delete[] _arg;
  delete[] _lastTime;
  delete[] _fireImmediately;
}

bool Scheduler::add(unsigned long period, void (*callback)(void*), void* arg, bool fireImmediately){
  // Check if there is room for a new task
  if (_numTasks >= _maxTasks) {
    return false;
  }

  // Add the new task to the next available index
  for (unsigned int index = 0; index < _maxTasks; index++){
    if (_callback[index] == nullptr){
        _period[index] = period;
        _callback[index] = callback;
        _arg[index] = arg;
        _lastTime[index] = millis();
        _fireImmediately[index] = fireImmediately;
        _numTasks++;
        return true;
    }
  }
  return false;
}

bool Scheduler::remove(unsigned int index) {
  // Check if the index is valid
  if (index >= _numTasks) {
    return false;
  }

  // Shift the remaining tasks to fill the gap
  for (unsigned int i = index; i < _numTasks - 1; i++) {
    _period[i] = _period[i+1];
    _callback[i] = _callback[i+1];
    _arg[i] = _arg[i+1];
    _lastTime[i] = _lastTime[i+1];
    _fireImmediately[i] = _fireImmediately[i+1];
  }

  // Clear the last task
  _period[_numTasks-1] = 0;
  _callback[_numTasks-1] = nullptr;
  _arg[_numTasks-1] = nullptr;
  _lastTime[_numTasks-1] = 0;
  _fireImmediately[_numTasks-1] = false;
  _numTasks--;
  return true;
}

void Scheduler::removeAll() {
  for (unsigned int i = 0; i < _numTasks; i++){
    _period[i] = 0;
    _callback[i] = nullptr;
    _arg[i] = nullptr;
    _lastTime[i] = 0;
    _fireImmediately[i] = false;
  }
  _numTasks = 0;
}
bool Scheduler::enable(unsigned int index) {
  // Check if the index is valid
  if (index >= _numTasks) {
    return false;
  }

  // Enable the task
  _lastTime[index] = millis();
  return true;
}

bool Scheduler::disable(unsigned int index) {
  // Check if the index is valid
  if (index >= _numTasks) {
    return false;
  }

  // Disable the task
  _lastTime[index] = 0;
  return true;
}

void Scheduler::enableAll() {
  // Enable all tasks
  for (unsigned int i = 0; i < _numTasks; i++) {
    _lastTime[i] = millis();
  }
}

void Scheduler::disableAll() {
  // Disable all tasks
  for (unsigned int i = 0; i < _numTasks; i++) {
    _lastTime[i] = 0;
  }
}

void Scheduler::instantFireAll() {
  //  Fire all immediately
  for (unsigned int i = 0; i < _numTasks; i++) {
    _fireImmediately[i] = true;
  }
}

void Scheduler::instantNoFireAll() {
  //  Stop firing all immediately
  for (unsigned int i = 0; i < _numTasks; i++) {
    _fireImmediately[i] = false;
  }
}

bool Scheduler::instantFireOne(unsigned int index) {
  //  Fire one immediately
  if (index >= _numTasks) {
    return false;
  }
  _fireImmediately[index] = true;
  return true;
}

bool Scheduler::instantNoFireOne(unsigned int index) {
  //  Stop firing immediately
  if (index >= _numTasks) {
    return false;
  }
  _fireImmediately[index] = false;
  return true;
}

void Scheduler::update() {
  unsigned long currentTime = millis();
  for (unsigned int i = 0; i < _numTasks; i++) {
    if (_lastTime[i] == 0) {
      // Task disabled
      continue;
    }
    if (_fireImmediately[i]) {
      // Execute the task immediately
      _lastTime[i] = currentTime;
      _callback[i](_arg[i]);
    }
    else if (currentTime - _lastTime[i] >= _period[i]) {
      // Execute the task if the period has elapsed
      _lastTime[i] += _period[i];
      _callback[i](_arg[i]);
    }
  }
}


// Debug
void Scheduler::printTasks() {
    Serial.println("Scheduler tasks:");
    for (unsigned int i = 0; i < _numTasks; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.print((uintptr_t)_callback[i], HEX);
      Serial.print(", arg: ");
      Serial.print((uintptr_t)_arg[i], HEX);
      Serial.print(", period: ");
      Serial.print(_period[i]);
      Serial.print(", lastTime: ");
      Serial.print(_lastTime[i]);
      Serial.print(", fireImmediately: ");
      Serial.println(_fireImmediately[i]);
    }
}

void Scheduler::printCallback() {
  Serial.print("Callback: ");
  for (unsigned int i = 0; i < _maxTasks; i++) {
    if (_callback == nullptr){
      Serial.print(i);
      Serial.print("is nullptr");
    }   
  }
}