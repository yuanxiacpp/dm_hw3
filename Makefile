all: project3

project3: project3.c heap.h
	gcc -g -o project3 project3.c -lm -Wall

clean:
	rm *~ project3
