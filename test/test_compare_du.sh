GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

# failures=0
# trap 'failures=$((failures+1))' ERR

test/compare_du.sh "-S test/test-folder"
test/compare_du.sh "-S ."
test/compare_du.sh "-S include"
test/compare_du.sh "-S acutest"

# if ["$failures" -eq "0"]; then
#     printf "${GREEN_INTENSIVE}SUCCESS: ${NC}All comparisons were successful."
# else
#     echo "${RED_INTENSIVE}$failures failures${NC}"
#     exit 1
# fi
