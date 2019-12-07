cmake_minimum_required(VERSION 3.14)
project(IPC_Crossplatform)

set(CMAKE_CXX_STANDARD 14)


include_directories(includes)
add_executable(IPC_Crossplatform main.cpp includes/MyHandler.h includes/MyProcess.h)

