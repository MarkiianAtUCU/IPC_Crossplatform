//
// Created by andriiprysiazhnyk on 12/8/19.
//

#include <iostream>
#include <string>

int main(int argc, char **argv) {
    for (std::string line; std::getline(std::cin, line);) {
        std::cout << "Pipe end: " << line << std::endl;
    }
    return 0;
}