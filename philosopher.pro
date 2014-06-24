TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# any of the below be comment out can help start the c++ 11 support
#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

SOURCES += \
    threadcpp.cpp \
    pthreadc.cpp \
    main.cpp \
    alarm_list.cpp \
    trylock.cpp \
    backoff.cpp \
    cond.cpp

HEADERS += \
    pthreadc.h \
    threadcpp.h \
    errors.h \
    alarm_list.h \
    trylock.h \
    backoff.h \
    cond.h

