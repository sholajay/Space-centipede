#include <stdio.h>  
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "player.h"
#include "console.h"
#include "globals.h"


char* PLAYER_BODY[PLAYER_BODY_ANIM_TILES][PLAYER_HEIGHT] = 
{
    {"[@]","===","/#\\"},
    {"[O]",">=<","/*\\"},
    {"[@]","===","/#\\"},
    {"[O]",">=<","/*\\"},
};

void init_player()
{
    player = (Player *)malloc (sizeof(Player));
    player->col = MID_POINT(GAME_COLS); //start at the middle
    player->row = GAME_ROWS - PLAYER_HEIGHT;
    player->lives = LIVES;
    player->score = ZERO;
}

void move_up()
{
    if(player->row > BOTTOM_WALL + OFFSET)   
    { 
        pthread_mutex_lock(&lock_player);
        player->row += UP; 
        pthread_mutex_unlock(&lock_player);
    }
    //signals the key for next input
    // pthread_cond_signal(&cond_kbd); 
}
void move_down()
{
    if(player->row < (BOTTOM - PLAYER_HEIGHT))
    { 
        pthread_mutex_lock(&lock_player);
        player->row += DOWN; 
        pthread_mutex_unlock(&lock_player);
    }
    //signals the key for next input
    // pthread_cond_signal(&cond_kbd); 
}
void move_left()
{
    if(player->col > LEFT_WALL) 
    { 
        pthread_mutex_lock(&lock_player);
        player->col += LEFT; 
        pthread_mutex_unlock(&lock_player);
    }
    //signals the key for next input
    // pthread_cond_signal(&cond_kbd); 
}
void move_right()
{
    if(player->col < (RIGHT_WALL - PLAYER_WIDHT))
    { 
        pthread_mutex_lock(&lock_player);
        player->col += RIGHT;
        pthread_mutex_unlock(&lock_player);
    }
    //signals the key for next input
    // pthread_cond_signal(&cond_kbd); 
}
void playerRun()
{
    int i = 0;
    char** tile;
    int prevrow = player->row;
    int prevcol = player->col;

    while (!killThreads)
    {
        i++;
        tile = PLAYER_BODY[i % PLAYER_BODY_ANIM_TILES];

        pthread_mutex_lock(&lock_console);
        consoleClearImage(prevrow, prevcol, PLAYER_WIDHT, PLAYER_HEIGHT);
        consoleDrawImage(player->row,player->col, tile, PLAYER_HEIGHT);
        pthread_mutex_unlock(&lock_console);

        prevrow = player->row;
        prevcol = player->col;

        sleepTicks(ANIM_SPEED);
    }
    pthread_exit(NULL);        
}