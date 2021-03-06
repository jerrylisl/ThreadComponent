#ifndef LockfreeQueue_H
#define LockfreeQueue_H


#include <atomic>
#include <queue>
#include <stdio.h>

#include "Base.h"

template<typename T>
struct ListNode
{
    T _val;
    ListNode* _next;
    ListNode(int val)
        : _val(val),
          _next(nullptr) { }

};
/*
 *这是一个尝试用C++11的atomic实现的方案，最后发现在push操作上存在无法回避的障碍
 *
template<typename T>   //由于无锁特性，size无法被支持
class LockfreeQueue : Noncopyable
{
public:
    using AtoNode = std::atomic<ListNode<T>*>;

    LockfreeQueue();

    bool empty();

    void push(T t);

    T pop();

    std::size_t size()
    {
        return _size.load();
    }



private:
    AtoNode _head;
    std::atomic<ListNode<T>*> _tail;

    std::atomic_uint _size;

};


template<typename T>
LockfreeQueue<T>::LockfreeQueue()
    : _head(new ListNode<T>(0)),
      _tail(_head.load()),
      _size(0) { }

template<typename T>
bool LockfreeQueue<T>::empty()
{
    return _head.load() == _tail.load();
}


template<typename T>
void LockfreeQueue<T>::push(T t)
{
   ListNode<T>* temp = new ListNode<T>(t);
   ListNode<T>* p = _tail.load();
   do
   {
       p->__next = temp;
   }
   while (!_tail.compare_exchange_weak(p, temp));
   //_tail.compare_exchange_weak(_tail.load(), temp);
   _size.fetch_add(1);

}

template<typename T>
T LockfreeQueue<T>::pop()
{
   ListNode<T>* temp = _head.load();
   if (!temp)
       return -3;
   T res;
   do
   {
       if (!temp)
           return -2;
   } while (!_head.compare_exchange_weak(temp, temp->__next));

   res = temp->_val;
   //if (res != -2)
      // _size.fetch_sub(1);
   //delete temp;
   return res;



}
*/



template <typename T>
class LockfreeQueue
{
    public:
        LockfreeQueue();
        //入队
        void push(T t);
        //出队
        T pop();
        //判队空
        bool empty();
    private:
        ListNode<T> *_head;
        ListNode<T> *_tail;
};

template <typename T>
LockfreeQueue<T>::LockfreeQueue()
    : _head(new ListNode<T>(0)),
      _tail(_head) { }

template <typename T>
void LockfreeQueue<T>::push(T t)
{
    ListNode<T>* temp = new ListNode<T>(t);
    ListNode<T>* p;
    do
    {
        p = _tail;
    }while(!__sync_bool_compare_and_swap(&_tail->_next,NULL,temp));

    __sync_bool_compare_and_swap(&_tail,_tail,temp);
}

template <typename T>
T LockfreeQueue<T>::pop()
{
    ListNode<T> *p = NULL;
    do
    {
        p = _head->_next;
        if(!p)
        {
            return 0;
        }
    }while(!__sync_bool_compare_and_swap(&_head->_next,p,p->_next));
    return p->_val;
}

template <typename T>
bool LockfreeQueue<T>::empty()
{
    return _head == _tail;
}



#endif // LockfreeQueue_H
