PROG   =simpledu

IFLAGS =$(IDIR)

CC     =gcc

SDIR   =./src
IDIR   =-I./include
ODIR   =./obj
TDIR   =./test
TEXE   =test

CFLAGS_PARANOID =-Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wbad-function-cast \
-Wstrict-prototypes -Winline -Wundef \
-Wnested-externs -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter \
-Wfloat-equal -pedantic -Wno-long-long -Wno-endif-labels -g $(IFLAGS)

CFLAGS =-Wall -g $(IFLAGS) #-O3

all: makefolders $(PROG)

clean:
	rm -rf $(ODIR)
	rm -f $(ODIR)/$(TEXE)
	rm -f $(PROG)

test: all
	@echo Current directory: $(PWD)
	$(CC) $(CFLAGS) -c -I./acutest/include $(TDIR)/tests.c -o $(ODIR)/tests.o
	$(CC) $(ODIR)/tests.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o $(ODIR)/simpledu_iterate.o -o $(ODIR)/$(TEXE)
	$(ODIR)/$(TEXE)
	chmod u+x $(TDIR)/*.sh
	$(TDIR)/test_compare_du.sh

testmem: all
	valgrind --leak-check=yes $(ODIR)/$(TEXE)

makefolders:
	mkdir -p $(ODIR)
	mkdir -p $(TDIR)

$(PROG):  $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o $(ODIR)/simpledu_iterate.o
	$(CC) $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o $(ODIR)/simpledu_iterate.o -o simpledu

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

GROUP=T5G06

zip: clean
	rm -rf $(GROUP)
	mkdir $(GROUP)
	cp -r include src test acutest makefile README.md $(GROUP)
	rm -f $(GROUP).zip
	zip --symlinks $(GROUP).zip -r $(GROUP)
	rm -rf $(GROUP)

DEST=~/Documents

testzip: zip
	rm -rf $(DEST)/$(GROUP)
	unzip $(GROUP).zip -d $(DEST)
	make -C $(DEST)/$(GROUP)/ test
