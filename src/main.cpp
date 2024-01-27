#include "common.h"
#include "raylib.h"
#include "game.h"
#include "assets.h"

#include "debug.h"
int main() {
    InitWindow(960, 540, "Window");
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    LoadAssets();

    Game* game = new Game();
    
    game->Init();
    
    while (!WindowShouldClose()) {
        game->Update();

        BeginDrawing();
            ClearBackground(BLACK);
            game->Draw();
        EndDrawing();
    }

    UnloadAssets();
    CloseWindow();

    return 0;
}