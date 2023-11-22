#include "parallelcomputingtf.h"
#include <taskflow/taskflow.hpp> 

namespace DHL_HPC
{
    int ParallelComputingTF::numThreads = 1;
    tf::Executor* ParallelComputingTF::executor = new tf::Executor(1);
    bool ParallelComputingTF::trace = false;

    void ParallelComputingTF::RunParallel (const std::function<void(size_t id, size_t nThreads)> & func){
        tf::Taskflow taskflow;
        size_t n = DHL_HPC::ParallelComputingTF::getNumThreads();
        for (size_t i = 0; i < n; ++i)
        { 
            taskflow.emplace([i,n,&func](){func(i,n);});   
        }
        DHL_HPC::ParallelComputingTF::executor->run(taskflow).wait();
    }
}

