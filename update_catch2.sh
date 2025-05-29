#!/bin/bash

####################################################
# Load Script-Fu
####################################################

source ./bashfu.sh
echo "$reset"

####################################################
# Load Build Config
####################################################

source ./build_config.cfg

printf "%sUpdate Catch2 Unittest Framework%s\n" $cyan $reset

REPRO_LINK="https://github.com/catchorg/Catch2.git"
REPRO_BRANCH="devel"
REPRO_TAGREF="v3.8.1"
DIR_NAME="Catch2"

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# GIT Version
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

get_git_version

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Test/Ensure target dir exists
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

EXT_DIR="./external"

test_ensure_folder $EXT_DIR

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Fetch Catch2 from Github
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

SRC_DIR="./$DIR_NAME"

clone_fetch_git $SRC_DIR $REPRO_LINK $REPRO_BRANCH

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Change to the Requestet Repository Revision / Commit
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

printf "%sSwitch to Revision%s\n" $green $reset
cd $SRC_DIR;
git switch --progress $REPRO_BRANCH
git checkout --detach tags/$REPRO_TAGREF;
cd ..;

git_info_about_repository $SRC_DIR

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

cd $EXT_DIR;
cd $SRC_DIR;

BUILD_DIR="./build"

test_ensure_folder $BUILD_DIR

cd ..;

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

####################################################
# Some Repository Information
####################################################

printf "$bold$green--> R E P O S I T O R Y   S T A T U S <--$reset\n"


# GIT
if [ ! -z $is_git ]; then
  get_is_git_repository
fi
if [ ! -z $inside_git_repo ]; then
  git status
fi

# Mercurial
if [ ! -z $is_mercurial ]; then
  get_is_hg_repository
fi
if [ ! -z $inside_hg_repo ]; then
  hg summary
fi

####################################################
# Build
####################################################

#explicit coloize Errors and Warnings from CMAKE
##cmake()
##{
##  /usr/bin/cmake "$@" 2>&1 | sed -E -e "s/[Ee]rror[: ]/ $(echo -e "\\033[31m" ERROR "\\033[0m"/g)" -e "s/[Ww]arning[: ]/ $(echo -e "\\033[0;33m" WARNING "\\033[0m"/g)" -e "s/Fehler/ $(echo -e "\\033[31m" Fehler "\\033[0m"/g)"
##  return ${PIPESTATUS[0]}
##}

printf "$bold$green--> B U I L D I N G   A L L   T A R G E T S <--$reset\n"
pwd
printf "$cyan--> Generate Makefile <--$reset\n"
#cmake -G "Your Generator"  path/to/your/source
cmake -S . -B $BUILD_DIR -D CMAKE_C_COMPILER=$is_c_compiler -D CMAKE_CXX_COMPILER=$is_cpp_compiler -D CMAKE_LINKER=$is_sel_linker -D CMAKE_COLOR_DIAGNOSTICS=ON
printf "$cyan--> Compile N Link <--$reset\n"
cmake --build $BUILD_DIR --config Debug --clean-first -- -j

####################################################
# Goodbye
####################################################

printf "$bold$green--> F I N I S H E D <--$reset\n"

echo "$reset"

pwd
ls -lh ./build/src/
ls -lh ./build/generated-includes/catch2/

cd ..;
cd ..;

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

cd ..;
