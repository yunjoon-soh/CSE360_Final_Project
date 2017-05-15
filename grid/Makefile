HDR=./header
BIN=./bin
SRC=./src
SCRIPT=./script

VPATH=/home/sekar/CSE360_Final_Project/grid/src

CC=gcc
#CFLAGS=-Wall -Werror -Wextra -g
CFLAGS=-Wall -Wextra -g -I$(HDR)
LDFLAGS2=-fPIC -shared -ldl 
RM=rm -f

# Application specific
HDRS=$(HDR)/builds.h $(HDR)/gridS.h
HDRS2=$(HDR)/builds.h $(HDR)/gridD.h

# force run ignoring the change of dependent file
.PHONY=buildNum

all: folder lib buildNum gridS gridD writeSample openSample rwSample fopenSample setuidSample

folder: 
	mkdir -p $(BIN)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(BIN)/$@

%Sample: %Sample.o
	$(CC) $(LDFLAGS) $(patsubst %.o,$(BIN)/%.o, $^) -o $@.out

#lib: libex.o net.o
#	gcc $(CFLAGS) $(LDFLAGS2) $(patsubst %.o,$(BIN)/%.o, $^) -o $@.so 
lib: ../libcall_intercept/lib.so
	cp $< ./

buildNum:
	$(SCRIPT)/buildN.sh

gridS: gridS.o net.o wrappers.o $(HDRS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(patsubst %.o,$(BIN)/%.o, $^) -o $@

gridD: gridD.o net.o wrappers.o $(HDRS2)
	$(CC) $(CFLAGS) $(LDFLAGS) $(patsubst %.o,$(BIN)/%.o, $^) -o $@

clean: 
	$(RM) -r ./*.out.dSYM
	$(RM) core
	$(RM) -r $(BIN)
	$(RM) gridS gridD *.out
	$(RM) *.so
