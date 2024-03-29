#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>

#include "Enemy.hpp"

class Enemy;
class Player;

class PlayerState {
    public:
        virtual ~PlayerState() {} // desctructor called when obj is deleted.
        virtual void Enter(Player& player) = 0;
        virtual void Update(Player& player, Enemy& enemy, float delta_time) = 0;
};

class PlayerIdle : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, Enemy& enemy, float delta_time);
};

class PlayerMoving : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, Enemy& enemy, float delta_time);
};

class PlayerBlocking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, Enemy& enemy, float delta_time);
};

class PlayerDodging : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, Enemy& enemy, float delta_time);
};

class PlayerAttacking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, Enemy& enemy, float delta_time);
};

class Player {
    public:
        Vector2 position;
        float radius;

        float hp;
        Color color;

        Vector2 velocity;
        Vector2 prevPos;
        float speed;
        float timer;

        bool isBlocking;
        bool isDodging;
        bool hasDamaged;

        Player(Vector2 pos, float rad, float spd);

        void Update(Enemy& enemy, float delta_time);

        void Draw();

        void SetState(PlayerState* new_state);

        PlayerIdle idle;
        PlayerMoving moving;
        PlayerBlocking blocking;
        PlayerDodging dodging;
        PlayerAttacking attacking;

    private:
        PlayerState* current_state;
};

#endif