#!/bin/bash

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

cmake -S . -B $DIR --trace-expand\
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
      -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++" \
      -DCMAKE_BUILD_TYPE=Release

# Check if the command failed
if [ $? -ne 0 ]; then
    # Command failed, handle the error
    echo "Cmake failed."
fi

cmake --build $DIR -- -j4

# Check if the command was successful
if [ $? -eq 0 ]; then
    # Command succeeded, execute further commands
    echo "The build was successful."
    ls -l $DIR/bin
else
    # Command failed, handle the error
    echo "The build failed."
fi
