all: memgrind.c mymalloc.o
	gcc -o memgrind memgrind.c mymalloc.o -w

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c 
clean:
	rm memgrind
	rm ./*.o

