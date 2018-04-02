CFLAGS		= -D_POSIX_SOURCE
LDFLAGS		= 
CC		= cc
LD		= cc

TARG = a4
OBJS = a4.o

all: $(TARG)

$(TARG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARG)

clean:
	rm -f $(TARG) $(OBJS) a.out
