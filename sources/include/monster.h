#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>
#include <player.h>

struct monster;
 
// init a monster
struct monster** monster_init(struct player* player, int max_monster);
void   monster_free(struct monster** monster);

// Set the position of the monster
void monster_set_position(struct monster *monster, int x, int y, int level, int slowness);

// Return the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

int monster_get_alive(struct monster* monster);
int monster_get_level(struct monster* monster);

 
// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);

// Move the monster according to the current direction
int monster_move(struct monster* monster, struct map* map, struct monster** tab_monster,int level);

// Display the monster on the screen
void monster_display(struct monster** monster, int level);

// Module d'IA du monstre
void monster_AI(struct monster* monster, struct map* map, struct monster ** tab_monster, struct player* player,int level);

//On tue le monstre
void monster_kill(struct monster* monster);

//On donne vie au monstre
void monster_put_alive(struct monster *monster);


#endif /* MONSTER_H_ */
