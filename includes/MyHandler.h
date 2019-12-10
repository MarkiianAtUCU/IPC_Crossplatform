#ifndef IPC_CROSSPLATFORM_MYHANDLER_H
#define IPC_CROSSPLATFORM_MYHANDLER_H

#include <string>
#include <windows.h>

typedef HANDLE t_handler;


class Handler {
public:
    virtual void child_callback() = 0;
    virtual void parent_callback() = 0;
    inline virtual t_handler get_handler() = 0;
    virtual ~Handler() {};
};


class MyHandler : public Handler {
private:
    t_handler handler;
public:
    explicit MyHandler(std::string file_name);
    explicit MyHandler(t_handler handler): handler(handler){};

    void child_callback() override {};
    void parent_callback() override {};
    inline t_handler get_handler() override {
        return handler;
    };

    ~MyHandler() override;
};

class PosixPipe : public Handler {
public:
    t_handler first, second;
public:
    PosixPipe(t_handler first, t_handler second): first(first), second(second){};

    void child_callback() override;
    void parent_callback() override;
    t_handler get_handler() override {
        return first;
    };

    ~PosixPipe() override;
};
#endif //IPC_CROSSPLATFORM_MYHANDLER_H
