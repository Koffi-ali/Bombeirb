/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <SDL_thread.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
//#include <monster.h>
#define NUMBER_MONSTER_MAX 60



struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int bomb_range;
	int life;
	int key;
	int last_dmg;
};

struct map1 {
	struct map* carte1;
	struct player* player;
	struct monster** monster;
	int level;

};

 struct map1 liste1 ;

struct player* player_init(int bombs,int life,int key) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = SOUTH;
	player->bombs = bombs;
	player->life= life;
	player->key=key;
	player->bomb_range=1;
	player->last_dmg=SDL_GetTicks;

	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

// déterminer la position(direction) actuelle du joueur
enum direction player_get_current_way(struct player* player){
	return player->direction;
}

// Fonction sur le nombre de bombes
int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
} 

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}
// détermination, augmentation et diminution du nombre de vies du joueur
int player_get_nb_life(struct player* player) {
	assert(player);
	return player->life;
}

void player_inc_nb_life(struct player* player) {
	assert(player);
	player->life +=1;
}

void player_dec_nb_life(struct player* player) {
	assert(player);
	if (SDL_GetTicks()-(player->last_dmg)>1000){
	player->life -=1;
	player->last_dmg=SDL_GetTicks();
	}
}

// détermination , augmentation,diminution du nombre de clés et de la range du joueur
int player_get_nb_key(struct player* player) {
	assert(player);
	return player->key;
}

void player_inc_nb_key(struct player* player) {
	assert(player);
	player->key +=1;
}
void player_dec_nb_key(struct player* player) {
	assert(player);
	player->key -=1;
 
}
int player_get_bomb_range(struct player* player){
	assert(player);
	return player->bomb_range;
}

void player_inc_bomb_range(struct player* player){
	assert(player);
	player->bomb_range+=1;
}

void player_dec_bomb_range(struct player* player){
	assert(player);
	if (player->bomb_range>=2){
		player->bomb_range-=1;
	}
}
//La fonction d'ajout de bonus
void provide_bonus(struct player* player,struct map* map,int x, int y){
	assert(player);
	switch(map_get_cell_compose_type(map,x,y)){
		case BONUS_BOMB_RANGE_DEC:
			player_dec_bomb_range(player);
			break ;

		case BONUS_BOMB_RANGE_INC:
			player_inc_bomb_range(player);
			break;
 
		case BONUS_BOMB_NB_INC:
			if (player_get_nb_bomb(player)<8){
			player_inc_nb_bomb(player);
			}
			break;

		case BONUS_BOMB_NB_DEC:
			player_dec_nb_bomb(player);
			break;

		case BONUS_LIFE:
			player_inc_nb_life(player);
			break;

		default :
			break;



	}
}



static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))

		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		if(map_get_cell_compose_type(map,x,y)!=SCENERY_PRINCESS){
			return 0;
		}
		
		break;

	case CELL_BOX:

		return 1;
		break;

	case CELL_BONUS:
		return 1;
		break;

	case CELL_MONSTER:
		break;


	default:
		return 1;
		break;
	}

	// Player has moved
	return 1;
}
// Dans cette fonction, on regarde si le joueur peut bouger sur la case, en fonction de si la case est vide, une caisse sans caisse ni monstre derrière (ni en dehors de la map)
//On fournit aussi le bonus associé grâce à la fonction provide bonus
int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
			if (player_move_aux(player, map, x, y - 1)) {
				if ( map_get_cell_type(map, x, y-1)== CELL_BOX){
					if (map_is_inside(map, x, y-2)){
						if (map_get_cell_type(map, x, y-2)== CELL_EMPTY){
							map_set_cell_compose_type(map,x,y-2, CELL_BOX|map_get_cell_compose_type(map,x,y-1));
							map_set_cell_type(map,x,y-1,CELL_EMPTY);
							player->y--;
							move=1;
							}
						}
					}
				else if (map_get_cell_type(map,x,y-1)==CELL_BONUS ){
							provide_bonus(player,map,x,y-1);

							player->y--;
							move=1;
						}
						else if (map_get_cell_type(map,x,y-1)==CELL_KEY){
									player->key++;
									player->y--;
									move=1;
						}
				else {

					player->y--;
					move=1;
					}


			}


			break;


	case SOUTH:

			if (player_move_aux(player, map, x, y + 1)) {
				if ( map_get_cell_type(map, x, y+1)== CELL_BOX){
					if (map_is_inside(map, x, y+2)){
						if (map_get_cell_type(map, x, y+2)== CELL_EMPTY){
							map_set_cell_compose_type(map,x,y+2, CELL_BOX|map_get_cell_compose_type(map,x,y+1));
							map_set_cell_type(map,x,y+1,CELL_EMPTY);
							player->y++;
							move=1;
							}
						}
					}
				else if (map_get_cell_type(map,x,y+1)==CELL_BONUS ){
							provide_bonus(player,map,x,y+1);

							player->y++;
							move=1;
						}
				else if (map_get_cell_type(map,x,y+1)==CELL_KEY){
							player->key++;
							player->y++;
							move=1;
				}

				else {
					player->y++;
					move=1;
					}
			}
			break;



	case WEST:
			if (player_move_aux(player, map, x - 1, y)) {
				if ( map_get_cell_type(map, x-1, y)== CELL_BOX){
					if (map_is_inside(map, x-2, y)){
						if (map_get_cell_type(map, x-2, y)== CELL_EMPTY){
							map_set_cell_compose_type(map,x-2,y, CELL_BOX|map_get_cell_compose_type(map,x-1,y));
							map_set_cell_type(map,x-1,y,CELL_EMPTY);
							player->x--;
							move=1;
							}
						}
					}
					else if (map_get_cell_type(map,x-1,y)==CELL_BONUS ){
							provide_bonus(player,map,x-1,y);

							player->x--;
							move=1;
						}
						else if (map_get_cell_type(map,x-1,y)==CELL_KEY){
									player->key++;
									player->x--;
									move=1;
						}

				else {
					player->x--;
					move=1;
					}
			}
			break;


	case EAST:

			if (player_move_aux(player, map, x + 1, y)) {
				if ( map_get_cell_type(map, x+1, y)== CELL_BOX){
					if (map_is_inside(map, x+2, y)){
						if (map_get_cell_type(map, x+2, y)== CELL_EMPTY){
							map_set_cell_compose_type(map,x+2,y, CELL_BOX|map_get_cell_compose_type(map,x+1,y));
							map_set_cell_type(map,x+1,y,CELL_EMPTY);
							player->x++;
							move=1;
							}
						}
					}
					else if (map_get_cell_type(map,x+1,y)==CELL_BONUS ){
							
							provide_bonus(player,map,x+1,y);

							player->x++  ;
							move=1;
						}
						else if (map_get_cell_type(map,x+1,y)==CELL_KEY){
									player->key++;
									player->x++;
									move=1;
						}
				else {
					player->x++;
					move=1;
					}
			}
			break;
	}

	if (move) {
		if (map_get_cell_type(map, x, y)==CELL_BONUS) {
		    map_set_cell_type(map, x, y, CELL_EMPTY);
		    }
		if (map_get_cell_type(map, x, y)==CELL_KEY) {
		    map_set_cell_type(map, x, y, CELL_EMPTY);
		    }


	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

void bomb_creation(struct map* map,struct player* player,struct monster** monster, int level) {
	liste1.carte1= map;
	liste1.player=player;
	liste1.monster=monster;
	liste1.level=level;
	


	SDL_CreateThread(player_bomb_display, (void*)&liste1);
}

	//on fait apparaitre les flammes dans les 4 directions, le tout limiter par la range, tout en plaçant des restrictions sur le types de case où la flamme peut apparaïtre
	//De plus, on inflige des dégats au joueur s'il se trouve sur une case avec une explosion
	void rec_left_flamme_display( struct map1* args, struct map* carte1,struct player* player,struct map* carte2,int i, int j, int compteur, int range,struct monster** monster, int level){
		if (compteur<range){
			compteur++;
			if( map_is_inside(carte1,i-compteur,j)) {
											map_set_cell_compose_type(carte2,i-compteur,j,map_get_cell_type(carte1,i-compteur,j)|map_get_cell_compose_type(carte1,i-compteur,j));


											switch(map_get_cell_type(carte1, i-compteur,j)) {
											case CELL_BONUS:
															map_set_cell_compose_type(carte1,i-compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i-compteur)  && (player_get_y( player)==j )) {
																							player_dec_nb_life(player);
															}
															rec_left_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);

															break;
											case CELL_MONSTER:
															map_set_cell_compose_type(carte1,i-compteur,j,CELL_BOMB_EXPLOSION);
															rec_left_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_KEY:
															map_set_cell_compose_type(carte1,i-compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i-compteur ) && (player_get_y( player)==j)) {
																							player_dec_nb_life(player);
															}
															rec_left_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_DOOR:
															//map_set_cell_compose_type(carte1,i-compteur,j,CELL_BOMB_EXPLOSION);
															//if ( (player_get_x( player)==i-compteur) && (player_get_y( player)==j)) {
															//								player_dec_nb_life(player);
															//}
															//rec_left_flamme_display( args,carte1,player,carte2,i, j, compteur, range);
															break;
											case CELL_BOX:
															map_set_cell_compose_type(carte1,i-compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i-compteur) && (player_get_y( player)==j)) {
																							player_dec_nb_life(player);
															}
															if(map_get_cell_compose_type(carte2,i-compteur,j) == 0x00) {
	                                                                        map_set_cell_type(carte1,i,j,CELL_EMPTY);
	                                                                }
																	//On regarde si la boite avait un monstre à l'intérieur, si oui, il apparait
																	

															break;

											case CELL_EMPTY:
															map_set_cell_compose_type(carte1,i-compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i-compteur) && (player_get_y( player)==j)) {
																							player_dec_nb_life(player);
															}
															//On regarde si un des monstres est sur la case de l'explosion, si oui, il meurt
															for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																if(monster_get_alive(monster[k]) && monster_get_level(monster[k])==level){
																	if( (monster_get_x(monster[k])==i-compteur) && (monster_get_y( monster[k])==j)) {
																		monster_kill(monster[k]);
																	}
																}
															}
															
															rec_left_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;




											default:
															break;
											}

										}
									}
	}


	void rec_up_flamme_display( struct map1* args, struct map* carte1,struct player* player,struct map* carte2,int i, int j, int compteur, int range,struct monster** monster , int level){
		if (compteur<range){
			compteur++;
			if( map_is_inside(carte1,i,j-compteur)) {
											map_set_cell_compose_type(carte2,i,j-compteur,map_get_cell_type(carte1,i,j-compteur)|map_get_cell_compose_type(carte1,i,j-compteur));


											switch(map_get_cell_type(carte1, i,j-compteur)) {
											case CELL_BONUS:
															map_set_cell_compose_type(carte1,i,j-compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i)  && (player_get_y( player)==j-compteur )) {
																							player_dec_nb_life(player);
															}
															rec_up_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_MONSTER:
															map_set_cell_compose_type(carte1,i,j-compteur,CELL_BOMB_EXPLOSION);
															rec_up_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_KEY:
															map_set_cell_compose_type(carte1,i,j-compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i ) && (player_get_y( player)==j-compteur)) {
																							player_dec_nb_life(player);
															}
															rec_up_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_DOOR:
															//map_set_cell_compose_type(carte1,i,j-compteur,CELL_BOMB_EXPLOSION);
															//if ( (player_get_x( player)==i) && (player_get_y( player)==j-compteur)) {
															//								player_dec_nb_life(player);
															//}
															//rec_up_flamme_display( args,carte1,player,carte2,i, j, compteur, range);
															break;
											case CELL_BOX:
															map_set_cell_compose_type(carte1,i,j-compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i) && (player_get_y( player)==j-compteur)) {
																							player_dec_nb_life(player);
															}
															if(map_get_cell_compose_type(carte2,i,j-compteur)==BONUS_MONSTER){
																		map_set_cell_type(carte1,i,j-compteur,CELL_EMPTY);
																		int a =0;
																		for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																			if(monster_get_alive(monster[k])==0 && a==0 ){
																				a = 1;
																				monster_set_position(monster[k], i, j-compteur, level,2000-100*level);
																				monster_put_alive(monster[k]);
																			}
																	}
																	}
															

																	

	                                                                
															break;

											case CELL_EMPTY:
															map_set_cell_compose_type(carte1,i,j-compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i) && (player_get_y( player)==j-compteur)) {
																							player_dec_nb_life(player);
															}
															for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																if(monster_get_alive(monster[k]) && monster_get_level(monster[k])==level){
																	if( (monster_get_x(monster[k])==i) && (monster_get_y( monster[k])==j-compteur)) {
																		monster_kill(monster[k]);
																	}
																}
															}
														
															
				
															rec_up_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;




											default:
															break;
											}

										}

									}
	}

	void rec_right_flamme_display( struct map1* args, struct map* carte1,struct player* player,struct map* carte2,int i, int j, int compteur, int range, struct monster** monster, int level){
		if (compteur<range){
			compteur++;
			if( map_is_inside(carte1,i+compteur,j)) {
											map_set_cell_compose_type(carte2,i+compteur,j,map_get_cell_type(carte1,i+compteur,j)|map_get_cell_compose_type(carte1,i+compteur,j));


											switch(map_get_cell_type(carte1, i+compteur,j)) {
											case CELL_BONUS:
															map_set_cell_compose_type(carte1,i+compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i+compteur)  && (player_get_y( player)==j )) {
																							player_dec_nb_life(player);
															}
															rec_right_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_MONSTER:
															map_set_cell_compose_type(carte1,i+compteur,j,CELL_BOMB_EXPLOSION);
															rec_right_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_KEY:
															map_set_cell_compose_type(carte1,i+compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i+compteur ) && (player_get_y( player)==j)) {
																							player_dec_nb_life(player);
															}
															rec_right_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_DOOR:
															//map_set_cell_compose_type(carte1,i+compteur,j,CELL_BOMB_EXPLOSION);
															//if ( (player_get_x( player)==i-compteur) && (player_get_y( player)==j)) {
															//								player_dec_nb_life(player);
														//	}
														//	rec_right_flamme_display( args,carte1,player,carte2,i, j, compteur, range);
															break;
											case CELL_BOX:
															map_set_cell_compose_type(carte1,i+compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i+compteur) && (player_get_y( player)==j)) {
																							player_dec_nb_life(player);
															}
															if(map_get_cell_compose_type(carte2,i+compteur,j)==BONUS_MONSTER){
																		map_set_cell_type(carte1,i+compteur,j,CELL_EMPTY);
																		int a =0;
																		for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																			if(monster_get_alive(monster[k])==0 && a==0 ){
																				a = 1;
																				monster_set_position(monster[k], i+compteur, j, level,2000-100*level);
																				monster_put_alive(monster[k]);
																			}
																	}
																	}

															break;

											case CELL_EMPTY:
															map_set_cell_compose_type(carte1,i+compteur,j,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i+compteur) && (player_get_y( player)==j)) {
																							player_dec_nb_life(player);
															}
															for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																if(monster_get_alive(monster[k]) && monster_get_level(monster[k])==level){
																	if( (monster_get_x(monster[k])==i+compteur) && (monster_get_y( monster[k])==j)) {
																		monster_kill(monster[k]);
																	}
																}
															}
															rec_right_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;




											default:
															break;
											}

										}

									}
	}

	void rec_down_flamme_display( struct map1* args, struct map* carte1,struct player* player,struct map* carte2,int i, int j, int compteur, int range, struct  monster ** monster, int level){
		
		if (compteur<range){
			compteur++;
			if( map_is_inside(carte1,i,j+compteur)) {
											map_set_cell_compose_type(carte2,i,j+compteur,map_get_cell_type(carte1,i,j+compteur)|map_get_cell_compose_type(carte1,i,j+compteur));


											switch(map_get_cell_type(carte1, i,j+compteur)) {
											case CELL_BONUS:
															map_set_cell_compose_type(carte1,i,j+compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i)  && (player_get_y( player)==j+compteur )) {
																							player_dec_nb_life(player);
															}
															rec_down_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);

															break;
											case CELL_MONSTER:
															map_set_cell_compose_type(carte1,i,j+compteur,CELL_BOMB_EXPLOSION);
															rec_down_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_KEY:
															map_set_cell_compose_type(carte1,i,j+compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i ) && (player_get_y( player)==j+compteur)) {
																							player_dec_nb_life(player);
															}
															rec_down_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;
											case CELL_DOOR:
														//	map_set_cell_compose_type(carte1,i,j+compteur,CELL_BOMB_EXPLOSION);
															//if ( (player_get_x( player)==i) && (player_get_y( player)==j+compteur)) {
															//								player_dec_nb_life(player);
															//}
															//rec_down_flamme_display( args,carte1,player,carte2,i, j, compteur, range);
															break;
											case CELL_BOX:
															map_set_cell_compose_type(carte1,i,j+compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i) && (player_get_y( player)==j+compteur)) {
																							player_dec_nb_life(player);
															}
															if(map_get_cell_compose_type(carte2,i,j+compteur)==BONUS_MONSTER){
																		map_set_cell_type(carte1,i,j+compteur,CELL_EMPTY);
																		int a =0;
																		for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																			if(monster_get_alive(monster[k])==0 && a==0 ){
																				a = 1;
																				monster_set_position(monster[k], i, j+compteur, level,2000-100*level);
																				monster_put_alive(monster[k]);
																			}
																	}
																	}
															

															break;

											case CELL_EMPTY:
															map_set_cell_compose_type(carte1,i,j+compteur,CELL_BOMB_EXPLOSION);
															if ( (player_get_x( player)==i) && (player_get_y( player)==j+compteur)) {
																							player_dec_nb_life(player);
															}
															for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																if(monster_get_alive(monster[k]) && monster_get_level(monster[k])==level){
																	if( (monster_get_x(monster[k])==i) && (monster_get_y( monster[k])==j+compteur)) {
																		monster_kill(monster[k]);
																	}
																}
															}
															rec_down_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);
															break;




											default:
															break;
											}

										}

									}
	}

	int player_bomb_display(void* arg) {


	        struct map1* args=(struct map1*)arg;
	        struct map* carte1=args->carte1;
	        struct player* player=args->player;
			struct monster** monster= args->monster;
			int level = args-> level;
	        struct map* carte2=map_new(map_get_width(carte1),map_get_height(carte1));
	        assert(carte1);
	        assert(player);
			assert(monster[0]);
			
			
	        int x=player->x;
	        int y=player->y;
					int compteur=0;
					int range=player->bomb_range;



	                if (player_get_nb_bomb(player) >0 ) {
	                          player_dec_nb_bomb(player);
	                                // display de la mèche des bombes

	                                map_set_cell_compose_type(carte1,x,y,CELL_BOMB_TTL4);
	                                SDL_Delay(500);

	                                map_set_cell_compose_type(carte1,x,y,CELL_BOMB_TTL3);
	                                SDL_Delay(500);

	                                map_set_cell_compose_type(carte1,x,y,CELL_BOMB_TTL2);
	                                SDL_Delay(500);

	                                map_set_cell_compose_type(carte1,x,y,CELL_BOMB_TTL1);

	                                SDL_Delay(500);

	                                // apparition des flammes




	                                for (int i=0; i<map_get_width(carte1); i++){
	                                        for (int j=0; j<map_get_height(carte1); j++) {
	                                                if( (map_get_cell_type(carte1, i,j) | map_get_cell_compose_type(carte1,i,j))== CELL_BOMB_TTL1) {
	                                                        map_set_cell_type(carte2,i,j,CELL_EMPTY);
	                                                        map_set_cell_compose_type(carte1,i,j,CELL_BOMB_EXPLOSION);
	                                                        if ( (player_get_x( player)==i) && (player_get_y( player)==j)) {
	                                                                player_dec_nb_life(player);
	                                                        }






	                                                // apparition des flammes dans la direction gauche
													rec_left_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);






	                                                // apparition des flammes dans la direction droite

	                                                rec_right_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster, level);

	                                                // apparition des flammes dans la direction en haut

	                                                rec_up_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster,level);

	                                                // apparition des flammes dans la direction en bas
	                                                rec_down_flamme_display( args,carte1,player,carte2,i, j, compteur, range,monster,level);
	                                        }
	                                }
	                        }





	                                SDL_Delay(200);

	                                // après l'explosion
	                                for (int i=0; i<map_get_width(carte1); i++){
	                                        for (int j=0; j<map_get_height(carte1); j++) {

	                                                if ( (map_get_cell_compose_type(carte1,i,j)|map_get_cell_type(carte1,i,j))==CELL_BOMB_EXPLOSION) {

	                                                        switch(map_get_cell_type(carte2,i,j)) {
	                                                        case CELL_BONUS:
	                                                                map_set_cell_type(carte1,i,j,CELL_EMPTY);
	                                                                break;
	                                                        case CELL_MONSTER:
	                                                                map_set_cell_type(carte1,i,j,CELL_EMPTY);
																	
	                                                                break;
	                                                        case CELL_KEY:
	                                                                map_set_cell_type(carte1,i,j,CELL_KEY);
	                                                                break;
	                                                        case CELL_DOOR:
	                                                                map_set_cell_compose_type(carte1,i,j,CELL_DOOR|map_get_cell_compose_type(carte2,i,j));

	                                                                break;
																	

	                                                        case CELL_EMPTY:
	                                                                map_set_cell_type(carte1,i,j,CELL_EMPTY);
	                                                                break;
	                                                        case CELL_BOX:
															if(map_get_cell_compose_type(carte2,i,j) == 0x00) {
	                                                                        map_set_cell_type(carte1,i,j,CELL_EMPTY);
	                                                                }
																	//On regarde si la boite avait un monstre à l'intérieur, si oui, il apparait
																	else if(map_get_cell_compose_type(carte2,i,j)==BONUS_MONSTER){
																		map_set_cell_type(carte1,i,j,CELL_EMPTY);
																		int a =0;
																		for(int k=0;k<NUMBER_MONSTER_MAX;k++){
																			if(monster_get_alive(monster[k])==0 && a==0 ){
																				a = 1;
																				monster_set_position(monster[k], i, j, level,2000-100*level);
																				monster_put_alive(monster[k]);
																			}
																	}
																	}

																	

	                                                                else {
	                                                                        map_set_cell_compose_type(carte1,i,j,map_get_cell_compose_type(carte2,i,j) | CELL_BONUS);

	                                                                }


	                                                                map_set_cell_compose_type(carte1,i,j,map_get_cell_compose_type(carte2,i,j) | CELL_BONUS);
	                                                                
	                                                                break;
	                                                        case CELL_BOMB:
	                                                                map_set_cell_type(carte1,i,j,CELL_EMPTY);
	                                                                break;

	                                                        default:
	                                                                break;

	                                                        }
	                                                }
	                                }

												}




									player->bombs++;
	                return 0;


	        }
					return 0;
				}
