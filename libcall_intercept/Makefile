CC=gcc
CFLAGS=-Wall -Werror -I../grid/header

all: lib app

run: all
	./app

app: hello.o
	gcc $^ -o $@

lib: libex.c ../grid/net.o
	gcc $^ -o $@.so -fPIC -shared -ldl -D_GNU_SOURCE -I../grid/header

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Run following line. 
#	export LD_PRELOAD=`pwd`/lib.so

clean:
	rm -rf app *.o *.so

