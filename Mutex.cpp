#include "Mutex.h"

Mutex::Mutex()
{
    pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void Mutex::lock()
{
    pthread_mutex_lock(&m_mutex);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

pthread_mutex_t& Mutex::getPthreadMutex()
{
    return m_mutex;
}




MutexLock::MutexLock(Mutex &m)
    : m_mutex(m)
{
    m_mutex.lock();
}

MutexLock::~MutexLock()
{
    m_mutex.unlock();
}



