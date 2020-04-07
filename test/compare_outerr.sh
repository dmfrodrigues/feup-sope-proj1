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
SIMPLEDU_ERR="$2"
simpledu_exit="$3"
ARGS="$4"
DU_OUT=/tmp/du.out
DU_ERR=/tmp/du.err

du $ARGS 2> $DU_ERR | sort -k2 > $DU_OUT
du_exit="${PIPESTATUS[0]}" 
EXIT_STATUS=0
diff -q $SIMPLEDU_OUT $DU_OUT > /dev/null 2>&1 || EXIT_STATUS=$?
diff -q $SIMPLEDU_ERR $DU_ERR > /dev/null 2>&1 || EXIT_STATUS=$?
[ $simpledu_exit == $du_exit ] ||  EXIT_STATUS=$?

if [[ $EXIT_STATUS -eq 0 ]] ; then
    printf "[ ${GREEN_INTENSIVE}OK${NC} ]\n"
    rm $DU_OUT $DU_ERR
else
    printf "[ ${RED_INTENSIVE}FAILED${NC} ]\n"
    rm $DU_OUT $DU_ERR
    exit 1
fi
