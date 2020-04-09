#!/bin/bash

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

SIMPLEDU_OUT=/tmp/simpledu.out
SIMPLEDU_ERR=/tmp/simpledu.err

s="${NC_INTENSIVE}Test '$1' with args '$2'...${NC}"
printf %-96b "$s"

$1 $2 > $SIMPLEDU_OUT 2> $SIMPLEDU_ERR
simpledu_exit="${PIPESTATUS[0]}"

mypath=$(dirname $0)
$mypath/compare_outerr.sh $SIMPLEDU_OUT $SIMPLEDU_ERR $simpledu_exit "$2"
exit_status=$?

rm $SIMPLEDU_OUT $SIMPLEDU_ERR

exit $exit_status
