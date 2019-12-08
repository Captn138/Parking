#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "car.h"

double PRICE_PER_TICK = 4.20;
int MAXCARDELAY = 20;
char* RED = "\033[0;31m";
char* RESET = "\033[0m";

#define clear() printf("\033[H\033[J") //simple function to clear the screen
#define gotoxy(lin,col) printf("\033[%d;%dH", lin, col) //simple function to go to coordinates on the screen

void delay(int milliseconds){ //delay function
    long pause;
    clock_t now,then;
    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

int** initmatrix(){
	//initialises the matrix
	char c;
	FILE* f = fopen("planchiffre.txt", "r");
	int** matrix = malloc(18*sizeof(int*)); // allocate the memory space for the rown
	for(int i=0; i<18; i++){
		matrix[i] = malloc(77*sizeof(int)); //then for the columns
	}
	for(int i=0; i<18; i++){
		for(int l=0; l<77; l++){
			fscanf(f, "%c", &c);
			if(c == '\n'){
				fscanf(f, "%c", &c);
				fscanf(f, "%c", &c);
			}
			matrix[i][l] = atoi(&c); //attribute to the matrix the values stored in the file
		}
	}
	//tricky thing
	for(int i=0; i<18; i++){
		matrix[i][0] = 1;
	}
	fclose(f);
	return matrix;
}

void printcar(nodec* mycar){
	//depending on the direction of the car, prints it
	if(mycar->dir == 0){
		gotoxy(mycar->lin+1, mycar->col+1);
		printf("╭╮");
		gotoxy(mycar->lin+2, mycar->col+1);
		printf("└┘");
	}else if(mycar->dir == 1){
		gotoxy(mycar->lin+1, mycar->col);
		printf("┌╮");
		gotoxy(mycar->lin+2, mycar->col);
		printf("└╯");
	}else if(mycar->dir == 2){
		gotoxy(mycar->lin, mycar->col);
		printf("┌┐");
		gotoxy(mycar->lin+1, mycar->col);
		printf("╰╯");
	}else{
		gotoxy(mycar->lin, mycar->col+1);
		printf("╭┐");
		gotoxy(mycar->lin+1, mycar->col+1);
		printf("╰┘");
	}
}

void afficheplan(FILE* f, double topay){
  clear();
  int i=0;
  char c;
  c = fgetc(f);
  while(c != EOF){
    if(c == '0'){
      printf("╓");
    }else if(c == '1'){
      printf("╌");
    }else if(c == '2'){
      printf("╖");
    }else if(c == '3'){
      printf("║");
    }else if(c == '4'){
      printf("╟");
    }else if(c == '5'){
      printf("╚");
    }else if(c == '6'){
      printf("═");
    }else if(c == '7'){
      printf("╝");
    }else if(c == '8'){
      printf("╢");
    }else if(c == '9'){
      printf("╫");
    }else if(c == 'w'){
		if(topay>=100){
			printf("%.2lf", topay);
		}else if(topay>=10){
			printf("%.2lf ", topay);
		}else{
			printf("%.2lf  ", topay);
		}
	}else if(c == 'b'){
		printf("à");
	}else if(c == 'c'){
		printf("€");
    }else{
      printf("%c", c);
    }
    c = fgetc(f);
	i++;
  }
  printf("\n");
}

void affichematrix(int** matrix){ //debug function
	clear();
	for(int i=0; i<18; i++){
		for(int l=0; l<77; l++){
			if(matrix[i][l]==8 || matrix[i][l]==0){
				printf(" ");
			}else{
				printf("%d", matrix[i][l]);
			}
		}
		printf("\n");
	}
}

void gameon(int** matrix, nodec* cars, int startlin, int startcol){
	int nbcars = 0; //number of cars in the parking
	int cardelay = MAXCARDELAY; //variable delay in "ticks" between two cars entering the parking
	nodec* tempnode; //temporary list to process the changes
	double topay = 0; //variable to print the amount of money to pay
	long double totalpay = 0;
	while(1){
	delay(200);
		if(cardelay == MAXCARDELAY){ //if we reached the delay, fixed here at 20
			cars = addnode(cars, startlin, startcol, 2); //we add a new car in the parking
			cardelay = 0; //reset the delay
			nbcars++; //and increment the counter of cars
		}else{
			cardelay++; //else we just increment the delay
		}
		tempnode = cars;
		//every tick, we will have to print the actualized map with the cars moving
		//affichematrix(matrix);
		FILE* plan = fopen("plan2.txt", "r"); //visual map of the parking
		afficheplan(plan, topay); //we first print the map
		fclose(plan);
		for(int i=0; i<nbcars; i++){ //for every car in the parking,
			if(tempnode->status == 0){ // •if the car is searching for a parking spot
				cardir(tempnode, matrix); //make it find a direction
				carmove(tempnode); //update its position
				isparked(tempnode, matrix); //and check if it is parked
			}else if(tempnode->status == 1){ // •if the car is parked
				if(tempnode->dir == 0){
					gotoxy(tempnode->lin, tempnode->col);
					printf("%s╌╌╌%s", RED, RESET);
				}else if(tempnode->dir == 2){
					gotoxy(tempnode->lin+2, tempnode->col);
					printf("%s╌╌╌%s", RED, RESET);
				}
				if(tempnode->t != tempnode->TIME){ //if the car must wait
					tempnode->t++; //increment the time waited
				}else{
					tempnode->dir = (tempnode->dir+2)%4; //else turn around the car
					tempnode->status = 2; //and changes the status
				}
			}else if(tempnode->status == 2){ // •if the car is leaving
				if(tempnode->lin == 6 && tempnode->col == 74){
					tempnode->dir = 0;
					matrix[tempnode->lin-1][tempnode->col] = 2;
					matrix[tempnode->lin][tempnode->col] = 0;
					carmove(tempnode);
				}else if(tempnode->lin == 1 && tempnode->col == 74){ //if the car reached the exit
					topay = pay(PRICE_PER_TICK, tempnode); //make it pay
					totalpay += topay;
					matrix[1][74] = 0;
					nodec* anothertemp = tempnode->next;
					cars = removenode(cars, tempnode); //get it out
					nbcars--; //and decrement the counter
					tempnode = anothertemp;
					goto balise;
				}else{
					carexit(tempnode, matrix); //else make if find the way to the exit
					carmove(tempnode); //and update its position
				}
			}
			printcar(tempnode);
			tempnode = tempnode->next;
			balise:
			gotoxy(19,0);
			printf("Ttoal money earned: %.2Lf €\n", totalpay);
		}
	}
}

int main(){
	srand(time(NULL));
	int** matrix = initmatrix(); // initialise the game matrix
	nodec* cars = NULL; //set the list of cars to NULL
	gameon(matrix, cars, 1, 2); //launch the game
	return 0;
}