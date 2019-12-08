prog: ncursesinstall main.o gui.o car.o
	gcc -o prog main.o gui.o car.o -lncurses
	rm main.o gui.o car.o
	./prog

main.o: main.c
	gcc -c -Wall main.c -lncurses

car.o: car.c
	gcc -c -Wall car.c

gui.o: gui.c
	gcc -c -Wall gui.c -lncurses

ncursesinstall:
	sudo apt-get install libncurses5-dev libncursesw5-dev
	sudo apt autoremove