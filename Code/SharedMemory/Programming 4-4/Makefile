COMPILER = gcc
FLAG = -g -Wall -std=c99
OPT = -O2
SRC = main.c
OUT = run

all:
	$(COMPILER) $(FLAG) $(OPT) -o run $(SRC) -lpthread

clean:
	rm -r run* *~

#To run, use:
# ./run 2
