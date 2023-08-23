#pragma once

#include "../AssetStore/AssetStore.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/TranformComponent.hpp"
#include "../ECS/ECS.hpp"
#include <SDL.h>

class RenderSystem : public System {
public:
  RenderSystem() {
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
  }

  void Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore) {
    // Loop all entities that the system is interested in
    for (auto entity : GetSystemEntities()) {
      const auto transform = entity.GetComponent<TransformComponent>();
      const auto sprite = entity.GetComponent<SpriteComponent>();

      // Set the source rectangle at our original sprite texture
      SDL_Rect srcRect = sprite.srcRect;

      // Set the destination rectangle with the x,y position to be rendered
      SDL_Rect dstRect = {static_cast<int>(transform.position.x),
                          static_cast<int>(transform.position.y),
                          static_cast<int>(sprite.width * transform.scale.x),
                          static_cast<int>(sprite.height * transform.scale.y)};

      SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId),
                       &srcRect, &dstRect, transform.rotation, NULL,
                       SDL_FLIP_NONE);
    }
  }
};
