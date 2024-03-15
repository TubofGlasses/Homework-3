#ifndef ENEMY
#define ENEMY

#include <raylib.h>
#include <raymath.h>

#include "Player.hpp"

class Enemy;

class EnemyState {
    public:
        virtual ~EnemyState() {} // desctructor called when obj is deleted.
        virtual void Enter(Enemy& enemy) = 0;
        virtual void Update(Enemy& enemy, Player& player, float delta_time) = 0;
};

class EnemyWandering : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, Player& player, float delta_time);
};

class EnemyChasing : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, Player& player, float delta_time);
};

class EnemyReadying : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, Player& player, float delta_time);
};

class EnemyAttacking : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, Player& player, float delta_time);
};

class Enemy {
    public:
        Vector2 position;
        float detection;
        float aggro;
        float attack;
        float width;
        float height;
        float rotation;

        float hp;
        Color color;

        Vector2 velocity;
        Vector2 prevPos;
        float speed;
        float timer;
        float timer2;
        
        float directionChangeTimer = 0.0f; // Timer to track time since last direction change
        const float directionChangeInterval = 5.0f;

        Vector2 lastPlayerPosition;
        float attackCooldown; // Cooldown time for attacks
        float attackCooldownDuration; // Duration of the attack cooldown

        bool isChasing;
        bool isAttacking;

        Enemy(Vector2 pos, float wdth, float hgt, float dct, float agr, float atk, float rot, float spd);

        void Update(Player& player, float delta_time);

        void Draw();

        void SetState(EnemyState* new_state);

        EnemyWandering wandering;
        EnemyChasing chasing;
        EnemyReadying readying;
        EnemyAttacking attacking;

    private:
        EnemyState* current_state;
};

#endif