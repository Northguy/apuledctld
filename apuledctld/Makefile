CC=c++
CFLAGS=-s -O2 -Wno-write-strings
CFLAGS=-g -Wall -Wextra -Werror -Wno-write-strings
CDFLAGS=-g -Wall -Wextra -Werror -Wno-write-strings -D__DEBUG_
PROG=apuledctld
LIB=
OBJS=common.o

all: $(OBJS) $(PROG)

$(PROG): $(OBJS) apuledctld.h
$(OBJS): apuledctld.h

.cpp.o:
	$(CC) $(OFLAGS) $(CFLAGS) -c $<

.cpp:
	$(CC) $(CFLAGS) -o $@ $< $(OBJS) $(LIB)

clean:
	rm -f *.o
	rm -f $(PROG)

install:
	cp -v $(PROG) $(DESTDIR)/usr/local/sbin
