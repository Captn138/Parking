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

void cardir(nodec* mynode, int** board){
    //chooses a direction for a car (parking mode)
    //0 up, 1 right, 2 down, 3 left
    int flag = 0;
    int tempdir = rand()%4;
    int lin = mynode->lin;
    int col = mynode->col;
    while(flag == 0){
        tempdir = (tempdir+1)%4;
        if(tempdir == 0 && tempdir != (mynode->dir+2)%4 && board[lin-1][col] == 0 && board[lin-5][col] != 2){
            flag = 1;
            mynode->dir = tempdir;
        }else if(tempdir == 1 && tempdir != (mynode->dir+2)%4 && board[lin][col+1] == 0 && board[lin][col+5] != 2){
            flag = 1;
            mynode->dir = tempdir;
        }else if(tempdir == 2 && tempdir != (mynode->dir+2)%4 && board[lin+1][col] == 0 && board[lin+5][col] != 2){
            flag = 1;
            mynode->dir = tempdir;
        }else if(tempdir == 3 && tempdir != (mynode->dir+2)%4 && board[lin][col-1] == 0 && board[lin][col-5] != 2){
            flag = 1;
            mynode->dir = tempdir;
        }
    }
}

void carexit(nodec* mynode, int** board){
    //chooses a direction for a car (exit mode)
    //0 up, 1 right, 2 down, 3 left
    int flag = 0;
    int tempdir = rand()%4;
    int lin = mynode->lin;
    int col = mynode->col;
    while(flag == 0){
        tempdir = (tempdir+1)%4;
        if(tempdir == 0 && tempdir != (mynode->dir+2)%4 && board[lin-1][col] == 0 && board[lin-5][col] != 2 && board[lin-5][col] != 1){
            flag = 1;
            mynode->dir = tempdir;
        }else if(tempdir == 1 && tempdir != (mynode->dir+2)%4 && board[lin][col+1] == 0 && board[lin][col+5] != 2 && board[lin][col+5] != 1){
            flag = 1;
            mynode->dir = tempdir;
        }else if(tempdir == 2 && tempdir != (mynode->dir+2)%4 && board[lin+1][col] == 0 && board[lin+5][col] != 2 && board[lin+5][col] != 1){
            flag = 1;
            mynode->dir = tempdir;
        }else if(tempdir == 3 && tempdir != (mynode->dir+2)%4 && board[lin][col-1] == 0 && board[lin][col-5] != 2 && board[lin][col-5] != 1){
            flag = 1;
            mynode->dir = tempdir;
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

void isparked(nodec* mynode, int** board){
    //returns 1 if the car is parked
    //returns 0 otherwise
    if(mynode->dir == 0){
        if(board[mynode->lin-1][mynode->col] == 1){
            mynode->status = 1;
        }
    }else if(mynode->dir == 1){
        if(board[mynode->lin][mynode->col+1] == 1){
            mynode->status = 1;
        }
    }else if(mynode->dir == 2){
        if(board[mynode->lin+1][mynode->col] == 1){
            mynode->status = 1;
        }
    }else{
        if(board[mynode->lin][mynode->col-1] == 1){
            mynode->status = 1;
        }
    }
}

double pay(double price, nodec* mynode){
    return price*mynode->t;
}