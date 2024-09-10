OUT_FILE = ./main

all: output

clean:
	rm *.o
	rm $(OUT_FILE)

debug: CC += -DEBUG -g3
debug: output

output: read.o screen.o stack.o main.o
	$(CC) read.o screen.o stack.o main.o -o $(OUT_FILE) -lncurses

main.o: main.c
	$(CC) -c main.c

read.o: read.c
	$(CC) -c read.c

screen.o: screen.c
	$(CC) -c screen.c

stack.o: stack.c
	$(CC) -c stack.c

