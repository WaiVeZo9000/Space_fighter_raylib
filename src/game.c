#include "game.h"

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

Texture2D player_texture = { 0 };
Texture2D enemy_texture = { 0 };
Texture2D bullet_texture = { 0 };
Texture2D heart_texture = { 0 };

void ResetGame(int screenWidth, int screenHeight)
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
    score = 0;
}

void LoadGameAssets(void){
    player_texture = LoadTexture("assets/spaceship.png");
    enemy_texture = LoadTexture("assets/planets.png");
    bullet_texture = LoadTexture("assets/space_bullets.png");
    heart_texture = LoadTexture("assets/heart_pixels.png");
}

void UnloadGameAssets(void){
    UnloadTexture(player_texture);
    UnloadTexture(bullet_texture);
    UnloadTexture(enemy_texture);
    UnloadTexture(heart_texture);
}