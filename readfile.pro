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
    $$PWD/src/evaluate_args.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/file_fkt.cpp \
    $$PWD/src/file_list_format.cpp \
    $$PWD/the_framework/enviroment/endianness_detect.cpp \
    $$PWD/the_framework/enviroment/standard_detect.cpp \
    $$PWD/the_framework/math/math_division.cpp \
    $$PWD/the_framework/strings/Stringhelper.cpp \
    $$PWD/the_framework/utility/ansiconsolecolor.cpp \
    $$PWD/the_framework/utility/terminal.cpp \
    src/file_list.cpp


HEADERS += \
    $$PWD/inc/evaluate_args.h \
    $$PWD/inc/file_fkt.h \
    $$PWD/inc/file_list.h \
    $$PWD/inc/file_list_format.h \
    $$PWD/inc/dtypes.h \
    $$PWD/inc/global_config.h \
    $$PWD/the_framework/core/compile_guards.h \
    $$PWD/the_framework/core/bit_func.h \
    $$PWD/the_framework/core/dtypes.h \
    $$PWD/the_framework/core/fields.h \
    $$PWD/the_framework/core/meta.h \
    $$PWD/the_framework/core/Iterators/iterator_base_api.h \
    $$PWD/the_framework/core/Iterators/iterator_ptr_wrapper.h \
    $$PWD/the_framework/core/Iterators/iterator_unaligned_data.h \
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
    $$PWD/the_framework/macros/needful_macros.h \
    $$PWD/the_framework/math/math_division.h \
    $$PWD/the_framework/strings/Stringhelper.h \
    $$PWD/the_framework/utility/ansiconsolecolor.h \
    $$PWD/the_framework/utility/terminal.h \
    $$PWD/the_framework/versioning/code_stamp.h \
    $$PWD/external/GSL/include/gsl/algorithm \
    $$PWD/external/GSL/include/gsl/assert \
    $$PWD/external/GSL/include/gsl/byte \
    $$PWD/external/GSL/include/gsl/gsl \
    $$PWD/external/GSL/include/gsl/narrow \
    $$PWD/external/GSL/include/gsl/pointers \
    $$PWD/external/GSL/include/gsl/span \
    $$PWD/external/GSL/include/gsl/span_ext \
    $$PWD/external/GSL/include/gsl/util \
    $$PWD/external/GSL/include/gsl/zstring \
    $$PWD/external/cxxopts/include/cxxopts.hpp


INCLUDEPATH += \
    $$PWD/inc \
    $$PWD/the_framework/core \
    $$PWD/the_framework/core/Iterators \
    $$PWD/the_framework/enviroment \
    $$PWD/the_framework/copyright \
    $$PWD/the_framework/macros \
    $$PWD/the_framework/math \
    $$PWD/the_framework/strings \
    $$PWD/the_framework/utility \
    $$PWD/the_framework/versioning \
    $$PWD/external/cxxopts/include \
    $$PWD/external/GSL/include


DISTFILES += \
  .gitignore \
  .gitattributes \
  CMakeLists.txt \
  build.sh \
  external/GSL/include/CMakeLists.txt
