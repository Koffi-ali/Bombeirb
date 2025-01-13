#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
//#include <player.h>
#define NUMBER_MONSTER_MAX 60

struct monster {
	int x, y;
	struct player* player;
	enum direction direction;
	int t0;
	int slowness;
	int Alive;
	int level;
};

struct monster** monster_init(struct player* player,int max_monster) {
	struct monster** monster = malloc(NUMBER_MONSTER_MAX*sizeof(struct monster*));
	if (!monster){
		error("Memory error");}
	//On initialise les monstres en vie sur chaques level
 


	//On initialise les monstres 
for(int i=0; i<max_monster; i++){
 monster[i]=malloc(sizeof(struct monster));

monster[i]->direction = SOUTH;
monster[i]->t0=SDL_GetTicks();
monster[i]->player = player;
monster[i]->Alive=0;
}
	return monster;
}

void monster_free(struct monster** monster) {
	assert(monster);
	free(monster);
}
 
//Set position of the monster
void monster_set_position(struct monster *monster, int x, int y,int level,int slowness) {
	assert(monster);
	monster->x = x;
  monster->y = y;
  monster->level= level;
  monster->slowness = slowness;
  monster->Alive=1;
}
//pour donner vie au monstre, notamment en sortant d'une caisse
void monster_put_alive(struct monster *monster){
	assert(monster);
	monster->Alive=1;
}
//pour tuer le monstre
void monster_kill(struct monster* monster){
		assert(monster);
		monster->Alive=0;
}



// pour avoir le x du monstre
int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}
// pour avoir le y du monstre
int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}
// pour avoir le alive du monstre
int monster_get_alive(struct monster* monster){
	assert(monster!= NULL);
	return monster->Alive;
}

//pour avoir le level du monstre
int monster_get_level(struct monster* monster){
	assert(monster!=NULL);
	return monster->level;
}
// pour tourner le monstre
void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))

		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:

		return 1;
		break;

	case CELL_BONUS:
		break;

	case CELL_MONSTER:
		break;

	default:
		return 1;
		break;
	}

	// Monster has moved
	return 1;
}
// conditions pour que le monstre bouge ; s'il n'a pas de caisse à coté, pas de monstres, pas de player, pas de bonus, pas de portes et de case à coté d'une porte
int monster_move(struct monster* monster, struct map* map, struct monster** tab_monster,int level) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
	int x_player_pos = player_get_x(monster->player);
	int y_player_pos = player_get_y(monster->player);

	switch (monster->direction) {
		case NORTH:
		if(map_is_inside(map,x,y-1)){
			if(map_get_cell_type(map,x,y-1)== CELL_EMPTY){
				if(map_is_inside(map,x,y-2)){
					if(map_get_cell_type(map,x,y-2)==CELL_DOOR){
						move= 0;
						return move;
					}
				}

				if(map_is_inside(map,x-1,y-1)){
					if(map_get_cell_type(map,x-1,y-1)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(map_is_inside(map,x+1,y-1)){
					if(map_get_cell_type(map,x+1,y-1)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(y_player_pos==y-1){
					move =0;
					return move;
				}
				for(int i=0;i<NUMBER_MONSTER_MAX;i++){
					if (y-1==monster_get_y(tab_monster[i]) && x==monster_get_x(tab_monster[i]) && monster_get_alive(tab_monster[i]) && monster_get_level(tab_monster[i])==level ){
						move =0;
						return move;
										}
									}
				monster->y--;
				move = 1;
			}
		}
		else{
			move=0;
		}
							//if(!(map_get_cell_type(map,x-1,y-1)==CELL_DOOR) && !(map_get_cell_type(map,x,y-2)==CELL_DOOR) && !(map_get_cell_type(map,x+1,y-1)==CELL_DOOR)){





				break;


		case SOUTH:
		if(map_is_inside(map,x,y+1)){
			if(map_get_cell_type(map,x,y+1)== CELL_EMPTY){
				if(map_is_inside(map,x,y+2)){
					if(map_get_cell_type(map,x,y+2)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(map_is_inside(map,x-1,y+1)){
					if(map_get_cell_type(map,x-1,y+1)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(map_is_inside(map,x+1,y+1)){
					if(map_get_cell_type(map,x+1,y+1)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(y_player_pos==y+1){
					move =0;
					return move;
				}
				for(int i=0;i<NUMBER_MONSTER_MAX;i++){
					if (y+1==monster_get_y(tab_monster[i]) && x==monster_get_x(tab_monster[i]) && monster_get_alive(tab_monster[i]) && monster_get_level(tab_monster[i])==level){
						move =0;
						return move;
										}
									}

				monster->y++;
				move = 1;
			}
		}
		else{
			move=0;
		}



							//if(!(map_get_cell_type(map,x-1,y+1)==CELL_DOOR) && !(map_get_cell_type(map,x,y+2)==CELL_DOOR) && !(map_get_cell_type(map,x+1,y+1)==CELL_DOOR)){

						break;



		case WEST:
			if(map_is_inside(map,x-1,y)){
				if(map_get_cell_type(map,x-1,y)== CELL_EMPTY){
					if(map_is_inside(map,x-2,y)){
						if(map_get_cell_type(map,x-2,y)==CELL_DOOR){
							move= 0;
							return move;
						}
					}
					if(map_is_inside(map,x-1,y-1)){
						if(map_get_cell_type(map,x-1,y-1)==CELL_DOOR){
							move= 0;
							return move;
						}
					}
					if(map_is_inside(map,x-1,y+1)){
						if(map_get_cell_type(map,x-1,y+1)==CELL_DOOR){
							move= 0;
							return move;
						}
					}
					if(x_player_pos==x-1){
						move =0;
						return move;
					}
					for(int i=0;i<NUMBER_MONSTER_MAX;i++){
						if (y==monster_get_y(tab_monster[i]) && x-1==monster_get_x(tab_monster[i]) && monster_get_alive(tab_monster[i]) && monster_get_level(tab_monster[i])==level){
							move =0;
							return move;
											}
										}
						monster->x--;
						move =1;

				}
			}
			else{
				move=0;
			}
							//if(!(map_get_cell_type(map,x-1,y-1)==CELL_DOOR) && !(map_get_cell_type(map,x-2,y)==CELL_DOOR) && !(map_get_cell_type(map,x-1,y+1)==CELL_DOOR)){

				break;


		case EAST:

		if(map_is_inside(map,x+1,y)){
			if(map_get_cell_type(map,x+1,y)== CELL_EMPTY){
				if(map_is_inside(map,x+2,y)){
					if(map_get_cell_type(map,x+2,y)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(map_is_inside(map,x+1,y-1)){
					if(map_get_cell_type(map,x+1,y-1)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(map_is_inside(map,x+1,y+1)){
					if(map_get_cell_type(map,x+1,y+1)==CELL_DOOR){
						move= 0;
						return move;
					}
				}
				if(x_player_pos==x+1){
					move =0;
					return move;
				}
				for(int i=0;i<NUMBER_MONSTER_MAX;i++){
					if (y==monster_get_y(tab_monster[i]) && x+1==monster_get_x(tab_monster[i]) && monster_get_alive(tab_monster[i]) && monster_get_level(tab_monster[i])==level ){
						move =0;
						return move;
										}
									}
				monster->x++;
				move = 1;
			}
		}
		else{
			move=0;
		}
							//if(!(map_get_cell_type(map,x+1,y-1)==CELL_DOOR) && !(map_get_cell_type(map,x+1,y)==CELL_DOOR) && !(map_get_cell_type(map,x+1,y+1)==CELL_DOOR)){

					break;

	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	} 
	return move;
}
void monster_AI(struct monster* monster, struct map* map, struct monster** tab_monster, struct player* player, int level){
	
	//Ici, le monstre se tourne et tape devant lui, si le joueur est présent
	if (SDL_GetTicks() - monster->t0  > monster->slowness){
		monster->t0=SDL_GetTicks();

		if (monster_get_x(monster)==player_get_x(player)+1 && monster_get_y(monster)==player_get_y(player)){
			monster_set_current_way(monster, WEST);
			player_dec_nb_life(player);
		}
		else if (monster_get_x(monster)==player_get_x(player)-1 && monster_get_y(monster)==player_get_y(player)){
			monster_set_current_way(monster, EAST);
			player_dec_nb_life(player);
		}

		else if(monster_get_x(monster)==player_get_x(player) && monster_get_y(monster)==player_get_y(player)+1){
			monster_set_current_way(monster, NORTH);
			player_dec_nb_life(player);
		}

		else if(monster_get_x(monster)==player_get_x(player) && monster_get_y(monster)==player_get_y(player)-1){
			monster_set_current_way(monster, SOUTH);
			player_dec_nb_life(player);
		}
		else {

		
		//ici, le joueur se déplace dans une direction aléatoire
		int random_direction = rand()%4;

		if (random_direction==0){

			monster_set_current_way(monster, EAST);
			monster_move(monster, map, tab_monster,level);

		}
		if (random_direction==1){
			monster_set_current_way(monster, SOUTH);
			monster_move(monster, map, tab_monster,level);

		}

		if (random_direction==2){
			monster_set_current_way(monster, WEST);
			monster_move(monster, map, tab_monster,level);

		}

		if (random_direction==3){
			monster_set_current_way(monster, NORTH);
			monster_move(monster, map, tab_monster,level);

		}

	}
	}
 

}


void monster_display(struct monster** monster,int level) {
	for (int i =0; i<NUMBER_MONSTER_MAX;i++){
		//on ne display que les monstres en vie
		if(monster_get_alive(monster[i])==1 && monster_get_level(monster[i])==level){
	assert(monster[i]);
	window_display_image(sprite_get_monster(monster[i]->direction),
			monster[i]->x * SIZE_BLOC, monster[i]->y * SIZE_BLOC);
		}
	}
}


