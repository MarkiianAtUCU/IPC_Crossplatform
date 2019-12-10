//
// Created by matsiuk on 07-Dec-19.
//

#ifndef IPC_CROSSPLATFORM_MYPROCESS_H
#define IPC_CROSSPLATFORM_MYPROCESS_H

#include "MyHandler.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
typedef PROCESS_INFORMATION t_phandler;
#elif __linux__ || __APPLE__
typedef int t_phandler;
#endif


class MyProcess {
private:
    std::string program_name;
    std::string arguments{};
    t_phandler pid{};
    Handler *in = nullptr;
    Handler *out = nullptr;

public:
    explicit MyProcess(std::string program_name) : program_name(program_name) {};

    void set_arguments(std::string args) {
        arguments = args;
    }

    void set_input_file(std::string file_name) {
        delete in;
        in = new MyHandler(file_name);
    }

    void set_output_file(std::string file_name) {
        delete out;
        out = new MyHandler(file_name);
    }

    void set_output_process(MyProcess &process);

    void kill();

    int wait();

    void start();

    ~MyProcess();
};

#endif //IPC_CROSSPLATFORM_MYPROCESS_H
