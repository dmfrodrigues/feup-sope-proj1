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

s="${NC_INTENSIVE}Test '$1' with args '$2'...${NC}"
printf %-88b "$s"

$1 $2 2> $SIMPLEDU_ERR | sort -k2 > $SIMPLEDU_OUT
simpledu_exit="${PIPESTATUS[0]}"

mypath=$(dirname $0)
$mypath/compare_outerr.sh $SIMPLEDU_OUT $SIMPLEDU_ERR $simpledu_exit "$2"

rm $SIMPLEDU_OUT $SIMPLEDU_ERR
