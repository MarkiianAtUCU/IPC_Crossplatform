#ifndef IPC_CROSSPLATFORM_MYHANDLER_H
#define IPC_CROSSPLATFORM_MYHANDLER_H

#include <string>
typedef int t_handler;


class AbstractHandler {
public:
    t_handler handler;
};


class MyHandler : public AbstractHandler {
public:
    MyHandler(std::string file_name);
    MyHandler(t_handler handler);
};

class StdInHandler : public AbstractHandler {
private:
    StdInHandler() {}
public:
    static StdInHandler& getInstance()
    {
        static StdInHandler    instance;
        return instance;
    }

    StdInHandler(StdInHandler const&)    = delete;
    void operator=(StdInHandler const&)  = delete;
};


class StdOutHandler : public AbstractHandler {
private:
    StdOutHandler() {}
public:
    static StdOutHandler& getInstance()
    {
        static StdOutHandler    instance;
        return instance;
    }

    StdOutHandler(StdOutHandler const&)    = delete;
    void operator=(StdOutHandler const&)  = delete;
};

#endif //IPC_CROSSPLATFORM_MYHANDLER_H
