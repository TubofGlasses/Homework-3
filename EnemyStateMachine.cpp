#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Enemy.hpp"
// we can remove this if not needed \/ \/

void Enemy::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

//Not done
void Enemy::Draw() {
    DrawRectanglePro(Rectangle{position.x, position.y, width, height}, Vector2{position.x + width / 2, position.y + height / 2}, rotation, color);
    // DrawCircle(position, detection, color);
    // Another DrawCircle for aggro
    // Another DrawCircle for attack
}

void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

//Needs fields for DrawRectanglePro
Enemy::Enemy(Vector2 pos, float wdth, float hgt, float dct, float agr, float atk, float rot, float spd) {
    position = pos;
    width = wdth;
    height = hgt;
    detection = dct;
    aggro = agr;
    attack = atk;
    rotation = rot;
    speed = spd;
    SetState(&wandering);
}

void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = YELLOW;
}

void EnemyChasing::Enter(Enemy& enemy) {
    enemy.color = ORANGE;
}

void EnemyReadying::Enter(Enemy& enemy) {
    enemy.color = BROWN;
}

void EnemyAttacking::Enter(Enemy& enemy) {
    enemy.color = RED;
}

void EnemyWandering::Update(Enemy& enemy, float delta_time) {
    // random movement
    // default, swaps to chasing.
    std::cout << "E Position X: " << enemy.position.x << std::endl;
    std::cout << "E Postion Y: " << enemy.position.y << std::endl;
    Vector2 direction = {0.0f, 0.0f};

    if (IsKeyDown(KEY_UP)) {
        direction.y -= 1.0f;
    }
    if (IsKeyDown(KEY_DOWN)) {
        direction.y += 1.0f;
    }
    if (IsKeyDown(KEY_LEFT)) {
        direction.x -= 1.0f;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        direction.x += 1.0f;
    }

    if (Vector2Length(direction) > 0) {
        direction = Vector2Normalize(direction);
        Vector2 movement = Vector2Scale(direction, enemy.speed * delta_time);
        enemy.position = Vector2Add(enemy.position, movement);
        enemy.velocity = movement;
    } else {
        enemy.velocity = {0.0f, 0.0f};
    }
}

void EnemyChasing::Update(Enemy& enemy, float delta_time) {
    // chases player and rotates body to face player direction
    // swaps to wandering if player is outside aggro zone
    // swaps to readying when in attack zone.=
}

void EnemyReadying::Update(Enemy& enemy, float delta_time) {
    // looks at player position
    // swaps to attack once timer and ready movement is done
}

void EnemyAttacking::Update(Enemy& enemy, float delta_time) {
    // swaps to wandering once enemy dashes to last location it was looking at
}