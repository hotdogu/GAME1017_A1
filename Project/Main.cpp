#include <chrono>
#include "Game.h"
#include <assert.h>

int main(int argc, char* argv[])
{
    Game* pMyGameInstance = Game::GetInstance();
    assert(pMyGameInstance != nullptr);
    int result = pMyGameInstance->Init("GAME1017", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    if (result == 0)
    {
        // Allocate a time_point outside that loop so that it is retained between frames.
        auto lastFrameTime = std::chrono::high_resolution_clock::now();

        while (pMyGameInstance->IsRunning())
        {
            // Get current time_point.
            auto thisFrameTime = std::chrono::high_resolution_clock::now();

            // Subtract last frame's time_point from this frame's time_point to get the duration of the frame.
            std::chrono::duration<float> lastFrameDuration = thisFrameTime - lastFrameTime;

            // Convert the duration to a float.
            float deltaTime = lastFrameDuration.count();

            // Don't forget to pass newFrameCounter into lastFrameCounter.
            lastFrameTime = thisFrameTime;

            pMyGameInstance->HandleEvents();
            pMyGameInstance->Update(deltaTime);
            pMyGameInstance->Render();
        }
        pMyGameInstance->Clean();
    }
    Game::DeleteInstance();

    return result;
}


