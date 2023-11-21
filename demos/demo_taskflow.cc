#include <parallelcomputingtf.h>

#include <iostream>
#include <sstream>


using namespace DHL_HPC;
using std::cout, std::endl;

int main()
{
  ParallelComputingTF t(3,false);
  t.StartWorkers();
  cout << "Running with " << t.getNumThreads() << " threads" << endl;

    tf::Taskflow taskflow;
    size_t n = 10;
    for (size_t i = 0; i < n; i++)
    { 
        taskflow.emplace( 
        [i, n] () { 
            size_t first = (i*100) / n;
            size_t next = ((i+1)*100) / n;
             cout << "I am task " << DHL_HPC::ParallelComputingTF::executor->this_worker_id() << " out of " << n << "first: " << first << ", next: " << next << endl;
        });   
    }
    DHL_HPC::ParallelComputingTF::executor->run(taskflow).wait(); 
  t.StopWorkers();
}
