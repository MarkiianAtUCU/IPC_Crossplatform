#include <iostream>
#include "MyProcess.h"

int main(int argc, char **argv) {
#if defined(__linux__) || defined(__APPLE__)
    MyProcess process("redirection_example");
#elif _WIN32
    MyProcess process("./redirection_example.exe");
#endif
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
    pipe_output.set_output_file("pipe_results");
    pipe_input.set_output_process(pipe_output);

    pipe_input.start();
    pipe_output.start();
    std::cout << pipe_output.wait() << std::endl;

    return 0;
}