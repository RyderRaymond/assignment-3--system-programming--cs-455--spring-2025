CC=gcc
FLAGS=-Wall -g
FILENAME=01--cat-using-read-write-open-and-close
#FILENAME=02--compare-two-files-and-print-first-line-where-they-differ

all: build run

build:
	$(CC) $(FLAGS) $(FILENAME).c -o $(FILENAME).out

run:
	$(FILENAME).out

clean:
	rm -fr $(FILENAME).out $(FILENAME).out.dSYM
