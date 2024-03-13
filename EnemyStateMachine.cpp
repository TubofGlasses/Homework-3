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
    enemy.color = WHITE;
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

    if (!enemy.isChasing) {
         enemy.isChasing = CheckCollisionCircles(enemy.position, enemy.detection, player.position, player.radius);
        if (enemy.isChasing) {
            enemy.SetState(&enemy.chasing);
            return;
        }   
    }
    
    if (enemy.isChasing) {
        enemy.SetState(&enemy.chasing);
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
    Vector2 chase = Vector2Subtract(player.position, enemy.position);
    enemy.isChasing = CheckCollisionCircles(enemy.position, enemy.aggro, player.position, player.radius);
    if (!enemy.isChasing) {
        enemy.rotation = 0.0f;
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

// void EnemyReadying::Update(Enemy& enemy, Player& player,float delta_time) {
//     enemy.timer -= delta_time;
    
//     if (enemy.timer <= 0) {
        
//         enemy.timer = 0.2f;
//         enemy.timer2 = 0.19f;
//         enemy.SetState(&enemy.attacking);
//     }
//     // looks at player position
//     // swaps to attack once timer and ready movement is done
// }

// void EnemyAttacking::Update(Enemy& enemy, Player& player,float delta_time) {
//     // swaps to wandering once enemy dashes to last location it was looking at

//     Vector2 playerLastPosition;
//     Vector2 distance;

//     enemy.timer -= delta_time;
//     enemy.timer2 -= delta_time;

//     if (enemy.timer2 <= 0){
//         Vector2 playerLastPosition = player.position;
//         Vector2 distance = Vector2Subtract(playerLastPosition, enemy.position);
//         std::cout << "PREVIOUS X: " << playerLastPosition.x << std::endl;
//         std::cout << "PREVIOUS Y: " << playerLastPosition.y << std::endl;
//         if (enemy.timer <= 0) {
//             std::cout << "NEXT X: " << playerLastPosition.x << std::endl;
//             std::cout << "NEXT Y: " << playerLastPosition.y << std::endl;
//             enemy.position = Vector2Add(enemy.position, Vector2Scale(distance, enemy.speed * delta_time));
//             enemy.rotation = 0.0f;
//             enemy.SetState(&enemy.wandering);
//         }
//     }
// }

void EnemyReadying::Update(Enemy& enemy, Player& player, float delta_time) {
    enemy.timer -= delta_time;
    
    if (enemy.timer <= 0) {
        enemy.lastPlayerPosition = player.position; // Capture the last known position of the player
        enemy.SetState(&enemy.attacking);
    }
}

void EnemyAttacking::Update(Enemy& enemy, Player& player, float delta_time) {
    // Check if the attack cooldown has elapsed
    if (enemy.attackCooldown > 0) {
        enemy.attackCooldown -= delta_time;
        return;
    }

    // Move towards the last known position of the player
    Vector2 distance = Vector2Subtract(enemy.lastPlayerPosition, enemy.position);

    // Check if the enemy has reached the last known position of the player
    if (Vector2Length(distance) <= 0.1f) {
        // Enemy has reached the last known position, transition back to wandering state
        enemy.rotation = 0.0f;
        enemy.SetState(&enemy.wandering);
        return;
    }

    // Calculate movement direction towards the player
    Vector2 direction = Vector2Normalize(distance);

    // Adjust the movement speed based on whether the enemy is attacking or not
    float movementSpeed = enemy.speed * delta_time;
    if (enemy.isAttacking) {
        // Increase movement speed when attacking
        movementSpeed *= 10.0f; // Adjust the multiplier as needed
    }

    // Move towards the last known position of the player with adjusted speed
    enemy.position = Vector2Add(enemy.position, Vector2Scale(direction, movementSpeed));

    // Optionally, update rotation to face the last known position of the player
    enemy.rotation = atan2f(distance.y, distance.x) * RAD2DEG;

    // Perform attack action
    if (!enemy.isAttacking) {
        // Execute attack action here (e.g., play animation, deal damage)
        // Once the attack action is complete, set isAttacking to true
        enemy.isAttacking = true;

        // Reset attack cooldown after attack action is complete
        enemy.attackCooldown = enemy.attackCooldownDuration;
    } else {
        // Reset isAttacking after attack action is fully executed
        // This ensures that the attack action is only executed once per attack cooldown
        enemy.isAttacking = false;
    }
}


