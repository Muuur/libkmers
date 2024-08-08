#!/usr/bin/make -f
all:
	mkdir -p bin
	gcc kmers.c -fPIC -shared -o ~/.local/lib/libkmers.so -lm
	ln -sf ~/.local/lib/libkmers.so ~/.local/lib/libkmers.so.1
	gcc smain.c -lkmers -L$(HOME)/.local/lib -o bin/smain
	gcc mmain.c -lkmers -L$(HOME)/.local/lib -o bin/mmain
	# "You must add $(HOME)/.local/lib to the LD_LIBRARY_PATH environment variable to run the executables"

clean:
	rm -rf bin
	rm -f ~/.local/lib/libkmers.so ~/.local/lib/libkmers.so.1
