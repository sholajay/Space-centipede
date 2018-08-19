#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

#include "console.h"
#include "game.h"
#include "player.h"
#include "game.h"
#include "bullet.h"
#include "enemy.h"
#include "globals.h"

/**** DIMENSIONS MUST MATCH the ROWS/COLS */
char *GAME_BOARD[] = {
  "                   Score:          Lives:",
  "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-centipiede!=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
  "",
  "",
  "",
  "",
  "",
  "", 
  "" };

//static Player *player;
pthread_cond_t cond_done;
pthread_mutex_t finished;

void keyPress();
void upkeep();
void refresh();
void create_bullet();
void end_game(char *str);
void create_enemy();
void player_shoot(int row, int col);
void reset();
void freeze();
void cleanUP();

static bool frozen = false;

void gameRun()
{

    init_player();
    bullet_list = list_create(sizeof(Bullet));
    
    init_mutex();

    if (consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD))
    {

        pthread_cond_init(&cond_done, NULL);
        create_threads();
        
        pthread_mutex_lock(&finished);

        pthread_cond_wait(&cond_done, &finished);
        pthread_mutex_unlock(&finished);
        
        destroy_mutex();
        //join_threads();
        cleanUP();

        pthread_mutex_destroy(&finished);
        pthread_cond_destroy(&cond_done);

         /* wait for final key before killing curses and game */  
        finalKeypress();
        consoleFinish();
    }       
}
void cleanUP()
{
    list_destroy(bullet_list);
    list_destroy(enemy_list);
    free(player);
}

int input_timeout (int filedes, unsigned int seconds)
{
    fd_set set; /* what to check for our select call */
    struct timeval timeout;


    /* Initialize the file descriptor set. */
    FD_ZERO (&set);
    FD_SET (filedes, &set);

    /* Initialize the timeout data structure. */
    timeout.tv_sec = seconds;
    timeout.tv_usec = ZERO;

    /* select returns 0 if timeout, 1 if input available, -1 if error. */
    return  (select (FD_SETSIZE,  &set, NULL, NULL, &timeout)); ;
}

void keyPress()
{
    char ch;
    while(!killThreads)
    {
        pthread_mutex_lock(&lock_kbd); //lock the keyboard input

        if(input_timeout (STDIN_FILENO, 1))/* duration of one tick */
        {
            ch = getchar();

            if(ch == MOVE_UP)   { move_up(); }
            if(ch == MOVE_DOWN) { move_down(); }
            if(ch == MOVE_LEFT) { move_left(); }
            if(ch == MOVE_RIGHT){ move_right(); }
            if(ch == QUIT) { end_game(BANNER1); }
            if(ch == SHOOT) 
            {
                player_shoot(player->row, player->col + PLAYER_BULLET_OFFSET);
            }
        }
        pthread_mutex_unlock(&lock_kbd); //unlock keyboard when signalled 
    }
    pthread_exit(NULL); 
}

void upkeep()
{
    char *lives =  (char *) malloc(INT_SIZE);
    char *score = (char *) malloc(INT_SIZE);

    int i = 0;
    int prev_score = player->score;
    sprintf(lives, "%d", player->lives);
    sprintf(score, "%d", player->score);

    pthread_mutex_lock(&lock_console);
    putString(lives, ZERO, LIVES_POS, strlen(lives));
    putString(score, ZERO , SCORE_POS, strlen(score));
    pthread_mutex_unlock(&lock_console);

    while(!killThreads)
    {
        if(prev_score != player->score)
        {
            sprintf(score, "%d", player->score);
            pthread_mutex_lock(&lock_console);
            putString(score, ZERO , SCORE_POS, strlen(score));
            pthread_mutex_unlock(&lock_console);
            prev_score = player->score;
        }
        if(player->dies)
        {
            player->lives--;
            if(player->lives > 0)
            {
                reset();
                player->dies = false;
                sprintf(lives, "%d", player->lives);
              
                pthread_mutex_lock(&lock_console);
                putString(lives, ZERO, LIVES_POS, strlen(lives));
                putString(score, ZERO , SCORE_POS, strlen(score));
                pthread_mutex_unlock(&lock_console);
            }
            else
            {
                end_game(BANNER2);
            }
            i++;
        }
    }       
    pthread_exit(NULL); 
}
void freeze()
{
    if(frozen)
    { 
        disableConsole(true);
        sleepTicks(FROZEN_TIME);
        disableConsole(false);
        frozen = false;
    }
}
void reset()
{
    pthread_mutex_lock(&lock_console);
    consoleClearImage(TOP, LEFT_WALL, GAME_ROWS, GAME_COLS);
    pthread_mutex_unlock(&lock_console);

    frozen = true;
    player->col = MID_POINT(GAME_COLS); //start at the middle
    player->row = GAME_ROWS - PLAYER_HEIGHT;

    consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);
}
void refresh()
{
    while(!killThreads)
    {
        pthread_mutex_lock(&lock_console);
        consoleRefresh();
        pthread_mutex_unlock(&lock_console);
        sleepTicks(REFRESH_RATE);
    }
    pthread_exit(NULL); 
}

void create_enemy()
{
    enemy_list = list_create(sizeof(Enemy));
    Enemy *enemy;

    while(!killThreads)
    {
        enemy = init_enemy(TOP_WALL, GAME_COLS - ENEMY_WIDTH, LEFT);
        if(pthread_create(&enemy->thread, NULL, (void *) &enemyRun, (void *) enemy))
        {
            printf("Error with Enemy thread\n");
        }
        list_insert(enemy_list, enemy);
        pthread_mutex_lock(&lock_enemy);
        pthread_cond_wait(&cond_enemy, &lock_enemy);
        pthread_mutex_unlock(&lock_enemy);
    }
    pthread_exit(NULL);
}
void player_shoot(int row, int col)
{
    Bullet *bullet;
    bullet = init_bullet(PLAYER_BULLET, row, col, UP);

    if(pthread_create(&bullet->thread, NULL, (void *) &bulletRun, (void *) bullet))
    {
        printf("Error with bullet thread\n");
    }

    list_insert(bullet_list, &bullet);
    player->score++; 
}

void end_game(char *msg)
{
    killThreads = true;

    pthread_mutex_lock(&lock_console);
    putBanner(msg);
    pthread_mutex_unlock(&lock_console);

    pthread_cond_signal(&cond_done);
}