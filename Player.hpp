#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>

class Player;

class PlayerState {
    public:
        virtual ~PlayerState() {} // desctructor called when obj is deleted.
        virtual void Enter(Player& player) = 0;
        virtual void Update(Player& player, float delta_time) = 0;
};

class PlayerIdle : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerMoving : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerBlocking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerDodging : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerAttacking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class Player {
    public:
        Vector2 position;
        float radius;

        int hp;
        Color color;

        Vector2 velocity;
        Vector2 prevPos;
        float speed;
        float timer;

        bool isBlocking;

        Player(Vector2 pos, float rad, float spd);

        void Update(float delta_time);

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