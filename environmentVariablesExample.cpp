//
// Created by vital on 26.12.2019.
//

#include <iostream>


int main(int argc, char *argv[], char *envp[]) {
    while (*envp != nullptr) {
        std::cout << *envp << std::endl;
        envp += 1;
    }
}