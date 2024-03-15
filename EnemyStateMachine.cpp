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
    hp = 2;

    float randomAngle = (rand() % 360) * DEG2RAD;
    velocity = {cos(randomAngle) * speed, sin(randomAngle) * speed};

    SetState(&wandering);
}

void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = WHITE;

    // Initialize velocity with a random direction upon entering the wandering state
    float randomAngle = (rand() % 360) * DEG2RAD;
    enemy.velocity = {cos(randomAngle) * enemy.speed, sin(randomAngle) * enemy.speed};
}
void EnemyChasing::Enter(Enemy& enemy) {
    enemy.color = ORANGE;
}

void EnemyReadying::Enter(Enemy& enemy) {
    enemy.color = BROWN;
}

void EnemyAttacking::Enter(Enemy& enemy) {
    enemy.hasDealtDamage = false;
    enemy.color = RED;
    
}

void EnemyWandering::Update(Enemy& enemy, Player& player, float delta_time) {
    enemy.directionChangeTimer += delta_time;
    enemy.isChasing = false;

    if (enemy.rotation != 0) {
        float rotationResetSpeed = 180.0f; // Degrees per second, adjust as needed
        if (enemy.rotation > rotationResetSpeed * delta_time) {
            enemy.rotation -= rotationResetSpeed * delta_time;
        } else if (enemy.rotation < -rotationResetSpeed * delta_time) {
            enemy.rotation += rotationResetSpeed * delta_time;
        } else {
            enemy.rotation = 0;
        }
    }
    // Check if the enemy should start chasing the player
    if (!enemy.isChasing) {
        enemy.isChasing = CheckCollisionCircles(enemy.position, enemy.detection, player.position, player.radius);
        if (enemy.isChasing) {
            enemy.SetState(&enemy.chasing);
            return;
        }
    }

    // Change direction at regular intervals or if hitting screen bounds
    if (enemy.directionChangeTimer >= enemy.directionChangeInterval) {
        float randomAngle = (rand() % 360) * DEG2RAD; // Random angle in radians
        Vector2 randomDirection = {cos(randomAngle), sin(randomAngle)};
        enemy.velocity = Vector2Scale(randomDirection, enemy.speed); // Apply new velocity
        enemy.directionChangeTimer = 0.0f; // Reset timer

    //     // Reverse direction if hitting screen bounds
    //     if (enemy.position.x <= 0 || enemy.position.x >= GetScreenWidth()) {
    //         enemy.velocity.x *= -1;
    //     }
    //     if (enemy.position.y <= 0 || enemy.position.y >= GetScreenHeight()) {
    //         enemy.velocity.y *= -1;
    //     }

    //     // Choose a new random direction if not at bounds
    //     if (enemy.position.x > 0 && enemy.position.x < GetScreenWidth() &&
    //         enemy.position.y > 0 && enemy.position.y < GetScreenHeight()) {
    //         float randomAngle = (rand() % 360) * DEG2RAD; // Random angle in radians
    //         Vector2 randomDirection = {cos(randomAngle), sin(randomAngle)};
    //         enemy.velocity = Vector2Scale(randomDirection, enemy.speed); // Apply new velocity
    //     }
        }

    // Move the enemy based on its velocity
    enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.velocity, delta_time));

    // Ensure the enemy stays within the screen bounds
    // enemy.position.x = fmax(0, fmin(enemy.position.x, GetScreenWidth()));
    // enemy.position.y = fmax(0, fmin(enemy.position.y, GetScreenHeight()));
}

void EnemyChasing::Update(Enemy& enemy, Player& player,float delta_time) {
    // chases player and rotates body to face player direction
    // swaps to wandering if player is outside aggro zone
    // swaps to readying when in attack zone.=
    Vector2 chase = Vector2Subtract(player.position, enemy.position);
    enemy.isChasing = CheckCollisionCircles(enemy.position, enemy.aggro, player.position, player.radius);
    if (!enemy.isChasing) {
        enemy.SetState(&enemy.wandering);
        return;
    }

    if (Vector2Length(chase) > 0) {
        chase = Vector2Normalize(chase);
        enemy.position = Vector2Add(enemy.position, Vector2Scale(chase, enemy.speed * delta_time));
    }

    // Optionally, set enemy rotation to face the player
    enemy.rotation = atan2f(chase.y, chase.x) * RAD2DEG;

    bool isAttacking = CheckCollisionCircles(enemy.position, enemy.attack, player.position, player.radius);
    if (isAttacking) {
        enemy.SetState(&enemy.readying);
        enemy.timer = 1.0f;
        return;
    }
}

void EnemyReadying::Update(Enemy& enemy, Player& player, float delta_time) {
    enemy.timer -= delta_time;
    Vector2 chase = Vector2Subtract(player.position, enemy.position);

    enemy.rotation = atan2f(chase.y, chase.x) * RAD2DEG;
    
    if (enemy.timer <= 0) {
        enemy.lastPlayerPosition = player.position; // Capture the last known position of the player
        enemy.timer = 0.6f;
        enemy.SetState(&enemy.attacking);
    }
}

void EnemyAttacking::Update(Enemy& enemy, Player& player, float delta_time) {
    if (enemy.attackCooldown > 0) {
        enemy.attackCooldown -= delta_time;
        return;
    }

    Vector2 attackDirection = {cosf(enemy.rotation * DEG2RAD), sinf(enemy.rotation * DEG2RAD)};

    attackDirection = Vector2Normalize(attackDirection);

    float attackSpeedMultiplier = 10.0f;
    float increasedSpeed = enemy.speed * attackSpeedMultiplier;

    enemy.position = Vector2Add(enemy.position, Vector2Scale(attackDirection, increasedSpeed * delta_time));

    if (CheckCollisionCircles(enemy.position, enemy.width / 2, player.position, player.radius) && !enemy.hasDealtDamage) {
        float damage = 1.0f; 

        if (player.isBlocking) {
            damage = 0.5f;
        } else if (player.isDodging) {
            damage = 0.0f;
        }

        player.hp -= damage;
        enemy.hasDealtDamage = true;

        return;
    }

    enemy.timer -= delta_time;
    if (enemy.timer <= 0) {
        enemy.SetState(&enemy.wandering);
        enemy.attackCooldown = enemy.attackCooldownDuration;
    }

    enemy.isAttacking = false;
}
