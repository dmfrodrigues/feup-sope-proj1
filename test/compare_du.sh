GREEN_INTENSIVE=$(tput bold)$(tput setaf 2)
RED_INTENSIVE=$(tput bold)$(tput setaf 1)
NC_INTENSIVE=$(tput bold)
NC=$(tput sgr0)

s="${NC_INTENSIVE}Test with args '$1'...${NC}"
printf %-58s "$s"

(./simpledu $1 || echo $?) | sort -k2 > /tmp/teste_simpledu.txt
(du         $1 || echo $?) | sort -k2 > /tmp/teste_du.txt
if diff -q /tmp/teste_simpledu.txt /tmp/teste_du.txt > /dev/null 2>&1 ; then
    printf "[ ${GREEN_INTENSIVE}OK${NC} ]\n"
    rm /tmp/teste_simpledu.txt /tmp/teste_du.txt
else
    printf "[ ${RED_INTENSIVE}FAILED${NC} ]\n"
    rm /tmp/teste_simpledu.txt /tmp/teste_du.txt
    exit 1
fi
