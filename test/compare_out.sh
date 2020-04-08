#!/bin/bash

# GREEN_INTENSIVE=$(tput bold)$(tput setaf 2)
# RED_INTENSIVE=$(tput bold)$(tput setaf 1)
# NC_INTENSIVE=$(tput bold)
# NC=$(tput sgr0)

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

SIMPLEDU_OUT="$1"
simpledu_exit="$2"
ARGS="$3"
DU_OUT=/tmp/du.out

du $ARGS 2> /dev/null | sort -k2 > $DU_OUT

EXIT_STATUS=0
diff -q $SIMPLEDU_OUT $DU_OUT > /dev/null 2>&1 || EXIT_STATUS=$?

if [[ $EXIT_STATUS -eq 0 ]] ; then
    printf "[ ${GREEN_INTENSIVE}OK${NC} ]\n"
    rm $DU_OUT $DU_ERR
else
    printf "[ ${RED_INTENSIVE}FAILED${NC} ]\n"
    rm $DU_OUT $DU_ERR
    exit 1
fi
