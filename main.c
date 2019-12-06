#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "car.h"

double PRICE_PER_TICK = 4.20;

#define clear() printf("\033[H\033[J")
#define gotoxy(lin,col) printf("\033[%d;%dH", (lin), (col))

int** initmatrix(){
	int c;
	FILE* f = fopen("planchiffre.txt", "r");
	int** matrix = malloc(43*sizeof(int*));
	for(int i=0; i<43; i++){
		matrix[i] = malloc(93*sizeof(int));
	}
	for(int i=0; i<43; i++){
		for(int l=0; l<93; l++){
			fscanf(f, "%d", &c);
			matrix[i][l] = c;
		}
	}
	fclose(f);
	return matrix;
}

void printcar(nodec* mycar){
	if(mycar->dir == 0){
		FILE* f = fopen("0", "r");
		char c;
		gotoxy(mycar->lin, mycar->col);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		gotoxy(mycar->lin+1, mycar->col);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fclose(f);
	}else if(mycar->dir == 1){
		FILE* f = fopen("1", "r");
		char c;
		gotoxy(mycar->lin, mycar->col-1);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		gotoxy(mycar->lin+1, mycar->col-1);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fclose(f);
	}else if(mycar->dir == 2){
		FILE* f = fopen("2", "r");
		char c;
		gotoxy(mycar->lin-1, mycar->col-1);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		gotoxy(mycar->lin, mycar->col-1);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fclose(f);
	}else{
		FILE* f = fopen("3", "r");
		char c;
		gotoxy(mycar->lin-1, mycar->col);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		gotoxy(mycar->lin, mycar->col);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fscanf(f, "%c", &c);
		printf("%c", c);
		fclose(f);
	}
}

void afficheplan(FILE* f, double topay){
	clear();
	char c;
	fscanf(f, "%c", &c);
	while(c != EOF){
		printf("%c", c);
		fscanf(f, "%c", &c);
	}
	gotoxy(0, 0);
	printf("%.2lf", topay);
}

void affichematrix(char** matrix){
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

char key_pressed(){
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
	int nbcars = 0;
	int cardelay = 20;
	int i;
	FILE* plan = fopen("plan.txt", "r");
	nodec* tempnode;
	double topay = 0;
	while(key_pressed() != 'q'){
		if(cardelay == 20){
			cars = addnode(cars, startlin, startcol, 2);
			cardelay = 0;
			nbcars++;
		}else{
			cardelay++;
		}
		tempnode = cars;
		afficheplan(plan, topay);
		for(i=0; i<nbcars; i++){
			if(tempnode->status == 0){ //if the car is searching for a parking spot
				cardir(tempnode, matrix);
				carmove(tempnode);
				isparked(tempnode, matrix);
			}else if(tempnode->status == 1){ //if the car is parked
				if(tempnode->t != tempnode->TIME){
					tempnode->t++;
				}else{
					tempnode->dir = (tempnode->dir+2)%4;
					tempnode->status = 2;
				}
			}else if(tempnode->status == 2){ //if the car is leaving
				if(matrix[tempnode->lin][tempnode->col] == 3){
					topay = pay(PRICE_PER_TICK, tempnode);
				}
				carexit(tempnode, matrix);
				carmove(tempnode);
			}
			printcar(tempnode);
			tempnode = tempnode->next;
		}
	}
	fclose(plan);
}

int main(){
	int** matrix = initmatrix();
	nodec* cars = NULL;
	gameon(matrix, 43, 93, cars, 1, 2);
	return 0;
}