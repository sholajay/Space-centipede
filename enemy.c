#include <stdio.h>  
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "enemy.h"
#include "console.h"
#include "bullet.h"
#include "globals.h"

char* ENEMY_HEAD[ENEMY_ANIM_TILES][ENEMY_HEIGHT] =
{
    {"@@", "><"},
    {"QQ", "=="},
};

char* ENEMY_BODY[ENEMY_ANIM_TILES][ENEMY_HEIGHT] = 
{

    {"Oo", "/\\"},
    {"oO", "()"},
};

void enemy_shoot();
void move_body(Body_part *body);
void move_head(Enemy *enemy);
Enemy *init_head(int row, int col, int direction);
void break_apart(List *body_list, int direction, int row, int col, int at);

//static Enemy *enemy;
void enemyRun(void *enemy_t)
{
    Enemy *enemy = (Enemy *)enemy_t;
    char** head;
    char** tail;
    int i = 0;
    int shoot = 0;
    int prevcol = enemy->row;
    int prevrow = enemy->col;
    Body_part *body;
    int new_enemy = 0;
    
    srand(time(NULL));

    //printf("f2 %d %d\n", enemy->col, enemy->row);
    i = 0;
    while (!killThreads && !enemy->dead)
    {
        new_enemy++;
        freeze(); //if frozen wait
        shoot = rand() % RAND;
         //shoot at some random interval(when shoot is btw Rand and shoot_now)
        if(shoot < SHOOT_NOW || (shoot > SHOOT_PLAYER &&
        enemy->col == player->col))
        { 
            enemy_shoot(enemy->row + BULLET_OFFSET, enemy->col); 
        }

        //head = enemy->HEAD[i % ENEMY_ANIM_TILES];
        head = ENEMY_HEAD[i % ENEMY_ANIM_TILES];
        
        pthread_mutex_lock(&lock_console);

        consoleClearImage(prevrow, prevcol, strlen(head[0]), ENEMY_HEIGHT);
        consoleDrawImage(enemy->row, enemy->col, head, ENEMY_HEIGHT);

        pthread_mutex_unlock(&lock_console);

        //tail = enemy->TAIL[i % ENEMY_ANIM_TILES];
        body = (Body_part*)list_firstItem(enemy->BODY);
        tail =  ENEMY_BODY[i % ENEMY_ANIM_TILES];
        //bool broken = false;
        int count = 0;
        if(new_enemy > GAME_COLS && shoot < SHOOT_NOW)
        {
            pthread_cond_signal(&cond_enemy);
            new_enemy = 0;
        }

        bool hit = false;
        while(body != NULL && !hit)
        {
            prevcol = body->col + ( -body->direction * ENEMY_WIDTH);
            prevrow = body->row;
            count++;

            // Bullet *bullet = get_bullet();
            // if(bullet != NULL && !hit)
            // {
            //     if(bullet->col == body->col && bullet->row == body->row)
            //     {
            //         player->score += POINT;
            //         bullet->hits = true;
            //         hit = true;
            //     }
            // }
            if(hit)
            {
                pthread_mutex_lock(&lock_console);
                consoleClearImage(body->row, body->col, strlen(tail[0]), ENEMY_HEIGHT);
                pthread_mutex_unlock(&lock_console);
            }
            else
            {
                pthread_mutex_lock(&lock_console);

                consoleDrawImage(body->row, body->col, tail, ENEMY_HEIGHT);
                consoleClearImage(prevrow, prevcol, strlen(tail[0]), ENEMY_HEIGHT);
                
                pthread_mutex_unlock(&lock_console);

                move_body(body); 
            }
            body = list_nextItem(enemy->BODY);
        }
        if(hit)
        {
            if(count <= LEN_LIVE_BODY)
            {
                enemy->dead = true;
            }
            break_apart(enemy->BODY, enemy->direction, prevcol, prevcol, count);
            enemy->speed += INCREASE_SPEED;
        }

        if(i == INT_OVERFLOW) { i = 0; } //control integer overflow
        i++;
        sleepTicks(enemy->speed);
        prevcol = enemy->col;
        prevrow = enemy->row;
        move_head(enemy);
    }
    
    pthread_exit(NULL);
}

void break_apart(List *body_list, int direction, int row, int col, int at)
{
    Enemy *enemy;

    enemy = init_head(row, col, direction);
    enemy->BODY = list_unlink(body_list, at+1);

    //create a new enemy thread after break up
    if(pthread_create(&enemy->thread, NULL, (void *) &enemyRun, (void *) enemy))
    {
        printf("Error with Enemy thread\n");
    }

    list_insert(enemy_list, enemy);
}
Enemy *init_enemy(int row, int col, int direction)
{
    Enemy *enemy = init_head(row, col, direction);

    //list containing the list of both parts
    List *body_list = list_create(sizeof(Body_part));

    int i = 0; int j = col;
    Body_part *body = (Body_part *)malloc(sizeof(Body_part));

    //initialize the rest of the enemy(body) with a given length
    while(i < BODY_LENGTH)
    {
        //body->TAIL = ENEMY_BODY;
        j+= ENEMY_WIDTH;
        body->col = j;
        body->row = row;
        body->direction = direction;
        body->onScr  = false;
        //add the body parts to the whole body
        list_insert(body_list, body);
        i++;
    }
    //add the body to the whole enemy
    enemy->BODY = body_list;

    return enemy;
}

Enemy *init_head(int row, int col, int direction)
{
    Enemy *enemy;
    enemy = (Enemy *)malloc(sizeof(Enemy));
    
    //initialize the enemy(head)
    enemy->col = col;
    enemy->row = row;
    enemy->speed = ENEMY_ANIM_SPEED;
    enemy->direction = direction;
    enemy->dead = false;
    //enemy->HEAD = ENEMY_HEAD

    return enemy;
}

void enemy_shoot(int row, int col)
{
    Bullet *bullet;
    bullet = init_bullet(ENEMY_BULLET, row, col, DOWN);

    if(pthread_create(&bullet->thread, NULL, (void *) &bulletRun, (void *) bullet))
    {
        printf("Error with bullet thread\n");
    }
    list_insert(bullet_list, &bullet);
}

//moves the head and the body part in the right direction
void move_body(Body_part *body)
{
    if(body != NULL)
    {
        if(body->onScr)
        {
            if(body->direction == LEFT)
            {
                if(body->col < LEFT_WALL && (body->row + ENEMY_HEIGHT) < BOTTOM_WALL)
                {
                    body->row += ENEMY_HEIGHT; //move down
                    body->direction = RIGHT;   //change direction
                }
            }
            if(body->direction == RIGHT)
            {
                if(body->col >= RIGHT_WALL && (body->row + ENEMY_HEIGHT) < BOTTOM_WALL)
                {
                    body->row += ENEMY_HEIGHT; //move down
                    body->direction = LEFT;   //change direction
                }
            }
        }
        else if(body->col <= RIGHT_WALL)
        {
            body->onScr = true;
        }
        body->col += body->direction;
    }
}
void move_head(Enemy *enemy)
{
    if(enemy->direction == LEFT)
    {
        if(enemy->col < LEFT_WALL && (enemy->row + ENEMY_HEIGHT) < BOTTOM_WALL)
        {
            enemy->row += ENEMY_HEIGHT; //move down
            enemy->direction = RIGHT;   //change direction
        }
    }
    if(enemy->direction == RIGHT)
    {
        if(enemy->col >= RIGHT_WALL && (enemy->row + ENEMY_HEIGHT) < BOTTOM_WALL)
        {
            enemy->row += ENEMY_HEIGHT; //move down
            enemy->direction = LEFT;   //change direction
        }
    }
    enemy->col += enemy->direction;
}