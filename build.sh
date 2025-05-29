#!/bin/bash

clear

####################################################
# Load Script-Fu
####################################################

source ./bashfu.sh
echo "$reset"

####################################################
# Load Build Config
####################################################

source ./build_config.cfg

####################################################

# Set the timeout duration in seconds
timeout_duration=3

####################################################
# ensure build directory exits
####################################################

test_ensure_folder build
cd ..

####################################################
# Test for Executables
####################################################

printf "$bold$green--> T E S T   F O R   E X E C U T A B L E S <--$reset\n"

#which hg 2>/dev/null || printf "$yellow--> Warning, Mercurial is not installed <--$reset\n"

#----- CMake

get_cmake_version

#----- Git / Mercurial versions

get_mercurial_version
get_git_version

#----- C Compiler

get_gcc_version
get_clang_version

printf "$bold$yellow--> Selected C Compiler <--$reset\n"
#echo $prefer_clang_over_gcc
#echo $is_gcc
#echo $is_clang
is_c_compiler=$is_gcc
if [ ! -z $prefer_clang_over_gcc ]
then
  if [ ! -z $is_clang ]
  then
    is_c_compiler=$is_clang
  fi
fi
printf "$grey--> %s <--$reset\n" $is_c_compiler

#----- C++ Compiler

get_gpp_version
get_clangpp_version

printf "$bold$yellow--> Selected C++ Compiler <--$reset\n"
is_cpp_compiler=$is_gpp
if [ ! -z $prefer_clang_over_gcc ]
then
  if [ ! -z $is_clangpp ]
  then
    is_cpp_compiler=$is_clangpp
  fi
fi
printf "$grey--> %s <--$reset\n" $is_cpp_compiler

#----- Linker

is_ldgnu=$(which ld)
is_ldgold=$(which ld.gold)
is_ldclang=$(which ld.lld)

is_sel_linker=$is_ldgnu

#echo $is_ldgnu
#echo $is_ldgold
#echo $is_ldclang

if [ ! -z $is_ldclang ];then
  printf "$bold$yellow--> Selected the Clang Linker <--$reset\n"
  printf "$grey--> %s <--$reset\n" $is_ldclang
  is_sel_linker=$is_ldclang
elif [ ! -z $is_ldgold ];then
  printf "$bold$yellow--> Selected the Gold Linker <--$reset\n"
  printf "$grey--> %s <--$reset\n" $is_ldgold
  is_sel_linker=$is_ldgold
else
  printf "$bold$yellow--> Selected the Gnu Linker <--$reset\n"
  printf "$grey--> %s <--$reset\n" $is_ldgnu
fi

#echo $is_sel_linker
$is_sel_linker --version

#-----

# Prompt the user
echo "Press any key to continue or wait for $timeout_duration seconds..."
# Use read with a timeout
read -t $timeout_duration -n 1 key

#-----

####################################################
# Some Repository Information
####################################################

printf "$bold$green--> R E P O S I T O R Y   S T A T U S <--$reset\n"

if [ ! -z $is_git ]; then
  get_is_git_repository
fi
if [ ! -z $is_mercurial ]; then
  get_is_hg_repository
fi

if [ ! -z $inside_git_repo ]; then
  git status
fi
if [ ! -z $inside_hg_repo ]; then
  hg summary
fi

#-----

# Prompt the user
echo "Press any key to continue or wait for $timeout_duration seconds..."
# Use read with a timeout
read -t $timeout_duration -n 1 key

#-----

####################################################
# prepare Build-Directory
####################################################

# Specify the directory you want to check
DIR="./build"

##DIR="path/to/your/directory"; [ ! -d "$DIR" ] && mkdir -p "$DIR"
# Check if the directory exists
if [ ! -d "$DIR" ]; then
    echo "Directory does not exist. Creating it now."
    mkdir -p "$DIR"  # Create the directory, -p ensures no error if it already exists
else
    echo "Directory already exists."
fi

rm -rf $DIR/*

####################################################
# Build
####################################################

printf "$bold$green--> B U I L D I N G   A L L   T A R G E T S <--$reset\n"

printf "$cyan--> Generate Makefile <--$reset\n"
#cmake -G "Your Generator"  path/to/your/source

if [ -n "$is_clangpp" ]; then
cmake -S . -B $DIR --trace-expand\
      -G $use_build_system\
      -DCMAKE_C_COMPILER=$is_c_compiler\
      -DCMAKE_CXX_COMPILER=$is_cpp_compiler\
      -DCMAKE_LINKER=$is_sel_linker\
      -DCMAKE_COLOR_DIAGNOSTICS=ON\
      -DCMAKE_BUILD_TYPE=Release
else
cmake -S . -B $DIR --trace-expand\
      -G $use_build_system\
      -DCMAKE_C_COMPILER=$is_c_compiler\
      -DCMAKE_CXX_COMPILER=$is_cpp_compiler\
      -DCMAKE_LINKER=$is_sel_linker\
      -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
      -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++" \
      -DCMAKE_COLOR_DIAGNOSTICS=ON\
      -DCMAKE_BUILD_TYPE=Release
fi

# Check if the command failed
if [ $? -ne 0 ]; then
    # Command failed, handle the error
    echo "Cmake failed."
fi

printf "$cyan--> Compile N Link <--$reset\n"
cmake --build $DIR --config Release --clean-first
##cmake --build $DIR --config Release --clean-first -- --stop-on-first-error -j

# Check if the command was successful
if [ $? -eq 0 ]; then
    # Command succeeded, execute further commands
    echo "The build was successful."
    ls -lh $DIR/bin
else
    # Command failed, handle the error
    echo "The build failed."
fi

####################################################
# Goodbye
####################################################

printf "$bold$green--> F I N I S H E D <--$reset\n"

echo "$reset"

pwd
