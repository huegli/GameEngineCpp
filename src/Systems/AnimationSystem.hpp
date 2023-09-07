#pragma once

#include "../Components/AnimationComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../ECS/ECS.hpp"
#include <SDL.h>

class AnimationSystem : public System
{
  public:
    AnimationSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<AnimationComponent>();
    };

    void Update()
    {
        for (auto entity : GetSystemEntities())
        {
            auto &animation = entity.GetComponent<AnimationComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            animation.currentFrame =
                ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
            sprite.srcRect.x = animation.currentFrame * sprite.width;
        }
    }
};
