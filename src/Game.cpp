#include "Constants.h"
#include "Game.h"
#include "./Components/TransformComponent.h"
#include "../lib/glm/glm.hpp"
#include "./Components/SpriteComponent.h"
#include "./Components/TransformComponent.h"
#include <iostream>

EntityManager manager;
AssetManager *Game::assetManager = new AssetManager(&manager);
SDL_Renderer *Game::renderer;

Game::Game()
{
    this->isRunning = false;
}

Game::~Game()
{
}

bool Game::IsRunning() const
{
    return this->isRunning;
}

void Game::Initialize(int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Init fail" << std::endl;
    }
    window = SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        std::cerr << "Error creating SDL window" << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;
};

void Game::LoadLevel(int levelNumber)
{
    std::string textureFilePath = "./assets/images/tank-big-right.png";
    assetManager->AddTexture("tank-image", textureFilePath.c_str());

    Entity &newEntity(manager.AddEntity("tank"));
    newEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    newEntity.AddComponent<SpriteComponent>("tank-image");
}

void Game::ProcessInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            isRunning = false;
        }
    default:
        break;
    }
}

void Game::Update()
{
    // If the frame is too fast, wait till the next frame time
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // If the frame is too slow, set a max time for it
    deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;

    ticksLastFrame = SDL_GetTicks();

    // Call manager.update to update all entities
    manager.Update(deltaTime);
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities())
    {
        return;
    }

    // Call manager.render to render all entities
    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}