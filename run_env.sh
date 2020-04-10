LOG_FILENAME=/tmp/out.log
export LOG_FILENAME
VAR="-l ../../.."
./simpledu $VAR 2> /tmp/simpledu.err | sort -k2 > /tmp/simpledu.out
du $VAR 2> /tmp/du.err | sort -k2 > /tmp/du.out
