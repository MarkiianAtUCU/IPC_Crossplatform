//
// Created by andriiprysiazhnyk on 12/27/19.
//

#ifndef SHARED_MEMORY_MYSHAREDMEMORY_H
#define SHARED_MEMORY_MYSHAREDMEMORY_H

#include <iostream>
#include <semaphore.h>


class MySharedMemory {
private:
    std::string key;
    void *addr = nullptr;
    sem_t *sem = nullptr;
    size_t mapping_size = 0;

public:
    explicit MySharedMemory(std::string key) : key(key) {}

    std::string error_message;

    bool create(size_t size);

    bool attach(size_t size);

    void *data();

    void detach();

    void lock();

    void unlock();

    ~MySharedMemory();
};

#endif //SHARED_MEMORY_MYSHAREDMEMORY_H
