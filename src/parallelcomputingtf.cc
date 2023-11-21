#include "parallelcomputingtf.h"
#include <taskflow/taskflow.hpp> 

namespace DHL_HPC
{
    int ParallelComputingTF::numThreads = 1;
    tf::Executor* ParallelComputingTF::executor = new tf::Executor(1);
    bool ParallelComputingTF::trace = false;
}

