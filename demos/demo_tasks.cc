#include <iostream>
#include <sstream>

#include <taskmanager.h>
#include <timer.h>

using namespace ASC_HPC;
using std::cout, std::endl;

int main()
{
  timeline = std::make_unique<TimeLine>("demo.trace");

  TaskManager t;
  t.StartWorkers();
  cout << "Running with " << t.getNumThreads() << " threads" << endl;

  TaskManager::RunParallel([](int i, int size)
                           {
    static Timer t("timer one");
    RegionTimer reg(t,i);
      cout << "I am task " << i << " out of " << size << endl; });

  TaskManager::RunParallel([](int i, int s)
                           { TaskManager::RunParallel([i](int j, int s2)
                                                      {
      std::stringstream str;
      str << "nested, i,j = " << i << "," << j << "\n";
      cout << str.str(); }); });

  TaskManager::RunParallel([](int i, int size)
                           {
    static Timer t("one of 100", { 0, 0, 1});
    RegionTimer reg(t,i); });

  {
    static Timer t("100x10 parallel runs", {0, 0, 1});
    RegionTimer reg(t,0);

    for (int k = 0; k < 100; k++)
      TaskManager::RunParallel([](int i, int size)
                               { ; });
  }

  {
    static Timer t("10x10x10 parallel runs", {0, 0, 1});
    RegionTimer reg(t,0);

    for (int k = 0; k < 10; k++)
      TaskManager::RunParallel([](int i, int size)
                               { TaskManager::RunParallel([](int j, int size) {}); });
  }

  TaskManager::RunParallel([](int i, int size)
                           {
    static Timer t("timer 1000 tasks", { 1, 0, 0});
    RegionTimer reg(t,i); });

  TaskManager::RunParallel([](int i, int s)
                           {
    static Timer t("timer 4", { 1, 1, 0});
    RegionTimer reg(t,i);    
    TaskManager::RunParallel([i](size_t j, size_t s2)
    {
      ;
    }); });

  t.StopWorkers();
}
