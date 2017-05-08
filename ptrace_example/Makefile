all: test hello.out

test: ptrace_test.o
	$(CC) $(CFLAGS) $^ -o $@

hello.out: hello.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o test hello.out
