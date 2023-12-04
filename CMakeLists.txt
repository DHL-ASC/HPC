cmake_minimum_required(VERSION 3.20)
project(DHL_HPC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")


include_directories(src concurrentqueue)
include_directories(src taskflow)


add_executable (demo_tasks demos/demo_tasks.cc 
                src/taskmanager.cc src/timer.cc)
                
target_sources (demo_tasks PUBLIC src/taskmanager.h src/timer.h)


add_executable (demo_taskflow demos/demo_taskflow.cc 
                src/parallelcomputingtf.cc src/timer.cc)
                
target_sources (demo_taskflow PUBLIC src/parallelcomputingtf.h src/timer.h)


add_executable (demo_simd demos/demo_simd.cc
                src/timer.cc)
                
target_sources (demo_simd PUBLIC src/simd.h src/timer.h)


add_executable (simd_timings demos/simd_timings.cc)
target_sources (simd_timings PUBLIC src/simd.h)


