set -e

#test/compare_du.sh "-l test/test-folder"
#test/compare_du.sh "-l test/test-folder -b"
#test/compare_du.sh "-l test/test-folder -b -S"
#test/compare_du.sh "-l test/test-folder -b -S --max-depth=2"
#test/compare_du.sh "-l test/test-folder -B 1"
#test/compare_du.sh "-l test/test-folder -a"
#test/compare_du.sh "-l test/test-folder -a -L"

test/compare_du.sh "-l -S test/test-folder"
test/compare_du.sh "-l -S ."
test/compare_du.sh "-l -S include"
test/compare_du.sh "-l -S acutest"

test/compare_du.sh "-l -S -a test/test-folder"
test/compare_du.sh "-l -S -a ."
test/compare_du.sh "-l -S -a include"
test/compare_du.sh "-l -S -a acutest"

test/compare_du.sh "-l -S -a -B 1 test/test-folder"
test/compare_du.sh "-l -S -a -B 1 ."
test/compare_du.sh "-l -S -a -B 1 include"
test/compare_du.sh "-l -S -a -B 1 acutest"

# test/compare_du.sh "-l test/test-folder"
# test/compare_du.sh "-l ."
# test/compare_du.sh "-l include"
# test/compare_du.sh "-l acutest"
