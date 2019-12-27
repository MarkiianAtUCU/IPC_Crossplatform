//
// Created by andriiprysiazhnyk on 12/26/19.
//

#ifndef IPC_CROSSPLATFORM_MYPROCESSENVIRONMENT_H
#define IPC_CROSSPLATFORM_MYPROCESSENVIRONMENT_H

#include <map>
#include <vector>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif


class MyProcessEnvironment {
private:
    std::map<std::string, std::string> env;

public:
    static MyProcessEnvironment systemEnvironment();

    void clear();

    void remove(std::string key);

    std::vector<std::string> keys();

    std::string get(std::string key);

    void set(std::string key, std::string value);

#ifdef _WIN32
    LPVOID getEnvironmentVariables();
#elif __linux__ || __APPLE__
    char **getEnvironmentVariables();
#endif
};

#endif //IPC_CROSSPLATFORM_MYPROCESSENVIRONMENT_H
