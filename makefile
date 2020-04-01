PROG   =simpledu

IFLAGS =$(IDIR)

CC     =gcc

SDIR   =./src
IDIR   =-I./include
ODIR   =./obj
TDIR   =./test
TEXE   =test

CFLAGS =-Wall -O0 -g $(IFLAGS)

all: makefolders $(PROG)

clean:
	rm -rf $(ODIR)
	rm -f $(ODIR)/$(TEXE)
	rm -f $(PROG)

test: all
	$(CC) $(CFLAGS) -c -I./acutest/include $(TDIR)/tests.c -o $(ODIR)/tests.o
	$(CC) $(ODIR)/tests.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o $(ODIR)/unnamed_functions.o -o $(ODIR)/$(TEXE)
	$(ODIR)/$(TEXE)
	chmod u+x $(TDIR)/test_compare_du.sh $(TDIR)/compare_du.sh
	$(TDIR)/test_compare_du.sh

testmem:
	valgrind --leak-check=yes $(ODIR)/$(TEXE)

makefolders:
	mkdir -p $(ODIR)
	mkdir -p $(TDIR)

$(PROG):  $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o $(ODIR)/unnamed_functions.o
	$(CC) $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o $(ODIR)/unnamed_functions.o -o simpledu

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
