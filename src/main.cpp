#include <iostream>
#include "MyProcess.h"
#include "MyHandler.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    auto c = MyProcess{"./pipe_child.exe"};
    auto c1 = MyProcess{"./pipe_child2.exe"};
    c.set_input_file("./in.txt");
    c.set_output_process(c1);
//    c1.set_input_file("./in.txt");

//    p.set_output_file("./out.txt");
    c.start();
    c1.start();
    c.wait();
    c1.wait();
    return 0;
}