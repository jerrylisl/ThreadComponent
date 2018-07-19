# ThreadComponent

目前完成了mutex，condition，thread，threadpool部分

添加了协程（用户态线程）功能

尝试用C++11的atomic实现无锁队列，遇到了一些问题,用gcc cas实现了无锁队列

后续目标：线程的一些语法优化，增加一个观察者模式的框架组件
