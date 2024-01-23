#include "BaseCharacter.h"
#include "raylib.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{
}

BaseCharacter::~BaseCharacter()
{
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{getScreenPos().x, getScreenPos().y, (width * scale) - 10, (height * scale) - 10};
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames)
            frame = 0;
    }

    // update movement
    if (Vector2Length(velocity) != 0.0)
    {
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed)); // set worldPos = worldPos + movement

        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;

        texture = run;
    }
    else
    {
        texture = idle;
    }
    velocity = {};

    // * Character

    Rectangle source{width * frame, 0.f, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}
