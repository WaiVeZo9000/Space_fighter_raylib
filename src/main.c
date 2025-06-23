#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Enemys structure defination
typedef struct Enemy{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

// Bullet structure defination
typedef struct Bullet{
    Rectangle rec;  // Position and size of the bullet
    Vector2 speed;  // Speed and direction of the bullet
    bool active;    // Is this bulllet currently in use?
    Color color;    // Color of the bullet
} Bullet;

// Max bullets
#define MAX_BULLETS 50
#define MAX_ENEMIES 10

int main(void){
    // fresh the random number every time the game restart
    srand(time(NULL));

    int screen_width = 800;
    int screen_height = 450;

    InitWindow(screen_width, screen_height, "Space fighter .");

    // set a player and its position (approximately in middle of the screen)
    Rectangle player = {screen_width / 2 - 25, screen_height / 2 - 25, 50, 50};

    // speed of player 
    float player_speed = 5.0f;
    
    // Bullet Array initialization
    Bullet bullets[MAX_BULLETS] = { 0 };
    float bullet_speed = 8.0f;
    float last_short_time = 0.0f;  // To control the firing rate
    float fire_rate = 0.2f;        // Time in seconds between shots

    // Enemy Array initialization
    Enemy enemies[MAX_ENEMIES] = { 0 };
    float enemy_speed = 2.0f;
    float last_spwan_time = 0.0f;  // To control the spawn rate
    float spawn_rate = 2.0f;


    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // This is where we update the game logic
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
            player.y -= player_speed;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            player.y += player_speed;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
            player.x += player_speed;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
            player.x -= player_speed;
        }

        if (player.x < 0) player.x = 0;
        if (player.x + player.width > GetScreenWidth()) player.x = screen_width - player.width;
        if (player.y < 0) player.y = 0;

        // Bullet Logic handle here

        if (IsKeyPressed(KEY_SPACE) && (GetTime() - last_short_time) >= fire_rate){
            // Find the inactive bullet and activate it
            for (int i = 0 ; i < MAX_BULLETS ; i++){
                if (!bullets[i].active){
                    bullets[i].rec.x = player.x + player.width / 2 - 2;  // center vertically 
                    bullets[i].rec.y = player.y - 10;  
                    bullets[i].rec.width = 5;
                    bullets[i].rec.height = 10;
                    bullets[i].speed = (Vector2) { 0, -bullet_speed}; // Move forward
                    bullets[i].active = true;
                    bullets[i].color = RED;
                    last_short_time = GetTime();
                    break; // one bullet at a time
                }
            }
        }

        // Deactivating the bullets that go beyond the screen 
        for (int i = 0 ; i < MAX_BULLETS ; i++){
            if (bullets[i].active){
                
                // Move the bullet upward
                bullets[i].rec.x += bullets[i].speed.x;  // zero for now
                bullets[i].rec.y += bullets[i].speed.y;
                
                // Deactivating the bullets
                if (bullets[i].rec.x < 0){ // top screen
                    bullets[i].active = false;
                }

            }
        }
        // Enemy Logic handle here
        // Spawn the enimes outside the screen
        if (GetTime() - last_spwan_time >= spawn_rate){
            // activate enemies
            for (int i = 0 ; i < MAX_ENEMIES ; i++){
                if (!enemies[i].active)
                {
                    enemies[i].rec.x = (float)GetRandomValue(0, screen_width - 40); // Random X Position
                    enemies[i].rec.y = -40;                                         // Spawn above the screen
                    enemies[i].rec.width = 40;
                    enemies[i].rec.height = 40;
                    enemies[i].speed = (Vector2){0, enemy_speed};
                    enemies[i].active = true;
                    enemies[i].color = GREEN;
                    last_spwan_time = GetTime();
                    break;
                }
            }
        }

        // Deactivate the enemies

        for (int i = 0 ; i < MAX_ENEMIES ; i++){

            if (enemies[i].active){
                enemies[i].rec.y += enemies[i].speed.y;
                if (enemies[i].rec.y > screen_height){
                    enemies[i].active = false;
                }
            }
        }

        // Rendering part
        BeginDrawing();

        ClearBackground(BLACK);
        // Draw the player
        DrawRectangleRec(player, RAYWHITE);
        
        // Draw the bullets
        for (int i = 0 ; i < MAX_BULLETS ; i++){
            if (bullets[i].active){
                DrawRectangleRec(bullets[i].rec, bullets[i].color);
            }
        }
        
        // Draw the enemies
        for (int i = 0 ; i < MAX_ENEMIES ; i++){
            if (enemies[i].active){
                DrawRectangleRec(enemies[i].rec, enemies[i].color);
            }
        }

        DrawText("This is a space fighter game", 200, 20, 20, BLUE);
        DrawFPS(screen_width - 100, 10);
        EndDrawing();
    }
    
    CloseWindow();
}