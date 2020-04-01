LOG_FILENAME=~/Documents/out.log
export LOG_FILENAME
./simpledu -l test/test-folder -b -S | sort -k2 > ~/Documents/simpledu.txt
