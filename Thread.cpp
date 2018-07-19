//#include<pthread.h>
//#include<assert.h>
#include "Thread.h"

Thread::~Thread()
{
    if (started && !joined)
        pthread_detach(pthreadId);
}

static void* startThread(void* obj)
{
    using ThreadFunc = std::function<void()>;
    ThreadFunc *f = static_cast<ThreadFunc*>(obj);
    (*f)();
    return NULL;
}


void Thread::start()
{
    assert(!started);
    started = true;
    if(pthread_create(&pthreadId, NULL, startThread, &threadFunc))
        started = false;
}

int Thread::join()
{
    assert(started && !joined);
    joined = true;
    return pthread_join(pthreadId, NULL);
}
