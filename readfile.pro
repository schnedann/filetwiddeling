TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    file_fkt.cpp

HEADERS += \
    file_fkt.h \
    dtypes.h \
    posix_detect.h
