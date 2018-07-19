#include <iostream>
#include <thread>
#include <unistd.h>
#include <unordered_set>
#include <algorithm>

#include "ThreadPool.h"
#include "Coroutine.h"
#include "Mutex.h"

#include "LockfreeQueue.h"


using namespace std;

LockfreeQueue<int> qqueue;

unordered_set<int> s;

vector<int> ss;

Mutex m;

static void add(int i)
{
    MutexLock lock(m);
    ss.push_back(i);

}


void func1(void)
{
    for(int i = 1; i <= 10000; i++)
    {
        qqueue.push(i);
    }
}

void func2(void)
{
    for(int i = 10001; i <= 20000; i++)
    {
        qqueue.push(i);
    }
}

void func3(void)
{
    for(int i = 0; i < 10000; i++)
        add(qqueue.pop());
        //cout << qqueue.pop();


}

void func4(void)
{
    for(int i = 0; i < 20000; i++)
        add(qqueue.pop());
        //cout << qqueue.pop();


}

int main(int argc,char **argv)
{
  std::thread t1(func1);
  std::thread t2(func2);
  t1.join();
  t2.join();
  cout << qqueue.size() << endl;
  //std::thread t3(func3);
  //std::thread t4(func3);
  std::thread t5(func4);

  //t3.join();
  //t4.join();
  t5.join();
  cout << qqueue.size() << endl;
  cout << ss.size() << endl;
  sort(ss.begin(), ss.end());
  ss.erase(unique(ss.begin(), ss.end()), ss.end());
  cout << ss.size() << endl;
  cout << ss[0] << endl;
  return 0;
}


