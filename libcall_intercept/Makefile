CC=gcc
CFLAGS=-Wall -Werror
run: all
	./app

all: lib app

app: hello.o
	gcc $^ -o $@

lib: libex.c
	gcc $^ -o $@.so -fPIC -shared -ldl -D_GNU_SOURCE

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Run following line. 
#	export LD_PRELOAD=`pwd`/lib.so

clean:
	rm -rf app *.o *.so
