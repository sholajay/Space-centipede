#ifndef MY_PLAYER_H
#define MY_PLAYER_H
#include <stdbool.h>

#define PLAYER_BULLET "*"
#define PLAYER_BULLET_OFFSET 1
#define ANIM_SPEED 20
#define MID_POINT(x) (x / 2)
#define PLAYER_BODY_ANIM_TILES 4 
#define OFFSET 1

typedef struct PLAYER Player;
struct PLAYER
{
    int lives;  //number of tries the player has left(0 lives + die = game over)
    int col;    //column position of the player
    int row;    //row position of the player
    int score;  //total number of points achieve in the game
    bool dies;  //true or false(initial) when player get shot
};

void *update_player();
void playerRun();

void init_player();
void move_up();
void move_down();
void move_left();
void move_right();

#endif