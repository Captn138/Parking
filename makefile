prog: main.o car.o
	gcc -o prog main.o car.o
	rm main.o car.o
	./prog

main.o: main.c
	gcc -c -Wall main.c -lncurses

car.o: car.c
	gcc -c -Wall car.c