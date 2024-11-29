all: a4

a4: main.o graph_functions.o
	gcc -Wall -g main.o graph_functions.o -o a4

main.o: main.c graph.h
	gcc -Wall -g -c main.c

graph_functions.o: graph_functions.c graph.h
	gcc -Wall -g -c graph_functions.c

clean:
	rm *.o a4
