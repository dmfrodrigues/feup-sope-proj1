LOG_FILENAME=~/Documents/out.log
export LOG_FILENAME
./simpledu -l test/test-folder/D1 -b -S | sort -k2 > ~/Documents/simpledu.txt
#./simpledu --apparent-size --block-size=1 -l -S --max-depth=2048 test/test-folder/D1/Db  | sort -k2 > ~/Documents/simpledu.txt
