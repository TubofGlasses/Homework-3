#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Enemy.hpp"

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

}

void EnemyChasing::Update(Enemy& enemy, float delta_time) {
    
}

void EnemyReadying::Update(Enemy& enemy, float delta_time) {
    
}

void EnemyAttacking::Update(Enemy& enemy, float delta_time) {
    
}