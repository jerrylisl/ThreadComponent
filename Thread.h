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
    template<typename Func>
    explicit Thread(const Func& func)
        :started(false),
         joined(false),
         pthreadId(0)
    {
        threadFunc = static_cast<ThreadFunc>(func);
    }

    ~Thread();

    void start();
    int join();
    bool isStarted() { return started; }
    pthread_t gettid() { return pthreadId; }

private:
    bool started;
    bool joined;
    pthread_t pthreadId;
    ThreadFunc threadFunc;
};

#endif // THREAD_H
