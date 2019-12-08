#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h> 
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>

void ncurses_initialiser(){  //Initialisation of ncurses (given)
  initscr();	        //Starts the ncurses mode
  noecho();             /* Désactive l'affichage des caractères saisis */
  refresh();            /* Met a jour l'affichage */
}

void ncurses_couleurs(){  //Initialisation of the colours (given)
  /* Vérification du support de la couleur */
  if(has_colors() == FALSE) {
    endwin();
    fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
    exit(-1);
  }
  /* Activation des couleurs */
  start_color();
  init_color(COLOR_WHITE, 1000, 1000, 1000);
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);
}

void afficheplan(FILE* f, double topay){
  char c;
  fscanf(f, "%c", &c);
  while(c != EOF){
    if(c == '0'){
      printw("╓");
    }else if(c == '1'){
      printw("╌");
    }else if(c == '2'){
      printw("╖");
    }else if(c == '3'){
      printw("║");
    }else if(c == '4'){
      printw("╟");
    }else if(c == '5'){
      printw("╚");
    }else if(c == '6'){
      printw("═");
    }else if(c == '7'){
      printw("╝");
    }else if(c == '8'){
      printw("╢");
    }else if(c == '9'){
      printw("╫");
    }else if(c == 'w'){
      printw("%.2lf", topay);
    }else{
      printw("%c", c);
    }
    fscanf(f, "%c", &c);
  }
}