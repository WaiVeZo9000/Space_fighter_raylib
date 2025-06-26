#ifndef GAME_HEADER
#define GAME_HEADER

#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Enemy structure definition
typedef struct Enemy {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

// Bullet structure definition
typedef struct Bullet {
    Rectangle rec;  // Position and size of the bullet
    Vector2 speed;  // Speed and direction of the bullet
    bool active;    // Is this bullet currently in use?
    Color color;    // Color of the bullet
} Bullet;

// Game state definition
typedef enum Game_State {
    PLAYING = 0,
    GAME_OVER,
    PAUSED
} Game_State;

// Constants
#define MAX_BULLETS 50
#define MAX_ENEMIES 10
#define MAX_LIVES 3  // Added constant for max lives

// Game state variables (extern declarations)
extern Rectangle player; 
extern int player_lives;  // Changed from array to counter
extern float player_speed;

extern Bullet bullets[MAX_BULLETS];
extern float bullet_speed;
extern float last_shot_time;  // Fixed typo
extern float fire_rate;

extern Enemy enemies[MAX_ENEMIES];
extern float enemy_speed;
extern float last_spawn_time;  // Fixed typo
extern float spawn_rate;

extern int score;
extern Game_State current_game_state;

// Game texture 
extern Texture2D player_texture;
extern Texture2D bullet_texture;
extern Texture2D enemy_texture;
extern Texture2D heart_texture;

// Function declarations
extern void ResetGame(int screenWidth, int screenHeight);

// Loading assets and unloading
extern void LoadGameAssets(void);
extern void UnloadGameAssets(void);

#endif // GAME_HEADER