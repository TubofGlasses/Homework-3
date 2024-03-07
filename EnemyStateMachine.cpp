#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>

#include "Enemy.hpp"
#include "Player.hpp"
// we can remove this if not needed \/ \/

void Enemy::Update(Player& player, float delta_time) {
    current_state->Update(*this, player, delta_time);
}

//Not done
void Enemy::Draw() {
    DrawCircleLines(static_cast<int>(position.x), static_cast<int>(position.y), aggro, BLUE);
    DrawCircleLines(static_cast<int>(position.x), static_cast<int>(position.y), detection, GREEN);
    DrawCircleLines(static_cast<int>(position.x), static_cast<int>(position.y), attack, RED);
    DrawRectanglePro(Rectangle{position.x, position.y, width, height}, Vector2{width / 2, height / 2}, rotation, color);
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

void EnemyWandering::Update(Enemy& enemy, Player& player,float delta_time) {
    enemy.directionChangeTimer += delta_time;

    bool isColliding = CheckCollisionCircles(enemy.position, enemy.detection, player.position, player.radius);
    if (isColliding) {
        enemy.SetState(&enemy.chasing);
        return;
    }

    if (enemy.directionChangeTimer >= enemy.directionChangeInterval ||
        enemy.position.x <= 0 || enemy.position.x >= 800 ||
        enemy.position.y <= 0 || enemy.position.y >= 600) {

        enemy.directionChangeTimer = 0.0f;

        // screen bounds
        if (enemy.position.x <= 0 || enemy.position.x >= 800) {
            enemy.velocity.x *= -1;
        }
        if (enemy.position.y <= 0 || enemy.position.y >= 600) {
            enemy.velocity.y *= -1;
        }

        // random direction
        if (enemy.position.x > 0 && enemy.position.x < 800 &&
            enemy.position.y > 0 && enemy.position.y < 600) {
            float randomAngle = (rand() % 360) * DEG2RAD; // Random angle in radians
            Vector2 randomDirection = {cos(randomAngle), sin(randomAngle)};
            // Update enemy velocity based on random direction and speed
            enemy.velocity = Vector2Scale(randomDirection, enemy.speed);
        }
    }

    // Move the enemy based on its velocity
    enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.velocity, delta_time));

    // Ensure the enemy stays within the screen bounds
    enemy.position.x = fmax(0, fmin(enemy.position.x, 800));
    enemy.position.y = fmax(0, fmin(enemy.position.y, 600));
}

void EnemyChasing::Update(Enemy& enemy, Player& player,float delta_time) {
    // chases player and rotates body to face player direction
    // swaps to wandering if player is outside aggro zone
    // swaps to readying when in attack zone.=
    bool isColliding = CheckCollisionCircles(enemy.position, enemy.detection, player.position, player.radius);
    if (!isColliding) {
        enemy.SetState(&enemy.wandering);
        return;
    }
}

void EnemyReadying::Update(Enemy& enemy, Player& player,float delta_time) {
    // looks at player position
    // swaps to attack once timer and ready movement is done
}

void EnemyAttacking::Update(Enemy& enemy, Player& player,float delta_time) {
    // swaps to wandering once enemy dashes to last location it was looking at
}