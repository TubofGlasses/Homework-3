#include "Screens.hpp"
#include "raylib.h"

Screens::Screens() : current_screen(ScreenType::Title) {}

void Screens::SetScreen(ScreenType screen) {
    current_screen = screen;
}

void Screens::Update(float delta_time) {
    // Update logic for different screens can go here
}

void Screens::Draw() {
    // Draw logic for different screens can go here
}

void Screens::OnWin(std::function<void()> callback) {
    win_callback = callback;
}

void Screens::OnLose(std::function<void()> callback) {
    lose_callback = callback;
}
