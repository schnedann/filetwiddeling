#!/bin/bash

source bashfu.sh

printf "%sUpdate Guideline Support Library%s\n" $cyan $reset

REPRO_LINK="https://github.com/microsoft/GSL.git"
REPRO_BRANCH="main"
REPRO_TAGREF="v4.2.0"
DIR_NAME="GSL"

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

clone_fetch_git $SRC_DIR $REPRO_LINK $REPRO_BRANCH

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

printf "%sSwitch to Revision%s\n" $green $reset
cd $SRC_DIR;
git switch --progress $REPRO_BRANCH
git checkout --detach tags/$REPRO_TAGREF;
cd ..;

git_info_about_repository $SRC_DIR

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

cd ..;
