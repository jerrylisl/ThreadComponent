TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS = '-std=c++11'


LIBS += -lpthread

SOURCES += main.cpp \
    ThreadPool.cpp \
    Thread.cpp \
    CoroutineMaster.cpp \
    Coroutine.cpp \
    Mutex.cpp \
    Condition.cpp \
    LockfreeQueue.cpp

HEADERS += \
    ThreadPool.h \
    Thread.h \
    Mutex.h \
    CoroutineMaster.h \
    Coroutine.h \
    Condition.h \
    Base.h \
    LockfreeQueue.h

