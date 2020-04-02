LOG_FILENAME=~/Documents/out.log
export LOG_FILENAME
VAR="-S -a -B 10 include"
./simpledu $VAR | sort -k2 > ~/Documents/simpledu.txt
du $VAR | sort -k2 > ~/Documents/du.txt
