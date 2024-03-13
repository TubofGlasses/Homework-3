#ifndef SCREENS
#define SCREENS

#include <functional>

enum class ScreenType {
    Title,
    Game,
    Win,
    Lose
};

class Screens {
public:
    Screens();

    void SetScreen(ScreenType screen);

    void Update(float delta_time);

    void Draw();

    void OnWin(std::function<void()> callback);

    void OnLose(std::function<void()> callback);

private:
    ScreenType current_screen;
    std::function<void()> win_callback;
    std::function<void()> lose_callback;
};

#endif SCREENS
