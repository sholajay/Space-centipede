#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bullet.h"
#include "console.h"
#include "globals.h"
#include "player.h"

bool check_hit(int row, int col);

static Bullet *b;

Bullet *init_bullet(char *head, int row, int col, int direction)
{
    Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));

    //initialize the bullet
    bullet->head[0] = head;
    bullet->col = col;
    bullet->row = row;
    bullet->hits = false;
    bullet->speed = BULLET_SPEED;
    bullet->direction = direction;

    return bullet; 
}

void update_bullet(Bullet * bullet)
{
    b = bullet;
}
Bullet *get_bullet()
{
    return b;
}
void bulletRun(void *bullet_t)
{
    Bullet *bullet = (Bullet *)bullet_t;

    while (!killThreads && !bullet->hits)
    { 
        if (bullet->row > TOP_WALL && bullet->row < BOTTOM)
        {
            pthread_mutex_lock(&lock_console);
            consoleClearImage(bullet->row, bullet->col, BULLET_SIZE, BULLET_SIZE);
            bullet->row += bullet->direction;
            consoleDrawImage(bullet->row, bullet->col, bullet->head, BULLET_SIZE);
            pthread_mutex_unlock(&lock_console);

            if(bullet->direction == DOWN)
            {
                if( bullet->row == player->row  && bullet->col >= player->col
                 && bullet->col < player->col + PLAYER_WIDHT)
                {
                    bullet->hits = true;
                    pthread_mutex_lock(&lock_player);
                    player->dies = true;
                    pthread_mutex_unlock(&lock_player);
                }
            }
            else if (bullet->direction == UP && !bullet->hits)
            {
                update_bullet(bullet);
            }
        }
        else
        {
            pthread_mutex_lock(&lock_console);
            consoleClearImage(bullet->row, bullet->col, BULLET_SIZE, BULLET_SIZE);
            pthread_mutex_unlock(&lock_console);
        	bullet->hits = true;
        }
       
        sleepTicks(bullet->speed);
    }
 
    pthread_exit(NULL);        
}