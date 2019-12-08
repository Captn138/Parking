prog: main.o car.o
	echo ""> debug.txt
	gcc main.o car.o -o prog
	rm main.o car.o
	./prog

main.o: main.c car.h
	gcc -Wall -c main.c

car.o: car.c car.h
	gcc -Wall -c car.c