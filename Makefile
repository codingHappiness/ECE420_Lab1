all: main.c lab1_IO.c 
	gcc -o matrixgen matrixgen.c
	gcc -o main main.c lab1_IO.c -I. -lpthread