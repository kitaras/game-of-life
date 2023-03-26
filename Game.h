#pragma once
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>

class Game {
private:
    std::vector<std::vector<bool>> cells;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;

    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
};