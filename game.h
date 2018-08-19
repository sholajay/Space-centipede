#ifndef GAME_H
#define GAME_H

#define REFRESH_RATE 1
#define SCORE_POS 26
#define LIVES_POS 42
#define DEAD "0"
#define FROZEN_TIME 100

#define INT_SIZE 16
#define BANNER1 "YOU QUIT!\n\t\t\t\tLOSSER" 
#define BANNER2 "GAME OVER!\n\t\t\t\tYOU ARE DEAD"
void gameRun();

void upkeep();
void keyPress();
void refresh();

#endif
