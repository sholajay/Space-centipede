#ifndef MY_BULLET_H
#define MY_BULLET_H
#include <pthread.h>

#define BULLET_SPEED 4
#define BULLET_UP_LIMIT 2
#define BULLET_SIZE 1

typedef struct BULLET Bullet;
struct BULLET
{
	char *head[BULLET_SIZE];
	int col;
	int row;
	int direction;
	pthread_t thread;
	int speed;
    int hits;
};
void bulletRun();

//creates a bullet give the starting position and the direction to go
Bullet *init_bullet(char *head, int row, int col, int direction);
 Bullet *get_bullet();

#endif