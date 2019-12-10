#ifndef IPC_CROSSPLATFORM_MYHANDLER_H
#define IPC_CROSSPLATFORM_MYHANDLER_H

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
typedef HANDLE t_handler;
#elif __linux__ || __APPLE__
typedef int t_handler;
#endif


class Handler {
public:
    virtual void child_callback() = 0;

    virtual void parent_callback() = 0;

    virtual t_handler get_handler() = 0;

    virtual ~Handler() = default;
};


class MyHandler : public Handler {
private:
    t_handler handler;
public:
    explicit MyHandler(std::string file_name);

    explicit MyHandler(t_handler handler) : handler(handler) {};

    void child_callback() override {};

    void parent_callback() override {};

    t_handler get_handler() override {
        return handler;
    };

    ~MyHandler() override;
};

#if defined(__linux__) || defined(__APPLE__)

class PosixPipe : public Handler {
private:
    t_handler first, second;
public:
    PosixPipe(t_handler first, t_handler second) : first(first), second(second) {};

    void child_callback() override;

    void parent_callback() override;

    t_handler get_handler() override {
        return first;
    };

    ~PosixPipe() override;
};

#endif

#endif //IPC_CROSSPLATFORM_MYHANDLER_H
