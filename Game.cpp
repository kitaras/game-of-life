#include "Game.h"
#include "SDL_render.h"

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true) {

}

bool Game::Initialize() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create an SDL Window
    mWindow = SDL_CreateWindow(
            "Game of life", // Window title
            100,    // Top left x-coordinate of window
            100,    // Top left y-coordinate of window
            1024,    // Width of window
            768,    // Height of window
            0        // Flags (0 for no flags set)
    );

    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    //// Create SDL renderer
    mRenderer = SDL_CreateRenderer(
            mWindow, // Window to create renderer for
            -1,         // Usually -1
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    cells = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE) mIsPaused = !mIsPaused;
        }
    }
}

void Game::UpdateGame() {
    // Wait until 16ms has elapsed since last frame
//    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
//        ;
//    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
//    if (deltaTime > 0.05f)
//    {
//        deltaTime = 0.05f;
//    }
//    mTicksCount = SDL_GetTicks();
    std::vector<std::vector<bool>> new_cells(cells.size());
    for (int i = 0; i < new_cells.size(); i++) {
        new_cells[i] = std::vector<bool>(cells[i].size());
    }
    for (int i = 1; i < cells.size()-1; i++) {
        for (int j = 1; j < cells[i].size()-1; j++) {

            int neighbours = cells[i-1][j-1] + cells[i][j-1] + cells[i+1][j-1] + cells[i-1][j] + cells[i+1][j] + cells[i-1][j+1] + cells[i][j+1] + cells[i+1][j+1];
            if ((cells[i][j] && (neighbours == 2 || neighbours == 3)) || (!cells[i][j] && neighbours == 3)) {
                new_cells[i][j] = true;
            } else {
                new_cells[i][j] = false;
            }
        }
    }
    for (int i = 0; i < new_cells.size(); i++) {
        std::copy(begin(new_cells[i]), end(new_cells[i]), begin(cells[i]));
    }
    SDL_Delay(50);
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0);
    SDL_RenderClear(mRenderer);
    for (int i = 1; i < cells.size() - 1; i++) {
        for (int j = 1; j < cells[i].size() - 1; j++) {
            if (cells[i][j]) {
                SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
            } else {
                SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 0);
            }
            SDL_Rect square = {50 * j, 50 * i, 50, 50};
            SDL_RenderFillRect(mRenderer, &square);
        }
    }
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
