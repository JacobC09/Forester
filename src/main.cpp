#include "common.h"
#include "raylib.h"
#include "game.h"
#include "textures.h"

int main() {
    InitWindow(960, 540, "Window");
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    LoadTextures();

    Game* game = new Game();

    game->Init();
    
    while (!WindowShouldClose()) {
        game->Update();

        BeginDrawing();
            ClearBackground(Color { 40, 181, 70, 255 });
            game->Draw();
        EndDrawing();
    }

    UnloadTextures();
    CloseWindow();

    return 0;
}