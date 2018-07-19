#include "Coroutine.h"
//#include <memory.h>
//#include <iostream>
//#include <vector>

size_t CoroutineMaster::_count = 0;
size_t CoroutineMaster::_currentNum = 0;
size_t CoroutineMaster::_maxCount = 1024;

CoroutineMaster Coroutine::_master;

Coroutine::~Coroutine()
{

    delete[] _stack;
    _stack = NULL;
}

void Coroutine::freeStack()
{
    //std::cout << (long long)_stack << std::endl;
    delete[] _stack;
   // std::cout << "again" << (long long)_stack << std::endl;
    _stack = NULL;
    //free(_stack);
}





Coroutine::COSTATUS Coroutine::getStatus()
{
    return _status;
}


int Coroutine::getId()
{
    return _id;
}

Coroutine::CoFunc* Coroutine::getFunc()
{
    return &_func;
}

int* Coroutine::getpS()
{
    return (int*)&_status;
}

char* Coroutine::getStack()
{
    return _stack;
}

Coroutine::COSTATUS Coroutine::checkStatus(int id)
{
    Coroutine* temp = _master.getCoroutine(id);
    if (temp == NULL)
        return COROUTINE_DEAD;
    return temp->getStatus();
}

int Coroutine::restCoroutines()
{
    return _master.getCount();
}


void Coroutine::resume()
{
    if (_keepStack)
        resume1();
    else
        resume2();
}

void Coroutine::resume1()
{
    assert(_master.getRunningID() == -1);

    int status = getStatus();
    switch(status) {
    case COROUTINE_READY:
        //std::cout << (long long)_stack << std::endl;
        getcontext(&_currContext);
        _currContext.uc_stack.ss_sp = _master.getStack();
        _currContext.uc_stack.ss_size = STACK_SIZE;
        _currContext.uc_link = _master.getMain();
        _master.setRunningID(_id);
        _status = COROUTINE_RUNNING;
        makecontext(&_currContext, &coroutineFunc, 0);
        swapcontext(_master.getMain(), &_currContext);
        break;
    case COROUTINE_SUSPEND:
        //memcpy(_master.getStack() + STACK_SIZE - _size, _stack, _size);
        std::copy(_stack, _stack + _size, _master.getStack() + STACK_SIZE - _size);
        _master.setRunningID(_id);
        _status = COROUTINE_RUNNING;
        swapcontext(_master.getMain(), &_currContext);
        break;
    default:
        assert(0);
    }
}

void Coroutine::resume2()
{
    assert(_master.getRunningID() == -1);

    int status = getStatus();
    switch(status) {
    case COROUTINE_READY:
        getcontext(&_currContext);
        //std::cout << (long long)_currContext.uc_stack.ss_sp << std::endl;
        //_stack = (char*)malloc(STACK_SIZE);
        _stack = new char[STACK_SIZE];  //主要的不同即在此处，每次都建立新的堆栈，而不是向主站借用
        //std::cout << (long long)_stack << std::endl;
        _currContext.uc_stack.ss_sp = _stack;
        _currContext.uc_stack.ss_size = STACK_SIZE;
        _currContext.uc_link = _master.getMain();
        _master.setRunningID(_id);
        _status = COROUTINE_RUNNING;
        makecontext(&_currContext, (void (*)(void))startFuncWithID, 2, &_func, _id);
        swapcontext(_master.getMain(), &_currContext);
        break;
    case COROUTINE_SUSPEND:
        //memcpy(_master.getStack() + STACK_SIZE - _size, _stack, _size);
        //std::copy(_stack, _stack + _size, _master.getStack() + STACK_SIZE - _size);
        _master.setRunningID(_id);
        _status = COROUTINE_RUNNING;
        //std::cout << (long long)_currContext.uc_stack.ss_sp << std::endl;
        swapcontext(_master.getMain(), &_currContext);
        break;
    default:
        //assert(0);
        return;
    }
}

void Coroutine::coroutineFunc()
{

    int id = _master.getRunningID();
    Coroutine* temp = _master.getCoroutine(id);
    temp->_func();
    char k = 0;
    //std::cout << (long long)&k << std::endl;
    //std::cout << (long long)&coroutineFunc << std::endl;
    //std::cout << (long long)temp->getStack() << std::endl;
    //std::cout << temp->_size << std::endl;
    //delete temp;
    _master.eraseCoroutine(id);
    temp->freeStack();
    _master.setRunningID(-1);
}

void Coroutine::saveStack(int id)
{
    Coroutine* temp = _master.getCoroutine(id);
   // std::cout << (long long)temp->getStack() << std::endl;
    //std::cout << (long long)temp << std::endl;
    char dummy = 0;
    assert(&dummy >= _master.getStack());
    if(temp->_size < _master.getStack() + STACK_SIZE - &dummy)
    {
        delete[] temp->_stack;
        //free(temp->_stack);
        temp->_size = _master.getStack() + STACK_SIZE - &dummy;
        temp->_stack = new char[temp->_size];
        //temp->_stack = (char*)malloc(temp->_size);
    }
    //memcpy(temp->_stack, &dummy, temp->_size);
    std::copy(&dummy, &dummy + temp->_size, temp->_stack);

}

void Coroutine::yield()
{
    int id = _master.getRunningID();
    assert(id >= 0);
    Coroutine* temp = _master.getCoroutine(id);
    if (temp->_keepStack)
        yield1();
    else
        yield2();
}


void Coroutine::yield1()
{
    int id = _master.getRunningID();
    //std::cout << id << std::endl;
    assert(id >= 0);
    Coroutine* temp = _master.getCoroutine(id);
    assert((char *)&temp > _master.getStack());
    saveStack(id);
    temp->_status = COROUTINE_SUSPEND;
    _master.setRunningID(-1);
    swapcontext(&temp->_currContext, _master.getMain());

}

void Coroutine::yield2()
{
    int id = _master.getRunningID();
    //std::cout << id << std::endl;
    assert(id >= 0);
    Coroutine* temp = _master.getCoroutine(id);
    assert((char *)&temp > _master.getStack());
    //saveStack(id);
    temp->_status = COROUTINE_SUSPEND;
    _master.setRunningID(-1);
    swapcontext(&temp->_currContext, _master.getMain());

}

void Coroutine::startFunc(void* obj)
{
    CoFunc *f = static_cast<CoFunc*>(obj);
    (*f)();
    int* m = (int*)((char*)f + 988);  //dark change
    *m = COROUTINE_DEAD;
    _master.setRunningID(-1);
}

void Coroutine::startFuncWithID(void* obj, int id)
{
    CoFunc *f = static_cast<CoFunc*>(obj);
    (*f)();
    _master.eraseCoroutine(id);
    _master.setRunningID(-1);
    //std::cout << "aa" << std::endl;
}



Coroutine::COSTATUS co_status(int id)
{
    return Coroutine::checkStatus(id);
}

void co_yield()
{
    Coroutine::yield();
}

void co_resume(Coroutine& coroutine)
{
    coroutine.resume();
}

void co_destory(Coroutine& coroutine)
{
    coroutine.~Coroutine();
}




