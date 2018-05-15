CFLAGS = -D_POSIX SOURCE
LDFLAGS = 
CC	= cc
LD	= cc
TARG	= a7
OBJS	= a7.o

all: $(TARG)

$(TARG): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARG)

clean:
rm -f $(OBJS) $(TARG) a.out
