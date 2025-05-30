TEMPLATE = app
CONFIG += console c++23
CONFIG -= app_bundle
CONFIG -= qt

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
    $$PWD/the_framework/macros


DISTFILES += \
  .gitignore \
  .gitattributes \
  CMakeLists.txt \
  build.sh
