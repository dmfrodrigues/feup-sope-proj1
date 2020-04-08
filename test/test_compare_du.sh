set -e

# Testes do guião
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -b"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -b -S"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -b -S --max-depth=2"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -B 1"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -a"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -a -L"

# Testes ''
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder"
test/compare_simpledu_du.sh "./simpledu" "-l ."
test/compare_simpledu_du.sh "./simpledu" "-l include"
test/compare_simpledu_du.sh "./simpledu" "-l acutest"


# Testes '-S'
test/compare_simpledu_du.sh "./simpledu" "-l -S test/test-folder"
test/compare_simpledu_du.sh "./simpledu" "-l -S ."
test/compare_simpledu_du.sh "./simpledu" "-l -S include"
test/compare_simpledu_du.sh "./simpledu" "-l -S acutest"

# Testes '-S -a'
test/compare_simpledu_du.sh "./simpledu" "-l -S -a test/test-folder"
test/compare_simpledu_du.sh "./simpledu" "-l -S -a ."
test/compare_simpledu_du.sh "./simpledu" "-l -S -a include"
test/compare_simpledu_du.sh "./simpledu" "-l -S -a acutest"

# Testes '-S -a -B 1'
test/compare_simpledu_du.sh "./simpledu" "-l -S -a -B 1 test/test-folder"
test/compare_simpledu_du.sh "./simpledu" "-l -S -a -B 1 ."
test/compare_simpledu_du.sh "./simpledu" "-l -S -a -B 1 include"
test/compare_simpledu_du.sh "./simpledu" "-l -S -a -B 1 acutest"

# Testes '-S -B 10'
test/compare_simpledu_du.sh "./simpledu" "-l -S -B 10 test/test-folder"
test/compare_simpledu_du.sh "./simpledu" "-l -S -B 10 ."
test/compare_simpledu_du.sh "./simpledu" "-l -S -B 10 include"
test/compare_simpledu_du.sh "./simpledu" "-l -S -B 10 acutest"

# Testes '-L'
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder/D2 -a -L"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder/D1/Db/Dx -a -L"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder -a -L"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder/D1/Db/Dx/symlnk -a -L"

# Testes com vários
test/compare_simpledu_du.sh "./simpledu" "-l -S -B 10 acutest include test"

# Testes com barra final
test/compare_simpledu_du.sh "./simpledu" "-l -a test/test-folder/"
test/compare_simpledu_du.sh "./simpledu" "-l -a ./"
test/compare_simpledu_du.sh "./simpledu" "-l -a include/"
test/compare_simpledu_du.sh "./simpledu" "-l -a acutest/"

# Testes com erro
test/compare_simpledu_du.sh "./simpledu" "-l a"
test/compare_simpledu_du.sh "./simpledu" "-l a include"
test/compare_simpledu_du.sh "./simpledu" "-l a include acutest"
test/compare_simpledu_du.sh "./simpledu" "-l include a acutest"
test/compare_simpledu_du.sh "./simpledu" "-l include acutest a"
test/compare_simpledu_du.sh "./simpledu" "-l a b"
test/compare_simpledu_du.sh "./simpledu" "-l b a"
test/compare_simpledu_du.sh "./simpledu" "-l include b acutest a test"

# Testes de outros sítios
curr_dir=${PWD##*/}
cd ..
$curr_dir/test/compare_simpledu_du.sh "$curr_dir/simpledu" "-l -a $curr_dir/test/test-folder/"
$curr_dir/test/compare_simpledu_du.sh "$curr_dir/simpledu" "-l -a $curr_dir/"
$curr_dir/test/compare_simpledu_du.sh "$curr_dir/simpledu" "-l -a $curr_dir/include/"
$curr_dir/test/compare_simpledu_du.sh "$curr_dir/simpledu" "-l -a $curr_dir/acutest/"
cd $curr_dir

# Testes para ficheiros e para symlinks que apontam para ficheiros
test/compare_simpledu_du.sh "./simpledu" "-l include/simpledu_args.h"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder/D1/Db/Dx/F3bin -a"
test/compare_simpledu_du.sh "./simpledu" "-l test/test-folder/D1/Db/Dx/F3bin -a -L"

# Testes grandes
test/compare_simpledu_du.sh "./simpledu" "-l .."
test/compare_simpledu_du.sh "./simpledu" "-l ../../.docker"
test/compare_simpledu_du.sh "./simpledu" "-l ../.."
test/compare_simpledu_du.sh "./simpledu" "-l ../../.."
test/compare_simpledu_du.sh "./simpledu" "-l /home"
