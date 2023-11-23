#ifndef PARALLELCOMPUTINGTF_H
#define PARALLELCOMPUTINGTF_H

#include <taskflow/taskflow.hpp> 

namespace DHL_HPC
{
  class ParallelComputingTF
  {
  private:

  public:
    static int numThreads;
    static tf::Executor* executor;
    static bool trace;
    ParallelComputingTF(bool tr = false) 
    { 
        trace = tr;
    }
    ParallelComputingTF(size_t nt, bool tr = false) 
    { 
        numThreads = nt;
        trace = tr;
    }
    void StartWorkers()
    {
        delete executor;
        executor = new tf::Executor(numThreads);
    }
    void StopWorkers()
    {
        delete executor;
        executor = new tf::Executor(1);
        numThreads = 1;

    }
    static int getNumThreads() { return numThreads; }
    static void RunParallel(const std::function<void(size_t id, size_t nThreads)> &func, size_t nTasks);
    ~ParallelComputingTF(){};
  };
}

#endif
