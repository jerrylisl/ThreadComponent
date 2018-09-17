# ThreadComponent

目前完成了mutex，condition，thread，threadpool部分

添加了协程（用户态线程）

添加了无锁队列（采用gcc的cas）

优化线程的语法                                

在Base.h中加入了抽象类Runable，现在，可以通过Java风格的继承runable重写run的方式来部署线程事件，保留了C/C++的函数指针与函数对象的装入方式，同时重载构造函数，引入一个变量在线程运行时进行判断所采用的风格

