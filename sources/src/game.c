/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/

#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
//
#include <SDL/SDL_image.h>

#define LEVELS 8
#define NUMBER_MONSTER_MAX 60






struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster** monster;
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	int levels;
	FILE *fp=fopen("./src/data/partie","r");
	fscanf(fp,"%i",&levels);

	game->levels=levels;
	int level;
	fscanf(fp,"%i:",&level);
	game->level=level;
	int x,y;
	fscanf(fp,"%i,%i",&x,&y);

	char s1[60];
	fscanf(fp,"%s",s1);

	int n=strlen(s1);



	// on précise que LEVELS vaut 8(car il y a 8 niveaux maximum)

	char **s=malloc(LEVELS*sizeof(char *));
	for(int i=0;i<LEVELS;i++){
		s[i]=malloc((n+1)*sizeof(char));
	}
	for (int i=0;i<LEVELS;i++) {
		for(int j=0;j<n;j++){
			s[i][j]=s1[j];
		}
		s[i][n]='\0';
	}




	game->maps = malloc(levels*sizeof(struct map*));
	

	// intégrer les différents mondes


	for (int i=0;i<levels;i++) {
		if (i==0) {
			char lien1[100]="./src/map/";
			char *lien2=s[0];
			char *lien3=strcat(lien2,"_0");
			game->maps[i] = map_loading(strcat(lien1,lien3));

		}
		if (i==1) {
			char lien1[100]="./src/map/";
			char *lien2=s[1];
			char *lien3=strcat(lien2,"_1");
			game->maps[i] = map_loading(strcat(lien1,lien3));
		}
		if (i==2) {
			char lien1[100]="./src/map/";
			char *lien2=s[2];
			char *lien3=strcat(lien2,"_2");
			game->maps[i] = map_loading(strcat(lien1,lien3));

		}
		if (i==3) {
			char lien1[100]="./src/map/";
			char *lien2=s[3];
			char *lien3=strcat(lien2,"_3");
			game->maps[i] = map_loading(strcat(lien1,lien3));
		}
		if (i==4) {
			char lien1[100]="./src/map/";
			char *lien2=s[4];
			char *lien3=strcat(lien2,"_4");
			game->maps[i] = map_loading(strcat(lien1,lien3));
		}
		if (i==5) {
			char lien1[100]="./src/map/";
			char *lien2=s[5];
			char *lien3=strcat(lien2,"_5");
			game->maps[i] = map_loading(strcat(lien1,lien3));
		}
		if (i==6) {
			char lien1[100]="./src/map/";
			char *lien2=s[6];
			char *lien3=strcat(lien2,"_6");
			game->maps[i] = map_loading(strcat(lien1,lien3));
		}
		if (i==7) {
			char lien1[100]="./src/map/";
			char *lien2=s[7];
			char *lien3=strcat(lien2,"_7");
			game->maps[i] = map_loading(strcat(lien1,lien3));
		}

	}
 
	game->player = player_init(3,3,0);

	// Set default location of the player
	player_set_position(game->player, x, y);
	// On initialise le tableau de monstre
	game->monster = monster_init(game->player,NUMBER_MONSTER_MAX);
	// Set default locaation and of monster depending on level

	 
	
	monster_set_position(game->monster[3],9,2,0,2200);
	//Monstre initiaux au niveau 1
	monster_set_position(game->monster[4], 1, 8,0,2300);
	monster_set_position(game->monster[5], 2, 8,0,2300);
	monster_set_position(game->monster[6], 0, 8,0,2300);
	monster_set_position(game->monster[7], 1, 9,0,2300);
	monster_set_position(game->monster[8], 2, 9,0,2300);
	monster_set_position(game->monster[9], 0, 9,0,2300);

	//Monstres initiaux au niveau 2
	monster_set_position(game->monster[10], 3, 0,1,3000);
	monster_set_position(game->monster[11], 5, 0,1,2900);
	monster_set_position(game->monster[12], 7, 0,1,2950);
	monster_set_position(game->monster[13],9,0,1,2950);
	monster_set_position(game->monster[14], 3, 4,1,3000);
	monster_set_position(game->monster[15], 5, 4,1,2900);
	monster_set_position(game->monster[16], 7, 4,1,2950);
	monster_set_position(game->monster[17],9,4,1,2950);


	monster_set_position(game->monster[18],11,1,2,1950);
	monster_set_position(game->monster[19],11,2,2,1925);
	monster_set_position(game->monster[20],11,3,2,1900);
	monster_set_position(game->monster[21],14,1,2,1920);
	monster_set_position(game->monster[22],14,2,2,1930);
	monster_set_position(game->monster[23],14,3,2,1940);
	monster_set_position(game->monster[24],13,1,2,1910);
	monster_set_position(game->monster[25],13,2,2,1900);
	monster_set_position(game->monster[26],13,3,2,1850);
	monster_set_position(game->monster[27],12,1,2,1840);
	monster_set_position(game->monster[28],12,2,2,1870);
	monster_set_position(game->monster[29],12,3,2,1880);
	
		//game->maps[0]=map_get_static();




	return game;
}


void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	monster_free(game->monster);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct monster** game_get_monster(struct game* game) {
	assert(game);
	return game->monster;
}

int game_get_level(struct game* game){
	assert(game);
	return game->level;
}


void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 10* SIZE_BLOC) / 6;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);

	// rajouté
	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_key(game_get_player(game))), x, y);

	x=5 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_banner_flag(), x, y);

	x=5 * white_bloc + 9 * SIZE_BLOC;
	window_display_image(sprite_get_number(game->level), x, y);




}
 // changement de map lorsqu'on traverse une porte
void  game_change_level(struct game* game) {
	assert(game);
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int x=player_get_x(player);
	int y=player_get_y(player);
	int level=door_level_reachable(map,x,y);

	if ((map_get_cell_type(map,x,y)==CELL_DOOR) && (player_get_current_way(player)==NORTH)){

		switch(map_get_cell_compose_type(map,x,y) & 0x01) { // on recupère le dernier bit
			case DOOR_OPENED:
				game->level=level;
				map=game_get_current_map(game);

				for(int i=0; i<map_get_width(map);i++){
					for(int j=0;j<map_get_height(map);j++){
						if (map_get_cell_type(map,i,j)==CELL_DOOR){
							player_set_position(player,i,j);
							player_set_current_way(player,SOUTH);
							break;
						}
					}

				}
				break;
			case DOOR_CLOSED:
				if (player_get_nb_key(player)>0){
					map_set_cell_compose_type(map,x,y,CELL_DOOR|map_get_cell_compose_type(map,x,y)|0x01);
					player_dec_nb_key(player);
					game->level=level;
					map=game_get_current_map(game);

					for(int i=0; i<map_get_width(map);i++){
						for(int j=0;j<map_get_height(map);j++){
							if (map_get_cell_type(map,i,j)==CELL_DOOR){
								player_set_position(player,i,j);
								player_set_current_way(player,SOUTH);
								break;
							}
						}

					}
				}
				break;
			default:
				break;

		}
	}


}



void game_display(struct game* game){
	window_clear();
	game_banner_display(game);
	game_change_level(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	monster_display(game->monster,game_get_level(game));

	window_refresh();
}

static int princess_is_reachable(struct game* game){
	struct player* player =game_get_player(game);
	struct map* map = game_get_current_map(game);
	int x=player_get_x(player);
	int y=player_get_y(player);
	if ((map_get_cell_compose_type(map,x,y)|map_get_cell_type(map,x,y))==CELL_PRINCESS){
		printf("La partie est terminée, le joueur a sauvé la princesse \n");
		return 0;
	}
	return 1;
}



static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct monster** monster = game_get_monster(game);
	struct map* map = game_get_current_map(game);
	int level = game_get_level(game);

	if(!princess_is_reachable(game)){
		return 1;
	}


	while (SDL_PollEvent(&event)) {
		int blocage =0;
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
					for(int i=0;i<NUMBER_MONSTER_MAX;i++){
						if (player_get_y(player)-1==monster_get_y(monster[i]) && player_get_x(player)==monster_get_x(monster[i]) && monster_get_alive(monster[i]) && monster_get_level(monster[i])==game_get_level(game) ){
							blocage = 1;
											}
										}

				if (blocage == 1){
					player_set_current_way(player, NORTH);
				}
				else{
					player_set_current_way(player, NORTH);
					player_move(player, map);
				}

				break;
			case SDLK_DOWN:
			for(int i=0;i<NUMBER_MONSTER_MAX;i++){
				if (player_get_y(player)+1==monster_get_y(monster[i]) && player_get_x(player)==monster_get_x(monster[i]) && monster_get_alive(monster[i]) && monster_get_level(monster[i])==game_get_level(game)){
					blocage = 1;
									}
								}
				if (blocage ==1){
					player_set_current_way(player, SOUTH);
			}
			else{
				player_set_current_way(player, SOUTH);
				player_move(player, map);
			}

				break;
			case SDLK_RIGHT:
			for(int i=0;i<NUMBER_MONSTER_MAX;i++){
				if (player_get_x(player)+1==monster_get_x(monster[i]) && player_get_y(player)==monster_get_y(monster[i]) && monster_get_alive(monster[i]) && monster_get_level(monster[i])==game_get_level(game)  ){
					blocage = 1;
									}
								}
			if (blocage ==1){
				player_set_current_way(player, EAST);
		}
		else{
			player_set_current_way(player, EAST);
			player_move(player, map);
		}
				//monster_set_current_way(monster, EAST);
				//monster_move(monster, map);
				break;
			case SDLK_LEFT:

			for(int i=0;i<NUMBER_MONSTER_MAX;i++){
				if (player_get_x(player)-1==monster_get_x(monster[i]) && player_get_y(player)==monster_get_y(monster[i]) && monster_get_alive(monster[i]) && monster_get_level(monster[i])==game_get_level(game) ){
					blocage = 1;
									}
								}
			if (blocage ==1){
				player_set_current_way(player, WEST);
		}
		else{
			player_set_current_way(player, WEST);
			player_move(player, map);
		}
				break;
			case SDLK_SPACE:
				bomb_creation( map,player,monster,level);
				break;


			default:
				break;
			}

			break;
		}
	}
	return 0;
}







 

int game_update(struct game* game) {
	if (player_get_nb_life(game->player)==0){
		return 1;
	}
	if (input_keyboard(game))
		return 1; // exit game

	for(int i=0;i<NUMBER_MONSTER_MAX;i++){
		if((monster_get_alive(game->monster[i]))==1 && monster_get_level(game->monster[i])==game_get_level(game)){
		monster_AI(game->monster[i], game->maps[game->level],game->monster,game_get_player(game),game->level);
	}
}
	


	return 0;
}
