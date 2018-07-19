#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>
#include <Base.h>

#include "Mutex.h"

class Condition : Noncopyable
{
public:
    Condition(Mutex &m);

    ~Condition();

    void wait();

    void notify();

    void notifyAll();

private:
    Mutex& mutex;
    pthread_cond_t cond;
};

#endif // CONDITION_H
