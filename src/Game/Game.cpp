#include "Game.hpp"
#include "../AssetStore/AssetStore.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/TranformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../Logger/Logger.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
  Logger::Log("Game constructor called!");
  registry = std::make_unique<Registry>();
  assetStore = std::make_unique<AssetStore>();
  isRunning = false;
}

Game::~Game() { Logger::Err("Game destructor called!"); }

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("Error initializing SDL.");
    return;
  }
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = 800;  // displayMode.w;
  windowHeight = 600; // displayMode.h;
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
  if (!window) {
    Logger::Err("Error creating SDL window.");
    return;
  }
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    Logger::Err("Error creating SDL Renderer.");
    return;
  }
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if ((sdlEvent.key.keysym.sym == SDLK_ESCAPE) ||
          (sdlEvent.key.keysym.sym == SDLK_q)) {
        isRunning = false;
      }
      break;
    }
  }
}

void Game::Setup() {
  // Add the systems that need to be processed in our game
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();

  // Adding assets to the Asset Store
  assetStore->AddTexture(renderer, "tank-image",
                         "./assets/images/tank-panther-right.png");
  assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

  // Create some entity
  Entity tank = registry->CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0),
                                        glm::vec2(1.0, 1.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0));
  tank.AddComponent<SpriteComponent>("tank-image", 10, 10);

  Entity truck = registry->CreateEntity();
  truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0),
                                         glm::vec2(1.0, 1.0), 0.0);
  truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0));
  truck.AddComponent<SpriteComponent>("truck-image", 10, 50);
}

void Game::Update() {
  // if we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
  int timeToWait =
      MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  // The difference in ticks since the last frame, converted to seconds
  double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

  // Store the current frame time
  millisecsPreviousFrame = SDL_GetTicks();

  // Ask all the systems to update
  registry->GetSystem<MovementSystem>().Update(deltaTime);

  // Update the registry to process the entities that are waiting to be
  // created/deleted
  registry->Update();
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Invoke all the systems that need to render
  registry->GetSystem<RenderSystem>().Update(renderer);

  SDL_RenderPresent(renderer);
}

void Game::Run() {
  Setup();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
