PROG   =simpledu

IFLAGS =$(IDIR)

CC     =gcc

SDIR   =./src
IDIR   =-I./include
ODIR   =./obj
TDIR   =./test
TEXE   =test

CFLAGS =-Wall -g $(IFLAGS)

all: makefolders $(PROG)

clean:
	rm -rf $(ODIR)
	rm -f $(ODIR)/$(TEXE)
	rm -f $(PROG)

test: all
	g++ -g $(IDIR) -I./Catch2/single_include/catch2 -c $(TDIR)/tests.cpp -o $(ODIR)/tests.o
	g++ -g $(ODIR)/tests.o $(ODIR)/simpledu_args.o -o $(ODIR)/$(TEXE)
	$(ODIR)/$(TEXE)

makefolders:
	mkdir -p $(ODIR)
	mkdir -p $(TDIR)

$(PROG):  $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o
	$(CC) $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o $(ODIR)/simpledu_envp.o $(ODIR)/simpledu_stat.o $(ODIR)/simpledu_log.o $(ODIR)/simpledu_time.o -o simpledu

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
