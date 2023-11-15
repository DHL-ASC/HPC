#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <functional>
#include <thread>
#include <timer.h>

namespace ASC_HPC
{
  class TaskManager
  {
  public:
    static int numThreads;
    static size_t writeTrace;
    TaskManager(bool trace = false) { 
      numThreads = std::thread::hardware_concurrency();
      if(trace)
      {
        writeTrace++;
        timeline = std::make_unique<TimeLine>("dhl.trace");
      }
    }
    TaskManager(size_t nt, bool trace = false) 
    { 
      numThreads = nt; 
      if(trace)
      {
        writeTrace++;
        timeline = std::make_unique<TimeLine>("dhl.trace");
      }
    }
    void StartWorkers();
    void StopWorkers();
    static int getNumThreads() { return numThreads; }
    static void RunParallel(const std::function<void(int nr, int size)> &func);
    ~TaskManager(){writeTrace--;}
  };
}

#endif
