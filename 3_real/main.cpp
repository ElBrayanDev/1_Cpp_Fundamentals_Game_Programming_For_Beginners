#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

using std::string;

int main()
{
    const int window_width{720};
    const int window_height{720};
    InitWindow(window_width, window_height, "Slash E'm All");

    Texture2D map = LoadTexture("nature_tileset/world_map.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0};

    Character knight{window_width, window_height};

    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}};

    Enemy goblin{
        Vector2{800.f, 300.f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy slime{
        Vector2{0.f, 0.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    goblin.setTarget(&knight);

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        // # LOGIC

        // * Map

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        for (auto prop : props)
        {
            prop.render(knight.getWorldPos());
        }

        // * character death

        if (!knight.getAlive())
        {
            DrawText("Game Over", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            string knightHealth{"Health: "};
            knightHealth.append(std::to_string(knight.getHealth()), 0, 3);
            DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        knight.tick(GetFrameTime());

        // Check map bounds
        if (knight.getWorldPos().x < 0 ||
            knight.getWorldPos().y < 0 ||
            knight.getWorldPos().x + window_width > map.width * mapScale ||
            knight.getWorldPos().y + window_height > map.height * mapScale)
        {
            knight.undoMovement();
        }

        for (auto prop : props)
        {
            if (CheckCollisionRecs(knight.getCollisionRec(), prop.getCollisionRec(knight.getWorldPos())))
            {
                knight.undoMovement();
            }
        }

        // * Enemies

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        // * Damage

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}