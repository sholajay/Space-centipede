#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include <stdbool.h>
#include "List.h"
#include "player.h"

#define GAME_ROWS 24
#define GAME_COLS 80

//define key press
#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define SHOOT ' '
#define QUIT 'q'

#define	ZERO 0

#define TOP 0           //top edge of the screen
#define TOP_WALL 2      //the centipede wall(=-=-=)
#define	LEFT_WALL 0     //left edge of the screen
#define RIGHT_WALL 80   //right edge of the screen
#define BOTTOM_WALL 16  //the player wall at the bottom(\"\"\)
#define BOTTOM 24       //bottom edge of the screen

pthread_t kbd_thread;
pthread_t player_thread;
pthread_t refresh_thread;
pthread_t score_thread;
pthread_t enemy_thread;

List *bullet_list;
List *enemy_list;
Player *player;

pthread_mutex_t lock_kbd;
pthread_mutex_t lock_player;
pthread_mutex_t lock_console;
pthread_mutex_t lock_enemy;

//signals to create new enemy
pthread_cond_t  cond_enemy; 

//condition variables to synchronize the activities of the keyboard thread
pthread_cond_t  cond_kbd;   

//condition variables that signals for a change in the score board
pthread_cond_t  cond_score;

static bool killThreads;

//player globals
#define LIVES 4
#define PLAYER_HEIGHT 3
#define PLAYER_WIDHT 3

//initialize all mutex lock and condition variable
void init_mutex();

//Clean up all mutex locks and condition variable
void destroy_mutex();

//creates the global threads
void create_threads();

//joins all created threads
void join_threads();

void create_enemy();

void freeze();

#endif