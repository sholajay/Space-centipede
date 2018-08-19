#include <stdio.h>
#include "console.h"
#include "game.h"
#include "player.h"
#include "game.h"
#include "bullet.h"
#include "enemy.h"
#include "globals.h"

void init_mutex()
{
    pthread_mutex_init(&lock_kbd, NULL);
    pthread_mutex_init(&lock_player, NULL);
    pthread_mutex_init(&lock_console, NULL);

    // cond_kbd = PTHREAD_COND_INITIALIZER;
    pthread_cond_init(&cond_kbd, NULL);
    pthread_cond_init(&cond_score, NULL);
}

void destroy_mutex()
{
    pthread_mutex_destroy(&lock_kbd);
    pthread_mutex_destroy(&lock_player);
    pthread_mutex_destroy(&lock_console);

    pthread_cond_destroy(&cond_kbd);
    pthread_cond_destroy(&cond_score);
}

void create_threads()
{

    if(pthread_create(&kbd_thread, NULL, (void *) &keyPress, NULL))
    {
        printf("Error with keyboard thread\n");
    }

    if(pthread_create(&player_thread, NULL, (void *) &playerRun, NULL))
    {
        printf("Error with Player thread\n");
    }

    if(pthread_create(&refresh_thread, NULL, (void *) &refresh, NULL))
    {
        printf("Error with refresh thread\n");
    }

    if(pthread_create(&score_thread, NULL, (void *) &upkeep, NULL))
    {
        printf("Error with score thread\n");
    }
    if (pthread_create(&enemy_thread, NULL, (void *) &create_enemy, NULL))
    {
        printf("Error with score thread\n");
    }

    killThreads = false;
}

void join_threads()
{
    pthread_join(refresh_thread, NULL);
    pthread_join(player_thread, NULL);
    pthread_join(kbd_thread, NULL);
    pthread_join(score_thread, NULL);
    pthread_join(enemy_thread, NULL);

    //join all enemy
    Enemy *enemy = (Enemy *)list_firstItem(enemy_list);

    while(enemy != NULL)
    {
       pthread_join(enemy->thread, NULL);
       enemy = (Enemy *)list_nextItem(enemy_list);
    }

    //join all bullet
    Bullet *bullet = (Bullet *)list_firstItem(bullet_list);

    while(bullet != NULL)
    {
       pthread_join(bullet->thread, NULL);
       bullet = (Bullet *)list_nextItem(bullet_list);
    }
}