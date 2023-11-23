#include "parallelcomputingtf.h"
#include <taskflow/taskflow.hpp> 

namespace DHL_HPC
{
    int ParallelComputingTF::numThreads = 1;
    tf::Executor* ParallelComputingTF::executor = new tf::Executor(1);
    bool ParallelComputingTF::trace = false;

    void ParallelComputingTF::RunParallel (const std::function<void(size_t id, size_t nThreads)> & func, size_t nTasks){
        tf::Taskflow taskflow;
        for (size_t i = 0; i < nTasks; ++i)
        { 
            taskflow.emplace([i,nTasks,&func](){func(i,nTasks);});   
        }
        DHL_HPC::ParallelComputingTF::executor->run(taskflow).wait();
    }
}

