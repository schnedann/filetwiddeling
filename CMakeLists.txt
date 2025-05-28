cmake_minimum_required(VERSION 3.10)
project(filetwiddeling)

set(CMAKE_VERBOSE_MAKEFILE ON)

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
    src/main.cpp
    src/file_fkt.cpp
)

# Add the executable
add_executable(filetwiddeling ${SOURCE_FILES})

# Add include directories
target_include_directories(filetwiddeling
  PUBLIC inc
  PRIVATE src
)

target_compile_features(filetwiddeling PRIVATE cxx_std_23)

# Link Boost libraries
target_link_libraries(filetwiddeling PRIVATE Boost::filesystem)
