//
// Created by andriiprysiazhnyk on 12/8/19.
//

#if defined(__linux__) || defined(__APPLE__)

#include <vector>
#include <stdexcept>
#include <unistd.h>

#endif

#ifdef __APPLE__
#include <sys/wait.h>
#elif __linux__

#include <wait.h>

#endif

#include "MyProcess.h"


#if defined(__linux__) || defined(__APPLE__)

static char *string_to_char_array(std::string token) {
    char *argument = new char[token.size() + 1];
    std::copy(&token[0], &token[0] + token.size() + 1, argument);
    return argument;
}


static std::vector<char *> get_tokens(std::string &arguments) {
    std::vector<char *> tokens;

    int start = -1;
    bool open_quotes = false;

    for (size_t i = 0; i < arguments.size(); ++i) {
        if (arguments[i] == '"') open_quotes = !open_quotes;

        if (arguments[i] != ' ') {
            if (start == -1) start = i;
        } else {
            if (start != -1 && !open_quotes) {
                tokens.push_back(string_to_char_array(arguments.substr(start, i - start)));
                start = -1;
            }
        }
        if (i == arguments.size() - 1 && start != -1)
            tokens.push_back(string_to_char_array(arguments.substr(start, i - start + 1)));
    }
    tokens.push_back(nullptr);
    return tokens;
}


static void release_arguments(std::vector<char *> &arguments) {
    for (auto arg: arguments)
        delete[] arg;
}

#elif _WIN32
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
#endif

void MyProcess::start() {
#if defined(__linux__) || defined(__APPLE__)
    arguments = program_name + " " + arguments;
    std::vector<char *> tokens = get_tokens(arguments);

    pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Can't fork.");
    } else if (pid == 0) { // child
        if (in) {
            dup2(in->get_handler(), 0);
            in->child_callback();
        }

        if (out) {
            dup2(out->get_handler(), 1);
            out->child_callback();
        }
        execve(program_name.c_str(), tokens.data(), nullptr);
        throw std::invalid_argument("Program not found.");
    }

    if (in)
        in->parent_callback();
    if (out)
        out->parent_callback();

    release_arguments(tokens);
#elif _WIN32
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

    if (!in) {
        WriteToPipe(hSaveStdin, hChildStdinWr);
    } else {
        WriteToPipe(in->get_handler(), hChildStdinWr);  // get data from in
    }
    CloseHandle(hChildStdoutWr);


    if (!out) {
        ReadFromPipe(hChildStdoutRd, hSaveStdout);
    } else {
        ReadFromPipe(hChildStdoutRd, out->get_handler()); // send data to out
    }
    delete out;
#endif
}


void MyProcess::set_output_process(MyProcess &process) {
#if defined(__linux__) || defined(__APPLE__)
    int p[2];
    while (pipe(p) == -1) {}


    delete out;
    delete process.in;

    out = new PosixPipe(p[1], p[0]);
    process.in = new PosixPipe(p[0], p[1]);
#elif _WIN32
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE rd, wr;
    CreatePipe(&rd, &wr, &saAttr, 0);

    delete this->out;
    delete process.in;

    process.in = new MyHandler(rd);
    this->out = new MyHandler(wr);
#endif
}


int MyProcess::wait() {
#if defined(__linux__) || defined(__APPLE__)
    int result = 0;
    waitpid(pid, &result, 0);
    return WEXITSTATUS(result);
#elif _WIN32
    return WaitForSingleObject( pid.hProcess, INFINITE );
#endif
}


void MyProcess::kill() {
#if defined(__linux__) || defined(__APPLE__)
    ::kill(pid, SIGKILL);
#elif _WIN32
    TerminateProcess(pid.hProcess, 9);
#endif
}

MyProcess::~MyProcess() {
    delete in;
    delete out;

#ifdef _WIN32
    CloseHandle( pid.hProcess);
    CloseHandle( pid.hThread);
#endif

}