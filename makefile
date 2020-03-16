IFLAGS =$(IDIR)

CC     =gcc

SDIR   =./src
IDIR   =-I./include
ODIR   =./obj

CFLAGS =-std=c99 $(IFLAGS)

all: makefolders simpledu

clean:
	rm -rf $(ODIR)
	rm -f simpledu

makefolders:
	mkdir -p $(ODIR)

simpledu:             $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o
	$(CC) $(ODIR)/simpledu.o $(ODIR)/simpledu_args.o -o simpledu

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
