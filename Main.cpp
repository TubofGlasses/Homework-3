#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"

const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const float FPS(60.0f);

int main() {
    srand(time(nullptr));
    Player player({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, 25.0f, 200.0f);
    Enemy enemy({WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT - 50.0f}, 50.0f, 50.0f, 150.0f, 300.0f, 75.0f, 0.0f, 50.0f);
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Homework 3");

    while(!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        player.Update(enemy, delta_time);
        enemy.Update(player, delta_time);

        BeginDrawing();
        ClearBackground(BLACK);
        enemy.Draw();
        player.Draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
