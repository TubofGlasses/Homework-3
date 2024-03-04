#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Enemy.hpp"
// we can remove this if not needed \/ \/
#include "Player.hpp"
#include "PlayerStateMachine.cpp"

void Enemy::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

//Not done
void Enemy::Draw() {
    DrawRectanglePro();
    // DrawCircle(position, detection, color);
    // Another DrawCircle for aggro
    // Another DrawCircle for attack
}

void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

//Needs fields for DrawRectanglePro
Enemy::Enemy(Vector2 pos, float wdth, float hgt, float dct, float agr, float atk, float spd) {
    width = wdth;
    height = hgt;
    detection = dct;
    aggro = agr;
    attack = atk;
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