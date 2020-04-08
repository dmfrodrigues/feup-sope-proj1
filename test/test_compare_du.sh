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

# Testes grandes
test/compare_simpledu_du.sh "./simpledu" "-l .."
test/compare_simpledu_du.sh "./simpledu" "-l ../.."
test/compare_simpledu_du.sh "./simpledu" "-l ../../.."
test/compare_simpledu_du.sh "./simpledu" "-l /home"

# Testes de outros sítios
cd ..
feup-sope-proj1/test/compare_simpledu_du.sh "feup-sope-proj1/simpledu" "-l -a test/test-folder/"
feup-sope-proj1/test/compare_simpledu_du.sh "feup-sope-proj1/simpledu" "-l -a ./"
feup-sope-proj1/test/compare_simpledu_du.sh "feup-sope-proj1/simpledu" "-l -a include/"
feup-sope-proj1/test/compare_simpledu_du.sh "feup-sope-proj1/simpledu" "-l -a acutest/"
cd feup-sope-proj1
