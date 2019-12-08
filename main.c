#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "car.h"
#include "gui.h"

double PRICE_PER_TICK = 4.20;

//#define clear() printf("\033[H\033[J") //simple function to clear the screen
#define gotoxy(lin,col) printw("\033[%d;%dH", lin, col) //simple function to go to coordinates on the screen

int** initmatrix(){
	//initialises the matrix
	int c;
	FILE* f = fopen("planchiffre.txt", "r");
	int** matrix = malloc(18*sizeof(int*)); // allocate the memory space for the rown
	for(int i=0; i<18; i++){
		matrix[i] = malloc(77*sizeof(int)); //then for the columns
	}
	for(int i=0; i<18; i++){
		for(int l=0; l<77; l++){
			fscanf(f, "%d", &c);
			matrix[i][l] = c; //attribute to the matrix the values stored in the file
		}
	}
	fclose(f);
	return matrix;
}

void printcar(nodec* mycar){
	//depending on the direction of the car, prints it
	if(mycar->dir == 0){
		FILE* f = fopen("0", "r");
		char c;
		gotoxy(mycar->lin, mycar->col);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		gotoxy(mycar->lin+1, mycar->col);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fclose(f);
	}else if(mycar->dir == 1){
		FILE* f = fopen("1", "r");
		char c;
		gotoxy(mycar->lin, mycar->col-1);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		gotoxy(mycar->lin+1, mycar->col-1);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fclose(f);
	}else if(mycar->dir == 2){
		FILE* f = fopen("2", "r");
		char c;
		gotoxy(mycar->lin-1, mycar->col-1);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		gotoxy(mycar->lin, mycar->col-1);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fclose(f);
	}else{
		FILE* f = fopen("3", "r");
		char c;
		gotoxy(mycar->lin-1, mycar->col);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printw("%c", c);
		gotoxy(mycar->lin, mycar->col);
		fscanf(f, "%c", &c);
		printw("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fclose(f);
	}
}

/*void afficheplan(FILE* f, double topay){
	//prints the visual map and the price to pay
	clear();
	char s[200] = "";
	while(strcmp(s, "aaa") != 0){
		printf("%s", s);
		fgets(s, 200, f);
	}
	gotoxy(3, 62);
	printf("%.2lf", topay); //print the amount to pay
}*/

void affichematrix(char** matrix){ //debug function
	printf("%c[2J", 0x1B);
	for(int i=0; i<43; i++){
		for(int l=0; l<93; l++){
			if(matrix[i][l]=='8'){
				printf(" ");
			}else{
				printf("%d", matrix[i][l]);
			}
		}
		printf("\n");
	}
}

char key_pressed(){ //returns 0 or the key pressed
	struct termios oldterm, newterm;
	int oldfd;
	char c, result = 0;
	tcgetattr (STDIN_FILENO, &oldterm);
	newterm = oldterm;
	newterm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
	oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
	c = getchar();
	tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
	fcntl (STDIN_FILENO, F_SETFL, oldfd);
	if (c != EOF) {
		ungetc(c, stdin);
		result = getchar();
	}
	return result;
}

void gameon(int** matrix, int nblin, int nbcol, nodec* cars, int startlin, int startcol){
	int nbcars = 0; //number of cars in the parking
	int cardelay = 20; //variable delay in "ticks" between two cars entering the parking
	FILE* plan = fopen("plan2.txt", "r"); //visual map of the parking
	nodec* tempnode; //temporary list to process the changes
	double topay = 0; //variable to print the amount of money to pay
	while(1){
	sleep(0.1);
		if(cardelay == 20){ //if we reached the delay, fixed here at 20
			cars = addnode(cars, startlin, startcol, 2); //we add a new car in the parking
			cardelay = 0; //reset the delay
			nbcars++; //and increment the counter of cars
		}else{
			cardelay++; //else we just increment the delay
		}
		tempnode = cars;
		//every tick, we will have to print the actualized map with the cars moving
		afficheplan(plan, topay); //we first print the map
		for(int i=0; i<nbcars; i++){ //for every car in the parking,
			if(tempnode->status == 0){ // •if the car is searching for a parking spot
				cardir(tempnode, matrix); //make it find a direction
				carmove(tempnode); //update its position
				isparked(tempnode, matrix); //and check if it is parked
			}else if(tempnode->status == 1){ // •if the car is parked
				if(tempnode->t != tempnode->TIME){ //if the car must wait
					tempnode->t++; //increment the time waited
				}else{
					tempnode->dir = (tempnode->dir+2)%4; //else turn around the car
					tempnode->status = 2; //and changes the status
				}
			}else if(tempnode->status == 2){ // •if the car is leaving
				if(matrix[tempnode->lin][tempnode->col] == 3){ //if the car reached the exit
					topay = pay(PRICE_PER_TICK, tempnode); //make it pay
					removenode(cars, tempnode); //get it out
					nbcars--; //and decrement the counter
				}else{
					carexit(tempnode, matrix); //else make if find the way to the exit
					carmove(tempnode); //and update its position
				}
			}
			printcar(tempnode);
			tempnode = tempnode->next;
		}
	}
	fclose(plan);
}

int main(){
	int** matrix = initmatrix(); // initialise the game matrix
	nodec* cars = NULL; //set the list of cars to NULL
	ncurses_initialiser();
	ncurses_couleurs();
	attron(COLOR_PAIR(1));
	gameon(matrix, 43, 93, cars, 1, 2); //launch the game
	return 0;
}