#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

struct AnimData
{
    Rectangle box;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

const bool isOnGround(AnimData data, int window_height)
{
    return data.pos.y >= window_height - data.box.height - 10;
}

void updateAnimData(AnimData &data)
{
    data.runningTime = 0.0;
    // update animation frame
    data.box.x = data.frame * data.box.width; // x = frame * width_sprite/6
    data.frame++;

    if (data.frame >= 6)
    {
        data.frame = 0;
    }
}

void updateNebData(AnimData &nebData)
{
    if (nebData.runningTime >= nebData.updateTime)
    {
        nebData.runningTime = 0.0;
        // update animation frame
        nebData.box.x = nebData.frame * nebData.box.width; // x = frame * width_sprite/6
        nebData.frame++;

        if (nebData.frame >= 8)
        {
            nebData.frame = 0;
        }
    }
}

int main()
{
    const int window_width{1920};
    const int window_height{1080};

    int score{0};
    std::string scoreText = "Score: ";

    InitWindow(window_width, window_height, "GhostRunner Lite");

    // # Runner

    Texture2D runner = LoadTexture("Resources/Runner.png");
    AnimData runnerData;
    runnerData.box.x = 0;
    runnerData.box.y = 0;
    runnerData.box.width = runner.width / 6;
    runnerData.box.height = runner.height;

    runnerData.pos.x = window_width / 4 - runnerData.box.width / 2;
    runnerData.pos.y = window_height - runnerData.box.height - 10;

    runnerData.frame = 0;               // Move along sprite sheet
    runnerData.runningTime = 0;         // Delta time, time since last frame
    runnerData.updateTime = 1.0 / 15.0; // 15 frames per second on the sprite

    const int gravity{1'200}; // acceleration due to gravity (pixels/second)/second
    int velocity{0};
    const int JUMPVELOCITY{-600}; // pixels per second
    bool isInAir{false};

    // # Nebula

    Texture2D nebula = LoadTexture("Resources/12_nebula_spritesheet.png");

    AnimData nebulaArray[1000]{};
    int nebulaArraySize = int(sizeof(nebulaArray) / sizeof(nebulaArray[0]));
    for (int i = 0; i < nebulaArraySize; i++)
    {
        nebulaArray[i].box.x = 0.0;
        nebulaArray[i].box.y = 0.0;
        nebulaArray[i].box.width = nebula.width / 8;
        nebulaArray[i].box.height = nebula.height / 8;
        nebulaArray[i].pos.y = window_height - nebula.height / 8;
        nebulaArray[i].frame = 0;
        nebulaArray[i].runningTime = 0.0;
        nebulaArray[i].updateTime = 1.0 / 14.0;

        float random = 0.7 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.3 - 0.7)));
        nebulaArray[i].pos.x = nebulaArray[i - 1].pos.x + (750 * random); // add the next nebula 'x' distance from the one before
    }

    float finishLine{nebulaArray[nebulaArraySize - 1].pos.x};

    int NEBULOSAVELOCITY{-500}; // Nebula x velocity 600 pixels/second

    Texture2D background = LoadTexture("Resources/far-buildings.png");
    Texture2D midground = LoadTexture("Resources/midground.png");
    Texture2D foreground = LoadTexture("Resources/foreground.png");
    float bg1X{};
    float mg1X{};
    float fg1X{};

    bool collision{};
    float errorNeb{50}; // Error on nebula png image (pixels)

    double executionTime{};

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        const float dT{GetFrameTime()}; //! Delta time, adjust movement according to time, not frames
        executionTime += dT;
        if (executionTime >= 1)
        {
            NEBULOSAVELOCITY -= 0.1;
            score += 100;
            // scoreText.append(std::to_string(score), 0, 5);
            executionTime = 0;
        }

        BeginDrawing();
        ClearBackground(GRAY);

        // * Background

        if (bg1X <= -background.width * (float(window_width) / float(background.width)))
        {
            bg1X = 0.0;
        }

        if (mg1X <= -midground.width * float((float(window_width) / float(midground.width)) - 0.25))
        {
            mg1X = 0.0;
        }

        if (fg1X <= -foreground.width * float((float(window_width) / float(foreground.width)) + 0.35))
        {
            fg1X = 0.0;
        }

        //! Move background in x by 60 pixels per second (1080p)
        bg1X -= (window_height / 15) * dT;
        mg1X -= (window_height / 12) * dT;
        fg1X -= (window_height / 8) * dT;

        Vector2 bg1Pos{bg1X, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, (float(window_width) / float(background.width)), WHITE); // 7.5 because 256px * 7.5 = 1920
        Vector2 bg2Pos{bg1X + background.width * (float(window_width) / float(background.width)), 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, (float(window_width) / float(background.width)), WHITE);

        Vector2 mg1Pos{mg1X, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, float((float(window_width) / float(midground.width)) - 0.25), WHITE); // 7.5 because 256px * 7.5 = 1920
        Vector2 mg2Pos{mg1X + midground.width * float((float(window_width) / float(midground.width)) - 0.25), 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, float((float(window_width) / float(midground.width)) - 0.25), WHITE);

        Vector2 fg1Pos{fg1X, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, (float(window_width) / float(foreground.width)) + 0.35, WHITE); // 7.5 because 256px * 7.5 = 1920
        Vector2 fg2Pos{fg1X + foreground.width * float((float(window_width) / float(foreground.width)) + 0.35), 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, (float(window_width) / float(foreground.width)) + 0.35, WHITE);

        // # LOGIC

        // Gravity effect
        if (isOnGround(runnerData, window_height)) // on ground
        {
            isInAir = false;
            velocity = 0;
        }
        else // on air
        {
            isInAir = true;
            velocity += gravity * dT; // Reduce velocity due to gravity
        }

        // Jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += JUMPVELOCITY;
        }

        // update nebulas position
        for (int i = 0; i < nebulaArraySize; i++)
        {
            nebulaArray[i].pos.x += NEBULOSAVELOCITY * dT;
        }

        //! update finish line
        finishLine += NEBULOSAVELOCITY * dT;

        // update runner position
        runnerData.pos.y += velocity * dT;

        //* Running animation
        runnerData.runningTime += dT;
        if (runnerData.runningTime >= runnerData.updateTime && !isInAir)
        {
            updateAnimData(runnerData);
        }

        //* Nebulas animation

        for (int i = 0; i < nebulaArraySize; i++)
        {
            nebulaArray[i].runningTime += dT;
            updateNebData(nebulaArray[i]);
        }

        for (AnimData nebula : nebulaArray)
        {
            Rectangle nebRec{
                nebula.pos.x + errorNeb,
                nebula.pos.y + errorNeb,
                nebula.box.width - 2 * errorNeb,
                nebula.box.height - 2 * errorNeb,
            };

            Rectangle runnerRec{
                runnerData.pos.x,
                runnerData.pos.y,
                runnerData.box.width,
                runnerData.box.height,
            };

            if (CheckCollisionRecs(nebRec, runnerRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("GAME OVER", window_width / 2, window_height / 2, 50, RED);
        }
        else if (runnerData.pos.x >= finishLine + 300)
        {
            //* You Win, crossed the finishLine
            DrawText("YOU WIN", window_width / 2, window_height / 2, 50, RED);
        }
        else
        {

            DrawTextureRec(runner, runnerData.box, runnerData.pos, WHITE);

            for (int i = 0; i < nebulaArraySize; i++)
            {
                DrawTextureRec(nebula, nebulaArray[i].box, nebulaArray[i].pos, WHITE);
            }
        }
        DrawText((scoreText + std::to_string(score)).c_str(), 10, 10, 50, BLUE);
        EndDrawing();
    }
    UnloadTexture(runner); // Unload texture properly
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}