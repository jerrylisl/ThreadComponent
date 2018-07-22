//#include<pthread.h>
//#include<assert.h>
#include "Thread.h"


//现在，这个线程支持两种形式的事件函数：Java风格的run（必须继承Runable），或者C++风格的operator（）（必须重载对应运算符）
//至于函数指针，会被转换为函数对象，并赋予其operator（）

Thread::Thread(const ThreadFunc& func)
:started(false),
 joined(false),
 pthreadId(0),
 canRun(false),
 threadFuncbyRun(NULL),
 threadFunc(func) { }

Thread::Thread(Runable& func)
    :started(false),
     joined(false),
     pthreadId(0),
     canRun(true),
     threadFuncbyRun(&func) { }

Thread::~Thread()
{
    if (started && !joined)
        pthread_detach(pthreadId);
}

void* Thread::startThread(void* obj)
{
    using ThreadFunc = std::function<void()>;
    Thread* f = static_cast<Thread*>(obj);
    if (f->canRun)
        f->threadFuncbyRun->run();
    else
        f->threadFunc();
    //ThreadFunc *f = static_cast<ThreadFunc*>(obj);
    //(*f)();
    return NULL;
}


void Thread::start()
{
    assert(!started);
    started = true;
    if(pthread_create(&pthreadId, NULL, Thread::startThread, this))
        started = false;
}

int Thread::join()
{
    assert(started && !joined);
    joined = true;
    return pthread_join(pthreadId, NULL);
}
