//
// Created by andriiprysiazhnyk on 12/26/19.
//

#ifdef _WIN32
#include "MyProcessEnvironment.h"
#include <windows.h>
#include <strsafe.h>
#elif __linux__ || __APPLE__

#include "MyProcessEnvironment.h"

#endif


extern char **environ;

void MyProcessEnvironment::clear() {
    env.clear();
}


void MyProcessEnvironment::remove(std::string key) {
    env.erase(key);
}


std::vector<std::string> MyProcessEnvironment::keys() {
    std::vector<std::string> env_keys;
    env_keys.reserve(env.size());

    for (auto &iter : env)
        env_keys.push_back(iter.first);

    return env_keys;
}


std::string MyProcessEnvironment::get(std::string key) {
    return env[key];
}


void MyProcessEnvironment::set(std::string key, std::string value) {
    env[key] = value;
}


char **MyProcessEnvironment::getEnvironmentVariables() {
    if (env.empty())
        return nullptr;
    char **env_variables = new char *[env.size() + 1];
#ifdef _WIN32
    LPTSTR currentEnvVariable;

    currentEnvVariable = (LPTSTR) envVariables;

    for (auto &iter : env) {
        std::string concat = iter.first + "=" + iter.second;

        if (FAILED(StringCchCopy(currentEnvVariable, concat.size() + 1, TEXT(concat.c_str()))))
        {
            return FALSE;
        }
        currentEnvVariable += lstrlen(currentEnvVariable) + 1;
    }

    *currentEnvVariable = (char)0;

    return envVariables;
#elif __linux__ || __APPLE__
    env_variables[env.size()] = nullptr;

    int counter = 0;
    for (auto &iter : env) {
        auto concat = iter.first + "=" + iter.second;
        auto res = new char[concat.size() + 1];

        std::copy(&concat[0], &concat[0] + concat.size() + 1, res);
        env_variables[counter++] = res;
    }

    return env_variables;
#endif
}


MyProcessEnvironment MyProcessEnvironment::systemEnvironment() {
    MyProcessEnvironment environment;

    for (char **iter = environ; *iter != nullptr; ++iter) {
        std::string current(*iter);

        int split_index = current.find('=');
        std::string key = current.substr(0, split_index);
        std::string value = current.substr(split_index + 1, current.size() - split_index - 1);

        environment.set(key, value);
    }

    return environment;
}