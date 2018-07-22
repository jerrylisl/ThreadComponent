#ifndef BASE
#define BASE

#include<utility>
#include<type_traits>

//这一页主要提供了一些基类，用于支持并发问题所需要的一些特殊功能


//C++11没有找到noncopyable，实现了一个简单的
class Noncopyable {
protected:
    Noncopyable(){}
    virtual ~Noncopyable(){}
private:
    Noncopyable(const Noncopyable&);
    Noncopyable& operator=(const Noncopyable&);
    Noncopyable(const Noncopyable&&);
    Noncopyable& operator=(const Noncopyable&&);
};



//Java提供了runable接口用于派生作为线程事件，c++的替代品则是纯虚函数

class Runable
{
public:
    Runable() {}
    virtual ~Runable() {}
    virtual void run() = 0;
};

//检查是否具有函数run,没有用到
template<typename T>
struct has_member_run
{
private:
    template<typename U>
        static auto Check(int) -> decltype( std::declval<U>().run(), std::true_type() );
    template<typename U>
        static std::false_type Check(...);
public:
    enum { value = std::is_same<decltype(Check<T>(0)),std::true_type>::value  };
};





#endif // BASE_H_INCLUDED
