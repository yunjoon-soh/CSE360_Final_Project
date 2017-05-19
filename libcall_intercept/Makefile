CC=gcc
CFLAGS=-Wall -Werror -I../grid/header

all: lib hello

run: all
	export LD_PRELOAD=$(shell pwd)/lib.so
	env | grep LD_PRELOAD
	./hello
	export LD_PRELOAD=

hello: hello.o
	gcc $^ -o $@

lib: libex.c
	gcc $^ -o $@.so -fPIC -shared -ldl -D_GNU_SOURCE -I../grid/include

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Run following line. 
#	export LD_PRELOAD=`pwd`/lib.so

clean:
	rm -rf app *.o *.so

