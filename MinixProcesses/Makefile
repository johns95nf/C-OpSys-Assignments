CFLAGS = -D_POSIX_SOURCE
LDFLAGS = 
CC = cc
LD = cc

TARG = a3
OBJS = a3.o

all: $(TARG)

$(TARG): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARG)

clean:
	rm -f $(OBJS) $(TARG) a.out
