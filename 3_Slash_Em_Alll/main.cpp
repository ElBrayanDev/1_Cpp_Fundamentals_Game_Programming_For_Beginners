#include "raylib.h"

int main()
{
    const int width{384};
    const int height{384};
    InitWindow(width, height, "Slash E'm All");

    Texture2D map = LoadTexture("nature_tileset\world_map.png");

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // # LOGIC

        DrawTexture(map, 0, 0, WHITE);

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}