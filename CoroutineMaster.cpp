#include "CoroutineMaster.h"

int CoroutineMaster::_runningID = -1;

CoroutineMaster::CoroutineMaster()
{

}


int CoroutineMaster::addCoroutine(Coroutine* coroutine)
{
    assert(_count < _maxCount);

    while (_coroutines.count(_currentNum))
        ++ _currentNum;
    _coroutines[_currentNum] = coroutine;
    ++ _count;
    return _currentNum;
}


void CoroutineMaster::eraseCoroutine(int id)
{
    if (_coroutines.count(id))
    {
        //delete _coroutines[id];
        //std::cout << (long long)(_coroutines[id]->getStack()) << std::endl;
        _coroutines.erase(id);
        -- _count;
    }
}

char* CoroutineMaster::getStack()
{
    return _stack;
}

void CoroutineMaster::setRunningID(int id)
{
    _runningID = id;
}

int CoroutineMaster::getRunningID()
{
    return _runningID;
}

Coroutine* CoroutineMaster::getCoroutine(int id)
{
    if (_coroutines.count(id))
        return _coroutines[id];
    return NULL;
}

ucontext_t* CoroutineMaster::getMain()
{
    return &_main;
}

size_t CoroutineMaster::getCount()
{
    return _count;
}

size_t CoroutineMaster::getLimit()
{
    return _maxCount;
}

size_t CoroutineMaster::setLimit(size_t newcount)
{
    _maxCount = std::max(_maxCount, newcount);
}
