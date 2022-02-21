.PHONY: clean all install uninstall lib bin

all:lib bin

lib:
	$(MAKE) -C ./lib

bin:
	$(MAKE) -C ./src

install:
	$(MAKE) -C ./lib install
	$(MAKE) -C ./src install

uninstall:
	$(MAKE) -C ./lib uninstall
	$(MAKE) -C ./src uninstall

clean:
	$(MAKE) -C ./lib clean
	$(MAKE) -C ./src clean
