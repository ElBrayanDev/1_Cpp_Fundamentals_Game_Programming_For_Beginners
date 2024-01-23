#ifndef PROP_H
#define PROP_H

#pragma once
#include "raylib.h"

class Prop
{
public:
    Prop(Vector2, Texture2D);
    ~Prop();
    void render(Vector2);
    Rectangle getCollisionRec(Vector2);

private:
    Vector2 worldPos{};
    Texture2D texture{};
    float scale{4.f};
};

#endif