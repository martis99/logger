CC:=gcc
CFLAGS:=-Wall
CPPFLAGS:=-I../lib
LDFLAGS:=-L../lib -llogger -lsqlite3
BIN:=log_reader
PREFIC:=/usr

.PHONY: clean all install uninstall

all: $(BIN)

$(BIN): main.o
	$(CC) -o $@ $^ $(LDFLAGS)

logger.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^

install:
	install -d $(PREFIX)/bin
	install -m 555 $(BIN) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(BIN)

clean:
	$(RM) $(BIN) *.o
