#pragma once

#include <SDL.h>
#include <map>
#include <string>

class AssetStore
{
  private:
    std::map<std::string, SDL_Texture *> textures;
    // TODO: create a map for fonts
    // TODO: create a map for audio
  public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void AddTexture(SDL_Renderer *renderer, const std::string &, const std::string &filePath);
    SDL_Texture *GetTexture(const std::string &assetId);
};
