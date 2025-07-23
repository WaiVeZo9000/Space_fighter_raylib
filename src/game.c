#include "game.h"

Game_State current_game_state = MENU;

// Boost init
Boost game_boost;

Collectable collectable[MAX_POWERUP];
float collectable_speed;
float last_collect_time;
float collect_rate;

// player variables
Rectangle player;
int player_lives = 3;  // Changed from array to counter
float player_speed;

// Bullet variables
Bullet bullets[MAX_BULLETS];
float bullet_speed;
float last_shot_time;
float fire_rate;

// Enemy variables
Enemy enemies[MAX_ENEMIES];
float enemy_speed;
float last_spawn_time;
float spawn_rate;
int score = 0;

float screen_shake_intensity = 0.0f;
Vector2 screen_shake_offset = { 0.0f , 0.0f };
// Explosion 
Explosion explosions[MAX_EXPLOSIONS];

Texture2D player_texture = { 0 };
Texture2D enemy_texture = { 0 };
Texture2D bullet_texture = { 0 };
Texture2D heart_texture = { 0 };
Texture2D explosion_texture = { 0 };
Texture2D speed_boost_texture = { 0 };
Texture2D bullet_boost_texture = { 0 };

Game_Menu current_game_menu = NEW_GAME;
Paused_Menu current_paused_menu = RESUME;

void InitGame(int screenWidth, int screenHeight)
{
    // Reset Player
    player = (Rectangle){ (float)screenWidth / 2 - 25, (float)screenHeight - 70, 90, 90 };
    player_speed = 5.0f;
    player_lives = 3;

    // Reset Bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i] = (Bullet){0};
    }
    bullet_speed = 8.0f;
    last_shot_time = 0.0f;
    fire_rate = 0.2f;

    // Reset Enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i] = (Enemy){0};
    }
    enemy_speed = 2.0f;
    last_spawn_time = 0.0f;
    spawn_rate = 2.0f;
    
    // Reset Collectable
    for (int i = 0 ; i < MAX_POWERUP ; i++){
        collectable[i] = (Collectable){0};
    }
    collectable_speed = 4.0f;
    collect_rate  = 5.0f;
    last_collect_time = 0.0f;

    score = 0;

    for (int i = 0; i < MAX_EXPLOSIONS; i++){
        explosions[i].active = false;
    }

    // Reset screen shake on reset
    screen_shake_intensity = 0.0f;
    screen_shake_offset = (Vector2){ 0.0f, 0.0f };
}

// Load the texture 
void LoadGameAssets(void){
    player_texture = LoadTexture("assets/spaceship.png");
    enemy_texture = LoadTexture("assets/planets.png");
    bullet_texture = LoadTexture("assets/space_bullets.png");
    heart_texture = LoadTexture("assets/heart_pixels.png");
    explosion_texture = LoadTexture("assets/explosion-animation.png");
    speed_boost_texture = LoadTexture("assets/speed_boost.png");
    bullet_boost_texture= LoadTexture("assets/bullet_boost.png");
}

// unload the texture 
void UnloadGameAssets(void){
    UnloadTexture(player_texture);
    UnloadTexture(bullet_texture);
    UnloadTexture(enemy_texture);
    UnloadTexture(heart_texture);
    UnloadTexture(explosion_texture);
    UnloadTexture(speed_boost_texture);
    UnloadTexture(bullet_boost_texture);
}