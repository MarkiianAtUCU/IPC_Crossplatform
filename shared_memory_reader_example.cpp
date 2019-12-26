//
// Created by andriiprysiazhnyk on 12/27/19.
//

#include <iostream>
#include "MySharedMemory.h"

int main() {
    size_t size = 4096;

    MySharedMemory sharedMemory("shared_memory_example");
    bool res = sharedMemory.attach(size);
    if (!res) {
        std::cout << sharedMemory.error_message;
        return 1;
    }

    char *data = (char *)sharedMemory.data();

    for(int counter = 0; counter < 100000; ++counter) {
        sharedMemory.lock();
        std::cout << data;
        sharedMemory.unlock();
    }

    return 0;
}