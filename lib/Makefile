CC:=gcc
CFALGS:=-Wall
LIB:=liblogger.so

PREFIX:=/usr

.PHONY: clean all install uninstall

all: $(LIB)

$(LIB): logger.o
	$(CC) -fpic -shared -o $@ $^

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

install:
	install -d $(PREFIX)/lib $(PREFIX)/include
	install -m 555 $(LIB) $(PREFIX)/lib
	install -m 644 logger.h $(PREFIX)/include

uninstall:
	$(RM) $(PREFIX)/lib/$(LIB)
	$(RM) $(PREFIX)/include/logger.h

clean:
	$(RM) $(LIB) *.o	
