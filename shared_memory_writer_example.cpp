//
// Created by andriiprysiazhnyk on 12/27/19.
//

#include <iostream>
#include <sstream>
#include "MySharedMemory.h"

int main() {
    size_t size = 4096;

    MySharedMemory sharedMemory("shared_memory_example");
    bool res = sharedMemory.create(size);
    if (!res) {
        std::cout << sharedMemory.error_message;
        return 1;
    }

    char *data = (char *)sharedMemory.data();

    std::string message_start = "Message number: ";
    for(int counter = 0;; ++counter) {
        sharedMemory.lock();

        std::stringstream ss;
        ss << counter;

        std::string message = message_start + ss.str() + "\n";
        std::copy(&message[0], &message[0] + message.size() + 1, data);

        sharedMemory.unlock();
    }

    return 0;
}