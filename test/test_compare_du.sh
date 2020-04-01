set -e

test/compare_du.sh "-l -S test/test-folder"
test/compare_du.sh "-l -S ."
test/compare_du.sh "-l -S include"
test/compare_du.sh "-l -S acutest"

test/compare_du.sh "-l -a -S test/test-folder"
test/compare_du.sh "-l -a -S ."
test/compare_du.sh "-l -a -S include"
test/compare_du.sh "-l -a -S acutest"
