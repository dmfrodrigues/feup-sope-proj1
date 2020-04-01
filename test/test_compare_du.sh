set -e

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

# failures=0
# trap 'failures=$((failures+1))' ERR

exit_stat=0
test/compare_du.sh "-l -S test/test-folder"
test/compare_du.sh "-l -S ."
test/compare_du.sh "-l -S include"
test/compare_du.sh "-l -S acutest"
