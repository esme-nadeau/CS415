all: pseudo-shell

pseudo-shell: main.o
	gcc -o pseudo-shell main.o 

main.o: main.c
	gcc -c main.c

clean:
	rm -f core *.o pseudo-shell
