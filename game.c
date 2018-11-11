/*



Student Number: n9405194
**   Name: Timothy Mollenhauer
**   Completed level 1 - 4
**   Level 5 is partialy complete - the enemy should move to the edge at a random interval
	 then once it has reached the edge move in a parabolic path to hit the player 
**     

Insperation was taken from the AMS week 6 

the parts of the control function was used from Week 6  AMS








*/



#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"

#define Number (10)  
#define Max_bomb (4)
//--------------------------------------------------------------------------- setup se

typedef struct Game { 
	bool player_death;                 
	bool over;
	bool over2;                            
	int score;	
	int primary_timer_delay;						  				  
	int lives;
	int count;
	int live_enemy;
	int level;
	int height_counter;
	timer_id height_counter_timer;
	int test;
	int offset_count;
	int enemy_that_launched_bomb_last;
	bool enemy_attacking;
	bool active_attack;
	double position[5];
	int attacking_enemy_number;
	int test_counter;

	int main_counter;
	sprite_id player;						  
										  
	sprite_id player_bullet;					  
	timer_id player_bullet_timer;	

	sprite_id enemy_bomb[Max_bomb];
	timer_id enemy_bomb_timer;
	timer_id launch_bomb_timer;			  
				

	sprite_id attacking_enemy;									  
	sprite_id enemy[Number];			  
	timer_id enemy_timer;	

} Game;	

void setup_game(Game * game){
	game->offset_count = 0;
	game->score = 0;
	game->over = false;
	game->primary_timer_delay = 20;
	game->lives = 3;
	game->count = 0;
	game->live_enemy = Number;
	game->level = 1;
	game->height_counter = 0;
	game->test = 0;
	game->enemy_that_launched_bomb_last = 0;
	game->over2 = false;
	game->player_death = false;
	game->enemy_attacking = false;
	game->active_attack = false;
	game->attacking_enemy_number = -1;
	game->test_counter = 0;
	game->main_counter = screen_width()/2;
}

void setup_player( Game * game ){

	game->player = create_sprite(screen_width()/2, screen_height()-3, 1, 1, "$");
}

void setup_enemy( Game * game ) {

static char bitmap[] = { '@' };
	for(int i = 0; i < Number; i++)
	{
		game->enemy[i] = create_sprite(-1, 0, 1, 1, bitmap);
		game->enemy[i]->dx = 1;
		
	}

	game->attacking_enemy = create_sprite(-1, 0, 1, 1, bitmap);


	game->height_counter_timer = create_timer(1000);
	game->enemy_timer = create_timer(500);
}

void enemy_position(Game * game){

	game->height_counter = 0;

	int enemy_x[] = {screen_width()/2, screen_width()/2 - 6, screen_width()/2 + 6, screen_width()/2 - 3, 
					 screen_width()/2 - 10, screen_width()/2 + 3, screen_width()/2 + 10, screen_width()/2,
					 screen_width()/2 - 6, screen_width()/2 + 6};
	int enemy_y[] = {screen_height()/4 - 1, screen_height()/4 - 1, screen_height()/4 - 1, screen_height()/4,
					 screen_height()/4, screen_height()/4, screen_height()/4, screen_height()/4 + 1,
					 screen_height()/4 + 1, screen_height()/4 + 1};

	for (int i = 0; i < Number; i++){
		game->enemy[i]->x = enemy_x[i];
 		game->enemy[i]->y = enemy_y[i];
  		game->enemy[i]->is_visible = true;
	}

}




void player_death ( Game * game){

	game->enemy_bomb[0]->is_visible = false;
	game->enemy_bomb[1]->is_visible = false;
	game->enemy_bomb[2]->is_visible = false;
	game->enemy_bomb[3]->is_visible = false;
	game->lives--;
	game->player->x = - 1;
	game->player_death = true;

	if (game->lives == 0){
		game->over = true;
	}
}






//--------------------------------------------------------------------------- player section

bool update_player( Game * game, int key_code ){

	int has_moved_x = game->player->x;

	if (key_code == 'a' && game->player->x > 0) game->player -> x--;
	else if (key_code == 'd' && game->player->x < screen_width() - 1) game->player -> x++;

	if (game->player->x != has_moved_x)
	{
		return true;	
	}
return false;
}
//--------------------------------------------------------------------------- bullet section
void setup_player_bullet(Game * game){

	static char bitmap[] = { '|' };
	game->player_bullet = create_sprite(0, 0, 1, 1, bitmap);
	game->player_bullet_timer = create_timer(10);
	game->player_bullet->is_visible = false; 
}

void launch_player_bullet( Game * game ) {

	if (game->player_bullet->is_visible == false)
	{	
		game->player_bullet->x = game->player->x;
		game->player_bullet-> y = game->player->y -1;
		game->player_bullet->dy = -1;
		reset_timer(game->player_bullet_timer);
		game->player_bullet->is_visible = true;
	}
}



bool move_player_bullet( Game * game ) {

	if (game->player_bullet->is_visible == false)
	{	
		return false;
	}
	else
		{
		if (!timer_expired(game->player_bullet_timer))
		{
			return false;
		}
		else
		{
			game->player_bullet->y += game->player_bullet->dy;

			int x = game->player_bullet->x;
			int y = game->player_bullet->y;

			if (y < 0)
			{
				game->player_bullet->is_visible = false;
			}

			for (int i = 0; i < Number; i++)
			{
				if (x == game->enemy[i]->x && y == game->enemy[i]->y)
				{

					game->player_bullet->is_visible = false;
					game->enemy[i]->is_visible = false;
					game->score += 30;
					game->enemy[i]->x = - 1;
					game->enemy[i]->y = 0;
					game->live_enemy --;
		

					if (game->live_enemy == 0){
						game->score += 500;
						game->live_enemy = Number;
						enemy_position( game );		
					}
				}
			}

			if(game->level == 5 && x == game->attacking_enemy->x && y == game->attacking_enemy->y){
				game->player_bullet->is_visible = false;
				game->score += 30;
				game->enemy_attacking = false; 
				game->active_attack = false;
				game->enemy[game->attacking_enemy_number]->x = game->test_counter;
				game->attacking_enemy_number = -1;
			}


			return true;
		}
	}
}

bool update_player_bullet( Game * game, int key_code ) {
	if ( game->player_bullet->is_visible ) {
		return move_player_bullet( game );
	}
	else if ( key_code == 's' ) {
		launch_player_bullet( game );
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------------------- enemy section 

bool move_enemy_level_1( Game*game, sprite_id enemy, bool y_move) {
	
	int x = enemy->x;
	int y = enemy->y;
	if (enemy->is_visible != true)
	{
		return false;
	}
	else
	{
		enemy->x += enemy->dx;
		if (y_move == true){
			enemy->y += 1;
		}

		if (enemy->x >= screen_width() - 1){
			enemy->x = 0;
		}
		if (enemy->y == screen_height() -2){
			enemy->y = 0;
		}
		if ( y == game->player->y && x == game->player->x ){
			enemy->is_visible = false;
			enemy->x = -1;
			player_death( game );
		}
	}
	game->test = 0;
	for (int i = 0; i < Number; i++){
				game->test = game->test + game->enemy[i]->x;
				if (game->test == -10){
					enemy_position( game );
				}
			}

	if (enemy->x != x)
	{
		return true;
	}
	return false;
}

bool move_enemy_level_2(Game * game){
  double test = 0;
  int amplitude = screen_height()/6;
  int period = 20;
  
  int offset = 0;
  bool y_move = false;
  bool changed = false;
  
  test = round(amplitude * sin((2*3.14)/period*(game->main_counter)) + screen_height()/4);

  for ( int i = 0; i < Number; i++ ) {
      if (i < 3){
        offset = -1;
      }
      else if (i > 2 && i < 7){
        offset = 0;
      }
      else if (i > 6){
        offset = 1;
      }

    sprite_id enemy = game->enemy[i];
    changed = move_enemy_level_1( game , enemy , y_move) || changed;
    game->enemy[i]->y = test + offset;
  }  
  return changed;
}

bool random_movement( Game*game, sprite_id enemy, int c) {
	int r = rand() % 100;
	int positive_offset = 70;
	int negative_offset = 30;
	int offset = 50;
	int positive = 0;
	int negative = 0;
	int current_position_0 = game->enemy[0]->x;
	int current_position_1 = game->enemy[1]->x;
	int current_position_2 = game->enemy[2]->x;
	int current_position_3 = game->enemy[3]->x;
	int current_position_4 = game->enemy[4]->x;
	int current_position_5 = game->enemy[5]->x;
	int current_position_6 = game->enemy[6]->x;
	int current_position_7 = game->enemy[7]->x;
	int current_position_8 = game->enemy[8]->x;
	int current_position_9 = game->enemy[9]->x;
	int forbiddin_position_1 = -1;
	int forbiddin_position_2 = -1;
	int forbiddin_position_3 = -1;
	int forbiddin_position_4 = -1;
	int forbiddin_position_5 = -1;
	int forbiddin_position_6 = -1;
	int forbiddin_position_7 = -1;
	int forbiddin_position_8 = -1;


	if (game->offset_count > 1){
		positive = negative_offset;
		negative = positive_offset;
	}
	else
	{
		positive = positive_offset;
		negative = negative_offset;
	}

//==============================================
	if (c == 0){
		
		forbiddin_position_2 = current_position_1 + 2;
		forbiddin_position_1 = current_position_2 - 2;
		offset = positive;
	}
	else if (c == 1){
		forbiddin_position_1 = current_position_0 - 2;
		forbiddin_position_3 = current_position_2 - 2;
		offset = positive;
	}
	else if (c == 2){
		
		forbiddin_position_2 = current_position_0 + 2;
		forbiddin_position_4 = current_position_1 + 2;

		offset = negative;
	}

	else if (c == 3){
		forbiddin_position_2 = current_position_4 + 2;
		forbiddin_position_1 = current_position_5 - 2;
		forbiddin_position_3 = current_position_6 - 2;
		offset = positive;
	}

	else if (c == 4){
		forbiddin_position_1 = current_position_3 - 2;
		forbiddin_position_7 = current_position_5 - 2;
		forbiddin_position_3 = current_position_6 - 2;
		offset = positive;
	}

	else if (c == 5){
		forbiddin_position_8 = current_position_4 + 2;
		forbiddin_position_2 = current_position_3 + 2;
		forbiddin_position_5 = current_position_6 - 2;
		offset = negative;
	}

	else if (c == 6){
		forbiddin_position_8 = current_position_3 + 2;
		forbiddin_position_4 = current_position_4 + 2;
		forbiddin_position_6 = current_position_5 + 2;
		offset = negative;
	}

	else if (c == 7){
		forbiddin_position_1 = current_position_9 - 2;
		forbiddin_position_2 = current_position_8 + 2;
		offset = negative;
		}
	else if (c == 8){
		forbiddin_position_1 = current_position_7 - 2;
		forbiddin_position_3 = current_position_9 - 2;
		offset = positive;
	}
	else if (c == 9){
		forbiddin_position_4 = current_position_8 + 2;
		forbiddin_position_2 = current_position_7 + 2;
		offset = negative;
	}

	int x = enemy->x;
	int y = enemy->y;
	if (enemy->is_visible != true)
	{
		return false;
	}
	else
	{	

		if (r > offset && x != forbiddin_position_1 && x != screen_width()-1 &&x != forbiddin_position_3 &&
			x != forbiddin_position_5 && x != forbiddin_position_7){
			enemy->x += enemy->dx;

	
		}
		else if (r < offset &&  x != forbiddin_position_2 && x != forbiddin_position_4 && 
				 x != forbiddin_position_6 && x != 0 && x != forbiddin_position_8){
			enemy->x -= enemy->dx;
		}
		
		if (enemy->y == screen_height() -2){
			enemy->y = 0;
		}

		if ( y == game->player->y && x == game->player->x ){
			enemy->is_visible = false;
			enemy->x = -1;
			player_death( game );
		}
	}

	game->test = 0;
	for (int i = 0; i < Number; i++){
				game->test = game->test + game->enemy[i]->x;
				if (game->test == -10){
					enemy_position( game );
				}
			}

	if (enemy->x != x)
	{
		return true;
	}
	return false;
}

bool move_enemy_level_4( Game * game ) {

	bool changed = false;
	for ( int i = 0; i < Number; i++ ) {
		sprite_id enemy = game->enemy[i];
		changed = random_movement( game , enemy, i ) || changed;
	}	

	if (changed == true){
		for (int m = 0; m < Number; m++){
				game->enemy[m]->y += 1;		
		}
	}
	return changed;
}

void choose_enemy_to_attack(Game * game){

	int r = rand() % 11;
	while (game->enemy[r]->is_visible == false){
		r = rand() % 11;
	}

	game->attacking_enemy->x = game->enemy[r]->x;
	game->attacking_enemy->y = game->enemy[r]->y;
	game->attacking_enemy->is_visible = true;
	game->test_counter = game->enemy[r]->x;
	game->enemy[r]->x = -1;
	game->enemy_attacking = true;
	game->attacking_enemy_number = r;

}

bool move_enemy_level_5( Game * game ) {

	bool changed = false;
	
	int r = rand() % 8;

	if (game->enemy_attacking == false && r < game->height_counter && game->live_enemy != 1){
		choose_enemy_to_attack(game);
	}

	bool y_move = true;
		for ( int i = 0; i < Number; i++ ) {
			sprite_id enemy = game->enemy[i];
			changed = move_enemy_level_1( game , enemy, y_move ) || changed;
			if (game->enemy_attacking == true && i == game->attacking_enemy_number){
				game->test_counter += 1;
				if (game->test_counter == screen_width() - 1){
					game->test_counter = 0;
				}
				game->enemy[game->attacking_enemy_number]->x = -1;
			}
		}
	
	if (game->enemy_attacking == true && game->attacking_enemy->x != 0 && game->active_attack == false){
		game->attacking_enemy->x--;
		if (game->attacking_enemy->x == 0){			
		game->active_attack = true;  

		game->position[2] = game->player->x -1;
		game->position[3] = game->player->y;

		game->position[1] = game->attacking_enemy->y;
		game->position[0] = game->attacking_enemy->x;
	 	game->position[4] = (game->position[3] - game->position[1])/(game->position[2]*game->position[2]);
		
		}
	}
	
	 if(game->enemy_attacking == true && game->active_attack == true){


	 	game->attacking_enemy->y = (game->position[4] * (game->attacking_enemy->x * game->attacking_enemy->x)) + game->position[1];
		game->attacking_enemy->x++;

		if (game->attacking_enemy->x == game->player->x && game->attacking_enemy->y == game->player->y){
			game->enemy_attacking = false; 
			game->active_attack = false;
			game->enemy[game->attacking_enemy_number]->x = game->test_counter;
			game->attacking_enemy_number = -1;
			player_death( game );

		}

		if (game->attacking_enemy->y > screen_height() - 2){
			game->enemy_attacking = false; 
			game->active_attack = false;
			game->enemy[game->attacking_enemy_number]->x = game->test_counter;
			game->attacking_enemy_number = -1;

		}
	}	
	return changed;
}

bool update_enemy( Game * game, int level ) {
	bool y_move = false;
	if ( ! timer_expired( game->enemy_timer ) ) {
		return false;
	}
	game->main_counter++;
		if (game->main_counter >= screen_width()){
			game->main_counter = 0;
		}
	game->height_counter++;

	if (game->height_counter > 20){
		game->height_counter = 0;
		game->offset_count++;
		if (game->offset_count > 3){
			game->offset_count = 0;
		}
	}

	game->level = level;
	bool changed = false;

	if (level == 1 ){
		for ( int i = 0; i < Number; i++ ) {
			sprite_id enemy = game->enemy[i];
			changed = move_enemy_level_1( game , enemy, y_move ) || changed;
		}
	}
	else if (level == 2){
		move_enemy_level_2( game ) || changed;
	}
	else if (level == 3){
		bool y_move = true;
		for ( int i = 0; i < Number; i++ ) {
			sprite_id enemy = game->enemy[i];
			changed = move_enemy_level_1( game , enemy, y_move ) || changed;
		}
	}
	else if (level == 4){
		move_enemy_level_4( game ) || changed;
	}
	else if (level == 5){
		move_enemy_level_5( game ) || changed;
	}

	if (game->player_death == true){
		for ( int m = 0; m < Number; m++ ) {
			if (game->enemy[m]->y < screen_height()/2 && game->enemy[m]->y > 4){
				game->player_death = false;
				game->player->x = screen_width()/2;
			}
		}
	}
	


	return changed;
}
//--------------------------------------------------------------------------- enemy bullet

void setup_enemy_bomb(Game * game){
	static char bitmap[] = { '*' };
	
		//static char bitmap[] = { '*' };
	for(int i = 0; i < Max_bomb; i++){
		game->enemy_bomb[i] = create_sprite(0, 0, 1, 1, bitmap);
	}

		for(int i = 0; i < Max_bomb; i++){
		game->enemy_bomb[i]->dy = 1;
		game->enemy_bomb[i]->is_visible = false; 
	}

	game->enemy_bomb_timer = create_timer(300);
	game->launch_bomb_timer = create_timer(3000);
}


void launch_enemy_bomb(Game * game, int i){
	if (game->enemy_bomb[i]->is_visible == false){
		int t = game->enemy_that_launched_bomb_last;
		if (game->live_enemy == 1){
			t = t + 1;
		}


		int r = rand() % 11;
		while (game->enemy[r]->is_visible == false || t == r ){
			r = rand() % 11;
		}
		
		game->enemy_bomb[i]->x = game->enemy[r]->x;
		game->enemy_bomb[i]->y = game->enemy[r]->y + 1;
		game->enemy_bomb[i]->is_visible = true;
		game->enemy_that_launched_bomb_last = r;

		reset_timer(game->enemy_bomb_timer);		
	}
}


bool move_enemy_bomb( Game * game, sprite_id enemy_bomb ){

	if (enemy_bomb->is_visible == false ){
		return false;
	}

	else{
		enemy_bomb->y += enemy_bomb->dy;
		int x = enemy_bomb->x;
		int y = enemy_bomb->y;

		if (y > screen_height()-2){
			enemy_bomb->is_visible = false;
		}
		if (x == game->player->x && y == game->player->y ){
			player_death(game);
		}

		return true;
	}
}

bool update_enemy_bomb( Game * game ) {

	if ( timer_expired(game->launch_bomb_timer) ) {
		launch_enemy_bomb( game, game->count);
		game->count++;

		if (game->count == Max_bomb){
			game->count = 0;
		}
			return true;
	}
	bool changed = false;
		
	if (!timer_expired(game->enemy_bomb_timer)){
		return false;
	}

for ( int i = 0; i < Number; i++ ) {
		sprite_id enemy_bomb = game->enemy_bomb[i];
		changed = move_enemy_bomb( game, enemy_bomb ) || changed;
	}
	return changed;
}

//--------------------------------------------------------------------------- Draw section

void draw_enemy( Game * game ) {
	for (int i = 0; i < Number; i++)
	{
		draw_sprite(game->enemy[i]);
	}
	draw_sprite(game->attacking_enemy);

}

void draw_player( Game *game ) {
	draw_sprite(game->player);
}

void draw_player_bullet( Game * game ) {
	draw_sprite(game->player_bullet);
}

void draw_enemy_bomb(Game * game){
	for (int i = 0; i < Max_bomb; i++){
		draw_sprite(game->enemy_bomb[i]);
	}
}
void draw_background( Game * game ) {
	draw_line(0, screen_height() - 2, screen_width(), screen_height() - 2, '-');
	draw_int(screen_width()-1, screen_height() - 1, game->lives);
	draw_string(screen_width()-8, screen_height() - 1, "Lives: ");

	draw_int(screen_width()-13, screen_height() - 1, game->score);
	draw_string(screen_width()-20, screen_height() - 1, "Score: ");
	draw_string(0, screen_height() - 1, "Timothy Mollenhauer n9405194 ");

	draw_int(screen_width()-22, screen_height() - 1, game->level);
	draw_string(screen_width()-29, screen_height() - 1, "level: ");
	
	
}

void draw_all( Game * game ) {
	clear_screen();
	draw_background( game );
	draw_enemy_bomb( game );
	draw_player( game );
	draw_player_bullet( game );
	draw_enemy( game );
	show_screen();
}

void cleanup_bullet( Game * game ){

	game->enemy_bomb[0]->is_visible = false;
	game->enemy_bomb[1]->is_visible = false;
	game->enemy_bomb[2]->is_visible = false;
	game->enemy_bomb[3]->is_visible = false;
}


//--------------------------------------------------------------------------- Control

void setup_all( Game * game ) {

	setup_enemy( game );
	enemy_position( game );
	setup_game( game );
	setup_player( game );
	setup_player_bullet( game );
	setup_enemy_bomb( game );
}

void control (void){
	Game game;
	setup_all( &game );
	draw_all( &game );
	int level = 1;

	while (!game.over2){

		while ( !game.over ) {
			int key_code = get_char();

			if ( key_code == 'q' ) {
				game.over2 = true;
				game.over = true;
			}

			if (key_code == 'r'){
				game.over = true;
				game.over2 = true;
				control();
			}

			if (key_code == 'l'){
				level = level + 1;
				enemy_position( &game );
				cleanup_bullet( &game );
				if (level == 6){
					level = 1;
				} 
			}

			else {
				bool show_player = update_player( &game, key_code );
				bool show_bullet = update_player_bullet( &game, key_code );
				bool show_bomb = update_enemy_bomb( &game );
				bool show_enemy = update_enemy( &game, level);

				if ( show_player || show_bullet || show_enemy || show_bomb ) {
					draw_all( &game );
				}
			}
			timer_pause( game.primary_timer_delay );
		}

		if (game.over2 == false){

			clear_screen();
			draw_string(screen_width()/2, screen_height()/2, "Game Over");
			draw_string(screen_width()/2, screen_height()/2 + 2, "Press q to quid or r to continue.");
					
			show_screen();

			int key = wait_char( );

			if (key == 'r'){
				game.over2 = true;
				control();
			}
			else if (key == 'q'){
				game.over2 = true;
			}
		}
	}
}

int main( void ) {
	srand( time( NULL ) );
	setup_screen();
	control();
 	cleanup_screen();
	return 0;
}
