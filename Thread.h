#ifndef THREAD_H
#define THREAD_H

#include<pthread.h>
#include<iostream>
#include<memory>
#include<functional>
#include<assert.h>

#include"Base.h"

class Thread : Noncopyable
{
public:
    //typedef void* (*ThreadFunc)(void*);
    using ThreadFunc = std::function<void()>;
    explicit Thread(Runable& func);


    explicit Thread(const ThreadFunc& func);


    ~Thread();

    void start();
    int join();
    bool isStarted() { return started; }
    pthread_t gettid() { return pthreadId; }

private:

    static void* startThread(void* obj);
    bool started;
    bool joined;
    pthread_t pthreadId;
    ThreadFunc threadFunc;
    bool canRun;
    Runable* threadFuncbyRun;
};

//template<typename Func>
/*
Thread::Thread(const ThreadFunc& func)
    :started(false),
     joined(false),
     pthreadId(0),
     canRun(false),
     threadFuncbyRun(NULL)
{
    threadFunc = func;
}
*/

//template<>




#endif // THREAD_H
