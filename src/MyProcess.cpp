#include "MyProcess.h"
#include <iostream>

#define BUFSIZE 4096
void WriteToPipe(HANDLE from, HANDLE to) {
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    for (;;)
    {
        if (! ReadFile(from, chBuf, BUFSIZE, &dwRead, NULL) ||
            dwRead == 0) break;
        if (! WriteFile(to, chBuf, dwRead,
                        &dwWritten, NULL)) break;
    }
    CloseHandle(to);
}
void ReadFromPipe(HANDLE from, HANDLE to) {
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    for (;;)
    {
        if( !ReadFile(from, chBuf, BUFSIZE, &dwRead,
                      NULL) || dwRead == 0) break;
        if (! WriteFile(to, chBuf, dwRead, &dwWritten, NULL))
            break;
    }
}

void MyProcess::start() {
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;


    HANDLE hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hChildStdoutRd, hChildStdoutWr;

    CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0);
    SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr);
    SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0);


    HANDLE hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hChildStdinRd, hChildStdinWr;

    CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0);
    SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd);
    SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0);


    STARTUPINFO si;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    CreateProcess( program_name.c_str(),   // No module name (use command line)
                   const_cast<char *>(arguments.c_str()),        // Command line
                   NULL,           // Process handle not inheritable
                   NULL,           // Thread handle not inheritable
                   TRUE,          // Set handle inheritance to FALSE
                   0,              // No creation flags
                   NULL,           // Use parent's environment block
                   NULL,           // Use parent's starting directory
                   &si,            // Pointer to STARTUPINFO structure
                   &pid );          // Pointer to PROCESS_INFORMATION structur


    SetStdHandle(STD_INPUT_HANDLE, hSaveStdin);
    SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout);

//    std::cout << program_name << " waiting for write from " << in->get_handler() << " -> " << hChildStdinWr << std::endl;
    if (!in) {
        WriteToPipe(hSaveStdin, hChildStdinWr);
    } else {
        WriteToPipe(in->get_handler(), hChildStdinWr);  // get data from in
    }
    CloseHandle(hChildStdoutWr);

//    std::cout << program_name << " waiting for read from " << hChildStdoutRd << " -> " << out->get_handler()  << std::endl;

    if (!out) {
        ReadFromPipe(hChildStdoutRd, hSaveStdout);
    } else {
        ReadFromPipe(hChildStdoutRd, out->get_handler()); // send data to out
    }
    delete out;

}

MyProcess::~MyProcess() {
    CloseHandle( pid.hProcess );
    CloseHandle( pid.hThread );
}

void MyProcess::set_output_process(MyProcess & process) {
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE rd, wr;
    CreatePipe(&rd, &wr, &saAttr, 0);

    process.in = new MyHandler(rd);
    this->out = new MyHandler(wr);
}

void MyProcess::kill() {
    TerminateProcess(pid.hProcess, 9);
}

int MyProcess::wait() {
    return WaitForSingleObject( pid.hProcess, INFINITE );
}