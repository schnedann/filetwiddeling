#!/bin/bash

function test_print {
# Print a red dot
echo -e "\e[31m•\e[0m"

# Print a green dot
echo -e "\e[32m•\e[0m"

# Print a red filled circle
echo -e "\e[31\u25CF\e[0m"

# Print a green filled circle
echo -e "\e[32m\u25CF\e[0m"

# Print a red empty circle
echo -e "\e[31m\u25CB\e[0m"

# Print a green empty circle
echo -e "\e[32m\u25CB\e[0m"
}

####################################################
# Ansi Color Codes
####################################################

red=$(tput setaf 1)
green=$(tput setaf 2)
yellow=$(tput setaf 3)
blue=$(tput setaf 4)
magenta=$(tput setaf 5)
cyan=$(tput setaf 6)
grey=$(tput setaf 7)
bg_red=$(tput setab 1)
bg_green=$(tput setab 2)
bg_yellow=$(tput setab 3)
bg_blue=$(tput setab 4)
bg_magenta=$(tput setab 5)
bg_cyan=$(tput setab 6)
bg_grey=$(tput setab 7)
reset=$(tput sgr0)


blink=$(tput blink) # blinking text on
bold=$(tput bold)   # Select bold mode
dim=$(tput dim)     # Select dim (half-bright) mode
rev=$(tput rev)     # reverse video on (switch text/background colors)
smul=$(tput smul)   # Enable underline mode
rmul=$(tput rmul)   # Disable underline mode
#rev=$(tput rev)    # Turn on reverse video mode
smso=$(tput smso)   # Enter standout (bold) mode
rmso=$(tput rmso)   # Exit standout mode


#sitm 	  \e[3m 	  italic text on
#ritm 	  \e[23m 	  italic text off
#smso 	  \e[7m 	  highlighted text on
#rmso 	  \e[27m 	  highlighted text off
#smul 	  \e[4m 	  underlined text on
#rmul 	  \e[24m 	  underlined text off
#setab #1 \e[4#1m 	set background color #1 (0-7)
#setaf #1 \e[3#1m 	set text color #1 (0-7)
#sgr0 	  \e(B\e[m


####################################################
# Get CMake Version
####################################################

function get_cmake_version {
  is_cmake=$(which cmake)
  if [ -z "$is_cmake" ]
  then
    printf "$bold$yellow--> Warning, CMake is not installed <--$reset\n"
  else
    printf "$bold$grey--> CMake <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_cmake"
    cmake --version
  fi
}

####################################################
# Get Mercurial Version
####################################################

function get_mercurial_version {
  is_mercurial=$(which hg)
  if [ -z "$is_mercurial" ]
  then
    printf "$bold$yellow--> Warning, Mercurial is not installed <--$reset\n"
  else
    printf "$bold$grey--> Mercurial <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_mercurial"
    hg --version

    #HG_VER_TMP=$(git version)
    #HG_VER_TMP1=$(echo $HG_VER_TMP | cut -b 13)
    #HG_VER_TMP2=$(echo $HG_VER_TMP | cut -b 15-16)
    #HG_VER=$(expr $HG_VER_TMP1 \* "100" + $HG_VER_TMP2)
    #printf "%s%s %s       : %s%s\n" $yellow $HG_VER_TMP "$reset"
    #printf "%sVersion as Integer: %s%s\n" $yellow $HG_VER "$reset"
  fi
}

####################################################
# Get Git Version
####################################################

function get_git_version {
  is_git=$(which git)
  if [ -z "$is_mercurial" ]
  then
    printf "$bold$yellow--> Warning, Git is not installed <--$reset\n"
  else
    printf "$bold$grey--> G i t <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_git"
    git --version

    GIT_VER_TMP=$(git version)
    GIT_VER_TMP1=$(echo "$GIT_VER_TMP" | cut -b 13)
    GIT_VER_TMP2=$(echo "$GIT_VER_TMP" | cut -b 15-16)
    GIT_VER=$(expr $GIT_VER_TMP1 \* "100" + "$GIT_VER_TMP2")
    printf "%s%s%s\n" "$yellow" "$GIT_VER_TMP" "$reset"
    printf "%sVersion as Integer: %s%s\n" "$yellow" "$GIT_VER" "$reset"
  fi
}

####################################################
# Check if Git / Mercurial repository
####################################################

function get_is_git_repository {
  inside_git_repo="$(git rev-parse --is-inside-work-tree 2>/dev/null)"
                    #git rev-parse --git-dir > /dev/null 2>&1
  toplevel_git_repo="$(git rev-parse --show-toplevel 2>/dev/null)"

  if [ "$inside_git_repo" ]; then
    if test "$(pwd)" = "$toplevel_git_repo"; then
      printf "$green-- root of a git repo --$reset\n"
    else
      printf "$yellow-- inside a git repo --$reset\n"
    fi

    if [[ $(git diff --stat) != '' ]]; then
      printf "$red-- Repository dirty --$reset\n"
    else
      printf "$green-- Repository Clean --$reset\n"
    fi

  else
    printf "$grey-- not in a git repo --$reset\n"
  fi
}

function get_is_hg_repository {
  inside_hg_repo="$(hg root 2>/dev/null)"
  toplevel_hg_repo="$(hg root 2>/dev/null)"

  if [ "$inside_hg_repo" ]; then
    if test "$(pwd)" = "$toplevel_hg_repo"; then
      printf "$green-- root of a hg repo --$reset\n"
    else
      printf "$yellow-- inside a hg repo --$reset\n"
    fi

    if [ -n "$(hg status -mar)" ]; then
      printf "$red-- Repository dirty --$reset\n"
    else
      printf "$green-- Repository Clean --$reset\n"
    fi

  else
    printf "$grey-- not in a hg repo --$reset\n"
  fi
}

####################################################
# Get Gcc Version
####################################################

function get_gcc_version {
  is_gcc=$(which gcc)
  if [ -z "$is_gcc" ]
  then
    printf "$bold$yellow--> Warning, Gnu C Compiler is not installed <--$reset\n"
  else
    printf "$bold$grey--> Gnu C Compiler <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_gcc"
    gcc --version
  fi
}

####################################################
# Get Clang Version
####################################################

function get_clang_version {
  is_clang=$(which clang)
  if [ -z "$is_clang" ]
  then
    printf "$bold$yellow--> Warning, Clang C Compiler is not installed <--$reset\n"
  else
    printf "$bold$grey--> Clang C Compiler <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_clang"
    clang --version
  fi
}

####################################################
# Get G++ Version
####################################################

function get_gpp_version {
  is_gpp=$(which g++)
  if [ -z "$is_gpp" ]
  then
    printf "$bold$yellow--> Warning, Gnu C++ Compiler is not installed <--$reset\n"
  else
    printf "$bold$grey--> Gnu C++ Compiler <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_gpp"
    g++ --version
  fi
}

####################################################
# Get Clang++ Version
####################################################

function get_clangpp_version {
  is_clangpp=$(which clang++)
  if [ -z "$is_clangpp" ]
  then
    printf "$bold$yellow--> Warning, Clang C++ Compiler is not installed <--$reset\n"
  else
    printf "$bold$grey--> Clang C++ Compiler <--$reset\n"
    printf "$grey--> %s <--$reset\n" "$is_clangpp"
    clang++ --version
  fi
}

####################################################
# Test/Ensure target dir exists
####################################################

function test_ensure_folder {
printf "%sTest Ensure Folder: %s%s\n" "$grey" "$1" "$reset"
if test ! -d "$1"
then
  mkdir "$1"
fi

cd "$1" || return
pwd
}

####################################################
# Clone Fetch with git
####################################################

function clone_fetch_git {
if [ ! -d "$1" ]
then
  printf "%sCloning...%s\n" "$green" "$reset"
  git clone "$2" --branch "$3";
else
  (
  printf "%sFetching...%s\n" "$green" "$reset"
  cd "$1" || return
  pwd;
  #git fetch $2 --tags --progress $3;
  git fetch --tags --progress --all;
  )
fi
}

####################################################
# git info
####################################################

GITLOLA="log --graph --decorate --pretty=oneline --abbrev-commit --all --date=local -n 20"
GITLOL="log --graph --decorate --pretty=oneline --abbrev-commit -n 20"

function git_info_about_repository {
(
cd "$1" || return
# Check if we are in a detached HEAD state
if ! git symbolic-ref -q HEAD; then
  echo -e "\e[31m\u25CF\e[0m You are in a detached HEAD state."
else
  echo -e "\e[32m\u25CF\e[0m You are on a branch."
fi
printf "%s---> Remotes <---%s\n" "$magenta" "$reset"
git remote -v
printf "%s---> Branches <---%s\n" "$magenta" "$reset"
git branch -va
printf "%s---> Tags <---%s\n" "$magenta" "$reset"
git tag -n5
printf "%s--> latest Tag <--%s\n" "$grey" "$reset"
git describe --tags
printf "%s---> Status <---%s\n" "$magenta" "$reset"
git status;
#git log --stat -n 1
git "$GITLOLA"
)
}

####################################################
#
####################################################
