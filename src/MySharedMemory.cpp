//
// Created by andriiprysiazhnyk on 12/27/19.
//


#ifdef _WIN32
#include "MySharedMemory.h"
#elif __linux__ || __APPLE__
#include "MySharedMemory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif



bool MySharedMemory::create(size_t size) {
    #ifdef _WIN32
    this->mapping_size = size;
    memoryHandle = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            mapping_size,                // maximum object size (low-order DWORD)
            key.c_str());                 // name of mapping object
    if (memoryHandle == NULL)
    {
        error_message =  "Could not create file mapping object\n";
//        GetLastError()
        return false;
    }
    this->attach(this->mapping_size);

    semaphoreHandle = CreateSemaphore(
            NULL,           // default security attributes
            1,  // initial count
            1,  // maximum count
            (key + "SEM").c_str());

    if (semaphoreHandle == NULL)
    {
        error_message = "CreateSemaphore error\n";
        return false;
    }

    return true;
    #elif __linux__ || __APPLE__
    int fd = shm_open(key.c_str(), O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd == -1) {
        error_message = "Error while creating shared memory object\n";
        return false;
    }

    ftruncate(fd, size);

    addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        error_message = "Error wile mapping a file\n";
        return false;
    }
    mapping_size = size;

    sem = sem_open(key.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 1);
    if (sem == SEM_FAILED) {
        error_message = "Error while opening a semaphor\n";
        return false;
    }

    close(fd);
    return true;
    #endif
}


bool MySharedMemory::attach(size_t size) {
    #ifdef _WIN32
    this->mapping_size = size;
    memoryHandle = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            key.c_str());
    if (memoryHandle == NULL)
    {
        error_message =  "Could not attach to file mapping object";
        return false;
    }
    addr = MapViewOfFile(memoryHandle, // handle to map object
                        FILE_MAP_ALL_ACCESS,  // read/write permission
                        0,
                        0,
                        this->mapping_size);
    if (addr == NULL)
    {
        error_message =  "Could not map view";
        return false;
    }

    semaphoreHandle = OpenSemaphore(
            SEMAPHORE_MODIFY_STATE | SYNCHRONIZE,
            FALSE,
            (key + "SEM").c_str());

    return true;
    #elif __linux__ || __APPLE__
    int fd = shm_open(key.c_str(), O_RDWR, 0666);
    if (fd == -1) {
        error_message = "Error while opening shared memory object\n";
        return false;
    }

    addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        error_message = "Error wile mapping a file\n";
        return false;
    }
    mapping_size = size;

    std::cout << "Hello\n";

    sem = sem_open(key.c_str(), O_RDWR);
    if (sem == SEM_FAILED) {
        error_message = "Error while opening a semaphor\n";
        return false;
    }

    close(fd);
    return true;
    #endif
}


void* MySharedMemory::data() {
    return addr;
}


void MySharedMemory::detach() {
    #ifdef _WIN32
    UnmapViewOfFile(addr);
    #elif __linux__ || __APPLE__
    sem_close(sem);
    munmap(addr, mapping_size);
    sem_unlink(key.c_str());
    shm_unlink(key.c_str());
    #endif
}


void MySharedMemory::lock() {
    #ifdef _WIN32
    auto wait_res = WaitForSingleObject(semaphoreHandle, INFINITE);
    #elif __linux__ || __APPLE__
    sem_wait(sem);
    #endif
}


void MySharedMemory::unlock() {
    #ifdef _WIN32
    ReleaseSemaphore(semaphoreHandle, 1, NULL);
    #elif __linux__ || __APPLE__
    sem_post(sem);
    #endif
}


MySharedMemory::~MySharedMemory() {
    #ifdef _WIN32
    CloseHandle(memoryHandle);
    #elif __linux__ || __APPLE__
    detach();
    #endif
}