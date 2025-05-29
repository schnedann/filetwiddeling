#!/bin/bash

echo "$(tput setaf 6)Query Git Version$(tput sgr 0)"

#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# GIT Version
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

GIT_VER_TMP=$(git version)
echo $GIT_VER_TMP
GIT_VER_TMP1=$(echo $GIT_VER_TMP | cut -b 13)
GIT_VER_TMP2=$(echo $GIT_VER_TMP | cut -b 15-16)
GIT_VER=$(expr $GIT_VER_TMP1 \* "100" + $GIT_VER_TMP2)
echo $GIT_VER
