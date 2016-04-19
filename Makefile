all:
	gcc -Wall main.c graphList.c fila.c fila.h graphList.h -o main -std=c99
run:
	./main
