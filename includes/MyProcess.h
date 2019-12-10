//
// Created by matsiuk on 07-Dec-19.
//

#ifndef IPC_CROSSPLATFORM_MYPROCESS_H
#define IPC_CROSSPLATFORM_MYPROCESS_H

#include "MyHandler.h"
#include <windows.h>

typedef PROCESS_INFORMATION t_phandler;

class MyProcess {
private:
    std::string program_name;
    std::string arguments;
    t_phandler pid;
    Handler * in = nullptr;
    Handler * out = nullptr;

public:
    MyProcess(std::string program_name) : program_name(program_name) {};

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

    void set_output_process(MyProcess & process);

    void kill();

    int wait();

    void start();

    ~MyProcess();
};

#endif //IPC_CROSSPLATFORM_MYPROCESS_H
