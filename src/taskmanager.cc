#include <chrono>
#include <thread>

#include <concurrentqueue.h>

#include "taskmanager.h"
#include "timer.h"

#include <functional>


namespace ASC_HPC
{
  int TaskManager::numThreads = 0;

  class Task
  {
  public:
    int nr, size;
    const std::function<void(int nr, int size)> * pfunc;
    std::atomic<int> * cnt;

    Task & operator++(int)
    {
      nr++;
      return *this;
    }
    Task & operator*() { return *this; }
  };

  
  typedef moodycamel::ConcurrentQueue<Task> TQueue; 
  typedef moodycamel::ProducerToken TPToken; 
  typedef moodycamel::ConsumerToken TCToken; 
  
  
  static std::atomic<bool> stop{false};
  static std::vector<std::thread> threads;
  static TQueue queue;
  
  void TaskManager::StartWorkers()
  {
    stop = false;
    for (int i = 0; i < numThreads; i++)
      {
        TimeLine * patl = timeline.get();
        threads.push_back
          (std::thread([patl]()
          {
            if (patl)
              timeline = std::make_unique<TimeLine>();
          
            TPToken ptoken(queue); 
            TCToken ctoken(queue); 
            
            while(true)
              {
                if (stop) break;

                Task task;
                if(!queue.try_dequeue_from_producer(ptoken, task)) 
                  if(!queue.try_dequeue(ctoken, task))  
                    continue; 
                
                (*task.pfunc)(task.nr, task.size);
                (*task.cnt)++;
              }
            
            if (patl)
              patl -> AddTimeLine(std::move(*timeline));
          }));
      }
  }

  void TaskManager::StopWorkers()
  {
    numThreads=0;
    stop = true;
    for (auto & t : threads)
      t.join();
    threads.clear();
  }

  
  void TaskManager::RunParallel (const std::function<void(int nr, int size)> & func)
  {
    TPToken ptoken(queue);
    TCToken ctoken(queue);
    
    std::atomic<int> cnt{0};

    int num = numThreads>1?numThreads:1;

    for (size_t i = 0; i < num; i++)
      {
        Task task;
        task.nr = i;
        task.size = num;
        task.pfunc = &func;
        task.cnt = &cnt;
        queue.enqueue(ptoken, task);
      }

    /*
    // faster with bulk enqueue (error with gcc-Release)
    Task firsttask;
    firsttask.nr = 0;
    firsttask.size = num;
    firsttask.pfunc=&func;
    firsttask.cnt = &cnt;
    queue.enqueue_bulk (ptoken, firsttask, num);    
    */
    
    while (cnt < num)
      {
        Task task;
        if(!queue.try_dequeue_from_producer(ptoken, task)) 
          if(!queue.try_dequeue(ctoken, task))
            continue; 
        
        (*task.pfunc)(task.nr, task.size);
        (*task.cnt)++;
      }
  }
}
