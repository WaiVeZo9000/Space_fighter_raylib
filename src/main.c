#include "game.h"
#include <rlgl.h>

int main(void){
    // fresh the random number every time the game restart
    srand(time(NULL));

    int screen_width = 1200;
    int screen_height = 650;

    InitWindow(screen_width, screen_height, "Space fighter .");

    LoadGameAssets();

    Rectangle heart[player_lives];

    // Background
    Texture2D background = LoadTexture("assets/space_background.jpeg");

    if (background.id == 0){
        TraceLog(LOG_WARNING, "Background image not found.");
        return 1;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // To get the each time frame
        float delta_time = GetFrameTime();

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

        if (current_game_state == PAUSED)
        {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                current_paused_menu--;
                if (current_paused_menu > 0)
                {
                    current_paused_menu = MENU_COUNT - 1;
                }
            }

            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                current_paused_menu++;
                if (current_paused_menu > MENU_COUNT - 1)
                {
                    current_paused_menu = 0;
                }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                switch (current_paused_menu)
                {
                case RESUME:
                    current_game_state = PLAYING;
                    break;

                case EXIT:
                    CloseWindow();
                    break;
                default:
                    break;
                }
            }
        }
        switch (current_game_state)
        {
        case MENU:
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                current_game_menu--;
                if (current_game_menu > 0){
                    current_game_menu = OPTION_COUNT - 1;
                }
            }
            
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                current_game_menu++;
                if (current_game_menu > OPTION_COUNT - 1){
                    current_game_menu = 0;
                }
            }

            if (IsKeyPressed(KEY_ENTER)){
                switch (current_game_menu)
                {
                case NEW_GAME:
                    InitGame(screen_width, screen_height);
                    current_game_state = PLAYING;
                    break;
                case EXIT_GAME:
                    CloseWindow();
                    break;
                default:
                    break;
                }
            }
            break;
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
                        bullets[i].rec.width = 10;
                        bullets[i].rec.height = 15;
                        bullets[i].speed = (Vector2){0, -bullet_speed}; // Move upward
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
            if (GetTime() - last_spawn_time >= spawn_rate && spawn_rate > 0)
            {
                // activate enemies
                int size_of_enemy = (float)GetRandomValue(100, 200);  
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (!enemies[i].active)
                    {
                        enemies[i].rec.x = (float)GetRandomValue(0, screen_width - size_of_enemy); // Random X Position
                        enemies[i].rec.y = -size_of_enemy;                                         // Spawn above the screen
                        enemies[i].rec.width = size_of_enemy;
                        enemies[i].rec.height = size_of_enemy;
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
                    enemies[i].rec.y += enemies[i].speed.y ;
                    if (enemies[i].rec.y > screen_height)
                    {
                        enemies[i].active = false;
                        player_lives--;
                    }
                }
            }

            // Collectable init
            if (GetTime() - last_collect_time >= collect_rate){
                for (int i = 0 ; i < MAX_POWERUP ; i++){
                    if (!collectable[i].active){
                        collectable[i].rec.x = (float)GetRandomValue(0, screen_width - 50);
                        collectable[i].rec.y = -60;
                        collectable[i].rec.width = 60;
                        collectable[i].rec.height = 60;
                        collectable[i].speed = (Vector2) { 0, collectable_speed};
                        last_collect_time = GetTime();
                        collectable[i].active = true;
                        collectable[i].buff = GetRandomValue(0,1);
                        collectable[i].color = BLUE;
                        break;
                    }
                }
            }

            // Deactivate collectable 
            for (int i = 0 ; i < MAX_POWERUP ; i++){
                if (collectable[i].active){
                    collectable[i].rec.y += collectable[i].speed.y;

                    if (collectable[i].rec.y > screen_height){
                        collectable[i].active = false;
                    }
                }
            }
            // Collisioin Logic

            // Power up
            for (int i = 0 ; i < MAX_POWERUP ; i++){
                if (collectable[i].active){
                    if (CheckCollisionRecs(player, collectable[i].rec)){
                        collectable[i].active = false;
                        switch (collectable[i].buff)
                        {
                        case SPEED_BOOST:
                            player_speed += 2.0f;
                            break;
                        
                        case BULLET_BOOST:
                            fire_rate -= 0.5f;                            
                            break;
                        default:
                            break;
                        }
                    }
                }
            }

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
                                
                                for (int k = 0 ; k < MAX_EXPLOSIONS ; k++){
                                    if (!explosions[k].active){
                                        explosions[k].rec = enemies[j].rec; 
                                        explosions[k].active = true;
                                        explosions[k].current_frame = 0;
                                        explosions[k].frame_time = 0.0f;
                                        explosions[k].life_time = 0.0f;
                                        break;
                                    }
                                }

                                if (spawn_rate > 0){
                                    spawn_rate -= 0.02f;
                                }
                                screen_shake_intensity = 5.0f;
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

                        for (int k = 0; k < MAX_EXPLOSIONS; k++)
                        {
                            if (!explosions[k].active)
                            {
                                explosions[k].rec = enemies[i].rec;
                                explosions[k].active = true;
                                explosions[k].current_frame = 0;
                                explosions[k].frame_time = 0.0f;
                                explosions[k].life_time = 0.0f;
                                break;
                            }
                        }
                        screen_shake_intensity = 8.0f;
                        break;
                    }
                }
            }

            if (player_lives <= 0)
            {
                current_game_state = GAME_OVER;
            }
            
            // Update the explosions
            for (int i = 0 ; i < MAX_EXPLOSIONS ; i++){
                if (explosions[i].active){
                    explosions[i].frame_time += delta_time; // Add the time passed since last frame
                    if (explosions[i].frame_time >= EXPLOSION_FRAME_DURATION){
                        explosions[i].current_frame++;
                        explosions[i].frame_time = 0.0f;
                        
                        if (explosions[i].current_frame >= EXPLOSION_FRAME_COUNT){
                            explosions[i].active = false;
                        }
                    }
                }
            }

            // Screen shake
            if (screen_shake_intensity > 0){

                screen_shake_offset.x = GetRandomValue(-screen_shake_intensity, screen_shake_intensity);
                screen_shake_offset.y = GetRandomValue(-screen_shake_intensity, screen_shake_intensity);
                
                screen_shake_intensity -= delta_time * 20.0f; // Decay over time
                if (screen_shake_intensity < 0) screen_shake_intensity = 0;
            }
            else screen_shake_offset = (Vector2){ 0.0f, 0.0f };

            break;

        case GAME_OVER:
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset the game
                InitGame(screen_width, screen_height);
                current_game_state = PLAYING;
            }
            break;
        
        default:
            break;
        }

        // Rendering part
        BeginDrawing();

        ClearBackground(BLACK);

        // Add the background
        DrawTexturePro(background,
                       (Rectangle){
                           0, 0, background.width, background.height},
                       (Rectangle){
                           0, 0, GetScreenWidth(), GetScreenHeight()},
                       (Vector2){
                           0, 0},
                       0,
                       WHITE);
                        
        // Push a translation matrix for screen shake
        rlPushMatrix();
        rlTranslatef(screen_shake_offset.x, screen_shake_offset.y , 0.0f);

        switch (current_game_state)
        {
        case MENU:
            
            DrawText("SPACE FIGHTER", screen_width / 2 - MeasureText("SPACE FIGHTER", 60) / 2, screen_height / 4, 60, RAYWHITE);
            
            Color game_hover_color = (current_game_menu == NEW_GAME) ? BLUE : RAYWHITE;
            DrawText("New Game", screen_width / 2 - MeasureText("New Game", 50) / 2, screen_height / 2, 50, game_hover_color);
            
            Color exit_hover_color = (current_game_menu == EXIT_GAME) ? BLUE : RAYWHITE;
            DrawText("Exit Game", screen_width / 2 - MeasureText("Exit Game", 30) / 2, screen_height / 2 + 60, 30, exit_hover_color);

            break;
        case PLAYING:
        case PAUSED:
            // Draw the player
            DrawTexturePro(player_texture,
                           (Rectangle){0, 0, player_texture.width, player_texture.height},
                           (Rectangle){
                               player.x, player.y, player.width, player.height},
                           (Vector2){
                               0, 0},
                           0.0f,
                           WHITE);
            // Draw the bullets
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    DrawTexturePro(bullet_texture,
                                   (Rectangle){0, 0, bullet_texture.width, bullet_texture.height},
                                   (Rectangle){
                                       bullets[i].rec.x, bullets[i].rec.y, bullets[i].rec.width, bullets[i].rec.height},
                                   (Vector2){
                                       0, 0},
                                   0.0f,
                                   bullets[i].color);
                }
            }

            // Draw collectable buff
            for (int i = 0 ; i < MAX_POWERUP ; i++){
                if (collectable[i].active){
                    switch (collectable[i].buff)
                    {
                        case SPEED_BOOST:
                            DrawTexturePro(speed_boost_texture,
                                           (Rectangle){0, 0, speed_boost_texture.width, speed_boost_texture.height},
                                           (Rectangle){collectable[i].rec.x, collectable[i].rec.y, collectable[i].rec.width, collectable[i].rec.height},
                                           (Vector2){0, 0},
                                           0.0f,
                                           WHITE);
                            break;
                        case BULLET_BOOST:
                            DrawTexturePro(bullet_boost_texture,
                                           (Rectangle){0, 0, bullet_boost_texture.width, bullet_boost_texture.height},
                                           (Rectangle){collectable[i].rec.x, collectable[i].rec.y, collectable[i].rec.width, collectable[i].rec.height},
                                           (Vector2){0, 0},
                                           0.0f,
                                           WHITE);
                            break;
                        default: 
                            break;
                    }
                }
            }
            // Draw the enemies
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    DrawTexturePro(enemy_texture,
                                   (Rectangle){0, 0, enemy_texture.width, enemy_texture.height},
                                   (Rectangle){
                                       enemies[i].rec.x, enemies[i].rec.y, enemies[i].rec.width, enemies[i].rec.height},
                                   (Vector2){
                                       0, 0},
                                   0.0f,
                                   enemies[i].color);
                }
            }

            // Draw Explosions
            for (int i = 0 ; i < MAX_EXPLOSIONS ; i++){
                if (explosions[i].active){
                    int frame_width = explosion_texture.width / EXPLOSION_FRAMES_PER_ROW;
                    int frame_height = explosion_texture.height / (EXPLOSION_FRAME_COUNT / EXPLOSION_FRAMES_PER_ROW);
                    int frame_col = explosions[i].current_frame % EXPLOSION_FRAMES_PER_ROW;
                    int frame_row = explosions[i].current_frame / EXPLOSION_FRAMES_PER_ROW;

                    Rectangle source_rec = {
                        (float) frame_col * frame_width,
                        (float) frame_row * frame_height,
                        (float) frame_width,
                        (float) frame_height
                    };

                    // Draw the current explosion frame
                    DrawTexturePro(explosion_texture,
                                   source_rec,
                                   explosions[i].rec,
                                   (Vector2){0, 0},
                                   0.0f,
                                   RAYWHITE);
                }
            }

            // Draw Hearts
            for (int i = 0; i < player_lives; i++)
            {
                DrawTexturePro(heart_texture, 
                    (Rectangle){
                        0, 0, heart_texture.width, heart_texture.height
                    },
                    (Rectangle){
                        heart[i].x, heart[i].y, heart[i].width, heart[i].height
                    },
                    (Vector2){
                        0, 0
                    },
                    0.0f,
                    RED);
            }

            DrawText(TextFormat("Score : %d ", score), 10, 10, 20, PINK);

            DrawFPS(screen_width - 100, 10);
            if (current_game_state == PAUSED)
            {
                DrawText("PAUSED", screen_width / 2 - MeasureText("PAUSED", 40) / 2, screen_height / 2 - 50, 40, YELLOW);

                Color game_hover_color = (current_paused_menu == RESUME) ? BLUE : RAYWHITE;
                DrawText("Resume", screen_width / 2 - MeasureText("Resume", 30) / 2, screen_height / 2, 30, game_hover_color);

                Color exit_hover_color = (current_paused_menu == EXIT) ? BLUE : RAYWHITE;
                DrawText("Exit Game", screen_width / 2 - MeasureText("Exit Game", 20) / 2, screen_height / 2 + 60, 20, exit_hover_color);
            }
            break;

        case GAME_OVER:
            DrawText("GAME OVER!", screen_width / 2 - MeasureText("GAME OVER!", 40) / 2, screen_height / 2 - 20, 40, RED);
            DrawText("Press ENTER to Restart", screen_width/ 2 - MeasureText("Press ENTER to Restart", 20) / 2,  screen_height/ 2 + 30, 20, RAYWHITE);
            break;
        default:
            break;
        }
        rlPopMatrix();

        EndDrawing();
    }
    UnloadTexture(background);
    UnloadGameAssets();
    CloseWindow();
}