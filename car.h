typedef struct nodec{
    struct nodec* next; //next car
    int lin; //y pos
    int col; //x pos
    int dir; //0:up, 1:right, 2:down, 3:left
    int t; //counter of the time the cas has been parked
    int TIME; //random variable of the time the car will stay parked
    int status; //0:searching, 1:parked, 2:leaving
} nodec;

nodec* addnode(nodec* list, int x, int y, int dir);
nodec* removenode(nodec* list, nodec* mynode);
void cardir(nodec* mynode, int** board);
void carexit(nodec* mynode, int** board);
void carmove(nodec* mynode);
void isparked(nodec* mynode, int** board);
double pay(double price, nodec* mynode);