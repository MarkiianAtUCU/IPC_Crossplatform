#include "MyHandler.h"


MyHandler::MyHandler(std::string filename) {
    this->handler = CreateFile(
            filename.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_READONLY,
            NULL);

    if (this->handler == INVALID_HANDLE_VALUE) {
        this->handler = CreateFile(
                filename.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_NEW,
                FILE_ATTRIBUTE_READONLY,
                NULL);
    }
}

//void MyHandler::child_callback() {
//    SetHandleInformation(handler, HANDLE_FLAG_INHERIT, 1);
//}
//
//void MyHandler::parent_callback() {
//    SetHandleInformation(handler, HANDLE_FLAG_INHERIT, 0);
//}




MyHandler::~MyHandler() {
    CloseHandle(this->handler);
}

void PosixPipe::child_callback() {
    SetHandleInformation(first, HANDLE_FLAG_INHERIT, 1);
    SetHandleInformation(second, HANDLE_FLAG_INHERIT, 0);
}

void PosixPipe::parent_callback() {
    SetHandleInformation(first, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(second, HANDLE_FLAG_INHERIT, 1);
}

PosixPipe::~PosixPipe() {
    CloseHandle(first);
    CloseHandle(second);
}