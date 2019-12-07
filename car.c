#include<stdio.h>
#include<stdlib.h>
#include "car.h"

nodec* addnode(nodec* list, int lin, int col, int dir){
    //creates a new node and adds it to the list
    //returns the list
    nodec* newnode = malloc(sizeof(nodec));
    newnode->next = NULL;
    newnode->lin = lin;
    newnode->col = col;
    newnode->dir = dir;
    newnode->t = 0;
    newnode->TIME = (rand()%90)+10;
    newnode->status = 0;
    nodec* temp = list;
    if(list == NULL){
        return newnode;
    }
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = newnode;
    return list;
}

nodec* removenode(nodec* list, nodec* mynode){
    //removes a node
    //returns the list
    nodec* temp = list;
    if(list == mynode){
        temp = mynode->next;
        free(mynode);
        return temp;
    }
    while(temp->next != NULL && temp->next != mynode){
        temp = temp->next;
    }
    temp->next = mynode->next;
    free(mynode);
    return list;
}

void cardir(nodec* mynode, int** matrix){
    //chooses a direction for a car (parking mode)
    //0 up, 1 right, 2 down, 3 left
    int flag = 0;
    int tempdir = rand()%4; //chooses a random direction
    int lin = mynode->lin;
    int col = mynode->col;
    while(flag == 0){ //while the direction is not approved
        tempdir = (tempdir+1)%4;
        if(tempdir == 0 && tempdir != (mynode->dir+2)%4 && matrix[lin-1][col] == 0 && matrix[lin-3][col] != 2 && matrix[lin-2][col] != 2 && matrix[lin-1][col] != 2){
            //if tempdir=up and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases
            flag = 1; //say we approve this direction
            mynode->dir = tempdir; //change the actual direction of the car
            matrix[lin-1][col] = 2; //place the car in the matrix for the mext position
            if(lin == 5 && col == 2){ //actualise the old position with 0, 4 (entrance only) or 5 (exit only)
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }else if(tempdir == 1 && tempdir != (mynode->dir+2)%4 && matrix[lin][col+1] == 0 && matrix[lin][col+3] != 2 && matrix[lin][col+2] != 2 && matrix[lin][col+1] != 2){
            //if tempdir=right and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases
            flag = 1;
            mynode->dir = tempdir;
            matrix[lin][col+1] = 2;
            if(lin == 5 && col == 2){
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }else if(tempdir == 2 && tempdir != (mynode->dir+2)%4 && matrix[lin+1][col] == 0 && matrix[lin+3][col] != 2 && matrix[lin+2][col] != 2 && matrix[lin+1][col] != 2){
            //if tempdir=down and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases
            flag = 1;
            mynode->dir = tempdir;
            matrix[lin+1][col] = 2;
            if(lin == 5 && col == 2){
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }else if(tempdir == 3 && tempdir != (mynode->dir+2)%4 && (matrix[lin][col-1] == 0 || matrix[lin][col-1] == 4) && matrix[lin][col-3] != 2 && matrix[lin][col-2] != 2 && matrix[lin][col-1] != 2){
            //if tempdir=left and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases
            flag = 1;
            mynode->dir = tempdir;
            matrix[lin][col-1] = 2;
            if(lin == 5 && col == 2){
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }
    }
}

void carexit(nodec* mynode, int** matrix){
    //chooses a direction for a car (exit mode)
    //0 up, 1 right, 2 down, 3 left
    int flag = 0;
    int tempdir = rand()%4; //chooses a random direction
    int lin = mynode->lin;
    int col = mynode->col;
    while(flag == 0){ //while the direction is not approved
        tempdir = (tempdir+1)%4;
        if(tempdir == 0 && tempdir != (mynode->dir+2)%4 && (matrix[lin-1][col] == 0 || matrix[lin-1][col] == 5) && matrix[lin-3][col] != 2 && matrix[lin-2][col] != 2 && matrix[lin-1][col] != 2 && matrix[lin-3][col] != 1){
            //if tempdir=up and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases and there is no wall 3 cases later
            flag = 1; //say we approve the direction
            mynode->dir = tempdir; //change the actual direction of the car
            matrix[lin-1][col] = 2; //place the car in the matrix for the mext position
            if(lin == 5 && col == 2){ //actualise the old position with 0, 4 (entrance only) or 5 (exit only)
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }else if(tempdir == 1 && tempdir != (mynode->dir+2)%4 && matrix[lin][col+1] == 0 && matrix[lin][col+3] != 2 && matrix[lin][col+2] != 2 && matrix[lin][col+1] != 2 && matrix[lin][col+3] != 1){
            //if tempdir=up and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases and there is no wall 3 cases later
            flag = 1;
            mynode->dir = tempdir;
            matrix[lin][col+1] = 2;
            if(lin == 5 && col == 2){
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }else if(tempdir == 2 && tempdir != (mynode->dir+2)%4 && matrix[lin+1][col] == 0 && matrix[lin+3][col] != 2 && matrix[lin+2][col] != 2 && matrix[lin+1][col] != 2 && matrix[lin+3][col] != 1){
            //if tempdir=up and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases and there is no wall 3 cases later
            flag = 1;
            mynode->dir = tempdir;
            matrix[lin+1][col] = 2;
            if(lin == 5 && col == 2){
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }else if(tempdir == 3 && tempdir != (mynode->dir+2)%4 && matrix[lin][col-1] == 0 && matrix[lin][col-3] != 2 && matrix[lin][col-2] != 2 && matrix[lin][col-1] != 2 && matrix[lin][col-3] != 1){
            //if tempdir=up and the car won't do a 180° and there in a path in this direction and there is no car in the 3 next cases and there is no wall 3 cases later
            flag = 1;
            mynode->dir = tempdir;
            matrix[lin][col-1] = 2;
            if(lin == 5 && col == 2){
                matrix[lin][col] = 4;
            }else if(lin == 5 && col == 74){
                matrix[lin][col] = 5;
            }else{
                matrix[lin][col] = 0;
            }
        }
    }
}

void carmove(nodec* mynode){
    if(mynode->dir == 0){
        mynode->lin--;
    }else if(mynode->dir == 1){
        mynode->col++;
    }else if(mynode->dir == 2){
        mynode->lin++;
    }else{
        mynode->col--;
    }
}

void isparked(nodec* mynode, int** matrix){
    //changes the status to 1 if the car is parked
    //doesnt change it otherwise
    if(mynode->dir == 0){
        if(matrix[mynode->lin-1][mynode->col] == 1){
            mynode->status = 1;
        }
    }else if(mynode->dir == 1){
        if(matrix[mynode->lin][mynode->col+1] == 1){
            mynode->status = 1;
        }
    }else if(mynode->dir == 2){
        if(matrix[mynode->lin+1][mynode->col] == 1){
            mynode->status = 1;
        }
    }else{
        if(matrix[mynode->lin][mynode->col-1] == 1){
            mynode->status = 1;
        }
    }
}

double pay(double price, nodec* mynode){
    return price*mynode->t;
}