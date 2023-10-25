#ifndef TASKMANAGER_H
#define TASKMANAGER_H


#include <functional>
#include <thread>

namespace ASC_HPC
{
  class TaskManager{
    public:
      static int numThreads;
      TaskManager(){numThreads = std::thread::hardware_concurrency();}
      void StartWorkers();
      void StopWorkers();
      static void RunParallel (const std::function<void(int nr, int size)> & func);
  };

  int TaskManager::numThreads = 0;

}



#endif
