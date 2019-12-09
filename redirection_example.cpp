//
// Created by andriiprysiazhnyk on 12/8/19.
//

#include <iostream>

int main(int argc, char **argv) {
    int i = 0;

    while (argv[i] != nullptr){
        std::cout << i << "th argument = " << argv[i] << std::endl;
        ++i;
    }
    
    return 0;
}