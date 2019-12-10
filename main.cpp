#include <iostream>
#include "MyProcess.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
    #include <zconf.h>
#endif

int main(int argc, char **argv) {
#if defined(__linux__) || defined(__APPLE__)
    MyProcess process("redirection_example");
#elif _WIN32
    MyProcess process("./redirection_example.exe");
#endif
    process.set_input_file("redirection_input");
    process.set_arguments("arg1 arg2 arg3");
    process.set_output_file("redirection_file");
    process.start();
    std::cout << process.wait() << std::endl;

#if defined(__linux__) || defined(__APPLE__)
    MyProcess pipe_input("pipe_input");
    MyProcess pipe_output("pipe_output");
#elif _WIN32
    MyProcess pipe_input("./pipe_input.exe");
    MyProcess pipe_output("./pipe_output.exe");
#endif
    pipe_input.set_input_file("in.txt");
    pipe_input.set_output_process(pipe_output);
    pipe_output.set_output_file("pipe_results");

    pipe_input.start();
    pipe_output.start();
    std::cout << pipe_input.wait() << std::endl;

    MyProcess kill_example("kill_example");
    kill_example.set_output_file("kill_example_file");
    kill_example.start();
#if defined(__linux__) || defined(__APPLE__)
    sleep(5);
#elif _WIN32
    Sleep(5000);
#endif
    kill_example.kill();

    return 0;
}
