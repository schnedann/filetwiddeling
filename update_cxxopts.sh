#!/bin/bash

source bashfu.sh

printf "%sUpdate CxxOpts%s\n" $cyan $reset

REPRO_LINK="https://github.com/jarro2783/cxxopts.git"
REPRO_BRANCH="master"
REPRO_TAGREF="v3.3.1"
DIR_NAME="cxxopts"

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
# Fetch GSL from Github
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

SRC_DIR="./$DIR_NAME"

##clone_fetch_git $SRC_DIR $REPRO_LINK $REPRO_BRANCH

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

printf "%sSwitch to Revision%s\n" $green $reset
##cd $SRC_DIR;

# Clone to Tag / Reference
if [ -z "$REPRO_TAGREF" ]; then
  # The variable is empty
  git clone --depth 1 --branch $REPRO_BRANCH $REPRO_LINK
# Clone to Branch
else
  # The variable is not empty
  git clone --depth 1 --branch $REPRO_TAGREF $REPRO_LINK
fi

##git switch --progress $REPRO_BRANCH
##git checkout --detach tags/$REPRO_TAGREF;
cd ..;

git_info_about_repository $SRC_DIR

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

cd ..;
