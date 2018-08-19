#ifndef MY_ENEMY_H
#define MY_ENEMY_H
#include "List.h"
#include <pthread.h>

#define ENEMY_WIDTH 1
#define ENEMY_HEIGHT 2
#define RAND 100
#define SHOOT_NOW 10		//enemy shoots 10% of the time if player is not within range
#define	SHOOT_PLAYER 60		//enemy shoots 40% of the time if player is within range
#define ENEMY_BULLET "."
#define BULLET_OFFSET 1
#define ENEMY_ANIM_TILES 2
#define ENEMY_ANIM_SPEED 40
#define LEN_LIVE_BODY 5		//body long enough to keep enemy alive
#define POINT 5				//each time you hit the enemy score + POINT
#define BODY_LENGTH 60
#define INCREASE_SPEED -5	//each time you break the enemy apart the other half speed increases
#define INT_OVERFLOW 2000000

typedef struct ENEMY Enemy;
struct ENEMY
{
	char** HEAD;
	List *BODY;		
	int col;
	int row;		
    bool dead;		//true or false(initial)
	int speed;		//pace of the animantion and movement(inherited by the body)
	int direction;	//which way the enemy is going(left or right)
    pthread_t thread;
};

//Inherited from the enemy 
typedef struct BODYPART Body_part;
struct BODYPART
{
	char **TAIL;
	int col;
	int row;
	int direction;	//which way the enemy is going(left or right)
	bool onScr;		//to check if the body part has been displayed(check once)
};

//initial the each enemy with a given start point and direction
Enemy *init_enemy(int row, int col, int direction);
void update_enemy(Enemy *uenemy);
void enemyRun();

#endif