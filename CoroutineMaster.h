#ifndef COROUTINEMASTER_H
#define COROUTINEMASTER_H

#include <ucontext.h>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include <algorithm>

#include "Base.h"
//#include "Coroutine.h"

class Coroutine;


class CoroutineMaster : Noncopyable
{
public:
    CoroutineMaster();

    int addCoroutine(Coroutine* coroutine);

    void eraseCoroutine(int id);


    char* getStack();

    void setRunningID(int id);

    static int getRunningID();

    Coroutine* getCoroutine(int id);

    ucontext_t* getMain();

    static size_t getCount();

    static size_t getLimit();

    static size_t setLimit(size_t newcount);


private:
    static const int STACK_SIZE = 1024 * 1024;
    static size_t _currentNum;
    static size_t _count;
    static size_t _maxCount;
    static int _runningID;

    char _stack[STACK_SIZE];
    ucontext_t _main;


    std::unordered_map<size_t, Coroutine*> _coroutines;
};

#endif // COROUTINEMASTER_H
