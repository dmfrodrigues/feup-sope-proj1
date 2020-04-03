#!/bin/bash

# GREEN_INTENSIVE=$(tput bold)$(tput setaf 2)
# RED_INTENSIVE=$(tput bold)$(tput setaf 1)
# NC_INTENSIVE=$(tput bold)
# NC=$(tput sgr0)

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

SIMPLEDU_OUT=/tmp/simpledu.out
SIMPLEDU_ERR=/tmp/simpledu.err
DU_OUT=/tmp/du.out
DU_ERR=/tmp/du.err

s="${NC_INTENSIVE}Test '$1' with args '$2'...${NC}"
printf %-88b "$s"

#(./simpledu $2 || echo $?) | sort -k2 > $SIMPLEDU_OUT 2> $SIMPLEDU_ERR
#(du         $2 || echo $?) | sort -k2 > $DU_OUT 2> $DU_ERR
$1 $2 2> $SIMPLEDU_ERR | sort -k2 > $SIMPLEDU_OUT
simpledu_exit="${PIPESTATUS[0]}"
du         $2 2> $DU_ERR       | sort -k2 > $DU_OUT
du_exit="${PIPESTATUS[0]}" 
EXIT_STATUS=0
diff -q $SIMPLEDU_OUT $DU_OUT > /dev/null 2>&1 || EXIT_STATUS=$?
diff -q $SIMPLEDU_ERR $DU_ERR > /dev/null 2>&1 || EXIT_STATUS=$?
[ $simpledu_exit == $du_exit ] ||  EXIT_STATUS=$?

if [[ $EXIT_STATUS -eq 0 ]] ; then
    printf "[ ${GREEN_INTENSIVE}OK${NC} ]\n"
    rm $SIMPLEDU_OUT $SIMPLEDU_ERR $DU_OUT $DU_ERR
else
    printf "[ ${RED_INTENSIVE}FAILED${NC} ]\n"
    rm $SIMPLEDU_OUT $SIMPLEDU_ERR $DU_OUT $DU_ERR
    exit 1
fi
