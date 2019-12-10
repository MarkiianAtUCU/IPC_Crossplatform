//
// Created by andriiprysiazhnyk on 12/8/19.
//

#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include "MyHandler.h"


MyHandler::MyHandler(std::string file_name) {
    t_handler desc = open(file_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

    if (desc == -1)
        throw std::invalid_argument("Can't open file.");

    handler = desc;
}

MyHandler::~MyHandler() {
    close(handler);
}


void PosixPipe::child_callback() {
    close(second);
}

void PosixPipe::parent_callback() {
    close(first);
}

PosixPipe::~PosixPipe() {
    close(first);
    close(second);
}