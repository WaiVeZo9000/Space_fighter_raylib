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

// Power up 
typedef enum Boost {
    SPEED_BOOST = 0,
    BULLET_BOOST,
} Boost;

// Explosion definition
typedef struct Explosion{
    Rectangle rec;
    bool active;
    float frame_time;  // time passed since last frame
    int current_frame;
    float life_time; // how long this explosion will be active
} Explosion;

// Collectable boost
typedef struct Collectable{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
    Boost buff;
} Collectable;

// Game state definition
typedef enum Game_State {
    MENU = 0,
    PLAYING,
    GAME_OVER,
    PAUSED
} Game_State;

// Constants
#define MAX_BULLETS 50
#define MAX_ENEMIES 10
#define MAX_LIVES 3  // Added constant for max lives
#define MAX_POWERUP 5
#define MAX_EXPLOSIONS 8

// Explosion animation constants
#define EXPLOSION_FRAMES_PER_ROW 7 // the explosion sprite is 7 rows wide
#define EXPLOSION_FRAME_COUNT 7
#define EXPLOSION_FRAME_DURATION 0.08f // Time each frame stay visible

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

extern Collectable collectable[MAX_POWERUP];
extern float collectable_speed;
extern float last_collect_time;
extern float collect_rate;

// explosion 
extern Explosion explosions[MAX_EXPLOSIONS];

extern int score;
extern enum Game_State current_game_state;

// Screen shake variables
extern float screen_shake_intensity;
extern Vector2 screen_shake_offset;

// Game texture 
extern Texture2D player_texture;
extern Texture2D bullet_texture;
extern Texture2D enemy_texture;
extern Texture2D heart_texture;
extern Texture2D explosion_texture;
extern Texture2D speed_boost_texture;
extern Texture2D bullet_boost_texture;

// Function declarations
extern void InitGame(int screenWidth, int screenHeight);

// Loading assets and unloading
extern void LoadGameAssets(void);
extern void UnloadGameAssets(void);

#endif // GAME_HEADER