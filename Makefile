all: demo.c lab1_IO.c 
	gcc -o matrixgen matrixgen.c
	gcc -o demo demo.c -lpthread -std=c99 lab1_IO.c -I.
