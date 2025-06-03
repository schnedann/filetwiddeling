cmake_minimum_required(VERSION 3.10)
project(filetwiddeling)

#---

set(CMAKE_VERBOSE_MAKEFILE ON)

# Set output directories for intermediate files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#---

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for std::filesystem support
include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
    "
    #include <filesystem>
    namespace fs = std::filesystem;
    int main() {
        fs::path p;
        return 0;
    }
    "
    HAS_FILESYSTEM
)

if(NOT HAS_FILESYSTEM)
    message(FATAL_ERROR "std::filesystem is not available. Please use a compiler that supports C++17 or later.")
endif()

#---

#set(BOOST_DEBUG YES)
set(BOOST_VERBOSE YES)

# Find Boost
find_package(Boost REQUIRED COMPONENTS filesystem)

# Check if Boost was found
if(Boost_FOUND)
  message(STATUS "Boost_INCLUDE_DIRS  : ${Boost_INCLUDE_DIRS}")
  message(STATUS "Boost_LIBRARY_DIRS  : ${Boost_LIBRARY_DIRS}")
  message(STATUS "Boost_VERSION_STRING: ${Boost_VERSION_STRING}")
else()
  message(WARNING "Boost not found!")
endif()

# Define a list of source files
set(SOURCE_FILES
    src/evaluate_args.cpp
    src/main.cpp
    src/file_fkt.cpp
    src/file_list_format.cpp
    the_framework/enviroment/endianness_detect.cpp
    the_framework/enviroment/standard_detect.cpp
    the_framework/utility/terminal.cpp
)

# Add the executable
add_executable(filetwiddeling ${SOURCE_FILES})

# Add include directories
target_include_directories(filetwiddeling
  PUBLIC
  inc
  the_framework/enviroment
  the_framework/copyright
  the_framework/macros
  the_framework/utility
  the_framework/versioning
  external/cxxopts/include
  external/GSL/include
  PRIVATE src
)

target_compile_features(filetwiddeling PRIVATE cxx_std_23)

# Link Boost libraries
target_link_libraries(filetwiddeling PRIVATE Boost::filesystem)

# Generate a map file
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set_target_properties(filetwiddeling PROPERTIES
    LINK_FLAGS "-Wl,-Map -Wl,${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filetwiddeling.map"
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set_target_properties(filetwiddeling PROPERTIES
    LINK_FLAGS "-Wl,-Map,${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filetwiddeling.map"
  )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  ##set_target_properties(filetwiddeling PROPERTIES
  ##  LINK_FLAGS "/MAP:${CMAKE_BINARY_DIR}/filetwiddeling.map"
  ##)
endif()
