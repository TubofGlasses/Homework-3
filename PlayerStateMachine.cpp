#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Player.hpp"

void Player::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

void Player::Draw() {
    DrawCircleV(position, radius, color);
}

void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

Player::Player(Vector2 pos, float rad, float spd) {
    position = pos;
    radius = rad;
    speed = spd;
    SetState(&idle);
}

void PlayerIdle::Enter(Player& player) {
    player.color = ORANGE;
}

void PlayerMoving::Enter(Player& player) {
    player.color = WHITE;
}

void PlayerBlocking::Enter(Player& player) {
    player.color = GRAY;
}

void PlayerDodging::Enter(Player& player) {
    player.color = RED;
}

void PlayerAttacking::Enter(Player& player) {
    player.color = YELLOW;
}

void PlayerIdle::Update(Player& player, float delta_time) {
    player.prevPos = player.position;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
        player.timer = 0.2;
        player.SetState(&player.moving);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        player.timer = 0.5;
        player.SetState(&player.attacking);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        player.isBlocking = true;
        player.SetState(&player.blocking);
    }
}

void PlayerMoving::Update(Player& player, float delta_time) {
    if (player.timer >= 0) {
        player.timer -= delta_time;
    }
    if (player.timer <= 0) {
        player.prevPos = player.position;
        player.timer = 0.2f;
    }
    Vector2 direction = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) {
        direction.y -= 1.0f;
    }
    if (IsKeyDown(KEY_S)) {
        direction.y += 1.0f;
    }
    if (IsKeyDown(KEY_A)) {
        direction.x -= 1.0f;
    }
    if (IsKeyDown(KEY_D)) {
        direction.x += 1.0f;
    }

    if (Vector2Length(direction) > 0) {
        direction = Vector2Normalize(direction);
        Vector2 movement = Vector2Scale(direction, player.speed * delta_time);
        player.position = Vector2Add(player.position, movement);
        player.velocity = movement;
    } else {
        player.velocity = {0.0f, 0.0f};
    }
    if (Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        player.timer = 0.2f;
        player.SetState(&player.dodging);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        player.timer = 0.5;
        player.SetState(&player.attacking);
    }
}

void PlayerBlocking::Update(Player& player, float delta_time) {
    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && player.isBlocking == true) {
        player.isBlocking = false;
        player.SetState(&player.idle);
    }
}

void PlayerDodging::Update(Player& player, float delta_time) {
    // std::cout << "timer " << player.timer << " speed " <<player.speed << std::endl;
    if (player.timer >= 0) {
        Vector2 direction = {0.0f, 0.0f};
        player.speed = 1000.0f;
        player.timer -= delta_time;
        if (player.prevPos.y < player.position.y) {
            direction.y += 1.0f;
        }
        if (player.prevPos.x < player.position.x) {
            direction.x += 1.0f;
        }
        if (player.prevPos.y > player.position.y) {
            direction.y -= 1.0f;
        }
        if (player.prevPos.x > player.position.x) {
            direction.x -= 1.0f;
        }
        if (Vector2Length(direction) > 0) {
        direction = Vector2Normalize(direction);
        Vector2 movement = Vector2Scale(direction, player.speed * delta_time);
        player.position = Vector2Add(player.position, movement);
    }
    }
    if (player.timer <= 0) {
        player.speed = 200.0f;
        player.SetState(&player.idle);
    }
}

void PlayerAttacking::Update(Player& player, float delta_time) {
    if (player.timer >= 0) {
        player.timer -= delta_time;
    }
    if (player.timer <= 0) {
        player.SetState(&player.idle);
    }
}