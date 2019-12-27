//
// Created by andriiprysiazhnyk on 12/27/19.
//

#ifndef SHARED_MEMORY_MYSHAREDMEMORY_H
#define SHARED_MEMORY_MYSHAREDMEMORY_H

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#elif __linux__ || __APPLE__
#include <semaphore.h>
#endif



class MySharedMemory {
private:
    std::string key;
    void *addr = nullptr;
    size_t mapping_size = 0;

    #ifdef _WIN32
    HANDLE memoryHandle = NULL;
    HANDLE semaphoreHandle = NULL;
    #elif __linux__ || __APPLE__
    sem_t *sem = nullptr;
    #endif


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
