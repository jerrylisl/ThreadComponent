#ifndef COROUTINE_H
#define COROUTINE_H

#include <functional>
#include <ucontext.h>
#include <stddef.h>
#include <assert.h>
#include <algorithm>
#include <iostream>

#include "CoroutineMaster.h"
#include "Base.h"


class Coroutine : Noncopyable
{
public:

    static const int STACK_SIZE = 1024 * 1024;
    using CoFunc = std::function<void()>;

    enum COSTATUS {COROUTINE_DEAD,
                   COROUTINE_READY,
                   COROUTINE_RUNNING,
                   COROUTINE_SUSPEND};

    template<typename F>   //和之前同样的问题，模板函数定义与声明不可分离
    explicit Coroutine(F f, bool keepStack = false);

    ~Coroutine();

    void freeStack();

    void resume();

    static void yield();

    COSTATUS getStatus();

    int getId();

    CoFunc* getFunc();

    int* getpS();

    char* getStack();

    static COSTATUS checkStatus(int id);

    static int restCoroutines();



private:
    static CoroutineMaster _master;

    void resume1(); //追求空间，每次复制到主栈

    void resume2(); //各协程固定栈区，在用完后删除

    static void yield1();

    static void yield2();


    static void coroutineFunc();
    static void startFunc(void*);
    static void startFuncWithID(void*, int);

    static void saveStack(int id);


    const bool _keepStack;
    CoFunc _func;
    ucontext_t _currContext;
    //ptrdiff_t _cap;   //堆栈大小
    ptrdiff_t _size;  //这两个应该是重复了……先看看吧
    int _id;
    COSTATUS _status;

    char* _stack;
    //char _stack[STACK_SIZE];
};

Coroutine::COSTATUS co_status(int id);

void co_yield();

void co_resume(Coroutine& coroutine);

void co_destory(Coroutine& coroutine);




template<typename F>   //和之前同样的问题，模板函数定义与声明不可分离
Coroutine::Coroutine(F f, bool keepStack)
    : _keepStack(keepStack),
      _func(static_cast<CoFunc>(f)),
      //_cap(0),
      _size(0),
      _status(COROUTINE_READY),
      _stack(NULL)
{
    _id = _master.addCoroutine(this);
}


#endif // COROUTINECORE_H
