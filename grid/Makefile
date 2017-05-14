CC=gcc
#CFLAGS=-Wall -Werror -Wextra -g
CFLAGS=-Wall -Wextra -g -I./header
LDFLAGS=
RM=rm -f

# Application specific
SRC=gridS.c
HDRS=builds.h ./header/gridS.h
BIN=gridS

SRC2=gridD.c
HDRS2=builds.h ./header/gridD.h
BIN2=gridD

# force run ignoring the change of dependent file
.PHONY=buildNum

all: buildNum $(BIN) $(BIN2) writeSample openSample rwSample fopenSample

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

buildNum: *.c
	./buildN.sh

gridS: gridS.o wrappers.o $(HDRS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BIN)

gridD: gridD.o net.o wrappers.o $(HDRS2)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BIN2)

%Sample: %Sample.o
	$(CC) $(LDFLAGS) $^ -o $@.out

clean: 
	$(RM) $(BIN) $(BIN2)
	$(RM) -r ./*.out.dSYM
	$(RM) core
	$(RM) ./*.o
	$(RM) ./*.out
