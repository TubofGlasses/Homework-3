#ifndef ENEMY
#define ENEMY

#include <raylib.h>
#include <raymath.h>

class Enemy;

class EnemyState {
    public:
        virtual ~EnemyState() {} // desctructor called when obj is deleted.
        virtual void Enter(Enemy& enemy) = 0;
        virtual void Update(Enemy& enemy, float delta_time) = 0;
};

class EnemyWandering : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, float delta_time);
};

class EnemyChasing : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, float delta_time);
};

class EnemyReadying : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, float delta_time);
};

class EnemyAttacking : public EnemyState {
    public:
        void Enter(Enemy& enemy);
        void Update(Enemy& enemy, float delta_time);
};

class Enemy {
    public:
        Vector2 position;
        float radius;
        Color color;

        Vector2 velocity;
        Vector2 prevPos;
        float speed;
        float timer;

        Enemy(Vector2 pos, float rad, float spd);

        void Update(float delta_time);

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