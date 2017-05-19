HDR=./include
BIN=./bin
SRC=./src
SCRIPT=./script

VPATH=/home/sekar/CSE360_Final_Project/grid/src

CC=gcc
#CFLAGS=-Wall -Werror -Wextra -g
CFLAGS=-Wall -Wextra -g -I$(HDR) -D_GNU_SOURCE
RM=rm -f

# Application specific
COMMON_SRC=net.c wrappers.c
HDRS=$(HDR)/builds.h $(HDR)/gridS.h
HDRS2=$(HDR)/builds.h $(HDR)/gridD.h

# force run ignoring the change of dependent file
.PHONY=buildNum

all: folder lib buildNum gridS gridD gridDPseudo writeSample openSample rwSample fopenSample setuidSample

folder: 
	mkdir -p $(BIN)

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%Sample: $(BIN)/%Sample.o
	$(CC) $^ -o $@.out

lib: $(BIN)/libex.o $(BIN)/net.o $(BIN)/lookupTable.o $(BIN)/helper.o
	gcc $^ -o $@.so -fPIC -shared -ldl -D_GNU_SOURCE -I$(HDR)
buildNum:
	$(SCRIPT)/buildN.sh

gridS: $(BIN)/gridS.o $(patsubst %.c,$(BIN)/%.o, $(COMMON_SRC)) $(HDRS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

gridD: $(BIN)/gridD.o $(patsubst %.c,$(BIN)/%.o, $(COMMON_SRC)) $(HDRS2)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

gridDPseudo: $(BIN)/gridDPseudo.o $(BIN)/helper.o $(BIN)/lookupTable.o $(patsubst %.c,$(BIN)/%.o, $(COMMON_SRC)) $(HDRS3)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean: 
	$(RM) -r ./*.out.dSYM
	$(RM) core
	$(RM) -r $(BIN)
	$(RM) gridS gridD gridDPseudo *.out
	$(RM) *.so
