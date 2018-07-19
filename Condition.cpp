#include "Condition.h"

Condition::Condition(Mutex &m)
    : mutex(m)
{
    pthread_cond_init(&cond, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&cond);
}

void Condition::wait()
{
    pthread_cond_wait(&cond, &mutex.getPthreadMutex());
}

void Condition::notify()
{
    pthread_cond_signal(&cond);
}

void Condition::notifyAll()
{
    pthread_cond_broadcast(&cond);
}
