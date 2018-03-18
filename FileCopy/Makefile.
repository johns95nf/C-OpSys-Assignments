CDFLAGS = -D_POSIX SOURCE
LDFLAGS = 
CC	= cc
LD	= cc
TARG	= a2
OBJS	= a2.o

all: $(TARG)

$(TARG): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARG)

clean:
	rm -f $(OBJS) $(TARG) a.out
