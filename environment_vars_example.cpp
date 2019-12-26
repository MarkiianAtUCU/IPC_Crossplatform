//
// Created by andriiprysiazhnyk on 12/26/19.
//

#include <iostream>


int main(int argc, char *argv[], char *envp[]) {
    while (*envp != nullptr) {
        std::cout << *envp << std::endl;
        envp += 1;
    }
}