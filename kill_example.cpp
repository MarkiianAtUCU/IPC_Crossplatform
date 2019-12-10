//
// Created by Vitaliy Vorobyov on 12/10/19.
//

#include <iostream>
#if defined(__linux__) || defined(__APPLE__)
    #include <zconf.h>
#elif _WIN32
    #include <windows.h>
#endif

int main() {
    for (int i=0;;i++) {
        std::cout << "Endless Printing of: " << i << std::endl;
        #if defined(__linux__) || defined(__APPLE__)
            sleep(1);
        #elif _WIN32
            Sleep(1000);
        #endif
    }
}