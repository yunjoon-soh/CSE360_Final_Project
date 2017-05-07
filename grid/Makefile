CC=gcc
CFLAGS=-Wall -Werror -Wextra -g
LDFLAGS=
RM=rm -f

# Application specific
SRC=gridS.c
HDRS=builds.h gridS.h
BIN=gridS

SRC2=gridD.c
HDRS2=builds.h gridD.h
BIN2=gridD

# force run ignoring the change of dependent file
.PHONY=buildNum

all: buildNum $(BIN) $(BIN2)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

buildNum: *.c
	./buildN.sh

gridS: gridS.o wrappers.o $(HDRS)
	$(CC) $(LDFLAGS) $^ -o $(BIN)

gridD: gridD.o wrappers.o $(HDRS2)
	$(CC) $(LDFLAGS) $^ -o $(BIN2)

clean: 
	$(RM) $(BIN) $(BIN2)
	$(RM) -r ./*.out.dSYM
	$(RM) core
	$(RM) ./*.o
	$(RM) ./*.out
