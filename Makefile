all: project3

project3: project3.c heap.h
	gcc -g -o project3 project3.c -lm

clean:
	rm *~ project3
