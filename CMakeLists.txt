cmake_minimum_required(VERSION 3.14)
project(IPC_Crossplatform)

set(CMAKE_CXX_STANDARD 14)

include_directories(includes)
add_executable(IPC_Crossplatform main.cpp includes/MyHandler.h includes/MyProcess.h src/MyHandler.cpp src/MyProcess.cpp includes/MyProcessEnvironment.h src/MyProcessEnvironment.cpp)

add_executable(redirection_example redirection_example.cpp)
add_executable(pipe_input pipe_example1.cpp)
add_executable(pipe_output pipe_example2.cpp)
add_executable(kill_example kill_example.cpp)
add_executable(environment_vars_example environment_vars_example.cpp)
