set -e
test/compare_du.sh "-l -S test/test-folder"
test/compare_du.sh "-l -S ."
test/compare_du.sh "-l -S include"
test/compare_du.sh "-l -S acutest"
