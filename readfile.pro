TEMPLATE = app
CONFIG += console c++23
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/file_fkt.cpp

HEADERS += \
    $$PWD/inc/file_fkt.h \
    $$PWD/inc/dtypes.h \
    $$PWD/inc/posix_detect.h

INCLUDEPATH += \
    $$PWD/inc

DISTFILES += \
  .gitignore \
  .gitattributes \
  CMakeLists.txt \
  build.sh
