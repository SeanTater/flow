TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    dataset.cpp \
    row.cpp \
    flow.cpp

HEADERS += \
    dataset.h \
    row.h \
    flow.h

LIBS += \
    /usr/lib/x86_64-linux-gnu/libsqlite3.so.0.8.6 \
    /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.53.0 \
    /usr/lib/x86_64-linux-gnu/libboost_system.so.1.53.0
#    /usr/lib/libsqlite.so.0
