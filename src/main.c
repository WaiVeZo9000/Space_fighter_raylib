#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include "game.h"

Game_State current_game_state = PLAYING;

int main(void){
    // fresh the random number every time the game restart
    srand(time(NULL));

    int screen_width = 800;
    int screen_height = 450;

    InitWindow(screen_width, screen_height, "Space fighter .");

    ResetGame(screen_width, screen_height);

    Rectangle heart[player_lives];

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_Q))
        {
            if (current_game_state == PLAYING)
            {
                current_game_state = PAUSED;
            }
            else if (current_game_state == PAUSED)
            {
                current_game_state = PLAYING;
            }
        }

        switch (current_game_state)
        {
        case PLAYING:

            // This is where we update the game logic
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
            {
                player.y -= player_speed;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
            {
                player.y += player_speed;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
            {
                player.x += player_speed;
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
            {
                player.x -= player_speed;
            }

            if (player.x < 0)
                player.x = 0;
            if (player.x + player.width > GetScreenWidth())
                player.x = screen_width - player.width;
            if (player.y < 0)
                player.y = 0;
            if (player.y + player.height > GetScreenHeight())
                player.y = screen_height - player.height;

            // Set the player hearts position
            for (int i = 0; i < player_lives; i++)
            {
                heart[i].x = 20 + i * 30; // Spacing the hearts
                heart[i].y = 30;
                heart[i].width = 20;
                heart[i].height = 20;
            }

            // Bullet Logic handle here

            if (IsKeyPressed(KEY_SPACE) && (GetTime() - last_shot_time) >= fire_rate)
            {
                // Find the inactive bullet and activate it
                for (int i = 0; i < MAX_BULLETS; i++)
                {
                    if (!bullets[i].active)
                    {
                        bullets[i].rec.x = player.x + player.width / 2 - 2; // center vertically
                        bullets[i].rec.y = player.y - 10;
                        bullets[i].rec.width = 5;
                        bullets[i].rec.height = 10;
                        bullets[i].speed = (Vector2){0, -bullet_speed}; // Move forward
                        bullets[i].active = true;
                        bullets[i].color = RED;
                        last_shot_time = GetTime();
                        break; // one bullet at a time
                    }
                }
            }

            // Deactivating the bullets that go beyond the screen
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {

                    // Move the bullet upward
                    bullets[i].rec.x += bullets[i].speed.x; // zero for now
                    bullets[i].rec.y += bullets[i].speed.y;

                    // Deactivating the bullets
                    if (bullets[i].rec.y < 0)
                    { // top screen
                        bullets[i].active = false;
                    }
                }
            }
            // Enemy Logic handle here
            // Spawn the enimes outside the screen
            if (GetTime() - last_spawn_time >= spawn_rate)
            {
                // activate enemies
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (!enemies[i].active)
                    {
                        enemies[i].rec.x = (float)GetRandomValue(0, screen_width - 40); // Random X Position
                        enemies[i].rec.y = -40;                                         // Spawn above the screen
                        enemies[i].rec.width = 40;
                        enemies[i].rec.height = 40;
                        enemies[i].speed = (Vector2){0, enemy_speed};
                        enemies[i].active = true;
                        enemies[i].color = GREEN;
                        last_spawn_time = GetTime();
                        break;
                    }
                }
            }
            // Deactivate the enemies

            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    enemies[i].rec.y += enemies[i].speed.y;
                    if (enemies[i].rec.y > screen_height)
                    {
                        enemies[i].active = false;
                        player_lives--;
                        if (player_lives <= 0)
                        {
                            current_game_state = GAME_OVER;
                        }
                    }
                }
            }

            // Collisioin Logic

            // Bullet - Enemy
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    for (int j = 0; j < MAX_ENEMIES; j++)
                    {
                        if (enemies[j].active)
                        {
                            if (CheckCollisionRecs(enemies[j].rec, bullets[i].rec))
                            {
                                bullets[i].active = false;
                                enemies[j].active = false;
                                score += 10;
                                break;
                            }
                        }
                    }
                }
            }

            // Player - Enemy
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    if (CheckCollisionRecs(player, enemies[i].rec))
                    {
                        enemies[i].active = false;
                        player_lives--;
                        if (player_lives <= 0)
                        {
                            current_game_state = GAME_OVER;
                        }
                        break;
                    }
                }
            }

            break;
        
        case GAME_OVER:
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset the game
                ResetGame(screen_width, screen_height);
                current_game_state = PLAYING;
            }
            break;
        
        default:
            break;
        }

        // Rendering part
        BeginDrawing();

        ClearBackground(BLACK);

        switch (current_game_state)
        {
        case PLAYING:
        case PAUSED:
            // Draw the player
            DrawRectangleRec(player, RAYWHITE);

            // Draw the bullets
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    DrawRectangleRec(bullets[i].rec, bullets[i].color);
                }
            }

            // Draw the enemies
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    DrawRectangleRec(enemies[i].rec, enemies[i].color);
                }
            }

            // Draw Hearts
            for (int i = 0; i < player_lives; i++)
            {
                DrawRectangleRec(heart[i], RED);
            }

            DrawText(TextFormat("Score : %d ", score), 10, 10, 20, PINK);

            DrawText("This is a space fighter game", 200, 20, 20, BLUE);
            DrawFPS(screen_width - 100, 10);
            if (current_game_state == PAUSED)
            {
                DrawText("PAUSED", screen_width / 2 - MeasureText("PAUSED", 40) / 2, screen_height / 2 - 20, 40, YELLOW);
                DrawText("Press Q to Resume", screen_width / 2 - MeasureText("Press ESC to Resume", 20) / 2, screen_height / 2 + 30, 20, RAYWHITE);
            }
            break;

        case GAME_OVER:
            DrawText("GAME OVER!", screen_width / 2 - MeasureText("GAME OVER!", 40) / 2, screen_height / 2 - 20, 40, RED);
            DrawText("Press ENTER to Restart", screen_width/ 2 - MeasureText("Press ENTER to Restart", 20) / 2,  screen_height/ 2 + 30, 20, RAYWHITE);
            break;
        default:
            break;
        }
        EndDrawing();
    }

    CloseWindow();
}