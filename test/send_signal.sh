SIMPLEDU_OUT=/tmp/simpledu.out
SIMPLEDU_ERR=/tmp/simpledu.err

PROG="./simpledu"
ARGS="../.."

$PROG $ARGS | sort -k2 > $SIMPLEDU_OUT
test/compare_out.sh $SIMPLEDU_OUT $ARGS

echo "y" | $PROG $ARGS | sort -k2 > $SIMPLEDU_OUT &
PID=$(ps u | grep "$PROG $ARGS" | head -n 1 | awk '{print $2}')
kill -SIGINT $PID
sleep 1s
test/compare_out.sh $SIMPLEDU_OUT $ARGS
