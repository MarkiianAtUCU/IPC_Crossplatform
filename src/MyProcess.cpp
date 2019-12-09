//
// Created by andriiprysiazhnyk on 12/8/19.
//

#include <vector>
#include <stdexcept>
#include <unistd.h>
#include <wait.h>
#include <csignal>
#include "MyProcess.h"


static char *string_to_char_array(std::string token) {
    char *argument = new char[token.size() + 1];
    std::copy(&token[0], &token[0] + token.size() + 1, argument);
    return argument;
}


static std::vector<char*> get_tokens(std::string &arguments) {
    std::vector<char*> tokens;

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
        if (i == arguments.size() - 1 && start != -1) tokens.push_back(string_to_char_array(arguments.substr(start, i - start + 1)));
    }
    return tokens;
}


static void release_arguments(std::vector<char *> &arguments) {
    for (auto arg: arguments)
        delete[] arg;
}

void MyProcess::start() {
    arguments = program_name + " " + arguments;
    std::vector<char*> tokens = get_tokens(arguments);

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
}


void MyProcess::set_output_process(MyProcess &process) {
    int p[2];
    while (pipe(p) == -1) {}


    delete out;
    delete process.in;

    out = new PosixPipe(p[1], p[0]);
    process.in = new PosixPipe(p[0], p[1]);
}


int MyProcess::wait() {
    int result = 0;
    waitpid(pid, &result, 0);
    return WEXITSTATUS(result);
}


void MyProcess::kill() {
    ::kill(pid, SIGKILL);
}

MyProcess::~MyProcess() {
    delete in;
    delete out;
}