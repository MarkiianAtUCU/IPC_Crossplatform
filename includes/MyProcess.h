//
// Created by matsiuk on 07-Dec-19.
//

#ifndef IPC_CROSSPLATFORM_MYPROCESS_H
#define IPC_CROSSPLATFORM_MYPROCESS_H

#include "MyHandler.h"

typedef int t_phandler;

class MyProcess {
private:
    std::string program_name;
    t_phandler pid;

    void _start(std::string, AbstractHandler in, AbstractHandler out);
public:
    MyProcess(std::string program_name) : program_name(program_name);

    inline void start(std::string arguments) {
        this->_start(
                arguments,
                StdInHandler::getInstance(),
                StdOutHandler::getInstance()
                );
    };


    inline void start(std::string arguments, MyHandler in, MyHandler out) {
        this->_start(
                arguments,
                in, out
        );
    };

    void kill();
    void terminate();
    int wait();
};

#endif //IPC_CROSSPLATFORM_MYPROCESS_H
