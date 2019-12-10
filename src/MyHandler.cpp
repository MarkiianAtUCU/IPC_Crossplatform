//
// Created by andriiprysiazhnyk on 12/8/19.
//

#if defined(__linux__) || defined(__APPLE__)

#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>

#endif

#include "MyHandler.h"


MyHandler::MyHandler(std::string file_name) {
#if defined(__linux__) || defined(__APPLE__)
    t_handler desc = open(file_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

    if (desc == -1)
        throw std::invalid_argument("Can't open file.");

    handler = desc;

#elif _WIN32
    this->handler = CreateFile(
            file_name.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_READONLY,
            NULL);

    if (this->handler == INVALID_HANDLE_VALUE) {
        this->handler = CreateFile(
                file_name.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_NEW,
                FILE_ATTRIBUTE_READONLY,
                NULL);
    }
#endif
}

MyHandler::~MyHandler() {
#if defined(__linux__) || defined(__APPLE__)
    close(handler);
#elif _WIN32
    CloseHandle(this->handler);
#endif
}

#if defined(__linux__) || defined(__APPLE__)

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

#endif
