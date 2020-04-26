all: compile execute

compile: main.o car.o
	gcc -Wall main.o car.o -o exec

main.o: main.c car.h
	gcc -Wall -c main.c

car.o: car.c car.h
	gcc -Wall -c car.c

execute: exec
	./exec

clean:
	rm *.o exec