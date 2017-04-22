all: lib app

app: hello.c
	gcc $^ -o $@

lib: libex.c
	gcc $^ -o $@ -fPIC -shared -ldl -D_GNU_SOURCE

clean:
	rm -rf lib app
