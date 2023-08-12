#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
  
class Scheduler {
public:
  Scheduler(unsigned int maxTasks);
  ~Scheduler();

  bool add(unsigned long period, void (*callback)(void*), void* arg = nullptr, bool fireImmediately = false);
  bool remove(unsigned int index);
  bool enable(unsigned int index);
  bool disable(unsigned int index);
  void enableAll();
  void disableAll();
  void update();
  void printTasks();
  void printCallback();
  void instantFireAll();
  void instantNoFireAll();
  bool instantFireOne(unsigned int index);
  bool instantNoFireOne(unsigned int index);
  void removeAll();


private:
  typedef void (*TaskCallback)(void*);
  unsigned int _maxTasks;
  unsigned int _numTasks;
  unsigned long* _period;
  TaskCallback* _callback;
  void** _arg;
  unsigned long* _lastTime;
  bool* _fireImmediately;
};

#endif
