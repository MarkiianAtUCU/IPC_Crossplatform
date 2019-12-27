//
// Created by andriiprysiazhnyk on 12/8/19.
//

#include <iostream>
#include <string>

int main(int argc, char **argv) {
    std::string message = "Hello from another end of a pipe!\n";
    std::string input;
    std::cin >> input;
    std::cout << input << std::endl;
    return 0;
}