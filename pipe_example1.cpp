//
// Created by andriiprysiazhnyk on 12/8/19.
//

#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {
    std::string message = "Hello from another end of a pipe!\n";
    write(1, &message[0], message.size());
    return 0;
}