CC=c++
CFLAGS=-s -O2 -Wno-write-strings
CFLAGS=-g -Wall -Wextra -Werror -Wno-write-strings
CDFLAGS=-g -Wall -Wextra -Werror -Wno-write-strings -D__DEBUG_
PROG=apuledctld
LIB=-lpthread
OBJS=common.o log.o modesw.o conf.o led.o

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
	cp -v al_chscheme $(DESTDIR)/usr/local/sbin
	cp -v $(PROG).conf.sample $(DESTDIR)/usr/local/etc
