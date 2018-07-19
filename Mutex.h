#ifndef MUTEX_H
#define MUTEX_H

#include<pthread.h>

#include"Base.h"

class Mutex : Noncopyable
{
public:
    Mutex();

    ~Mutex();

    void lock();

    void unlock();

    pthread_mutex_t& getPthreadMutex();

private:
    pthread_mutex_t m_mutex;
};

class MutexLock
{
public:
    explicit MutexLock(Mutex &m);

    ~MutexLock();

private:
    Mutex& m_mutex;
};

#endif // MUTEX_H
