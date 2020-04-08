LOG_FILENAME=~/Documents/out.log
export LOG_FILENAME
VAR="-l include/simpledu_args.h"
./simpledu $VAR 2> ~/Documents/simpledu.err | sort -k2 > ~/Documents/simpledu.out
du $VAR 2> ~/Documents/du.err | sort -k2 > ~/Documents/du.out
