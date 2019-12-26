//
// Created by andriiprysiazhnyk on 12/27/19.
//

#include "MySharedMemory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


bool MySharedMemory::create(size_t size) {
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
}


bool MySharedMemory::attach(size_t size) {
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

    sem = sem_open(key.c_str(), O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO, 1);
    if (sem == SEM_FAILED) {
        error_message = "Error while opening a semaphor\n";
        return false;
    }

    close(fd);
    return true;
}


void* MySharedMemory::data() {
    return addr;
}


void MySharedMemory::detach() {
    sem_close(sem);
    munmap(addr, mapping_size);
    sem_unlink(key.c_str());
    shm_unlink(key.c_str());
}


void MySharedMemory::lock() {
    sem_wait(sem);
}


void MySharedMemory::unlock() {
    sem_post(sem);
}


MySharedMemory::~MySharedMemory() {
    detach();
}