set -e

# Testes do guião
test/compare_du.sh "-l test/test-folder"
test/compare_du.sh "-l test/test-folder -b"
test/compare_du.sh "-l test/test-folder -b -S"
test/compare_du.sh "-l test/test-folder -b -S --max-depth=2"
test/compare_du.sh "-l test/test-folder -B 1"
test/compare_du.sh "-l test/test-folder -a"
test/compare_du.sh "-l test/test-folder -a -L"

# Testes ''
test/compare_du.sh "-l test/test-folder"
test/compare_du.sh "-l ."
test/compare_du.sh "-l include"
test/compare_du.sh "-l acutest"


# Testes '-S'
test/compare_du.sh "-l -S test/test-folder"
test/compare_du.sh "-l -S ."
test/compare_du.sh "-l -S include"
test/compare_du.sh "-l -S acutest"

# Testes '-S -a'
test/compare_du.sh "-l -S -a test/test-folder"
test/compare_du.sh "-l -S -a ."
test/compare_du.sh "-l -S -a include"
test/compare_du.sh "-l -S -a acutest"

# Testes '-S -a -B 1'
test/compare_du.sh "-l -S -a -B 1 test/test-folder"
test/compare_du.sh "-l -S -a -B 1 ."
test/compare_du.sh "-l -S -a -B 1 include"
test/compare_du.sh "-l -S -a -B 1 acutest"

# Testes '-S -B 10'
test/compare_du.sh "-l -S -B 10 test/test-folder"
test/compare_du.sh "-l -S -B 10 ."
test/compare_du.sh "-l -S -B 10 include"
test/compare_du.sh "-l -S -B 10 acutest"

# Testes com vários
test/compare_du.sh "-l -S -B 10 acutest include test"

# Testes com barra final
test/compare_du.sh "-l -a test/test-folder/"
test/compare_du.sh "-l -a ./"
test/compare_du.sh "-l -a include/"
test/compare_du.sh "-l -a acutest/"

# Testes com erro
test/compare_du.sh "-l a"
test/compare_du.sh "-l a include"
test/compare_du.sh "-l a include acutest"
test/compare_du.sh "-l include a acutest"
test/compare_du.sh "-l include acutest a"
test/compare_du.sh "-l a b"
test/compare_du.sh "-l b a"
test/compare_du.sh "-l include b acutest a test"
