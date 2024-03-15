#include <raylib.h>
#include <raymath.h>
#include <string>
#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"

const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const float FPS(60.0f);

int main() {
    int cam_type;
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];

    srand(time(nullptr));
    Player player({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, 25.0f, 200.0f);
    Enemy enemy({WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT - 50.0f}, 50.0f, 50.0f, 150.0f, 300.0f, 75.0f, 0.0f, 50.0f);

    camera_view.target = player.position;
    camera_view.offset = player.position;
    camera_view.zoom = 1.0;
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Homework 3");

    while(!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        player.Update(delta_time);
        enemy.Update(player, delta_time);

        camera_view.target = player.position;
        camera_view.offset = {static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) / 2};

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera_view);
        enemy.Draw();
        player.Draw();  
        EndMode2D();
        DrawText(("HP: " + std::to_string(player.hp)).c_str(), 10, 10, 20, WHITE);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
