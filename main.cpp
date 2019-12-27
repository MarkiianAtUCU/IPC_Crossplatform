#include <iostream>
#include "MyProcess.h"
#include "MyProcessEnvironment.h"

#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#edif


int main(int argc, char **argv) {
    MyProcess process("redirection_example");
    process.set_input_file("../Test/In/redirection_in.txt");
    process.set_arguments("arg1 arg2 arg3");
    process.set_output_file("../Test/Out/redirection_out.txt");
    process.start();
    std::cout << process.wait() << std::endl;


    MyProcess pipe_input("pipe_input");
    MyProcess pipe_output("pipe_output");
    pipe_input.set_input_file("../Test/In/pipe_in.txt");
    pipe_input.set_output_process(pipe_output);
    pipe_output.set_output_file("../Test/Out/pipe_out.txt");

    pipe_input.start();
    pipe_output.start();
    std::cout << pipe_input.wait() << std::endl;
    std::cout << pipe_output.wait() << std::endl;

    MyProcess kill_example("kill_example");
    kill_example.set_input_file("../Test/In/kill_in.txt");
    kill_example.set_output_file("../Test/Out/kill_out.txt");
    kill_example.start();

#if defined(__linux__) || defined(__APPLE__)
    sleep(5);
#elif _WIN32
    Sleep(5000);
#endif

    kill_example.kill();

    MyProcessEnvironment env;
    env.set("key1", "value1");
    env.set("key2", "value2");
    env.set("key3", "value3");

    MyProcess env_example("environment_vars_example");
    env_example.set_output_file("../Test/Out/env_example_out.txt");
    env_example.set_environment(env);

    env_example.start();
    std::cout << env_example.wait() << std::endl;

    return 0;
}