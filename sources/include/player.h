/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>
//
#include <SDL/SDL.h>
#include <monster.h>

struct player;
struct map1;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number,int life_number,int key);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);
 
// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Get the current position of player
enum direction player_get_current_way(struct player* player);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// (rajouté) Set, Increase, Decrease the number of life that player can have
int  player_get_nb_life(struct player * player);
void player_inc_nb_life(struct player* player);
void player_dec_nb_life(struct player* player);

// (rajouté) Set, Increase, Decrease the number of range that player can have
int player_get_bomb_range(struct player* player);
void player_inc_bomb_range(struct player* player);
void player_dec_bomb_range(struct player* player);

// (rajouté) Set, Increase, Decrease the number of key's player
int  player_get_nb_key(struct player * player);
void player_inc_nb_key(struct player* player);
void player_dec_nb_key(struct player* player);


// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

// Display the bomb's player on the screen

int player_bomb_display(void *arg);

// creation of thread for displaying the bomb

void bomb_creation(struct map* map,struct player* player,struct monster** monster,int level);



#endif /* PLAYER_H_ */
