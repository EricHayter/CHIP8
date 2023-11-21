

main: *.o
	gcc $^ -o main -lncurses -g3

%.o: %.c
	gcc -c $@ 

clean: *.o
	rm $^
