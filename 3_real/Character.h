#ifndef CHARACTER_H
#define CHARACTER_H

#pragma once
#include "raylib.h"
#include "raymath.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character(int, int);

    virtual void tick(float) override;

    // * setter/getter
    virtual Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec() { return weaponCollisionRec; };
    float getHealth() const { return health; }; // const getters
    void takeDamage(float damage);

    ~Character();

private:
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{LoadTexture("characters/weapon_sword.png")};
    Rectangle weaponCollisionRec{};

    float health{100.f};
};

#endif