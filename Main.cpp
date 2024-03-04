#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"

const int WINDOW_WIDTH(800);
const int WINDOW_HEIGHT(600);
const float FPS(60.0f);

int main() {
    Player player({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, 50.0f, 200.0f);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Homework 3");

    while(!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        player.Update(delta_time);

        BeginDrawing();
        ClearBackground(BLACK);
        player.Draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
