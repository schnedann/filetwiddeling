TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG   += -Wall -Wextra -std=c++23 -rdynamic
#-fprofile-arcs -ftest-coverage
#-save-temps -Wa,-adhln -g
#QMAKE_CXXFLAGS_RELEASE += -Wall -Wextra -std=c++23 -Ofast -fexpensive-optimizations -mtune=native -march=native -flto -funroll-loops -frename-registers  -Wformat -Wformat-security -fpic -shared -fasynchronous-unwind-tables -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -fstack-protector-all -fcf-protection -pipe -msse3
QMAKE_CXXFLAGS_RELEASE += -Wall -Wextra -std=c++23 -O3 -mtune=native -march=native -msse2 -rdynamic

QMAKE_LFLAGS_DEBUG   += -Wl,-z,now -Wl,-z,relro
QMAKE_LFLAGS_RELEASE += -Wl,-z,now -Wl,-z,relro

QMAKE_LFLAGS += -Wl,-Map=CountbitSet.map

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/file_fkt.cpp \
    the_framework/enviroment/endianness_detect.cpp \
    the_framework/enviroment/standard_detect.cpp


HEADERS += \
    $$PWD/inc/file_fkt.h \
    $$PWD/inc/dtypes.h \
    $$PWD/inc/global_config.h \
    $$PWD/the_framework/enviroment/posix_detect.h \
    $$PWD/the_framework/enviroment/architecture_detect.h \
    $$PWD/the_framework/enviroment/compiler_detect.h \
    $$PWD/the_framework/enviroment/endian_convert.h \
    $$PWD/the_framework/enviroment/endianness_detect.h \
    $$PWD/the_framework/enviroment/enviroment_detection.h \
    $$PWD/the_framework/enviroment/gccversion.h \
    $$PWD/the_framework/enviroment/os_detect.h \
    $$PWD/the_framework/enviroment/standard_detect.h \
    $$PWD/the_framework/copyright/modified_4Cbsd.h \
    $$PWD/the_framework/copyright/modified_bsd.h \
    $$PWD/the_framework/macros/bitmacros.h \
    $$PWD/the_framework/macros/build_annotations.h \
    $$PWD/the_framework/macros/needful_macros.h


INCLUDEPATH += \
    $$PWD/inc \
    $$PWD/the_framework/enviroment \
    $$PWD/the_framework/copyright \
    $$PWD/the_framework/macros \
    $$PWD/external/cxxopts/include \
    $$PWD/external/GSL/include


DISTFILES += \
  .gitignore \
  .gitattributes \
  CMakeLists.txt \
  build.sh
