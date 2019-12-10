cmake_minimum_required(VERSION 3.14)
project(IPC_Crossplatform)

set(CMAKE_CXX_STANDARD 14)


include_directories(includes)
add_executable(IPC_Crossplatform src/main.cpp includes/MyHandler.h includes/MyProcess.h src/MyProcess.cpp src/MyHandler.cpp)
add_executable(child samples/child.cpp)

add_executable(pipe_child samples/pipe_child.cpp)
add_executable(pipe_child2 samples/pipe_child2.cpp)