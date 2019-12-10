//#include <windows.h>
//#define BUFSIZE 4096
//
#include <iostream>
//
//int main()
//{
////    CHAR chBuf[BUFSIZE];
////    DWORD dwRead, dwWritten;
////    HANDLE hStdin, hStdout;
////    BOOL fSuccess;
////    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
////    hStdin = GetStdHandle(STD_INPUT_HANDLE);
//
//    std::cout << "Child hello" << std::endl;
//
////    if ((hStdout == INVALID_HANDLE_VALUE) ||
////        (hStdin == INVALID_HANDLE_VALUE))
////        ExitProcess(1);
////
////
////    WriteFile(hStdout, "Pedor", 6, &dwWritten, NULL);
////    for (;;)
////    {
//         Read from standard input.
////        fSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);
////        if (! fSuccess || dwRead == 0)
////            break;
////
//         Write to standard output.
////        fSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL);
////        if (! fSuccess)
////            break;
////    }
//}

int main(int argc, const char *argv[], const char *env[] ) {
    std::string dummy;
//    std::cin >> dummy;
//    std::cout << "Hello from child, recieved: " << dummy << std::endl;

    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "]==" << argv[i] << std::endl;
    }
    int c = 0;
    while (env[c]) {
        std::cout << "env==" << env[c] << std::endl;
        c++;
    }
}
