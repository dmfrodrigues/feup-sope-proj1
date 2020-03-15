IFLAGS =$(IDIR)

CC     =gcc

SDIR   =./src
IDIR   =-I./include
ODIR   =./obj

CFLAGS =$(IFLAGS)

all: makefolders simpledu

clean:
	rm -rf $(ODIR)
	rm -f simpledu

makefolders:
	mkdir -p obj
	mkdir -p bin

simpledu:             $(ODIR)/simpledu.o
	$(CC) $(ODIR)/simpledu.o -o simpledu

$(ODIR)/%.o:          $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
