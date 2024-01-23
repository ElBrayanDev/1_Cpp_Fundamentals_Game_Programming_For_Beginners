#ifndef BASECHARACTER_H
#define BASECHARACTER_H

#pragma once
#include "raylib.h"

class BaseCharacter
{
public:
    BaseCharacter();

    Vector2 getWorldPos() { return worldPos; };
    void undoMovement();
    Rectangle getCollisionRec();
    //! Virtual
    virtual void tick(float deltaTime);
    virtual Vector2 getScreenPos() = 0;

    //* Setter/getter

    bool getAlive() { return alive; };
    void setAlive(bool isAlive) { alive = isAlive; };

    ~BaseCharacter();

private:
    bool alive{true};

protected:
    Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
    Vector2 worldPos{};

    Vector2 worldPosLastFrame{};

    float speed{4.0};

    float width{};
    float height{};
    float scale{4.0f};

    // * Animation variables

    float runningTime{};
    int frame{};
    const int maxFrames{6};
    const float updateTime{1.f / 12.f};
    float rightLeft{1.f}; // 1: facing right, -1 facing left
    Vector2 velocity{};
};

#endif