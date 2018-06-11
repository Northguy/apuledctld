CC=g++
#CFLAGS=-s -O2 -Wno-write-strings
CFLAGS=-g -Wall -Wextra -Werror -Wno-write-strings
CDFLAGS=-g -Wall -Wextra -Werror -Wno-write-strings -D__DEBUG_
PROG=apuledctld
LIB=

all: $(PROG)

%: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LIB)

%.o: %.cpp
	$(CC) $(OFLAGS) $(CFLAGS) -c $<

clean:
	rm -f $(PROG)

install:
	cp -v $(PROG) $(DESTDIR)/usr/local/sbin
