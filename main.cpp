#include <iostream>
#include "MyProcess.h"

int main(int argc, char **argv) {
    MyProcess process("redirection_example");
    process.set_arguments("arg1 arg2 arg3");
    process.set_output_file("redirection_file");
    process.start();
    std::cout << process.wait() << std::endl;

    MyProcess pipe_input("pipe_input");
    MyProcess pipe_output("pipe_output");
    pipe_output.set_output_file("pipe_results");
    pipe_input.set_output_process(pipe_output);

    pipe_input.start();
    pipe_output.start();
    std::cout << pipe_output.wait() << std::endl;

    return 0;
}
